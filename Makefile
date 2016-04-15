# Makefile for Tiva-C (TM4C123) boards
# Licensed under MIT License
# Copyright 2016 Akkenoth
# [insert-MIT-here]

################
# Additional functions
################

# Recursive wildcard function, as seen here: http://stackoverflow.com/a/12959764 . Don't remove or change, it works.
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

################
# Main configuration variables - change these to match your system, directory structure etc!
################

# Binary file name. ".bin" extension will be added automatically.
BIN_NAME=tiva
# Output directory name.
OUT_DIR=Out
# TivaWare directory (one containing "driverlib" and "boards" directories etc) path
TIVAWARE_DIR=/home/akkenoth/documents/micromouse/tivaware
# OpenOCD board config file, used for flashing ('make flash')
OCD_BOARD_CONFIG=/usr/share/openocd/scripts/board/ek-tm4c123gxl.cfg
# OpenOCD flash script, executed during 'make flash'
OCD_RUN_SCRIPT=tiva_ocd_script.cfg
# Additional dependencies to link against
ADDITIONAL_DEPS=

# Uncomment to enable debug
# DEBUG=1

# Project source files. Modify accordingly.
# Provided example assumes only main.c in main directory and all other source files in subdirectories. Common sense.
SRC=$(call rwildcard,*/,*.c) main.c
# TI's utils used in project (include "utils/..."). Will be hot-softlinked and added to SRC automagically.
TIVA_UTILS_LIBS=uartstdio.c ustdlib.c

# Binaries config - change only if using another compiler collection (e.g. arm-stellaris-eabi-*).
CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

################
### End of configuration variables.
################
### Below be dragons and Makefile magic.
### Don't wander there unless you're brave (or stupid).
################

# Compiler flags - based on official makefiles. You most likely don't need to change these (besides standard and warning ones)
CFLAGS= -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -ffunction-sections -fdata-sections -DPART_TM4C123GH6PM
CFLAGS+= -I${TIVAWARE_DIR}
CFLAGS+= -std=c99 -Wall
ifdef DEBUG
CFLAGS+=-g -D DEBUG -O0
else
CFLAGS+=-Os
endif

# Assembly compiler flags. Compiler flags comment applies here too.
AFLAGS=-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MD
AFLAGS+=" -I${TIVAWARE_DIR}"

# Linker flags. Compiler flags comment applies here too.
LDFLAGS=--gc-sections

# Additional source files. Don't change unless REALLY sure about it.
SRC+= $(addprefix utils/,${TIVA_UTILS_LIBS})
SRC+= startup_gcc.c

# Loader file. Don't change unless REALLY sure about it.
LDFILE=${TIVAWARE_DIR}/boards/ek-tm4c123gxl/hello/hello.ld

# Object and dependencies files generation. Don't change, it works.
OBJECTS=$(addprefix ${OUT_DIR}/,${SRC:.c=.o})
DEPS=${OBJECTS:.o=.d}

# Additional libraries location probing. Don't change unless REALLY sure about it.
LIBGCC:=$(shell ${CC} ${CFLAGS} -print-libgcc-file-name)
LIBC:=$(shell ${CC} ${CFLAGS} -print-file-name=libc.a)
LIBM:=$(shell ${CC} ${CFLAGS} -print-file-name=libm.a)
LIBDRIVER=${TIVAWARE_DIR}/driverlib/gcc/libdriver.a

################
### Build targets
################

# Main target - build main project binary
all: ${OUT_DIR}/${BIN_NAME}.bin

${OUT_DIR}/${BIN_NAME}.bin: ${OBJECTS} ${DEPS} ${ADDITIONAL_DEPS}
	@mkdir -p ${@D}
	@echo " [LD] ${@}"
	@${LD} -T ${LDFILE} --entry ResetISR ${LDFLAGS} -o ${OUT_DIR}/${BIN_NAME}.axf $(filter %.o %.a, ${^}) '${LIBM}' '${LIBC}' '${LIBGCC}' '${LIBDRIVER}'
	@echo " [OC] ${@}"
	@${OBJCOPY} -O binary ${OUT_DIR}/${BIN_NAME}.axf ${OUT_DIR}/${BIN_NAME}.bin
	@echo "Output binary file: ${@}"

${OUT_DIR}/%.a:
	@mkdir -p ${@D}
	@echo " [AR] ${@}"
	@${AR} -cr ${@} ${^}

${OUT_DIR}/%.o: %.c ${OUT_DIR}/%.d
	@echo " [CC] ${@}"
	@mkdir -p ${@D}
	@${CC} ${CFLAGS} -c -Dgcc ${<} -o ${@}

${OUT_DIR}/%.o: %.S
	@echo " [CC] ${@}"
	@mkdir -p ${@D}
	@${CC} ${AFLAGS} -c -Dgcc ${<} -o ${@}

${OUT_DIR}/%.d: %.c
	@echo " [DD] ${@}"
	@mkdir -p ${@D}
	@echo -n ${@D} > ${@}
	@echo -n "/" >> ${@}
	@${CC} -MM ${CFLAGS} ${<} >> ${@}

utils/%.c: prepare

flash: ${OUT_DIR}/${BIN_NAME}.bin
	openocd -f ${OCD_BOARD_CONFIG} -f ${OCD_RUN_SCRIPT} -c "tiva_flash ${OUT_DIR}/${BIN_NAME}.bin" -c shutdown

clean:
	@rm -vRf ${OUT_DIR}/*
	@rm -vf startup_gcc.c
	@find utils -maxdepth 1 -type l -exec rm -vf {} \;

# Only include deps if make goal is different than 'clean' and 'prepare'
ifeq (,$(filter clean ,${MAKECMDGOALS}))
# Dirty hack for dynamic softlinking
HACK:=$(shell ln -sf ${TIVAWARE_DIR}/boards/ek-tm4c123gxl/hello/startup_gcc.c ./startup_gcc.c)
ifneq (,${TIVA_UTILS_LIBS})
HACK:=$(shell mkdir -p utils)
endif
HACK:=$(shell for FILE in ${TIVA_UTILS_LIBS}; do echo " [LN] $$FILE"; ln -sf ${TIVAWARE_DIR}/utils/$$FILE ./utils/$$FILE; done)
-include ${DEPS}
endif

.PHONY: all clean flash
