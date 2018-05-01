/*  Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/*
    Wireless keyboard firmware.
    This firmware is essentially a combination of 2 main features:
    * Wireless connectivity to PC via Bluetooth Low Energy HID protocol. This allows it to work on iOS devices.
    * Remapping of alpha keys so that they can be arranged in ABC order instead of the usual QWERTY.

    The BLE code in this project originated from Nordic Semiconductor's BLE HID Keyboard Sample.
*/
#include <stdint.h>
#include <string.h>
#include <nordic_common.h>
#include <nrf.h>
#include <nrf_assert.h>
#include <app_error.h>
#include <nrf_gpio.h>
#include <ble.h>
#include <ble_hci.h>
#include <ble_srv_common.h>
#include <ble_advertising.h>
#include <ble_advdata.h>
#include <ble_hids.h>
#include <ble_bas.h>
#include <ble_dis.h>
#include <ble_conn_params.h>
#include <bsp.h>
#include <sensorsim.h>
#include <app_scheduler.h>
#include <softdevice_handler_appsh.h>
#include <app_timer_appsh.h>
#include <device_manager.h>
#include <app_button.h>
#include <pstorage.h>
#include <app_trace.h>
#include <nrf_drv_gpiote.h>
#include <app_uart.h>
#include "KeyboardMatrix.h"
#include "MCP23018.h"
#include "nrf_drv_timer.h"


// nRF51 pins connected to the I2C pins on the MCP23018 I/O expanders used with the keyboard matrix.
#define KEYBOARD_SCL_PIN                15
#define KEYBOARD_SDA_PIN                5

// The INTA & INTB pins for each of the MCP23018 devices.
#define KEYBOARD_INTA1_PIN              3
#define KEYBOARD_INTB1_PIN              8
#define KEYBOARD_INTA2_PIN              11
#define KEYBOARD_INTB2_PIN              21

// There are two MCP23018 16-bit I/O expanders used to connect to all of the columns and rows of the keyboard matrix.
// The following defines provide the I2C addresss of these two MCP23018 expanders.
#define KEYBOARD_MCP23018_1_I2C_ADDRESS 0x20
#define KEYBOARD_MCP23018_2_I2C_ADDRESS 0x27

// Configuration options for the interrupt driven UART driver.
#define UART_TX_PIN      6
#define UART_RX_PIN      12
// CTS & RTS not actually used in this implementation.
#define UART_CTS_PIN     0xFF
#define UART_RTS_PIN     0xFF
#define UART_TX_BUF_SIZE 256
#define UART_RX_BUF_SIZE 1

// nRF51 pins used for caps and num lock LEDs.
#define CAPS_LOCK_PIN   0
#define NUM_LOCK_PIN    24

// Set to non-zero value to dump BLE HID reports.
#define DEBUG_SHOW_HID_REPORT       1

// Set to non-zero value if we want to remap keyboard to be special alphabetical order keyboard.
#define KEYBOARD_IS_SPECIAL_ABC     1

// BLE HID is based on USB HID. The keyboard communicates with 8-byte input reports and 1-byte output report.
// Version number of base USB HID Specification implemented by this application.
#define BASE_USB_HID_SPEC_VERSION        0x0101
// Index of Output Report.
#define OUTPUT_REPORT_INDEX              0
// Maximum length of Output Report.
#define OUTPUT_REPORT_MAX_LEN            sizeof(HidKeyboardOutputReport)
// Id of reference to Keyboard Output Report.
#define OUTPUT_REP_REF_ID                0
// Index of Input Report.
#define INPUT_REPORT_KEYS_INDEX          0
// Id of reference to Keyboard Input Report.
#define INPUT_REP_REF_ID                 0
// Maximum length of the Input Report characteristic.
#define INPUT_REPORT_KEYS_MAX_LEN        sizeof(HidKeyboardInputReport)


// UNDONE: Need to update for new keyboard matrix code.
// Configure keyboard type (QWERTY/ABC) macros based on previously defined macro settings.
#if KEYBOARD_IS_SPECIAL_ABC
    #define ALPHA_SCAN_CODES g_scanCodesABC
    #define ALPHA_HID_CODES  g_scanCodesToHidABC
#else
    #define ALPHA_SCAN_CODES g_scanCodesQWERTY
    #define ALPHA_HID_CODES  g_scanCodesToHidQWERTY
#endif


// Include or not the service_changed characteristic. 
// If not enabled, the server's database cannot be changed for the lifetime of the device.
#define IS_SRVC_CHANGED_CHARACT_PRESENT  0                                              

// Name of device. Will be included in the advertising data.
#define DEVICE_NAME                      "Nordic_Keyboard"                              
// Manufacturer. Will be passed to Device Information Service.
#define MANUFACTURER_NAME                "NordicSemiconductor"
// Product/Vendor IDs reported by Device Information Service.
#define PNP_ID_VENDOR_ID_SOURCE          0x02
#define PNP_ID_VENDOR_ID                 0x1915
#define PNP_ID_PRODUCT_ID                0xEEEE
#define PNP_ID_PRODUCT_VERSION           0x0001

