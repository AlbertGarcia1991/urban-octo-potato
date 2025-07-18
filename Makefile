.PHONY: build flash monitor clean all build_server build_client all_server all_client

DEBIAN_PORT := /dev/ttyUSB0
MAC_PORT := $(shell ls /dev/cu.usbserial* 2>/dev/null | head -n 1)

ifeq ($(shell uname),Darwin)
	CURR_PORT := $(MAC_PORT)
else
	CURR_PORT := $(DEBIAN_PORT)
endif

build:
	idf.py build

build_server:
	idf.py build -DBUILD_TARGET=SERVER

build_client:
	idf.py build -DBUILD_TARGET=CLIENT

flash:
	idf.py -p $(CURR_PORT) flash

monitor:
	idf.py -p $(CURR_PORT) monitor

clean:
	idf.py fullclean

all: build flash monitor

all_server: build_server flash monitor

all_client: build_client flash monitor