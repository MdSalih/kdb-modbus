DEBUG?=0

TARGET = kdb_modbus

CC = gcc
CFLAGS = -DKXVER=3 -I/usr/include/modbus -g -Wall -shared -fPIC
LFLAGS = -L/usr/include/modbus -lmodbus

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET).so $(LFLAGS)

clean:
	rm $(TARGET).so

run: all
	rlwrap $(QHOME)/l64/q modbus.q

test: all
	rlwrap $(QHOME)/l64/q modbus_test.q -e $(DEBUG)