// RTC1 low speed (32.768kHz) based application timer.
#define APP_TIMER_PRESCALER              0
#define APP_TIMER_OP_QUEUE_SIZE          4

// Update battery level measurement every 2000 milliseconds (2 seconds).
#define BATTERY_LEVEL_MEAS_INTERVAL      APP_TIMER_TICKS(2000, APP_TIMER_PRESCALER)
// Settings for battery simulation.
#define MIN_BATTERY_LEVEL                81
#define MAX_BATTERY_LEVEL                100
#define BATTERY_LEVEL_INCREMENT          1

// Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.).
#define APP_ADV_FAST_INTERVAL            0x0028                                         
// Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds).
#define APP_ADV_SLOW_INTERVAL            0x0C80
// The duration of the fast advertising period (in seconds).
#define APP_ADV_FAST_TIMEOUT             30
// The duration of the slow advertising period (in seconds).
#define APP_ADV_SLOW_TIMEOUT             180

// Minimum connection interval (7.5 ms).
#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(7.5, UNIT_1_25_MS)
// Maximum connection interval (30 ms).
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(30, UNIT_1_25_MS)
// Slave latency.
#define SLAVE_LATENCY                    6
// Connection supervisory timeout (430 ms).
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(430, UNIT_10_MS)

// Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds).
#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)     
// Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds).
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)
// Number of attempts before giving up the connection parameter negotiation.
#define MAX_CONN_PARAMS_UPDATE_COUNT     3

// Perform bonding.
#define SEC_PARAM_BOND                   1                                              
// Man In The Middle protection not required.
#define SEC_PARAM_MITM                   0
// No I/O capabilities.
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE
// Out Of Band data not available.
#define SEC_PARAM_OOB                    0
// Minimum encryption key size.
#define SEC_PARAM_MIN_KEY_SIZE           7
// Maximum encryption key size.
#define SEC_PARAM_MAX_KEY_SIZE           16                                             

// Reply when unsupported features are requested.
#define APP_FEATURE_NOT_SUPPORTED       BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2            

// Value used as error code on stack dump, can be used to identify stack location on stack unwind.
#define DEAD_BEEF                        0xDEADBEEF                                     

// Application Scheduler Settings.
#define SCHED_MAX_EVENT_DATA_SIZE        (MAX(APP_TIMER_SCHED_EVT_SIZE,BLE_STACK_HANDLER_SCHED_EVT_SIZE))
#define SCHED_QUEUE_SIZE                 10                                  



// BLE Advertising Modes.
typedef enum
{
    BLE_NO_ADV,               // No advertising running.
    BLE_DIRECTED_ADV,         // Direct advertising to the latest central.
    BLE_FAST_ADV_WHITELIST,   // Advertising with whitelist.
    BLE_FAST_ADV,             // Fast advertising running.
    BLE_SLOW_ADV,             // Slow advertising running.
    BLE_SLEEP,                // Go to system-off.
} ble_advertising_mode_t;


// Battery timer.
APP_TIMER_DEF(g_batteryTimerId);                                                      

// Structure used to identify the HID service.
static ble_hids_t                   g_hids;
// Structure used to identify the battery service.
static ble_bas_t                    g_bas;
// Handle of the current connection.
static uint16_t                     g_connHandle = BLE_CONN_HANDLE_INVALID;       
// Current protocol mode. Keyboard can run in standard HID mode or boot mode where it uses a fixed report format.
static bool                         g_inBootMode = false;

// Battery Level sensor simulator configuration.
static sensorsim_cfg_t              g_batterySimConfig;
// Battery Level sensor simulator state.
static sensorsim_state_t            g_batterySimState;                           

// Application identifier allocated by device manager.
static dm_application_instance_t    g_appHandle;
// Device reference handle to the current bonded central.
static dm_handle_t                  g_bondedPeerHandle;

// Advertise that this is a standard BLE HID device.
static ble_uuid_t                   g_advertisingUuids[] = {{BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE, BLE_UUID_TYPE_BLE}};

// Object used to interact with the keyboard matrix.
static KeyboardMatrix               g_keyboardMatrix;


// Forward function declarations.
static void initUart(void);
static void handleUartEvent(app_uart_evt_t* pEvent);
static void initRtcTimers(void);
static void handleBatteryLevelTimerEvent(void* pContext);
static void updateBatteryLevel(void);
static void keyboardTimerHandler(nrf_timer_event_t eventType, void* pvContext);
static void initNumCapsLEDs();
static void initBspLeds();
static void handleBspEvent(bsp_event_t event);
static void enterSleepMode(void);
static void initBleStack(void);
static void handleBleEvent(ble_evt_t * pBleEvent);
static void handleBleEventForApplication(ble_evt_t * pBleEvent);
static void handleSystemEvent(uint32_t sysEvent);
static void initScheduler(void);
static void initDeviceManager(bool eraseBonds);
static uint32_t handleDeviceManagerEvent(dm_handle_t const* pHandle,
                                         dm_event_t const*  pEvent,
                                         ret_code_t         eventResult);
