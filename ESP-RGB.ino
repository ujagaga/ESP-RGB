/*
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *
 *  ESP8266 (ESP-01) WS2811 RGB lamp, controlled over HTTP.
 */

#include "wifi_connection.h"
#include "http_server.h"
#include "led.h"
#include "ota.h"
#include "config.h"

void setup(void) {
  /* Need to wait for background processes to complete. Otherwise trouble with gpio.*/
  delay(100);
#if LED_DATA_PIN != 1
  Serial.begin(115200);   // GPIO1 is TX; with the strip on it there is no serial log.
#endif

  while(!WIFIC_initStorage()){
    Serial.println("EEPROM init failed; retrying before loading settings.");
    delay(1000);
  }
  LED_init();
  OTA_init();
  WIFIC_init();
  HTTP_SERVER_init();
}

void loop(void) {
  HTTP_SERVER_process();
  WIFIC_process();
  OTA_process();
  LED_process();
}
