#include <Arduino.h>

#include <WiFi.h>
#include "wifi_login.h"

#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8X8_SSD1327_EA_W128128_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// setup the terminal (U8X8LOG) and connect to u8g2 for automatic refresh of the display
// The size (width * height) depends on the display 

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 12
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;

void setup() {

  //Init Display  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char


  //Init wifi connection
  WiFi.begin(ssid, password);

  u8x8log.print("Connecting to SSID:\n");
  u8x8log.print(ssid);
  u8x8log.print("\n");
  u8x8log.print("\n");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    u8x8log.print(".");
  }

  u8x8log.print("\n");
  u8x8log.print("WiFi connected\n");
  u8x8log.print("IP address:\n");
  u8x8log.println(WiFi.localIP());
  u8x8log.print("\n");
  u8x8log.print("\n");

}

void loop() {

  //Clear screen
  u8x8log.print("\f");
  delay(10000);

}
