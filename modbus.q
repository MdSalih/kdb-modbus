system"l kdb_modbus.q";

.modbus.connTcp[`$"192.168.1.185";502i];
.modbus.setSlave[11i];
show `$"c"$.modbus.readRegisters[31000i;10i]

