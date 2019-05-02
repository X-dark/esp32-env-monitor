#include <Arduino.h>

#include <WiFi.h>
#include "wifi_login.h"

#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include <BME280I2C.h>
#include <Wire.h>

U8X8_SSD1327_EA_W128128_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

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

  //Init BME280 sensor
  Wire.begin();

  while(!bme.begin())
  {
    u8x8log.print("Could not find BME280 sensor!\n");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       u8x8log.print("Found BME280 sensor! Success.\n");
       break;
     case BME280::ChipModel_BMP280:
       u8x8log.print("Found BMP280 sensor! No Humidity available.\n");
       break;
     default:
       u8x8log.print("Found UNKNOWN sensor! Error!\n");
  }

}

void loop() {

  //Clear screen
  u8x8log.print("\f");

  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  u8x8log.print("Temp: ");
  u8x8log.print(temp);
  u8x8log.print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
  u8x8log.print("\nHumidity: ");
  u8x8log.print(hum);
  u8x8log.print("% RH");
  u8x8log.print("\nPressure: ");
  u8x8log.print(pres);
  u8x8log.print("Pa\n");

  delay(10000);

}
