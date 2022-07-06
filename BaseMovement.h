#ifndef _BASE_MOVEMENT_H_
#define _BASE_MOVEMENT_H_
#include <TMCStepper.h>
char BMinitialize();
void driverEnable();
void driverDisable();
short inline cs(double);
void MoveAtVelocity(double, double,double);
void Break();
void Move(double, double,double,double);
void MoveAtVelocityUntil(double, double,double,double);

constexpr uint8_t EN_PIN   = 16;
constexpr uint8_t DIAG_PIN = 6;

constexpr uint8_t sgthrs = 130;
constexpr uint8_t semin = 6;
constexpr uint8_t semax = 2;

#endif