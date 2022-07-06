#ifndef _SERIAL_COMM_H_
#define _SERIAL_COMM_H_
struct Pack{
    String command;
    String arg1;
    String arg2;
    String arg3;
    String arg4;
}typedef Pack;

Pack ReadPack();
void SendPack(Pack);
void SendPack(String,String,String,String,String);
Pack EmptyPack();

#endif