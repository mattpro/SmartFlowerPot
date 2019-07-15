#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_temp.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_saadc.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "config.h"
#include "system_nrf52.h"
#include "ble_radio_notification.h"
#include "SEGGER_RTT.h"
#include "myRtc.h"
#include "myAdc.h"


#define CENTRAL_LINK_COUNT              0                                 /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           0                                 /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                 /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define APP_CFG_NON_CONN_ADV_TIMEOUT    0                                 /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(SEND_DATA_TIME_MS, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */


// Eddystone common data
#define APP_EDDYSTONE_UUID              0xFEAA                            /**< UUID for Eddystone beacons according to specification. */
#define APP_EDDYSTONE_RSSI              0xEE                              /**< 0xEE = -18 dB is the approximate signal strength at 0 m. */

// Eddystone TLM data
#define APP_EDDYSTONE_TLM_FRAME_TYPE    0x20                              /**< TLM frame type is fixed at 0x20. */
#define APP_EDDYSTONE_TLM_VERSION       0x00                              /**< TLM version might change in the future to accommodate other data according to specification. */
#define APP_EDDYSTONE_TLM_BATTERY       0x12, 0x34                        /**< Mock value. Battery voltage in 1 mV per bit. */
#define APP_EDDYSTONE_TLM_TEMPERATURE   0x0F, 0x00                        /**< Mock value. Temperature [C]. Signed 8.8 fixed-point notation. */
#define APP_EDDYSTONE_TLM_ADV_COUNT     0x00, 0x00, 0x00, 0x00            /**< Running count of advertisements of all types since power-up or reboot. */
#define APP_EDDYSTONE_TLM_SEC_COUNT     0x00, 0x00, 0x00, 0x00            /**< Running count in 0.1 s resolution since power-up or reboot. */

#define DEAD_BEEF                       0xDEADBEEF                        /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define FPU_EXCEPTION_MASK 0x0000009F



static uint8_t eddystone_tlm_data[] =   /**< Information advertised by the Eddystone TLM frame type. */
{
    APP_EDDYSTONE_TLM_FRAME_TYPE,   // Eddystone TLM frame type.
    APP_EDDYSTONE_TLM_VERSION,      // Eddystone TLM version.
    APP_EDDYSTONE_TLM_BATTERY,      // Battery voltage in mV/bit.
    APP_EDDYSTONE_TLM_TEMPERATURE,  // Temperature [C].
    APP_EDDYSTONE_TLM_ADV_COUNT,    // Number of advertisements since power-up or reboot.
    APP_EDDYSTONE_TLM_SEC_COUNT     // Time since power-up or reboot. 0.1 s increments.
};

static uint32_t adcVcc;
static uint32_t batteryVccMv;
static uint8_t  batteryVccPercent;

static int32_t temperaturaNrfTemp;
static int32_t temperaturaNrf;

static int16_t adcTemp;
static int16_t temperatura;

static int16_t adcSoli;
static int16_t soli;

static int32_t adcLight;
static int32_t light;



uint32_t advertisingCounter = 0;

ble_advdata_manuf_data_t manuf_specific_data;
ble_advdata_t advdata;
uint32_t licznik;
uint32_t licznikTempNrf;
volatile uint32_t processCounter;

uint8_array_t eddystone_data_array; 
ble_advdata_t advdata;
ble_advdata_service_data_t service_data; 
ble_uuid_t    adv_uuids[] = {{APP_EDDYSTONE_UUID, BLE_UUID_TYPE_BLE}};

static bool m_saadc_initialized = false;   

static uint32_t  m_adc_evt_counter = 0;

static ble_gap_adv_params_t m_adv_params;                                 /**< Parameters to be passed to the stack when starting advertising. */


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


