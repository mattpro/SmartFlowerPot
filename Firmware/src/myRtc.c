#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_error.h"
#include "nrf.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "myRtc.h"
#include "config.h"

extern void rtc_handler(nrf_drv_rtc_int_type_t int_type);

const  nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2); 


void resetRtcCounter( uint32_t time )
{
	// Jako time wpisujemy w przybizeniu wartosc w ms ( dokladnie time = 1024 -> t=1s )
	nrf_drv_rtc_cc_set(&rtc,0,time,true);       
	nrf_drv_rtc_counter_clear(&rtc);            
}

void lfclk_config(void)
{
	// Nalezy pamietac aby dobrze skofigurowac zegar w sdk_config.h
	ret_code_t err_code = nrf_drv_clock_init();                    
	APP_ERROR_CHECK(err_code);
	nrf_drv_clock_lfclk_request(NULL);
}

void rtcInit( void )
{
	uint32_t err_code;

	nrf_drv_rtc_config_t rtc_config = NRF_DRV_RTC_DEFAULT_CONFIG;
	rtc_config.prescaler = 32; // 32768/32 = 1024 Hz
	
	err_code = nrf_drv_rtc_init(&rtc, &rtc_config, rtc_handler);
	APP_ERROR_CHECK(err_code);
	
	nrf_drv_rtc_enable(&rtc); 
}
