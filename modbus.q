system"l kdb_modbus.q";

.modbus.lib.connTcp[`$"192.168.1.185";502i];
show `$"c"$.modbus.readRegistersOnSlave[11i;31000i;10i]

