.PHONY: build flash monitor clean all

build:
	idf.py build

flash:
	idf.py -p /dev/ttyUSB0 flash

monitor:
	idf.py -p /dev/ttyUSB0 monitor

clean:
	idf.py fullclean

all: build flash monitor