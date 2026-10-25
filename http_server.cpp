/*
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *
 *  HTTP server: home page, settings page, lamp API and WiFi configuration.
 *
 *  GET /set?color=ff8800&brightness=50   - set colour (hex RRGGBB) and/or brightness (0-100 %)
 *  GET /update                           - check GitHub for newer firmware now
 */

#include <ESP8266WebServer.h>
#include <pgmspace.h>
#include "wifi_connection.h"
#include "led.h"
#include "ota.h"
#include "config.h"
#include "ui.h"

static ESP8266WebServer webServer(80);

/* Keep page assets in flash: concatenating the entire page can exhaust heap. */
static void sendFlash(PGM_P content){
  char chunk[512];
  size_t remaining = strlen_P(content);
  while(remaining){
    size_t count = remaining < sizeof(chunk) ? remaining : sizeof(chunk);
    memcpy_P(chunk, content, count);
    webServer.sendContent(chunk, count);
    content += count;
    remaining -= count;
  }
}

static void beginPage(void){
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.sendHeader("Cache-Control", "no-store");
  webServer.send(200, "text/html; charset=utf-8", "");
  sendFlash(HTML_BEGIN);
  webServer.sendContent(String(FPSTR(UI_TITLE_OPEN)) + WIFIC_getHostname() + String(FPSTR(UI_TITLE_CLOSE)));
  sendFlash(HTML_LAYOUT);
}

static void endPage(void){
  sendFlash(HTML_END);
  webServer.sendContent("");
}

static void sendPage(const String& body){
  beginPage();
  webServer.sendContent(body);
  endPage();
}

static String state(void){
  char text[12];
  snprintf(text, sizeof(text), "%06lx,%u", (unsigned long)LED_getColor(), LED_getBrightness());
  return String(text);
}

static void showStartPage(void){
  beginPage();
  sendFlash(INDEX_HTML);
  webServer.sendContent(String(FPSTR(UI_LAMP_SET)) + state() + String(FPSTR(UI_STRING_CALL_END)));
  endPage();
}

static void showSettings(void){
  String body = String(FPSTR(UI_HOST_STATE)) + WIFIC_getHostname() + String(FPSTR(UI_STRING_SCRIPT_END));
  beginPage();
  webServer.sendContent(body);
  sendFlash(SETTINGS_HTML);
  webServer.sendContent(String(FPSTR(UI_PARAGRAPH_OPEN)) + WIFIC_getStatus() + String(FPSTR(UI_BREAK)) + OTA_getStatus() + String(FPSTR(UI_SYSTEM_END)));
  endPage();
}

static void setLamp(void){
  uint32_t rgb = LED_getColor();
  int brightness = LED_getBrightness();
  if(webServer.hasArg("color")){
    String hex = webServer.arg("color");
    char* end;
    rgb = strtoul(hex.c_str(), &end, 16);
    if(hex.length() != 6 || *end){
      webServer.send(400, "text/plain", "color must be 6 hex digits (RRGGBB).");
      return;
    }
  }
  if(webServer.hasArg("brightness")){
    brightness = webServer.arg("brightness").toInt();
    if(brightness < 0 || brightness > 100){
      webServer.send(400, "text/plain", "brightness must be 0-100.");
      return;
    }
  }
  LED_set(rgb, brightness);
  webServer.send(200, "text/plain", state());
}

static void checkUpdate(void){
  if(OTA_requestCheck()){
    webServer.send(200, "text/plain", "Update check started. Reload the page later for status.");
  }else{
    webServer.send(409, "text/plain", "Not connected to WiFi or check already in progress.");
  }
}

static void showNotFound(void){
  webServer.send(404, "text/plain", "Not Found");
}

static void selectAP(void){
  beginPage();
  sendFlash(APLIST_HTML);
  endPage();
}

static void apList(void){
  webServer.send(200, "text/plain", WIFIC_getApList());
}

static void saveWiFi(void){
  String ssid = webServer.arg("s");
  String pass = webServer.arg("p");

  if((ssid.length() >= SSID_SIZE) || (pass.length() >= WIFI_PASS_SIZE)){
    sendPage(String(FPSTR(UI_WIFI_INVALID)));
    return;
  }

  WIFIC_setStSSID(ssid);
  WIFIC_setStPass(pass);

  String msg;
  if(ssid.length() > 0){
    msg = String(FPSTR(UI_WIFI_CONNECTING)) + ssid;
    msg += String(FPSTR(UI_WIFI_LINK)) + WIFIC_getHostname() + String(FPSTR(UI_LOCAL_LINK_TEXT)) + WIFIC_getHostname() + String(FPSTR(UI_WIFI_LINK_END));
  }else{
    msg = String(FPSTR(UI_WIFI_AP));
  }
  sendPage(String(FPSTR(UI_CONNECTION_TITLE)) + msg + String(FPSTR(UI_PARAGRAPH_CLOSE)));

  /* Let the response go out, then restart to apply.
   Note: ESP.restart may hang the first time after serial programming. Reset once manually after programming. */
  delay(1000);
  ESP.restart();
}

static void saveHostname(void){
  if(!webServer.hasArg("hostname")){
    webServer.send(400, "text/plain", "Missing hostname.");
    return;
  }
  String name = webServer.arg("hostname");
  name.trim();
  name.toLowerCase();
  if(!name.isEmpty() && !WIFIC_validHostname(name)){
    sendPage(String(FPSTR(UI_HOST_INVALID)));
    return;
  }
  if(!WIFIC_saveHostname(name)){
    webServer.send(500, "text/plain", "Could not save hostname. Please try again.");
    return;
  }
  if(name.isEmpty()) name = HOSTNAME;
  sendPage(String(FPSTR(UI_HOST_SAVED)) + name + String(FPSTR(UI_LOCAL_LINK_TEXT)) + name + String(FPSTR(UI_HOST_LINK_END)));
  delay(1000);
  ESP.restart();
}

void HTTP_SERVER_process(void){
  webServer.handleClient();
}

static void sendIcon(PGM_P content){
  webServer.sendHeader("Cache-Control", "public, max-age=31536000, immutable");
  webServer.sendHeader("X-Content-Type-Options", "nosniff");
  webServer.setContentLength(strlen_P(content));
  webServer.send(200, "image/svg+xml", "");
  sendFlash(content);
}

static void handleNotFound(void){
  if(webServer.uri().startsWith("/icons/")){
    webServer.sendHeader("Cache-Control", "no-store");
    showNotFound();
    return;
  }
  showStartPage();
}

void HTTP_SERVER_init(void){
  for(const auto& icon : ICON_ASSETS){
    PGM_P content = icon.content;
    webServer.on(icon.url, HTTP_GET, [content](){ sendIcon(content); });
  }
  webServer.on("/", showStartPage);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/settings", showSettings);
  webServer.on("/hostname", HTTP_POST, saveHostname);
  webServer.on("/set", setLamp);
  webServer.on("/update", checkUpdate);
  webServer.on("/selectap", selectAP);
  webServer.on("/aplist", apList);
  webServer.on("/wifisave", saveWiFi);
  webServer.onNotFound(handleNotFound);

  webServer.begin();
}
