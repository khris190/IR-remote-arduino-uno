#colors
C_GREEN = \033[92m
C_RESET = \033[0m


MAKEFILE := $(lastword $(MAKEFILE_LIST))
BASE_DIR = "$(realpath $(dir $(MAKEFILE)))"
APP_NAME = "$(shell basename '$(CURDIR)')"

AVRDUDE_PORT = /dev/ttyACM0
AVRDUDE_BAUD = 115200

USART_BAUDRATE = 38400
CPU_MHZ = 16
DF_CPU = $(CPU_MHZ)000000UL
OBJ_FLAGS = -Os -ffunction-sections -DUSART_BAUDRATE=$(USART_BAUDRATE) -DF_CPU=$(DF_CPU) -mmcu=atmega328p -c \
 	-I$(BASE_DIR)/src/include -o $(BASE_DIR)/tmp/$(APP_NAME).o $(BASE_DIR)/src/main.cpp

ASM_FLAGS = -S -fverbose-asm -Os $(BASE_DIR)/src/main.cpp

#-Wl,-gc-sections flags remove unused function during linking that was put into its own section by -ffunction-sections during compilation
EXE_FLAGS = -mmcu=atmega328p -Wl,-gc-sections $(BASE_DIR)/tmp/$(APP_NAME).o -o $(BASE_DIR)/target/$(APP_NAME)
HEX_FLAGS = -O ihex -R .eeprom $(BASE_DIR)/target/$(APP_NAME) $(BASE_DIR)/target/$(APP_NAME).hex
EEP_FLAGS =  -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma \
 	.eeprom=0  --no-change-warnings -O ihex $(BASE_DIR)/target/$(APP_NAME) $(BASE_DIR)/target/$(APP_NAME).eep


AVRDUDE_FLAGS = -F -V -c arduino -p ATMEGA328P -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -U
AVRDUDE_WRITE_FLASH = flash:w:$(BASE_DIR)/target/$(APP_NAME).hex
AVRDUDE_WRITE_EEPROM = eeprom:w:$(BASE_DIR)/target/$(APP_NAME).eep

hello:
	@echo $(BASE_DIR)

upload: build_hex build_eep
	@echo ""
	@echo -e "$(C_GREEN)uploading file to $(AVRDUDE_PORT)$(C_RESET)"
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

upload_hex: build_hex
	@echo ""
	@echo -e "$(C_GREEN)uploading hex file to $(AVRDUDE_PORT)$(C_RESET)"
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

upload_eep: build_eep
	@echo ""
	@echo -e "$(C_GREEN)uploading eep file to $(AVRDUDE_PORT)$(C_RESET)"
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)


build-size: build size

build: build_hex build_eep

build_eep: build_exe
	@echo ""
	@echo -e "$(C_GREEN)building eep file$(C_RESET)"
	avr-objcopy $(EEP_FLAGS)

build_hex: build_exe
	@echo ""
	@echo -e "$(C_GREEN)building hex file$(C_RESET)"
	avr-objcopy $(HEX_FLAGS)

build_exe: build_obj
	@echo ""
	@echo -e "$(C_GREEN)building executable file$(C_RESET)"
	avr-gcc $(EXE_FLAGS)

build_obj:
	@echo ""
	@echo -e "$(C_GREEN)building object files$(C_RESET)"
	avr-gcc $(OBJ_FLAGS)

clear: 
	@echo -e "$(C_GREEN)cleaning$(C_RESET)"
	rm -f $(BASE_DIR)/tmp/*
	rm -f $(BASE_DIR)/target/*

size:
	@avr-size $(BASE_DIR)/target/$(APP_NAME).eep
	@avr-size $(BASE_DIR)/target/$(APP_NAME).hex
	
asm:
	avr-gcc $(ASM_FLAGS)