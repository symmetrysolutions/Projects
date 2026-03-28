#include <Arduino.h>

#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

struct motor {
  int speed = 0;
  int speedPin = -1;
  int channel = -1;
  int directionPin1 = -1;
  int directionPin2 = -1;

  struct {
    byte input1 = LOW;
    byte input2 = LOW;
  } direction;
};

void setupMotor(motor &motor, int channel, int speedPin, int directionPin1, int directionPin2);
void sendAllToMotor(motor motor);
void sendDirectionToMotor(motor motor);

void stopMotor(motor &motor);
void setMotorDirectionForward(motor &motor);
void setMotorDirectionBackward(motor &motor);
bool isMotorDirectionStopped(motor &motor);
bool isMotorDirectionForward(motor &motor);
bool isMotorDirectionBackward(motor &motor);
void setMotorSpeed(motor &motor, int speed);
void minMotorSpeed(motor &motor);
void maxMotorSpeed(motor &motor);
byte getMotorSpeed(motor &motor);
#endif
