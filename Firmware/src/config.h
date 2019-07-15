#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"

#include "nrf_drv_gpiote.h"

// SETTINGS
#define SEND_DATA_TIME_MS						200 // w ms

#define STABILIZATION_TIME					15 		// w ms
#define BATTERY_MIN_MV      				2500 	// w mV

#define SAADC_SAMPLES_IN_BUFFER 		4 
#define SAADC_CALIBRATION_INTERVAL 	1000 

#define LP_CONST							      0.5f

// CONSTANS
#define MAX_12BIT 									4096.f
#define GAIN_VCC 										6.f    // odwrotnosc 1/6
#define GAIN_TEMP 									6.f
#define VREF 												0.6f
#define VREF_MV 										600.f



// HARDWARE PIN DEFINITION
#define LED_PIN_NUMBER          		26
// Piny pomiar swiatla
#define FOTO_ON_1_PIN_NUMBER				20
#define FOTO_ON_2_PIN_NUMBER				8
#define ADC_FOTO_PIN_NUMBER					4  // AIN2
// Piny pomiar temperatury
#define TEMP_3V3_ON_PIN_NUMBER  		7
#define ADC_TEMP_PIN_NUMBER     		2  // AIN0
// Piny pomiar wilgotnosci
#define SOIL_3V3_ON_PIN_NUMBER  		12
#define ADC_SOLI_PIN_NUMBER     		3  // AIN1


// OTHER
#define PIN_SET(pin)   	NRF_GPIO->OUTSET = ( 1 << pin##_PIN_NUMBER )
#define PIN_RESET(pin) 	NRF_GPIO->OUTCLR = ( 1 << pin##_PIN_NUMBER )
#define PIN_NEG(pin) 	 	NRF_GPIO->OUT   ^= ( 1 << pin##_PIN_NUMBER )
#define PIN_IS_SET(pin)	( NRF_GPIO->IN 	& ( 1 << pin##_PIN_NUMBER ) ) 


void gpioInit( void );
void measureSystemOff( void );
void measureSystemOn( void );


#endif
