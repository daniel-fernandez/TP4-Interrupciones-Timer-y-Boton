#include <stdint.h>
#include "bsp/bsp.h"

/**
 * @brief Delay por software
 *
 * @param nCount Numero de ciclos del delay
 */
void Delay(volatile uint32_t nCount);

/**
 * @brief Se encarga de prender un led y apagarlo luego de un tiempo
 *
 * @param led    Numero de led a pulsar
 * @param tiempo Numero de ciclos del delay entre prendido y apagado
 */
void pulsoLed(uint8_t led, uint32_t tiempo);

/**
 * @brief Aplicacion principal
 */
int main(void) {
	bsp_init();
	int brillo = 0;
	int flag = 0, sw_flag = 1, cant_leds, vin, temp=0;
	float acc_x, acc_y, acc_z, prender_x, prender_y, porcentaje;
	uint16_t valor_pote;
	while (1) {
	//	bsp_delayMs(30);
		/*
		 if (prender_x < 0) {
		 prender_x = prender_x * (-1);
		 brillo = prender_x;
		 led_setBright(0, brillo);
		 } else {
		 brillo = prender_x;
		 led_setBright(1, brillo);
		 }
		 if (prender_y < 0) {
		 prender_y = prender_y * (-1);
		 brillo = prender_y;
		 led_setBright(3, brillo);
		 } else {
		 brillo = prender_y;
		 led_setBright(2, brillo);
		 }*/
/*		led_setBright(0, brillo);
		led_setBright(1, brillo);
		led_setBright(2, brillo);
		led_setBright(3, brillo);
		if (brillo >= 100)
			flag = 0;
		if (brillo <= 0)
			flag = 1;
		if (flag)
			brillo++;
		else
			brillo--;

		acc_x = bsp_get_acc('x');
		acc_y = bsp_get_acc('y');

		prender_x = acc_x * 100;
		prender_y = acc_y * 100;*/

		valor_pote = bsp_get_pote();
		vin = valor_pote * 100/4095;

		cant_leds = vin / 12;
		if(cant_leds != temp){
			APP_POTE(cant_leds);
			temp = cant_leds;
		}
	}
}

void pulsoLed(uint8_t led, uint32_t tiempo) {
	led_on(led);
	Delay(tiempo);
	led_off(led);
}

void Delay(volatile uint32_t nCount) {
	while (nCount--) {
	}
}

void APP_ISR_sw(void) {

}

void APP_ISR_1ms(void) {
	static uint16_t count_1s = 1000;
	count_1s--;
	if (!count_1s) {
		led_toggle(0);
		count_1s = 1000;
	}
}

void APP_POTE(int cant_leds){
	int i;
	for(i=0; i<=cant_leds;i++){
		led_on(i);
	}
	for(i=cant_leds; i<8;i++){
		led_off(i);
	}

}