static void initGapParams(void);
static void initAdvertising(void);
static void handleAdvertisingEvent(ble_adv_evt_t bleAdvertisingEvent);
static void handleBleAdvertisingError(uint32_t errorCode);
static void initBleServices(void);
static void initDeviceInformationService(void);
static void initBatteryService(void);
static void initHidServiceForKeyboard(void);
static void handleHidEvent(ble_hids_t * p_hids, ble_hids_evt_t *p_evt);
static void handleHidOutputReportWrite(ble_hids_evt_t *p_evt);
static void updateKeyboardLeds(uint8_t hidLedBitmask);
static void handleServiceError(uint32_t errorCode);
static void initSensorSimulator(void);
static void initConnectionParams(void);
static void handleConnectionParamsError(uint32_t errorCode);
static void startRtcTimers(void);
static void sendInputReportHandler(HidKeyboardInputReport* pReportToSend, void* pvContext, bool sleepPressed);
static void enterLowPowerModeAndWakeOnEvent(void);



int main(void)
{
    bool     eraseBonds = false;
    uint32_t errorCode;

    initUart();
    initRtcTimers();
    errorCode = kbmatrixInit(&g_keyboardMatrix,
                             KEYBOARD_MCP23018_1_I2C_ADDRESS, KEYBOARD_MCP23018_2_I2C_ADDRESS,
                             KEYBOARD_SCL_PIN, KEYBOARD_SDA_PIN, 
                             KEYBOARD_INTA1_PIN, KEYBOARD_INTB1_PIN, KEYBOARD_INTA2_PIN, KEYBOARD_INTB2_PIN,
                             APP_TIMER_PRESCALER,
                             sendInputReportHandler, NULL,
                             &eraseBonds);
    APP_ERROR_CHECK(errorCode);
    initNumCapsLEDs();
    initBspLeds();
    initBleStack();
    initScheduler();
    initDeviceManager(eraseBonds);
    initGapParams();
    initAdvertising();
    initBleServices();
    initSensorSimulator();
    initConnectionParams();

    printf("\nBleHidKeyboard starting...\n");
    startRtcTimers();
    errorCode = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(errorCode);

    errorCode = kbmatrixStart(&g_keyboardMatrix);
    APP_ERROR_CHECK(errorCode);
    
    // Sleep until a new event occurs and then check the scheduler to execute any queued operations.
    while (1)
    {
        app_sched_execute();
        enterLowPowerModeAndWakeOnEvent();
    }
}

static void initUart(void)
{
    const bool noParity = false;
    uint32_t   errorCode;

    const app_uart_comm_params_t comm_params =
    {
        UART_RX_PIN,
        UART_TX_PIN,
        UART_RTS_PIN,
        UART_CTS_PIN,
        APP_UART_FLOW_CONTROL_DISABLED,
        noParity,
        UART_BAUDRATE_BAUDRATE_Baud9600
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       handleUartEvent,
                       APP_IRQ_PRIORITY_LOW,
                       errorCode);
    APP_ERROR_CHECK(errorCode);

    // Pull-up the Rx pin since it is usually just left floating otherwise.
    nrf_gpio_cfg_input(UART_RX_PIN, NRF_GPIO_PIN_PULLUP);
}

static void handleUartEvent(app_uart_evt_t* pEvent)
{
    if (pEvent->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(pEvent->data.error_communication);
    }
    else if (pEvent->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(pEvent->data.error_code);
    }
}

static void initRtcTimers(void)
{
    uint32_t errorCode;

    // Initialize timer module, making it use the scheduler.
    APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

    // Create battery timer.
    errorCode = app_timer_create(&g_batteryTimerId,
                                APP_TIMER_MODE_REPEATED,
                                handleBatteryLevelTimerEvent);
    APP_ERROR_CHECK(errorCode);
}

static void handleBatteryLevelTimerEvent(void* pContext)
{
    updateBatteryLevel();
}

