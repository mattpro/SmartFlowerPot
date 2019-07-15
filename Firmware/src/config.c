#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"

#include "config.h"




void measureSystemOff( void )
{
	// Zasilanie czujnika temperatury jako wejscie bez pullup/down
	nrf_gpio_cfg_input(TEMP_3V3_ON_PIN_NUMBER, GPIO_PIN_CNF_PULL_Disabled);
	// Masa i pullup czujnika swiatla jako wejscie bez pullup/down
	nrf_gpio_cfg_input(FOTO_ON_1_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(FOTO_ON_2_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
	// Zasilanie do elektrod pomiaru wilgotnosci jako wejscie bez pullup/down 
	nrf_gpio_cfg_input(SOIL_3V3_ON_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
	//nrf_gpio_cfg_output(SOIL_3V3_ON_PIN_NUMBER);
	//PIN_RESET(SOIL_3V3_ON);	
}


void measureSystemOn( void )
{
	// Zasilanie czujnika temperatury jako output w stanie wysokim
	nrf_gpio_cfg_output(TEMP_3V3_ON_PIN_NUMBER);
	PIN_SET(TEMP_3V3_ON);
	// Wlaczamy mase ( FOTO1 ) czujnika oraz pull up ( FOTO2 )
	nrf_gpio_cfg_output(FOTO_ON_1_PIN_NUMBER);
	PIN_RESET(FOTO_ON_1);
	nrf_gpio_cfg_output(FOTO_ON_2_PIN_NUMBER);
	PIN_SET(FOTO_ON_2);
	// Zasilanie do elektrod pomiaru wilgotnosci jako wejscie bez pullup/down 
	nrf_gpio_cfg_output(SOIL_3V3_ON_PIN_NUMBER);
	PIN_SET(SOIL_3V3_ON);	
}


void gpioInit( void )
{
	uint8_t i = 0;
	
	nrf_gpio_cfg_output( LED_PIN_NUMBER );
	//measureSystemOff();

	for ( i = 0 ; i < 20 ; i ++ )
  {
		PIN_NEG(LED);
		nrf_delay_ms(25);
  }
}





