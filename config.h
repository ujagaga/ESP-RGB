#ifndef CONFIG_H
#define CONFIG_H

/* Release: increase FIRMWARE_VERSION, run tools/build.sh, commit config.h together with build/ESP-RGB.ino.bin.
 * After each WiFi connection the device compares this number on GitHub and updates itself when it is higher. */
#define FIRMWARE_VERSION        1
#define OTA_GITHUB_REPOSITORY   "ujagaga/ESP-RGB"
#define OTA_GITHUB_BRANCH       "main"
#define OTA_FIRMWARE_PATH       "build/ESP-RGB.ino.bin"

#define PASSWORD                "rgblamp1"      // Config AP password (min 8 chars)
#define AP_NAME_PREFIX          "RGB_Lamp_"     // Will be appended by device MAC
#define HOSTNAME                "rgb-lamp"      // Default hostname until changed in Settings

#define AP_MODE_TIMEOUT_S       120 // While in AP only mode, retry STA connection after this period.
#define STA_CONNECT_TIMEOUT_S   30  // If STA does not connect within this period, fall back to AP only and retry after AP_MODE_TIMEOUT_S.

/* WS2811 strip data line. ESP-01: GPIO1 (TX) or GPIO2. With GPIO1 the serial log is disabled. */
#define LED_DATA_PIN            1
#define LED_COUNT               60
/* The previous firmware sent bytes in blue, red, green order at 400 kHz (WS2811 slow mode). */
#define LED_TYPE                (NEO_BRG + NEO_KHZ400)
#define LED_SAVE_DELAY_MS       3000  // Store colour this long after the last change, to spare flash.
#define LED_DEFAULT_COLOR       {255, 140, 40, 50}  // red, green, blue, brightness % (warm white)

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (64)  // WPA2 passphrase up to 63 chars + terminator
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (33)  // SSID up to 32 chars + terminator
#define OTA_TARGET_EEPROM_ADDR  (SSID_EEPROM_ADDR + SSID_SIZE)
#define OTA_TARGET_SIZE         (4)   // uint32_t version being installed
#define HOSTNAME_EEPROM_ADDR    (OTA_TARGET_EEPROM_ADDR + OTA_TARGET_SIZE)
#define HOSTNAME_SIZE           (32)  // 31 characters + terminator (ESP8266 hostname limit)
#define COLOR_EEPROM_ADDR       (HOSTNAME_EEPROM_ADDR + HOSTNAME_SIZE)
#define COLOR_SIZE              (4)   // red, green, blue, brightness %
#define LAYOUT_MARKER_ADDR      (COLOR_EEPROM_ADDR + COLOR_SIZE)
#define LAYOUT_MARKER_SIZE      (4)
#define EEPROM_SIZE             (LAYOUT_MARKER_ADDR + LAYOUT_MARKER_SIZE)

#endif
