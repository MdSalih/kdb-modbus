# Kdb+/q modbus lib

shared lib wrappers for [libmodbus](https://libmodbus.org/) - for now, only read registers implemented for TCP

### done

- connect to modbus tcp endpoint
- set modbus slave address
- read registers + get raw bytes to kdb

### todo

- data type conversions

# Example

```
$ make run
  gcc -DKXVER=3 -I/usr/include/modbus -g -Wall -shared -fPIC kdb_modbus.c -o kdb_modbus.so -L/usr/include/modbus -lmodbus
  rlwrap /home/salih/apps/kdb/current/l64/q modbus.q
  KDB+ 4.0 2021.07.12 Copyright (C) 1993-2021 Kx Systems
  l64/ ...

  `Cellar Sockets
```
See [modbus.q](modbus.q)


