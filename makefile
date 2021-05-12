OUT_NAME = simon_machine
OUT_DIR = build
CC = arm-linux-gnueabihf-g++-8
main = src/main.cpp
src_deps = src/analog_bbb/analog_input.cpp src/state_monitor/state_monitor.cpp src/state_monitor/thread_conf.cpp src/util/util.cpp src/analog_bbb/PWMuniv.cpp src/gpio_bbb/GPIO.cpp src/simon/simon_interfaces/simon_leds.cpp src/simon/simon_interfaces/simon_buttons.cpp src/simon/simon_led_strip.cpp src/simon/simon_interfaces/simon_dial.cpp
libs = -lpthread

MKDIR_P = mkdir -p

#BBB Configuration
BBB_IP = 192.168.1.111
BBB_USR = root
BBB_TARGET_DIR = targets

all: build/project

build/project:
	${MKDIR_P} ${OUT_DIR}
	${CC} ${main} ${src_deps} $(libs) -o ${OUT_DIR}/${OUT_NAME}
	scp ${OUT_DIR}/${OUT_NAME} ${BBB_USR}@${BBB_IP}:/${BBB_USR}/${BBB_TARGET_DIR}/ 

.PHONY: clean
clean:
	@rm -f ./build/project