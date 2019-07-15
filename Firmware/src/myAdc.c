#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf.h"
#include "app_util_platform.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_error.h"
#include "nrf.h"
#include "nrf_clock.h"
#include "nrf_drv_saadc.h"
#include "myAdc.h"
#include "config.h"

extern void saadc_callback(nrf_drv_saadc_evt_t const * p_event);

static nrf_saadc_value_t       m_buffer_pool[2][SAADC_SAMPLES_IN_BUFFER];

void saadc_init(void)
{
	// Nalezy pamietac ze jezeli robimy low power to burst i oversample off
	ret_code_t err_code;
	nrf_drv_saadc_config_t saadc_config;
  nrf_saadc_channel_config_t channel_config_SOLI;
  nrf_saadc_channel_config_t channel_config_VCC;
  nrf_saadc_channel_config_t channel_config_LIGHT;
  nrf_saadc_channel_config_t channel_config_TEMP;

	//Configure SAADC
	saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;              						//Set SAADC resolution to 12-bit. This will make the SAADC output values from 0 (when input voltage is 0V) to 2^12=2048 (when input voltage is 3.6V for channel gain setting of 1/6).
	saadc_config.oversample = NRF_SAADC_OVERSAMPLE_DISABLED;           						//Set oversample to 4x. This will make the SAADC output a single averaged value when the SAMPLE task is triggered 4 times.
	saadc_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;            						//Set SAADC interrupt to low priority.

	//Initialize SAADC
	err_code = nrf_drv_saadc_init(&saadc_config, saadc_callback);      						//Initialize the SAADC with configuration and callback function. The application must then implement the saadc_callback function, which will be called when SAADC interrupt is triggered
	APP_ERROR_CHECK(err_code);

	//Configure SAADC SOIL channel
	channel_config_SOLI.reference = NRF_SAADC_REFERENCE_INTERNAL;                              //Set internal reference of fixed 0.6 volts
	channel_config_SOLI.gain = NRF_SAADC_GAIN1_6;                                                //Set input gain to 1/6. The maximum SAADC input voltage is then 0.6V/(1/6)=3.6V. The single ended input range is then 0V-3.6V
	channel_config_SOLI.acq_time = NRF_SAADC_ACQTIME_40US;                                     //Set acquisition time. Set low acquisition time to enable maximum sampling frequency of 200kHz. Set high acquisition time to allow maximum source resistance up to 800 kohm, see the SAADC electrical specification in the PS. 
	channel_config_SOLI.mode = NRF_SAADC_MODE_SINGLE_ENDED;                                    //Set SAADC as single ended. This means it will only have the positive pin as input, and the negative pin is shorted to ground (0V) internally.
	channel_config_SOLI.pin_p = NRF_SAADC_INPUT_AIN1;                                          //Select the input pin for the channel. AIN0 pin maps to physical pin P0.02.
	channel_config_SOLI.pin_n = NRF_SAADC_INPUT_DISABLED;                                      //Since the SAADC is single ended, the negative pin is disabled. The negative pin is shorted to ground internally.
	channel_config_SOLI.resistor_p = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pullup resistor on the input pin
	channel_config_SOLI.resistor_n = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pulldown resistor on the input pin
	//Configure SAADC VCC channel
	channel_config_VCC.reference = NRF_SAADC_REFERENCE_INTERNAL;                              //Set internal reference of fixed 0.6 volts
	channel_config_VCC.gain = NRF_SAADC_GAIN1_6;                                              //Set input gain to 1/6. The maximum SAADC input voltage is then 0.6V/(1/6)=3.6V. The single ended input range is then 0V-3.6V
	channel_config_VCC.acq_time = NRF_SAADC_ACQTIME_10US;                                     //Set acquisition time. Set low acquisition time to enable maximum sampling frequency of 200kHz. Set high acquisition time to allow maximum source resistance up to 800 kohm, see the SAADC electrical specification in the PS. 
	channel_config_VCC.mode = NRF_SAADC_MODE_SINGLE_ENDED;                                    //Set SAADC as single ended. This means it will only have the positive pin as input, and the negative pin is shorted to ground (0V) internally.
	channel_config_VCC.pin_p = NRF_SAADC_INPUT_VDD;                                           //Select the input pin for the channel. AIN0 pin maps to physical pin P0.02.
	channel_config_VCC.pin_n = NRF_SAADC_INPUT_DISABLED;                                      //Since the SAADC is single ended, the negative pin is disabled. The negative pin is shorted to ground internally.
	channel_config_VCC.resistor_p = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pullup resistor on the input pin
	channel_config_VCC.resistor_n = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pulldown resistor on the input pin
	//Configure SAADC LIGHT channel
	channel_config_LIGHT.reference = NRF_SAADC_REFERENCE_INTERNAL;                              //Set internal reference of fixed 0.6 volts
	channel_config_LIGHT.gain = NRF_SAADC_GAIN1_6;                                              //Set input gain to 1/6. The maximum SAADC input voltage is then 0.6V/(1/6)=3.6V. The single ended input range is then 0V-3.6V
	channel_config_LIGHT.acq_time = NRF_SAADC_ACQTIME_10US;                                     //Set acquisition time. Set low acquisition time to enable maximum sampling frequency of 200kHz. Set high acquisition time to allow maximum source resistance up to 800 kohm, see the SAADC electrical specification in the PS. 
	channel_config_LIGHT.mode = NRF_SAADC_MODE_SINGLE_ENDED;                                    //Set SAADC as single ended. This means it will only have the positive pin as input, and the negative pin is shorted to ground (0V) internally.
	channel_config_LIGHT.pin_p = NRF_SAADC_INPUT_AIN2;                                          //Select the input pin for the channel. AIN0 pin maps to physical pin P0.02.
	channel_config_LIGHT.pin_n = NRF_SAADC_INPUT_DISABLED;                                      //Since the SAADC is single ended, the negative pin is disabled. The negative pin is shorted to ground internally.
	channel_config_LIGHT.resistor_p = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pullup resistor on the input pin
	channel_config_LIGHT.resistor_n = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pulldown resistor on the input pin
	//Configure SAADC TEMP channel
	channel_config_TEMP.reference = NRF_SAADC_REFERENCE_INTERNAL;                              //Set internal reference of fixed 0.6 volts
	channel_config_TEMP.gain = NRF_SAADC_GAIN1_6;                                              //Set input gain to 1/6. The maximum SAADC input voltage is then 0.6V/(1/6)=3.6V. The single ended input range is then 0V-3.6V
	channel_config_TEMP.acq_time = NRF_SAADC_ACQTIME_10US;                                     //Set acquisition time. Set low acquisition time to enable maximum sampling frequency of 200kHz. Set high acquisition time to allow maximum source resistance up to 800 kohm, see the SAADC electrical specification in the PS. 
	channel_config_TEMP.mode = NRF_SAADC_MODE_SINGLE_ENDED;                                    //Set SAADC as single ended. This means it will only have the positive pin as input, and the negative pin is shorted to ground (0V) internally.
	channel_config_TEMP.pin_p = NRF_SAADC_INPUT_AIN0;                                          //Select the input pin for the channel. AIN0 pin maps to physical pin P0.02.
	channel_config_TEMP.pin_n = NRF_SAADC_INPUT_DISABLED;                                      //Since the SAADC is single ended, the negative pin is disabled. The negative pin is shorted to ground internally.
	channel_config_TEMP.resistor_p = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pullup resistor on the input pin
	channel_config_TEMP.resistor_n = NRF_SAADC_RESISTOR_DISABLED;                              //Disable pulldown resistor on the input pin

	nrf_drv_saadc_channel_init(0, &channel_config_SOLI);    
	nrf_drv_saadc_channel_init(1, &channel_config_VCC);    
	nrf_drv_saadc_channel_init(2, &channel_config_LIGHT);    
	nrf_drv_saadc_channel_init(3, &channel_config_TEMP);   

	err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0],SAADC_SAMPLES_IN_BUFFER);    //Set SAADC buffer 1. The SAADC will start to write to this buffer
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1],SAADC_SAMPLES_IN_BUFFER);    //Set SAADC buffer 2. The SAADC will write to this buffer when buffer 1 is full. This will give the applicaiton time to process data in buffer 1.
	APP_ERROR_CHECK(err_code);
}

// Dodatkowe info:
// Samplowanie 3uS - 24,7 uA
// Samplowanie 40 us - 26,4 uA
