#include <Arduino.h>
#include <Wire.h>

#include <WiFi.h>
#include "wifi_login.h"

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include <BME280I2C.h>

#include <Adafruit_SGP30.h>

U8G2_SSD1327_WS_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 17, /* dc=*/ 3, /* reset=*/ 16);

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

Adafruit_SGP30 sgp;

// setup the terminal (U8G2LOG) and connect to u8g2 for automatic refresh of the display
// The size (width * height) depends on the display
// u8g2_font_t0_15_mf: BBX Width 8, Height 15
// 128/8 = 16
#define U8LOG_WIDTH 16
// 128/15 = 8
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8G2LOG u8g2log;

/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void setup() {

  //Init Display
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_15_mf);
  u8g2.enableUTF8Print();

  u8g2log.begin(U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);	// set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char


  //Init wifi connection
  WiFi.begin(ssid, password);

  u8g2log.print("Connecting to SSID:\n");
  u8g2log.print(ssid);
  u8g2log.print("\n");
  u8g2log.print("\n");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    u8g2log.print(".");
  }

  u8g2log.print("\n");
  u8g2log.print("WiFi connected\n");
  u8g2log.print("IP address:\n");
  u8g2log.println(WiFi.localIP());
  u8g2log.print("\n");
  u8g2log.print("\n");

  //Init BME280 sensor
  Wire.begin();

  while(!bme.begin())
  {
    u8g2log.print("Could not find BME280 sensor!\n");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       u8g2log.print("Found BME280 sensor! Success.\n");
       break;
     case BME280::ChipModel_BMP280:
       u8g2log.print("Found BMP280 sensor! No Humidity available.\n");
       break;
     default:
       u8g2log.print("Found UNKNOWN sensor! Error!\n");
  }


  //Init SGP30 sensor
  while(!sgp.begin())
  {
    u8g2log.print("Could not find SGP30 sensor!\n");
    delay(1000);
  }

  u8g2log.print("Found SGP30 serial #");
  u8g2log.print(sgp.serialnumber[0], HEX);
  u8g2log.print(sgp.serialnumber[1], HEX);
  u8g2log.print(sgp.serialnumber[2], HEX);
  u8g2log.print("\n");

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!


}

int counter = 0;
void loop() {

  //Clear screen
  u8g2log.print("\f");

  //Read Temperature, humidity and pressure
  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  //Read Air Quality
  sgp.setHumidity(getAbsoluteHumidity(temp, hum));

  if (! sgp.IAQmeasure()) {
    u8g2log.print("Air Quality Measurement failed\n");
    return;
  }

  u8g2log.print("Temp: ");
  u8g2log.print(temp);
  u8g2log.print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
  u8g2log.print("\nHumidity: ");
  u8g2log.print(hum);
  u8g2log.print("% RH");
  u8g2log.print("\nPressure: ");
  u8g2log.print(pres);
  u8g2log.print("Pa\n");

  u8g2log.print("TVOC ");
  u8g2log.print(sgp.TVOC);
  u8g2log.print(" ppb\n");
  u8g2log.print("eCO2 ");
  u8g2log.print(sgp.eCO2);
  u8g2log.print(" ppm\n");

  //Get Baseline readings every 30s
  counter++;
  if (counter == 30) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      u8g2log.print("Failed to get baseline readings\n");
      return;
    }
    u8g2log.print("Baseline values: eCO2: 0x");
    u8g2log.print(eCO2_base, HEX);
    u8g2log.print(" & TVOC: 0x");
    u8g2log.print(TVOC_base, HEX);
    u8g2log.print("\n");
  }

  delay(1000);

}
