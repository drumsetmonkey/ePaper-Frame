# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

BOARD=feather32u4

all:
	pio -f -c vim run -e $(BOARD)

upload:
	pio -f -c vim run --target upload -e $(BOARD)

clean:
	pio -f -c vim run --target clean

program:
	pio -f -c vim run --target program -e $(BOARD)

uploadfs:
	pio -f -c vim run --target uploadfs -e $(BOARD)

update:
	pio -f -c vim update

verbose:
	pio -f -c vim run -v -e $(BOARD)
