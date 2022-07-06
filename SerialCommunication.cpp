#include <Arduino.h>
#include "SerialCommunication.h"

Pack ReadPack()
{
    Pack pack = EmptyPack();
    int p = 0;
    while (Serial2.available())
    {
        char c = Serial2.read();
        if (c == '\r')
            continue;
        if (c == '\n')
            break;
        if (c == '#')
        {
            p++;
            continue;
        }
        switch (p)
        {
        case 0:
            pack.command += c;
            break;
        case 1:
            pack.arg1 += c;
            break;
        case 2:
            pack.arg2 += c;
            break;
        case 3:
            pack.arg3 += c;
            break;
        case 4:
            pack.arg4 += c;
            break;
        }
        if (p >= 4)
            break;
        delay(2);
    }
    return pack;
}

void SendPack(Pack p)
{
    Serial2.println(p.command + "#" + p.arg1 + "#" + p.arg2 + "#" + p.arg3 + "#" + p.arg4);
}

void SendPack(String cmd, String a1, String a2, String a3, String a4)
{
    Serial2.println(cmd + "#" + a1 + "#" + a2 + "#" + a3 + "#" + a4);
}

Pack EmptyPack()
{
    Pack pack;
    pack.command = "";
    pack.arg1 = "";
    pack.arg2 = "";
    pack.arg3 = "";
    pack.arg4 = "";
    return pack;
}