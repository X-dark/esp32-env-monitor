# esp32-env-monitor

Air quality and other environment data monitoring for an ESP32 board

## Hardware Used

* Board: Espressif ESP32
  * [Espressif chip page](https://www.espressif.com/en/esp-wroom-32/resources)
  * [DOIT ESP32 page](https://docs.zerynth.com/latest/official/board.zerynth.doit_esp32/docs/index.html) (I am using this version)
  * [Useful Pin detailling page](https://www.learnarduinoraspberrypi.com/2018/08/doit-esp32-devkit-getting-started-programming.html)
* Display: Waveshare 1.5 Oled Display (based on a SDD1327 chip)
* Temperature, pressure and humidity sensor: BME280
  * [Data Sheet](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf)
* Air Quality Sensor: SGP30
  * [Data Sheet](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/0_Datasheets/Gas/Sensirion_Gas_Sensors_SGP30_Datasheet.pdf)

## Compiling and Uploading

A Makefile is provided to compile and upload to the board. This Makefile uses:

* [arduino-cli](https://github.com/arduino/arduino-cli) for compilation and uploading
