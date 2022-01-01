#include <stdio.h>
#include <errno.h>
#include <modbus.h>
#include "k.h"

modbus_t *ctx = NULL;

K kdb_modbus_connect_tcp(K addr, K port){
  if(ctx != NULL)
    return krr("already connected, close existing conn first");

  if(addr->t != -KS)
    return krr("type: addr must be a symbol");

  if(port->t != -KI)
    return krr("type: port must be an int");

  ctx = modbus_new_tcp(addr->s, port->i);
  if(ctx == NULL)
    return orr("unable to allocate libmodbus context");

  if(modbus_connect(ctx) == -1){
    modbus_free(ctx);
    ctx = NULL;
    return orr("unable to connect");
  };

  return (K)0;
};

K kdb_modbus_close(K x){
  if(ctx == NULL)
    return krr("Not connected");

  modbus_close(ctx);
  modbus_free(ctx);
  ctx = NULL;

  return (K)0;
};

K kdb_modbus_set_slave(K slaveAddr){
  if(ctx == NULL)
    return krr("Not connected");

  if(slaveAddr->t != -KI)
    return krr("type: slave address must be an int");

  if(modbus_set_slave(ctx, slaveAddr->i) == -1)
    return orr("Unable to set slave address");

  return (K)0;
}

K kdb_modbus_read_registers(K addr, K n){
  if(ctx == NULL)
    return krr("Not connected");

  if(addr->t != -KI)
    return krr("type: address must be an int");

  if(n->t != -KI)
    return krr("type: register count (n) must be an int");

  uint16_t data[n->i];
  int regCount,i;
  K res;

  regCount = modbus_read_registers(ctx, addr->i, n->i, data);
  if(regCount == -1){
    // TODO: fixup, not pretty, but convenient way for now to pass through modbus errors
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    return krr(modbus_strerror(errno));
#pragma GCC diagnostic pop
  }

  res = ktn(KG, regCount*2);
  // copy bytes to K obj - iterating through 16bit vals, so set 2 bytes per iteration
  for(i = 0; i < regCount; i++){
    kG(res)[i*2] = MODBUS_GET_HIGH_BYTE(data[i]);
    kG(res)[1+i*2] = MODBUS_GET_LOW_BYTE(data[i]);
  }

  return res;
}