void updataAdvData( void ) 
{
  eddystone_tlm_data[2] = (uint8_t)( temperatura >> 8 ); //data[13]//(uint8_t)(batteryVccPercent); 
  eddystone_tlm_data[3] = (uint8_t)( temperatura );
	
  eddystone_tlm_data[4] = (uint8_t)(temperaturaNrf >> 8 ); 
	eddystone_tlm_data[5] = (uint8_t)(temperaturaNrf );  
  
  eddystone_tlm_data[6] = (uint8_t)( 0x11 ); // 5
  eddystone_tlm_data[7] = (uint8_t)( 0x22 ); // 6
  eddystone_tlm_data[8] = (uint8_t)( soli );
  eddystone_tlm_data[9] = (uint8_t)( light );
	
  eddystone_tlm_data[10]  = (uint8_t)batteryVccPercent;
	eddystone_tlm_data[11]  = 0xEB;
	eddystone_tlm_data[12]  = 0xAA;
	eddystone_tlm_data[13]  = 0x9A;
  
	ble_advdata_set(&advdata, NULL); 
	
}

/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */

    
static void advertising_init(void)
{
    uint32_t      err_code;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
  
/** @snippet [Eddystone data array] */
    eddystone_data_array.p_data = (uint8_t *)eddystone_tlm_data;   // Pointer to the data to advertise.
    eddystone_data_array.size = sizeof(eddystone_tlm_data);         // Size of the data to advertise.
/** @snippet [Eddystone data array] */

    service_data.service_uuid = APP_EDDYSTONE_UUID;                 // Eddystone UUID to allow discoverability on iOS devices.
    service_data.data = eddystone_data_array;                       // Array for service advertisement data.

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.flags                   = flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;
    advdata.p_service_data_array    = &service_data;                // Pointer to Service Data structure.
    advdata.service_data_count      = 1;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_adv_params.p_peer_addr = NULL;                                // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = NON_CONNECTABLE_ADV_INTERVAL;
    m_adv_params.timeout     = APP_CFG_NON_CONN_ADV_TIMEOUT;
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);

    //err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;
		
		nrf_clock_lf_cfg_t clock_lf_cfg1 = {.source       = NRF_CLOCK_LF_SRC_XTAL,            
																			 .rc_ctiv       = 0,                                
																			 .rc_temp_ctiv  = 0,                                
																			 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM};
	

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg1, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
}



void processAllRawData( void )
{
	processCounter ++;
	// 1. Przeliczenia napiecia na baterii w mV i w procenty
	batteryVccMv = (uint32_t)( GAIN_VCC * adcVcc / MAX_12BIT * VREF * 1000.f );
	batteryVccPercent = (uint8_t) ( 100 - ( ( 3100.f - batteryVccMv ) / 10.f ) );
	if ( batteryVccPercent > 100 ) 
		{
		batteryVccPercent = 100;
		}
	// 2. Przeliczenie tempertaury nRF52 w 10*st C
	temperaturaNrf = (int16_t) ( 10.f * (float)temperaturaNrfTemp / 4.f );
	// 3. Przeliczenie temperatury MCP9700AT-E/TT * 10* st C
	temperatura = (int16_t)( GAIN_TEMP * adcTemp * VREF_MV / MAX_12BIT - 500.f ) ;
	// 4. Przeliczenie wilgotnosci
	soli = (uint8_t)( 100.f * (adcSoli )/ 2400.f );
	if ( soli > 100 )
	{
		soli = 100;
	}
	// 5. Przeliczenie oswietlenia
	light = (int32_t) ( ( 2850.f - (float)adcLight ) / 28.5f ) ;
	if ( light > 100 ) 
	{
		light = 100;
	}
	else if ( light < 0 ) 
	{
		light = 0;
	}
	
	// gdy ciemno adcLight = 2800 - 0%
	// gdy  jasno adcLight = 1900 - 100%
	SEGGER_RTT_printf(0, "Process number: %d \r\n", processCounter );	
	SEGGER_RTT_printf(0, "VCC      = %4dmV / %3d%% \r\n", batteryVccMv, batteryVccPercent );
	SEGGER_RTT_printf(0, "Temp nRF = %3d st C \r\n", temperaturaNrf );
	SEGGER_RTT_printf(0, "Temp     = %3d st C \r\n", temperatura );
	SEGGER_RTT_printf(0, "Light    = %3d %% \r\n", light );
	SEGGER_RTT_printf(0, "Soli     = %3d %% \r\n", soli );
	SEGGER_RTT_printf(0, "####### RAW DATA #######\r\n" );
	SEGGER_RTT_printf(0, "Adc VCC     = %5u \r\n", adcVcc );
	SEGGER_RTT_printf(0, "Adc Light   = %5u \r\n", adcLight );
	SEGGER_RTT_printf(0, "Adc Soli    = %5d  \r\n", adcSoli );
	SEGGER_RTT_printf(0, "Adc Temp    = %5u \r\n", adcTemp );
	SEGGER_RTT_printf(0, "Adc TempNrf = %5u \r\n \r\n", temperaturaNrfTemp );
}




