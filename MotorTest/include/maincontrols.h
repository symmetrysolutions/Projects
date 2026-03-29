#include "sensorUtils.h"
#include "motorControl.h"
#include "esp32_s3_pins.h"

#ifndef MAINCONTROLS_H
#define MAINCONTROLS_H

#define SERVICE_UUID "1241C000-1241-1241-1241-1241C00000AA"
#define TX_UUID      "1241C000-1241-1241-1241-1241C00000AB"
#define RX_UUID      "1241C000-1241-1241-1241-1241C00000AC"


#define MIN_SPEED 50
#define MAX_SPEED 255

void setupMotorPINS();
void setAllMotorSpeed(int speed);
void setMotorDirectionForward(motor &m);
void setMotorDirectionBackward(motor &m);
void setRightMotorsSpeed(int speed);
void setLeftMotorsSpeed(int speed);
void setRightTurn(int speed, int turnSpeed);
void setLeftTurn(int speed, int turnSpeed);
void setForwardMotion();
void setBackwardMotion();
void setRotateRight();
void setRotateLeft();
void stopMotors();
boolean isObstacleDetected(float threshold);
void checkObstacleTest();
void motorTest();
void increaseMotorsSpeed();
void decreaseMotorsSpeed();
void frequencyTest();
#endif
