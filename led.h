#ifndef LED_H
#define LED_H

#include <Arduino.h>

extern void LED_init(void);
extern void LED_process(void);
extern void LED_set(uint32_t rgb, uint8_t brightness);
extern uint32_t LED_getColor(void);
extern uint8_t LED_getBrightness(void);

#endif
