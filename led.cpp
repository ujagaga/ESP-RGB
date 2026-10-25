/*
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *
 *  WS2811 strip: one colour for all pixels plus brightness. The setting is
 *  restored at boot and saved to EEPROM a few seconds after the last change.
 */
#include <Adafruit_NeoPixel.h>
#include <ESP_EEPROM.h>
#include "config.h"
#include "led.h"

static Adafruit_NeoPixel strip(LED_COUNT, LED_DATA_PIN, LED_TYPE);
static uint8_t color[COLOR_SIZE];   // red, green, blue, brightness %
static bool unsaved = false;
static uint32_t changedTime = 0;

static void show(void){
  /* Brightness is perceived roughly logarithmically, so the slider uses a square curve. */
  uint32_t level = (uint32_t)color[3] * color[3] * 255 / 10000;
  if(color[3] && !level) level = 1;
  strip.fill(strip.Color(color[0] * level / 255, color[1] * level / 255, color[2] * level / 255));
  strip.show();
}

void LED_set(uint32_t rgb, uint8_t brightness){
  color[0] = rgb >> 16;
  color[1] = rgb >> 8;
  color[2] = rgb;
  color[3] = brightness > 100 ? 100 : brightness;
  show();
  unsaved = true;
  changedTime = millis();
}

uint32_t LED_getColor(void){
  return ((uint32_t)color[0] << 16) | ((uint32_t)color[1] << 8) | color[2];
}

uint8_t LED_getBrightness(void){
  return color[3];
}

void LED_init(void){
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(COLOR_EEPROM_ADDR, color);
  if(color[3] > 100) color[3] = 100;
  strip.begin();
  show();
}

void LED_process(void){
  if(unsaved && (millis() - changedTime) > LED_SAVE_DELAY_MS){
    unsaved = false;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(COLOR_EEPROM_ADDR, color);
    EEPROM.commit();
  }
}
