/*
 * NEOPIXEL.h
 *
 *  Created on: May 10, 2026
 *      Author: itzle
 */

#ifndef INC_NEOPIXEL_H_
#define INC_NEOPIXEL_H_

#include "main.h"

//NUMERO DE LEDS
#define numPixels 8

//VALORES DE ANCHO DE PULSO DEL UNO Y EL CERO
#define CCR_0 34
#define CCR_1 67

//TIMER
extern TIM_HandleTypeDef htim1;
#define neoPixel_timer htim1
#define neoPixel_canal TIM_CHANNEL_4

//FUNCIONES
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void setBrightness(uint8_t b);
void pixelShow(void);
void pixelClear(void);

#endif /* INC_NEOPIXEL_H_ */
