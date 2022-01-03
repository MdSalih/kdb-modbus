.modbus.lib.connTcp:(`kdb_modbus 2:(`kdb_modbus_connect_tcp;2));
.modbus.lib.close:(`kdb_modbus 2:(`kdb_modbus_close;1));
.modbus.lib.setSlave:(`kdb_modbus 2:(`kdb_modbus_set_slave;1));
.modbus.lib.readRegisters:(`kdb_modbus 2:(`kdb_modbus_read_registers;2));

.modbus.readRegistersOnSlave:{[slave;addr;n]
  .modbus.lib.setSlave[`int$slave];
  :.modbus.lib.readRegisters[`int$addr;`int$n];
  };
