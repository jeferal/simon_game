OUT_NAME = project
OUT_DIR = ../build
CC = arm-linux-gnueabihf-g++-8
main = main.cpp
src_deps = analog_bbb/analog_input.cpp state_monitor/state_monitor.cpp state_monitor/thread_conf.cpp util/util.cpp analog_bbb/PWMuniv.cpp gpio_bbb/GPIO.cpp simon/simon_leds.cpp simon/simon_buttons.cpp
libs = -lpthread

MKDIR_P = mkdir -p

all: build/project

build/project:
	${MKDIR_P} ${OUT_DIR}
	${CC} ${main} ${src_deps} $(libs) -o ${OUT_DIR}/${OUT_NAME}

.PHONY: clean
clean:
	@rm -f ./build/project