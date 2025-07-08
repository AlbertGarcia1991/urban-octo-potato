.PHONY: build flash monitor clean all

DEBIAN_PORT := /dev/ttyUSB0
MAC_PORT := $(shell ls /dev/cu.usbserial* 2>/dev/null | head -n 1)

ifeq ($(shell uname),Darwin)
	CURR_PORT := $(MAC_PORT)
else
	CURR_PORT := $(DEBIAN_PORT)
endif

build:
	idf.py build

flash:
	idf.py -p $(CURR_PORT) flash

monitor:
	idf.py -p $(CURR_PORT) monitor

clean:
	idf.py fullclean

all: build flash monitor