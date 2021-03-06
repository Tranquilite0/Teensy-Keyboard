# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# WinAVR Makefile Template written by Eric B. Weddington, J�rg Wunsch, et al.
#
# Released to the Public Domain
#
# Additional material for this makefile was written by:
# Peter Fleury
# Tim Henigan
# Colin O'Flynn
# Reiner Patommel
# Markus Pfaff
# Sander Pool
# Frederik Rouleau
# Carlos Lamas
#
#
# Fredrik Atmer then deleted most of the content for the Teensy-Keyboard
#
#----------------------------------------------------------------------------
# On command line:
#
# make = Make software.
#
# make clean = Clean out built project files.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# Keyboard type (with micro controller code and speed)
BOARD = phantom
MCU = atmega32u4
F_CPU = 16000000

# BOARD = iso
# MCU = at90usb1286
# F_CPU = 16000000

# BOARD = symmetric
# MCU = at90usb1286
# F_CPU = 16000000

# BOARD = pontus
# MCU = at90usb1286
# F_CPU = 16000000

# List C source files here.
SRC =	teensy_keyboard.c \
	usb_keyboard.c \
	$(BOARD).c \

# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL -D__INCLUDE_KEYBOARD=\"$(BOARD).h\"

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPTLEVEL = 2

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(CDEFS)
CFLAGS += -O$(OPTLEVEL)
CFLAGS += -ffunction-sections
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:%.c=%.lst)
CFLAGS += -std=gnu99

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=teensy_keyboard.map,--cref
LDFLAGS += -Wl,--relax
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -lm

# Define programs and commands.
SHELL = sh

# Define all object files.
OBJ = $(SRC:%.c=%.o)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)

# Change the build target to build a HEX file or a library.
build: teensy_keyboard.hex teensy_keyboard.eep end

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo Creating load file for Flash: $@
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.eep: %.elf
	@echo
	@echo Creating load file for EEPROM: $@
	-avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ || exit 0

# Create library from object files.
%.a: $(OBJ)
	@echo
	@echo Creating library: $@
	avr-ar rcs $@ $(OBJ)

# Link: create ELF output file from object files.
%.elf: $(OBJ)
	@echo
	@echo Linking: $@
	avr-gcc $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo Compiling C: $<
	avr-gcc -c $(ALL_CFLAGS) $< -o $@ 

end: 
	@echo
	rm -f *.map *.lst
	rm -rf .dep

clean:
	@echo
	@echo Cleaning everything:
	rm -f *.hex *.eep *.cof *.elf *.map *.sym *.lss *.o *.lst *.s *.d *.i
	rm -rf .dep

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY : build hex eep end clean
