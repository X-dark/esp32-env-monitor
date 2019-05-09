FQBN	:= esp32:esp32:esp32
NAME	:= esp32-env-monitor
SOURCE 	:= $(addsuffix .ino, $(NAME)) wifi_login.h
TARGET	:= $(addsuffix .$(subst :,.,$(FQBN)).bin, $(NAME))
ELF	:= $(addsuffix .$(subst :,.,$(FQBN)).elf, $(NAME))
ESPTOOL	:= ~/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py
PORT	:= /dev/ttyUSB0
ADDRESS	:= 0x10000

.PHONY: compile clean upload

compile: $(TARGET)

clean:
	rm -f $(TARGET) $(ELF)

upload: $(TARGET)
	$(ESPTOOL) --port $(PORT) write_flash $(ADDRESS) $(TARGET)

$(TARGET): $(SOURCE)
	ARDUINO_SKETCHBOOK_DIR=$(CURDIR) arduino-cli compile -b $(FQBN)
