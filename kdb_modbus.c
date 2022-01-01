#include <stdio.h>
#include <errno.h>
#include <modbus.h>
#include "k.h"
#define ASSERT_MODBUS_CONNECTED_KRR() if(ctx==NULL) return krr("Not connected")
#define ASSERT_TYPE_KRR(V,T,M) if(V->t != T) return krr(M)

modbus_t *ctx = NULL;

K kdb_modbus_connect_tcp(K addr, K port){
  ASSERT_TYPE_KRR(addr, -KS, "type: addr must be a symbol");
  ASSERT_TYPE_KRR(port, -KI, "type: port mist be an int");
  if(ctx != NULL)
    return krr("already connected, close existing conn first");

  ctx = modbus_new_tcp(addr->s, port->i);
  P(ctx == NULL, orr("unable to allocate libmodbus context"));

  if(modbus_connect(ctx) == -1){
    modbus_free(ctx);
    ctx = NULL;
    return orr("unable to connect");
  };

  return (K)0;
};

K kdb_modbus_close(K x){
  ASSERT_MODBUS_CONNECTED_KRR();
  modbus_close(ctx);
  modbus_free(ctx);
  ctx = NULL;
  return (K)0;
};

K kdb_modbus_set_slave(K slaveAddr){
  ASSERT_MODBUS_CONNECTED_KRR();
  ASSERT_TYPE_KRR(slaveAddr, -KI, "type: slave address must be an int");
  P(modbus_set_slave(ctx, slaveAddr->i) == -1, orr("Unable to set slave address"));
  return (K)0;
}

K kdb_modbus_read_registers(K addr, K n){
  ASSERT_MODBUS_CONNECTED_KRR();
  ASSERT_TYPE_KRR(addr, -KI, "type: address must be an int");
  ASSERT_TYPE_KRR(n, -KI, "type: register count (n) must be an int");

  uint16_t data[n->i];
  int regCount,i;
  K res;

  regCount = modbus_read_registers(ctx, addr->i, n->i, data);
// TODO: fixup, not pretty, but convenient way for now to pass through modbus errors
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
  P(regCount == -1,krr(modbus_strerror(errno)));
#pragma GCC diagnostic pop

  res = ktn(KG, regCount*2);
  // copy bytes to K obj - iterating through 16bit vals, so set 2 bytes per iteration
  for(i = 0; i < regCount; i++){
    kG(res)[i*2] = MODBUS_GET_HIGH_BYTE(data[i]);
    kG(res)[1+i*2] = MODBUS_GET_LOW_BYTE(data[i]);
  }
  return res;
}
