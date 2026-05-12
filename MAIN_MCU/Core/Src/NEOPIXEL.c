/*
 * NEOPIXEL.c
 *
 *  Created on: May 10, 2026
 *      Author: itzle
 */


#include "NEOPIXEL.h"

uint8_t brightness = 50;
uint8_t ledData[numPixels][3];
uint16_t pwmData[24 * numPixels + 50];

void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    if (n < numPixels) {
        ledData[n][0] = g;
        ledData[n][1] = r;
        ledData[n][2] = b;
    }
}

void setBrightness(uint8_t b) {
    brightness = b;
}

void pixelShow(void) {
    uint32_t indx = 0;
    uint32_t color;

    for (int i = 0; i < numPixels; i++) {
        uint8_t g = (ledData[i][0] * brightness) / 255;
        uint8_t r = (ledData[i][1] * brightness) / 255;
        uint8_t b = (ledData[i][2] * brightness) / 255;
        color = (g << 16) | (r << 8) | b;

        for (int bit = 23; bit >= 0; bit--) {
            if (color & (1 << bit)) {
                pwmData[indx] = CCR_1;
            } else {
                pwmData[indx] = CCR_0;
            }
            indx++;
        }
    }

    //RETARDO PARA EL RESET
    for (int i = 0; i < 50; i++) {
        pwmData[indx] = 0;
        indx++;
    }

    //INICIAR TRANSMISION PWM DMA
    HAL_TIM_PWM_Start_DMA(&neoPixel_timer, neoPixel_canal, (uint32_t *)pwmData, indx);
}

void pixelClear(void) {
    for (int i = 0; i < numPixels; i++) {
        ledData[i][0] = 0;
        ledData[i][1] = 0;
        ledData[i][2] = 0;
    }
    pixelShow();
}