static void updateBatteryLevel(void)
{
    uint32_t errorCode;
    uint8_t  batteryLevel;

    // UNDONE: Should sample actual battery voltage.
    batteryLevel = (uint8_t)sensorsim_measure(&g_batterySimState, &g_batterySimConfig);

    errorCode = ble_bas_battery_level_update(&g_bas, batteryLevel);
    if ((errorCode != NRF_SUCCESS) &&
        (errorCode != NRF_ERROR_INVALID_STATE) &&
        (errorCode != BLE_ERROR_NO_TX_BUFFERS) &&
        (errorCode != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
    {
        APP_ERROR_HANDLER(errorCode);
    }
}

static void initNumCapsLEDs()
{
    nrf_gpio_cfg_output(CAPS_LOCK_PIN);
    nrf_gpio_cfg_output(NUM_LOCK_PIN);
    nrf_gpio_pin_clear(CAPS_LOCK_PIN);
    nrf_gpio_pin_clear(NUM_LOCK_PIN);
}

static void initBspLeds()
{
    uint32_t errorCode = bsp_init(BSP_INIT_LED,
                                  APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
                                  handleBspEvent);
    APP_ERROR_CHECK(errorCode);
}

static void handleBspEvent(bsp_event_t event)
{
    uint32_t errorCode;

    // UNDONE: Should be done through keyboard key presses now.
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            enterSleepMode();
            break;

        case BSP_EVENT_DISCONNECT:
            errorCode = sd_ble_gap_disconnect(g_connHandle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (errorCode != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(errorCode);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            errorCode = ble_advertising_restart_without_whitelist();
            if (errorCode != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(errorCode);
            }
            break;

        default:
            break;
    }
}

static void enterSleepMode(void)
{
    // Turn LEDs off to save power.
    nrf_gpio_pin_clear(CAPS_LOCK_PIN);
    nrf_gpio_pin_clear(NUM_LOCK_PIN);

    uint32_t errorCode = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(errorCode);

    // Prepare wakeup buttons.
    errorCode = kbmatrixConfigureForWakeupOnSpacebar(&g_keyboardMatrix);
    APP_ERROR_CHECK(errorCode);
    
    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    errorCode = sd_power_system_off();
    APP_ERROR_CHECK(errorCode);
}

static void initBleStack(void)
{
    uint32_t errorCode;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_APPSH_INIT(NRF_CLOCK_LFCLKSRC_XTAL_50_PPM, true);

    // Enable BLE stack 
    ble_enable_params_t bleEnableParams;
    memset(&bleEnableParams, 0, sizeof(bleEnableParams));
#if (defined(S130) || defined(S132))
    bleEnableParams.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
    bleEnableParams.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    errorCode = sd_ble_enable(&bleEnableParams);
    APP_ERROR_CHECK(errorCode);

    // Register with the SoftDevice handler module for BLE events.
    errorCode = softdevice_ble_evt_handler_set(handleBleEvent);
    APP_ERROR_CHECK(errorCode);

    // Register with the SoftDevice handler module for BLE events.
    errorCode = softdevice_sys_evt_handler_set(handleSystemEvent);
    APP_ERROR_CHECK(errorCode);
}

static void handleBleEvent(ble_evt_t * pBleEvent)
{
    dm_ble_evt_handler(pBleEvent);
    handleBleEventForApplication(pBleEvent);
    ble_advertising_on_ble_evt(pBleEvent);
    ble_conn_params_on_ble_evt(pBleEvent);
    ble_hids_on_ble_evt(&g_hids, pBleEvent);
    ble_bas_on_ble_evt(&g_bas, pBleEvent);
}

// Function for handling BLE stack events that are specific to this firmware.
static void handleBleEventForApplication(ble_evt_t * pBleEvent)
{
    uint32_t                              errorCode;
    ble_gatts_rw_authorize_reply_params_t authorizeReply;

    switch (pBleEvent->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            errorCode = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(errorCode);
            g_connHandle = pBleEvent->evt.gap_evt.conn_handle;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            g_connHandle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_EVT_USER_MEM_REQUEST:
            errorCode = sd_ble_user_mem_reply(g_connHandle, NULL);
            APP_ERROR_CHECK(errorCode);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            if(pBleEvent->evt.gatts_evt.params.authorize_request.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if ((pBleEvent->evt.gatts_evt.params.authorize_request.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ) || 
                    (pBleEvent->evt.gatts_evt.params.authorize_request.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) || 
                    (pBleEvent->evt.gatts_evt.params.authorize_request.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
                {
                    if (pBleEvent->evt.gatts_evt.params.authorize_request.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
                    {
                        authorizeReply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                    }
                    else
                    {
                        authorizeReply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
                    }
                    authorizeReply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
                    errorCode = sd_ble_gatts_rw_authorize_reply(g_connHandle,&authorizeReply);
                    APP_ERROR_CHECK(errorCode);
                }
            }
            break;

        case BLE_GATTC_EVT_TIMEOUT:
        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server and Client timeout events.
            errorCode = sd_ble_gap_disconnect(g_connHandle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(errorCode);
            break;

        default:
            // No implementation needed.
            break;
    }
}

static void handleSystemEvent(uint32_t sysEvent)
{
    pstorage_sys_event_handler(sysEvent);
    ble_advertising_on_sys_evt(sysEvent);
}

static void initScheduler(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void initDeviceManager(bool eraseBonds)
{
    uint32_t                errorCode;
    dm_init_param_t         initParam = {.clear_persistent_data = eraseBonds};
    dm_application_param_t  registerParam;

    // Initialize peer device handle.
    errorCode = dm_handle_initialize(&g_bondedPeerHandle);
    APP_ERROR_CHECK(errorCode);
    
    // Initialize persistent storage module.
    errorCode = pstorage_init();
    APP_ERROR_CHECK(errorCode);

    errorCode = dm_init(&initParam);
    APP_ERROR_CHECK(errorCode);
    
    memset(&registerParam.sec_param, 0, sizeof(ble_gap_sec_params_t));

    registerParam.sec_param.bond         = SEC_PARAM_BOND;
    registerParam.sec_param.mitm         = SEC_PARAM_MITM;
    registerParam.sec_param.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    registerParam.sec_param.oob          = SEC_PARAM_OOB;
    registerParam.sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    registerParam.sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    registerParam.evt_handler            = handleDeviceManagerEvent;
    registerParam.service_type           = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;

    errorCode = dm_register(&g_appHandle, &registerParam);
    APP_ERROR_CHECK(errorCode);
}

static uint32_t handleDeviceManagerEvent(dm_handle_t const* pHandle,
                                         dm_event_t const*  pEvent,
                                         ret_code_t         eventResult)
{
    APP_ERROR_CHECK(eventResult);

    switch(pEvent->event_id)
    {
        case DM_EVT_DEVICE_CONTEXT_LOADED: // Fall through.
        case DM_EVT_SECURITY_SETUP_COMPLETE:
            g_bondedPeerHandle = (*pHandle);
            break;
    }

    return NRF_SUCCESS;
}

static void initGapParams(void)
{
    uint32_t                errorCode;
    ble_gap_conn_params_t   gapConnParams;
    ble_gap_conn_sec_mode_t securityMode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&securityMode);

    errorCode = sd_ble_gap_device_name_set(&securityMode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(errorCode);

    errorCode = sd_ble_gap_appearance_set(BLE_APPEARANCE_HID_KEYBOARD);
    APP_ERROR_CHECK(errorCode);

    memset(&gapConnParams, 0, sizeof(gapConnParams));

    gapConnParams.min_conn_interval = MIN_CONN_INTERVAL;
    gapConnParams.max_conn_interval = MAX_CONN_INTERVAL;
    gapConnParams.slave_latency     = SLAVE_LATENCY;
    gapConnParams.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    errorCode = sd_ble_gap_ppcp_set(&gapConnParams);
    APP_ERROR_CHECK(errorCode);
}

static void initAdvertising(void)
{
    uint32_t       errorCode;
    uint8_t        advertiseFlags;
    ble_advdata_t  advertiseData;

    // Build and set advertising data
    memset(&advertiseData, 0, sizeof(advertiseData));

    advertiseFlags                        = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    advertiseData.name_type               = BLE_ADVDATA_FULL_NAME;
    advertiseData.include_appearance      = true;
    advertiseData.flags                   = advertiseFlags;
    advertiseData.uuids_complete.uuid_cnt = sizeof(g_advertisingUuids) / sizeof(g_advertisingUuids[0]);
    advertiseData.uuids_complete.p_uuids  = g_advertisingUuids;

    ble_adv_modes_config_t options =
    {
        BLE_ADV_WHITELIST_ENABLED,
        BLE_ADV_DIRECTED_ENABLED,
        BLE_ADV_DIRECTED_SLOW_DISABLED, 0,0,
        BLE_ADV_FAST_ENABLED, APP_ADV_FAST_INTERVAL, APP_ADV_FAST_TIMEOUT,
        BLE_ADV_SLOW_ENABLED, APP_ADV_SLOW_INTERVAL, APP_ADV_SLOW_TIMEOUT
    };

    errorCode = ble_advertising_init(&advertiseData, NULL, &options, handleAdvertisingEvent, handleBleAdvertisingError);
    APP_ERROR_CHECK(errorCode);
}

static void handleAdvertisingEvent(ble_adv_evt_t bleAdvertisingEvent)
{
    uint32_t errorCode;

    switch (bleAdvertisingEvent)
    {
        case BLE_ADV_EVT_DIRECTED:
            errorCode = bsp_indication_set(BSP_INDICATE_ADVERTISING_DIRECTED);
            APP_ERROR_CHECK(errorCode);
            break;
        case BLE_ADV_EVT_FAST:
            errorCode = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(errorCode);
            break;
        case BLE_ADV_EVT_SLOW:
            errorCode = bsp_indication_set(BSP_INDICATE_ADVERTISING_SLOW);
            APP_ERROR_CHECK(errorCode);
            break;
        case BLE_ADV_EVT_FAST_WHITELIST:
            errorCode = bsp_indication_set(BSP_INDICATE_ADVERTISING_WHITELIST);
            APP_ERROR_CHECK(errorCode);
            break;
        case BLE_ADV_EVT_SLOW_WHITELIST:
            errorCode = bsp_indication_set(BSP_INDICATE_ADVERTISING_WHITELIST);
            APP_ERROR_CHECK(errorCode);
            break;
        case BLE_ADV_EVT_IDLE:
            enterSleepMode();
            break;

        case BLE_ADV_EVT_WHITELIST_REQUEST:
        {
            ble_gap_whitelist_t whitelist;
            ble_gap_addr_t*     pWhitelistAddr[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
            ble_gap_irk_t*      pWhitelistIrk[BLE_GAP_WHITELIST_IRK_MAX_COUNT];

            whitelist.addr_count = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
            whitelist.irk_count  = BLE_GAP_WHITELIST_IRK_MAX_COUNT;
            whitelist.pp_addrs   = pWhitelistAddr;
            whitelist.pp_irks    = pWhitelistIrk;

            errorCode = dm_whitelist_create(&g_appHandle, &whitelist);
            APP_ERROR_CHECK(errorCode);

            errorCode = ble_advertising_whitelist_reply(&whitelist);
            APP_ERROR_CHECK(errorCode);
            break;
        }
        case BLE_ADV_EVT_PEER_ADDR_REQUEST:
        {
            ble_gap_addr_t peerAddress;

            // Only Give peer address if we have a handle to the bonded peer.
            if(g_bondedPeerHandle.appl_id != DM_INVALID_ID)
            {
                errorCode = dm_peer_addr_get(&g_bondedPeerHandle, &peerAddress);
                if (errorCode == NRF_SUCCESS)
                {
                    errorCode = ble_advertising_peer_addr_reply(&peerAddress);
                    APP_ERROR_CHECK(errorCode);
                }

            }
            break;
        }
        default:
            break;
    }
}

static void handleBleAdvertisingError(uint32_t errorCode)
{
    APP_ERROR_HANDLER(errorCode);
}

static void initBleServices(void)
{
    initDeviceInformationService();
    initBatteryService();
    initHidServiceForKeyboard();
}

static void initDeviceInformationService(void)
{
    uint32_t         errorCode;
    ble_dis_init_t   disInitObject;
    ble_dis_pnp_id_t pnpId;

    pnpId.vendor_id_source = PNP_ID_VENDOR_ID_SOURCE;
    pnpId.vendor_id        = PNP_ID_VENDOR_ID;
    pnpId.product_id       = PNP_ID_PRODUCT_ID;
    pnpId.product_version  = PNP_ID_PRODUCT_VERSION;

    memset(&disInitObject, 0, sizeof(disInitObject));

    ble_srv_ascii_to_utf8(&disInitObject.manufact_name_str, MANUFACTURER_NAME);
    disInitObject.p_pnp_id = &pnpId;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&disInitObject.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&disInitObject.dis_attr_md.write_perm);

    errorCode = ble_dis_init(&disInitObject);
    APP_ERROR_CHECK(errorCode);
}

static void initBatteryService(void)
{
    uint32_t       errorCode;
    ble_bas_init_t basInitObject;

    memset(&basInitObject, 0, sizeof(basInitObject));

    basInitObject.evt_handler          = NULL;
    basInitObject.support_notification = true;
    basInitObject.p_report_ref         = NULL;
    basInitObject.initial_batt_level   = 100;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&basInitObject.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&basInitObject.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&basInitObject.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&basInitObject.battery_level_report_read_perm);

    errorCode = ble_bas_init(&g_bas, &basInitObject);
    APP_ERROR_CHECK(errorCode);
}

static void initHidServiceForKeyboard(void)
{
    uint32_t                   errorCode;
    ble_hids_init_t            hidsInitObject;
    ble_hids_inp_rep_init_t    inputReportArray[1];
    ble_hids_inp_rep_init_t  * pInputReport;
    ble_hids_outp_rep_init_t   outputReportArray[1];
    ble_hids_outp_rep_init_t * pOutputReport;
    uint8_t                    hidInfoFlags;

    memset((void *)inputReportArray, 0, sizeof(ble_hids_inp_rep_init_t));
    memset((void *)outputReportArray, 0, sizeof(ble_hids_outp_rep_init_t));
    
    static uint8_t report_map_data[] =
    {
        0x05, 0x01,                 // Usage Page (Generic Desktop)
        0x09, 0x06,                 // Usage (Keyboard)
        0xA1, 0x01,                 // Collection (Application)
        0x05, 0x07,                 //     Usage Page (Key Codes)
        0x19, 0xe0,                 //     Usage Minimum (224)
        0x29, 0xe7,                 //     Usage Maximum (231)
        0x15, 0x00,                 //     Logical Minimum (0)
        0x25, 0x01,                 //     Logical Maximum (1)
        0x75, 0x01,                 //     Report Size (1)
        0x95, 0x08,                 //     Report Count (8)
        0x81, 0x02,                 //     Input (Data, Variable, Absolute)

        0x95, 0x01,                 //     Report Count (1)
        0x75, 0x08,                 //     Report Size (8)
        0x81, 0x01,                 //     Input (Constant) reserved byte(1)

        0x95, 0x05,                 //     Report Count (5)
        0x75, 0x01,                 //     Report Size (1)
        0x05, 0x08,                 //     Usage Page (Page# for LEDs)
        0x19, 0x01,                 //     Usage Minimum (1)
        0x29, 0x05,                 //     Usage Maximum (5)
        0x91, 0x02,                 //     Output (Data, Variable, Absolute), Led report
        0x95, 0x01,                 //     Report Count (1)
        0x75, 0x03,                 //     Report Size (3)
        0x91, 0x01,                 //     Output (Data, Variable, Absolute), Led report padding

        0x95, 0x06,                 //     Report Count (6)
        0x75, 0x08,                 //     Report Size (8)
        0x15, 0x00,                 //     Logical Minimum (0)
        0x25, 0x65,                 //     Logical Maximum (101)
        0x05, 0x07,                 //     Usage Page (Key codes)
        0x19, 0x00,                 //     Usage Minimum (0)
        0x29, 0x65,                 //     Usage Maximum (101)
        0x81, 0x00,                 //     Input (Data, Array) Key array(6 bytes)

        0x09, 0x05,                 //     Usage (Vendor Defined)
        0x15, 0x00,                 //     Logical Minimum (0)
        0x26, 0xFF, 0x00,           //     Logical Maximum (255)
        0x75, 0x08,                 //     Report Count (2)
        0x95, 0x02,                 //     Report Size (8 bit)
        0xB1, 0x02,                 //     Feature (Data, Variable, Absolute)

        0xC0                        // End Collection (Application)
    };

    // Initialize HID Service
    pInputReport                      = &inputReportArray[INPUT_REPORT_KEYS_INDEX];
    pInputReport->max_len             = INPUT_REPORT_KEYS_MAX_LEN;
    pInputReport->rep_ref.report_id   = INPUT_REP_REF_ID;
    pInputReport->rep_ref.report_type = BLE_HIDS_REP_TYPE_INPUT;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&pInputReport->security_mode.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&pInputReport->security_mode.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&pInputReport->security_mode.write_perm);

    pOutputReport                      = &outputReportArray[OUTPUT_REPORT_INDEX];
    pOutputReport->max_len             = OUTPUT_REPORT_MAX_LEN;
    pOutputReport->rep_ref.report_id   = OUTPUT_REP_REF_ID;
    pOutputReport->rep_ref.report_type = BLE_HIDS_REP_TYPE_OUTPUT;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&pOutputReport->security_mode.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&pOutputReport->security_mode.write_perm);

    hidInfoFlags = HID_INFO_FLAG_REMOTE_WAKE_MSK | HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK;

    memset(&hidsInitObject, 0, sizeof(hidsInitObject));

    hidsInitObject.evt_handler                    = handleHidEvent;
    hidsInitObject.error_handler                  = handleServiceError;
    hidsInitObject.is_kb                          = true;
    hidsInitObject.is_mouse                       = false;
    hidsInitObject.inp_rep_count                  = 1;
    hidsInitObject.p_inp_rep_array                = inputReportArray;
    hidsInitObject.outp_rep_count                 = 1;
    hidsInitObject.p_outp_rep_array               = outputReportArray;
    hidsInitObject.feature_rep_count              = 0;
    hidsInitObject.p_feature_rep_array            = NULL;
    hidsInitObject.rep_map.data_len               = sizeof(report_map_data);
    hidsInitObject.rep_map.p_data                 = report_map_data;
    hidsInitObject.hid_information.bcd_hid        = BASE_USB_HID_SPEC_VERSION;
    hidsInitObject.hid_information.b_country_code = 0;
    hidsInitObject.hid_information.flags          = hidInfoFlags;
    hidsInitObject.included_services_count        = 0;
    hidsInitObject.p_included_services_array      = NULL;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.rep_map.security_mode.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hidsInitObject.rep_map.security_mode.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.hid_information.security_mode.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hidsInitObject.hid_information.security_mode.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(
        &hidsInitObject.security_mode_boot_kb_inp_rep.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_boot_kb_inp_rep.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hidsInitObject.security_mode_boot_kb_inp_rep.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_boot_kb_outp_rep.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_boot_kb_outp_rep.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_protocol.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_protocol.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hidsInitObject.security_mode_ctrl_point.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&hidsInitObject.security_mode_ctrl_point.write_perm);

    errorCode = ble_hids_init(&g_hids, &hidsInitObject);
    APP_ERROR_CHECK(errorCode);
}

static void handleHidEvent(ble_hids_t * p_hids, ble_hids_evt_t *p_evt)
{
    switch (p_evt->evt_type)
    {
        case BLE_HIDS_EVT_BOOT_MODE_ENTERED:
            g_inBootMode = true;
            break;

        case BLE_HIDS_EVT_REPORT_MODE_ENTERED:
            g_inBootMode = false;
            break;

        case BLE_HIDS_EVT_REP_CHAR_WRITE:
            handleHidOutputReportWrite(p_evt);
            break;

        case BLE_HIDS_EVT_NOTIF_ENABLED:
        {
            dm_service_context_t   service_context;
            service_context.service_type = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;
            service_context.context_data.len = 0;
            service_context.context_data.p_data = NULL;

            if (g_inBootMode)
            {
                // Protocol mode is Boot Protocol mode.
                if (
                    p_evt->params.notification.char_id.uuid
                    ==
                    BLE_UUID_BOOT_KEYBOARD_INPUT_REPORT_CHAR
                )
                {
                    // The notification of boot keyboard input report has been enabled.
                    // Save the system attribute (CCCD) information into the flash.
                    uint32_t errorCode;

                    errorCode = dm_service_context_set(&g_bondedPeerHandle, &service_context);
                    if (errorCode != NRF_ERROR_INVALID_STATE)
                    {
                        APP_ERROR_CHECK(errorCode);
                    }
                    else
                    {
                        // The system attributes could not be written to the flash because
                        // the connected central is not a new central. The system attributes
                        // will only be written to flash only when disconnected from this central.
                        // Do nothing now.
                    }
                }
                else
                {
                    // Do nothing.
                }
            }
            else if (p_evt->params.notification.char_id.rep_type == BLE_HIDS_REP_TYPE_INPUT)
            {
                // The protocol mode is Report Protocol mode. And the CCCD for the input report
                // is changed. It is now time to store all the CCCD information (system
                // attributes) into the flash.
                uint32_t errorCode;

                errorCode = dm_service_context_set(&g_bondedPeerHandle, &service_context);
                if (errorCode != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(errorCode);
                }
                else
                {
                    // The system attributes could not be written to the flash because
                    // the connected central is not a new central. The system attributes
                    // will only be written to flash only when disconnected from this central.
                    // Do nothing now.
                }
            }
            else
            {
                // The notification of the report that was enabled by the central is not interesting
                // to this application. So do nothing.
            }
            break;
        }

        default:
            // No implementation needed.
            break;
    }
}

static void handleHidOutputReportWrite(ble_hids_evt_t *p_evt)
{
    if (p_evt->params.char_write.char_id.rep_type == BLE_HIDS_REP_TYPE_OUTPUT)
    {
        uint8_t  report_val = 0x00;
        uint8_t  report_index = p_evt->params.char_write.char_id.rep_index;

        if (report_index == OUTPUT_REPORT_INDEX && p_evt->params.char_write.len == 1)
        {
            report_val = *p_evt->params.char_write.data;
            updateKeyboardLeds(report_val);
        }
    }
}

static void updateKeyboardLeds(uint8_t hidLedBitmask)
{
    // Note: We ignore scroll lock since its LED is used for BLE connection state instead.
    nrf_gpio_pin_write(NUM_LOCK_PIN, hidLedBitmask & HID_OUTPUT_REPORT_LED_NUM_LOCK);
    nrf_gpio_pin_write(CAPS_LOCK_PIN, hidLedBitmask & HID_OUTPUT_REPORT_LED_CAPS_LOCK);
}

static void handleServiceError(uint32_t errorCode)
{
    APP_ERROR_HANDLER(errorCode);
}

// UNDONE: Shouldn't need this once we are really measuring the battery level.
static void initSensorSimulator(void)
{
    g_batterySimConfig.min          = MIN_BATTERY_LEVEL;
    g_batterySimConfig.max          = MAX_BATTERY_LEVEL;
    g_batterySimConfig.incr         = BATTERY_LEVEL_INCREMENT;
    g_batterySimConfig.start_at_max = true;

    sensorsim_init(&g_batterySimState, &g_batterySimConfig);
}

static void initConnectionParams(void)
{
    uint32_t               errorCode;
    ble_conn_params_init_t connectionParamInit;

    memset(&connectionParamInit, 0, sizeof(connectionParamInit));

    connectionParamInit.p_conn_params                  = NULL;
    connectionParamInit.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    connectionParamInit.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    connectionParamInit.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    connectionParamInit.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    connectionParamInit.disconnect_on_fail             = false;
    connectionParamInit.evt_handler                    = NULL;
    connectionParamInit.error_handler                  = handleConnectionParamsError;

    errorCode = ble_conn_params_init(&connectionParamInit);
    APP_ERROR_CHECK(errorCode);
}

static void handleConnectionParamsError(uint32_t errorCode)
{
    APP_ERROR_HANDLER(errorCode);
}

static void startRtcTimers(void)
{
    uint32_t errorCode;

    errorCode = app_timer_start(g_batteryTimerId, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
    APP_ERROR_CHECK(errorCode);
}

static void sendInputReportHandler(HidKeyboardInputReport* pReportToSend, void* pvContext, bool sleepPressed)
{
    uint32_t errorCode;

    if (sleepPressed)
    {
        enterSleepMode();
    }
    
    if (DEBUG_SHOW_HID_REPORT)
    {
        printf("{%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X}\n",
               pReportToSend->modifierBitmask,
               pReportToSend->reserved,
               pReportToSend->keyArray[0],
               pReportToSend->keyArray[1],
               pReportToSend->keyArray[2],
               pReportToSend->keyArray[3],
               pReportToSend->keyArray[4],
               pReportToSend->keyArray[5]);
    }
    
    if (g_connHandle == BLE_CONN_HANDLE_INVALID)
    {
        // Nothing to do if there is no BLE connection.
        return;
    }

    if (!g_inBootMode)
    {
        errorCode = ble_hids_inp_rep_send(&g_hids,
                                          INPUT_REPORT_KEYS_INDEX,
                                          sizeof(*pReportToSend),
                                          (uint8_t*)pReportToSend);
    }
    else
    {
        errorCode = ble_hids_boot_kb_inp_rep_send(&g_hids,
                                                  sizeof(*pReportToSend),
                                                  (uint8_t*)pReportToSend);
    }
    APP_ERROR_CHECK(errorCode);
}

static void enterLowPowerModeAndWakeOnEvent(void)
{
    uint32_t errorCode = sd_app_evt_wait();
    APP_ERROR_CHECK(errorCode);
}



// This function will be called in case of an assert in the SoftDevice.
// NOTE: The name of this function is set by the Nordic SDK.
void assert_nrf_callback(uint16_t lineNumber, const uint8_t * pFilename)
{
    app_error_handler(DEAD_BEEF, lineNumber, pFilename);
}

// Function for error handling, which is called when an error has occurred.
// NOTE: The name of this function is set by the Nordic SDK.
void app_error_handler(uint32_t errorCode, uint32_t lineNumber, const uint8_t* pFileName)
{
    // Break into the debugger on error so that the cause can be debugged.
    __asm volatile ("bkpt #0");
}
