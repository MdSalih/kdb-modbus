system"l kdb_modbus.q";

ATHROW:{[func;args;expect;msg]
  res:.[func;args;::];
  ok:res like expect;
  -1 out:$[ok;"PASSED";"!!! FAILED !!!"]," ",msg," - expected: ",.Q.s1[expect]," got: ",.Q.s1[res];
  if[not ok;'out];
  };

ATHROW[.modbus.close;1#(::);"Not connected";"invoke close when not connected throws error"];
ATHROW[.modbus.connTcp[;502i];enlist"127.0.0.1";"type*";"connect tcp called with symbol address throws type error"];
ATHROW[.modbus.connTcp[`$"127.0.0.1"];enlist 502;"type*";"connect tcp called with long type port throws type error"];
ATHROW[.modbus.connTcp[`$"127.0.0.1"];enlist 502i;"unable to connect*";"invalid modbus endpoint"];
ATHROW[.modbus.setSlave;enlist 1;"Not connected";"set slave when no connection throws error"];
/ATHROW[.modbus.setSlave;enlist 255;"type*";"set slave called with non int slave addr throws type error"]; / TODO: re-enable once we can mock modbus endpoint
ATHROW[.modbus.readRegisters[0i];enlist 1i;"Not connected";"read registers when no connection throws error"];

exit 0;
