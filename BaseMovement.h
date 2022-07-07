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

/* 驱动器参数 */
constexpr uint8_t sgthrs = 130;
constexpr uint8_t semin = 6;
constexpr uint8_t semax = 2;
constexpr uint16_t colthrs = 0x3FF;

/* 电机和车架参数 */
#define MOVING_CONST 0.0000209390195767 //位移常量 此常量为步进电机16微步下每Step移动的距离，单位为米
#define TURNING_CONST 0.120             //旋转常量 此常量为前后、左右轮距平均值的一半，单位为米
#define MAX_RELIABLE_SPEED 11000        //最大可靠步频，测量实验得到，因电机而异  超出该值电机扭矩不再符合要求
#define COIL_CURRENT_RMS 1600           //线圈电流目标，单位为毫安

#endif