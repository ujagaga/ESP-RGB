# ESP-RGB

WiFi RGB lamp on an ESP-01 (ESP8266, 1 MB flash) driving a 12 V WS2811 LED
strip. Colour and brightness are set from a web page or a plain HTTP GET
request. The lamp is reachable by hostname (`http://rgb-lamp.local`) and updates
its own firmware from this GitHub repository.

Replaces the `LED_lamp_RGB_Neopixel` sketch from OhanaCode-IOT, which only ran
its own access point at `192.168.1.1` and was updated with Arduino IDE OTA.

## Hardware

| ESP-01 pin | Connection |
|---|---|
| VCC, CH_EN, RESET | 3.3 V from a 12 V → 3.3 V DC/DC converter |
| GND | GND (common with the strip) |
| GPIO1 (TX) | Strip data in (`LED_DATA_PIN` in `config.h`) |

The old schematic shows the data line on GPIO2, the old code on GPIO1. If the
strip stays dark after flashing, set `LED_DATA_PIN` to `2` and release a new
version. With the strip on GPIO1 there is no serial log.

Strip settings in `config.h`: `LED_COUNT` (60), `LED_TYPE`
(`NEO_BRG + NEO_KHZ400`, the byte order and speed the old firmware used). If
red/green/blue come out swapped, change the `NEO_xxx` order.

## Building and flashing

Requires `arduino-cli` with the ESP8266 core 3.1.2. Install the core and
libraries (Adafruit NeoPixel, ESP_EEPROM) with:

```
tools/install_dependencies.sh
```

Then:

```
tools/build.sh        # builds into build/ (board esp8266:esp8266:generic, 1 MB, no filesystem)
tools/upload_usb.sh   # flashes build/ESP-RGB.ino.bin over /dev/ttyUSB0
```

After flashing over USB, reset the module once. The first software restart
after serial programming can hang otherwise.

### First update from the old firmware (no USB)

The old firmware accepts Arduino OTA after `/start_ota_update` is opened. Join
its access point, then:

```
tools/build.sh
curl http://192.168.1.1/start_ota_update
python3 ~/.arduino15/packages/esp8266/hardware/esp8266/3.1.2/tools/espota.py \
  -i 192.168.1.1 -p 8266 -a <old AP password> -f build/ESP-RGB.ino.bin
```

The new firmware does not read the old settings. It starts with a warm white
colour and its own access point (see below).

## WiFi setup

1. On first start (no WiFi saved) the lamp starts an access point
   `RGB_Lamp_<MAC>`, password `rgblamp1`.
2. Connect to it and open `http://192.168.4.1`.
3. Go to **Settings → Configure WiFi**, pick your network, enter the password
   and save. The lamp restarts and joins your network.
4. Open `http://rgb-lamp.local`, or the IP address your router assigned
   (shown on the Settings page). Some Android browsers don't resolve `.local`
   names. WiFi power saving is disabled on the lamp because in sleep it misses
   multicast mDNS queries, which made `.local` lookups unreliable. On Linux
   computers with WiFi power saving on, lookups can still fail; turn it off
   there too (NetworkManager: `wifi.powersave = 2`) or add the lamp to
   `/etc/hosts`.

Connection behaviour:

- With saved WiFi it connects at boot. The access point stays up until the
  connection works and nobody is connected to it.
- If the connection fails within 30 s, it falls back to the access point only
  and retries every 120 s.
- If the connection drops later, it reconnects the same way.

Saving an empty SSID switches to access point only mode.

**Hostname** (Settings → System): enter a name such as `bedroom`, then press
**Save hostname and restart**. Use 1–31 letters, numbers or hyphens, starting
and ending with a letter or number. Leave blank to restore `HOSTNAME` from
`config.h`.

## Web interface

- **Home:** a preview circle and three sliders: colour (hue), saturation
  (white to full colour) and brightness (0 = off). Changes are sent while
  dragging and are saved to EEPROM 3 s after the last change, so the lamp
  comes back with the same colour after a power cut.
- **Settings:** hostname, WiFi configuration, manual firmware update check,
  connection and firmware status.

## HTTP API

| Request | Action |
|---|---|
| `/set?color=ff8800&brightness=50` | Set colour (hex `RRGGBB`) and/or brightness (0–100 %). Returns `RRGGBB,brightness`. |
| `/update` | Check GitHub for a newer firmware now |

```
curl "http://rgb-lamp.local/set?color=ff0000&brightness=80"
curl "http://rgb-lamp.local/set?brightness=0"
```

## Firmware updates from GitHub

After each WiFi connection, and when **Check for update** is pressed, the
lamp:

1. Gets the latest commit of `ujagaga/ESP-RGB`, branch `main`.
2. Reads `FIRMWARE_VERSION` from `config.h` in that commit.
3. If that number is higher than the running version, downloads
   `build/ESP-RGB.ino.bin` from the same commit, installs it and restarts.

Failed checks are retried up to 3 times, one minute apart. If the lamp boots
and the expected version is not running, that version is marked invalid and
skipped, so it can't get stuck in an update loop.

HTTPS certificates are **not** verified, so anyone able to intercept the
lamp's internet traffic could serve it different firmware. The repository
must be public. The firmware must stay below about 500 KB to fit twice in the
1 MB flash.

To release a new version:

1. Increase `FIRMWARE_VERSION` in `config.h`.
2. Run `tools/build.sh`.
3. Commit `config.h` together with `build/ESP-RGB.ino.bin` (the only build
   output tracked by git) and push to `main`.

## Source files

| File | Purpose |
|---|---|
| `ESP-RGB.ino` | Setup and main loop |
| `config.h` | Pins, strip type, defaults, version, OTA source, EEPROM layout |
| `led.cpp/.h` | Strip output, brightness curve, stored colour |
| `http_server.cpp/.h` | HTTP API, routing and page streaming |
| `ui.h` | Static HTML, CSS, JavaScript and SVG assets |
| `wifi_connection.cpp/.h` | WiFi station/access point handling, mDNS, hostname |
| `ota.cpp/.h` | Firmware update from GitHub |
| `firmware_version.cpp/.h` | Parses `FIRMWARE_VERSION` from the downloaded `config.h` |
| `tools/` | Build, upload and dependency install scripts |
| `tests/check_web_icons.py` | Checks SVG asset hashes and page references |