// STEP 1 - Turn on all measurements and set RTC handler to stabilize 
void radioNotifyHandler(bool radio_active) 
{
	SEGGER_RTT_printf(0, "Radio Notify \r\n" );
	measureSystemOn();
	resetRtcCounter( STABILIZATION_TIME );
	PIN_SET(LED);
}


// STEP 2 - Start ADC Measurement from channel 0
void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    {    
			if(!m_saadc_initialized)
			{
					saadc_init();                           
			}
			m_saadc_initialized = true;  
			nrf_drv_saadc_sample();   
    }
}

// STEP 3 - Measure all channel and if its the last process all data, update adv data and put measurments off
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
	if (p_event->type == NRF_DRV_SAADC_EVT_DONE) 
	{
		nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAADC_SAMPLES_IN_BUFFER);
	  // put measurements system off
		measureSystemOff();
		
		adcSoli  = (uint32_t)( LP_CONST * (uint32_t)(p_event->data.done.p_buffer[0]) + ( ( 1.f - LP_CONST ) * adcSoli ) );
		adcVcc   = (uint32_t)( LP_CONST * (uint32_t)(p_event->data.done.p_buffer[1]) + ( ( 1.f - LP_CONST ) * adcVcc ) );
		adcLight = (uint32_t)( LP_CONST * (uint32_t)(p_event->data.done.p_buffer[2]) + ( ( 1.f - LP_CONST ) * adcLight ) );
		adcTemp  = (uint32_t)( LP_CONST * (uint32_t)(p_event->data.done.p_buffer[3]) + ( ( 1.f - LP_CONST ) * adcTemp ) );
		// start pomiaru TEMP NRF
		sd_temp_get(&temperaturaNrfTemp);
		// process all raw data
		processAllRawData();
		// update ADV data
		updataAdvData();	
		// Reset LED
		PIN_RESET(LED);			
		// 7. Deinicjalizacja SAADC
		nrf_drv_saadc_uninit();                                                                   //Unintialize SAADC to disable EasyDMA and save power
		NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);               //Disable the SAADC interrupt
		NVIC_ClearPendingIRQ(SAADC_IRQn);                                                         //Clear the SAADC interrupt if set
		m_saadc_initialized = false;     
	}
}




void FPU_IRQHandler(void)
{
    uint32_t *fpscr = (uint32_t *)(FPU->FPCAR+0x40);
    (void)__get_FPSCR();

    *fpscr = *fpscr & ~(FPU_EXCEPTION_MASK);
}

int main(void)
{
	SEGGER_RTT_printf(0, "\r\n >> Device START << \r\n " );
	SEGGER_RTT_printf(0, "### BlueFlowerPot ### \r\n" );	
	
	// 1. SYstem init
	SystemInit();
	// 2. DCDC on - to reduce power consuption
	NRF_POWER->DCDCEN = 1;  
	// 3. Patch for FPU 
	NVIC_SetPriority(FPU_IRQn, APP_IRQ_PRIORITY_LOW);
  NVIC_EnableIRQ(FPU_IRQn);
	// 4. Hardware init	
	gpioInit();
	measureSystemOff();
	// 5. RTC init
	lfclk_config();               
	rtcInit();      
	// 6. Bluetooth init
	ble_stack_init();
	ble_radio_notification_init(APP_IRQ_PRIORITY_LOW, NRF_RADIO_NOTIFICATION_DISTANCE_5500US, radioNotifyHandler);
	advertising_init();
	// 7. Set Tx Power to max - 4dBm
	sd_ble_gap_tx_power_set(RADIO_TXPOWER_TXPOWER_Pos4dBm);
	// 8. DCDC on - to reduce power consuption
	sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
	// 9. Advertising start
	advertising_start();
		
	while(1)
	{	
		sd_app_evt_wait();
	}
}



/**
 * @}
 */
