#include <Arduino.h>
#include <TMCStepper.h> // Branch: Release_v1

constexpr uint8_t EN_PIN   = 16;
constexpr uint8_t DIAG_PIN = 6;

constexpr uint8_t sgthrs = 130;
constexpr uint8_t semin = 6;
constexpr uint8_t semax = 2;

TMC2300Stepper driver(Serial1, 1.60, 0);
TMC2300Stepper driver2(Serial1, 1.60, 1);
TMC2300Stepper driver3(Serial1, 1.60, 2);
TMC2300Stepper driver4(Serial1, 1.60, 3);

void setup() {
  pinMode(EN_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);
  pinMode(12, OUTPUT);

  Serial2.begin(115200); // Init serial port and set baudrate
  while (!Serial2);      // Wait for serial port to initialize

  driver.begin(115200); // Set SW UART baudrate

  auto version = driver.version();
  while (version == 0) {
    Serial2.print("ERR#DriverCommError");
  }

  driver.GSTAT(0b111); // Reset error flags
  driver2.GSTAT(0b111); // Reset error flags
  driver3.GSTAT(0b111); // Reset error flags
  driver4.GSTAT(0b111); // Reset error flags

  driver.enable_drv(true);
  driver2.enable_drv(true);
  driver3.enable_drv(true);
  driver4.enable_drv(true);

  driver.rms_current(1600);
  driver2.rms_current(1600);
  driver3.rms_current(1600);
  driver4.rms_current(1600);
  driver.microsteps(16);
  driver2.microsteps(16);
  driver3.microsteps(16);
  driver4.microsteps(16);
  driver.TCOOLTHRS(0x3FF); // 10bit max
  driver2.TCOOLTHRS(0x3FF); // 10bit max
  driver3.TCOOLTHRS(0x3FF); // 10bit max
  driver4.TCOOLTHRS(0x3FF); // 10bit max

  driver.semin(semin);   // If SG value is below this * 32, the current will be increased
  driver2.semin(semin);   // If SG value is below this * 32, the current will be increased
  driver3.semin(semin);   // If SG value is below this * 32, the current will be increased
  driver4.semin(semin);   // If SG value is below this * 32, the current will be increased

  driver.semax(semax);   // If SG value is over (this + semin + 1) * 32, the current will be decreased
  driver2.semax(semax);   // If SG value is over (this + semin + 1) * 32, the current will be decreased
  driver3.semax(semax);   // If SG value is over (this + semin + 1) * 32, the current will be decreased
  driver4.semax(semax);   // If SG value is over (this + semin + 1) * 32, the current will be decreased

  driver.sedn(0b00);     // Set current reduction rate
  driver2.sedn(0b00);     // Set current reduction rate
  driver3.sedn(0b00);     // Set current reduction rate
  driver4.sedn(0b00);     // Set current reduction rate

  driver.seup(0b01);     // Set current increase rate
  driver2.seup(0b01);     // Set current increase rate
  driver3.seup(0b01);     // Set current increase rate
  driver4.seup(0b01);     // Set current increase rate

  driver.SGTHRS(sgthrs); // If SG value falls below this * 2 then the diag output will become active
  driver2.SGTHRS(sgthrs); // If SG value falls below this * 2 then the diag output will become active
  driver3.SGTHRS(sgthrs); // If SG value falls below this * 2 then the diag output will become active
  driver4.SGTHRS(sgthrs); // If SG value falls below this * 2 then the diag output will become active
  driver.VACTUAL(11000);
  driver2.VACTUAL(11000);
  driver3.VACTUAL(11000);
  driver4.VACTUAL(11000);
  driverDisable();
  Serial2.println("OK#Standby");
}

void beep(short len) {
  digitalWrite(12, HIGH);
  delay(len);
  digitalWrite(12, LOW);
}

void driverEnable() {
  digitalWrite(EN_PIN, HIGH);
}

void driverDisable() {
  digitalWrite(EN_PIN, LOW);
}

void driverMove(short LH, short RH, short LB, short RB, bool timing = true) {
  if (abs(LH) > 11000 || abs(RH) > 11000 || abs(LB) > 11000 || abs(RB) > 11000) {
    Serial2.print("ERR#Unreliable speed#");
    Serial2.print(LH);
    Serial2.print("#");
    Serial2.print(RH);
    Serial2.print("#");
    Serial2.print(LB);
    Serial2.print("#");
    Serial2.println(RB);
    return;
  }
  
  if (timing)driverDisable();
  driver.VACTUAL(LH);
  driver2.VACTUAL(RH);
  driver3.VACTUAL(LB);
  driver4.VACTUAL(RB);
  driverEnable();
}

short inline cs(double s) {
  return s / 0.0000209390195767;
}

void MoveAtVelocity(double f, double r,double turnr) {
  short LH = cs(f+r+(turnr*0.109));
  short RH = cs(f-r-(turnr*0.109));
  short LB = cs(f-r+(turnr*0.109));
  short RB = cs(f+r-(turnr*0.109));
  driverMove(LH, RH, LB, RB, false);
}

void Break(){
  driver.VACTUAL(0);
  driver2.VACTUAL(0);
  driver3.VACTUAL(0);
  driver4.VACTUAL(0);
}

void Move(double f,double r,double turnr,double t){
  MoveAtVelocity(f/t,r/t,turnr/t);
  delay(t*1000);
  Break();
}

String ReadSerial(){
  String seralstr= "";
  while(Serial.available()){
    
  }
}

void loop() {
  if (Serial.available()){
    
  }
  //MoveAtVelocity(0.1, 0.1,0.17);
}
