// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------


// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------
#include "motorControl.h"

void setupMotor(motor &motor, int speedPin, int directionPin1, int directionPin2)
{
  motor.speedPin = speedPin;
  motor.directionPin1 = directionPin1;
  motor.directionPin2 = directionPin2;
  pinMode(motor.speedPin, OUTPUT);
  pinMode(motor.directionPin1, OUTPUT);
  pinMode(motor.directionPin2, OUTPUT);

  stopMotor(motor);
}

void stopMotor(motor &motor)
{
  motor.direction.input1 = LOW;
  motor.direction.input2 = LOW;
  motor.speed = 0;
  sendAllToMotor(motor);
}

void setMotorDirectionForward(motor &motor)
{
  motor.direction.input1 = HIGH;
  motor.direction.input2 = LOW;
  sendDirectionToMotor(motor);
}

void setMotorDirectionBackward(motor &motor)
{
  motor.direction.input1 = LOW;
  motor.direction.input2 = HIGH;
  sendDirectionToMotor(motor);
}

bool isMotorDirectionStopped(motor &motor)
{
  return motor.direction.input1 == LOW && motor.direction.input2 == LOW;
}

bool isMotorDirectionForward(motor &motor)
{
  return motor.direction.input1 == HIGH && motor.direction.input2 == LOW;
}

bool isMotorDirectionBackward(motor &motor)
{
  return motor.direction.input1 == LOW && motor.direction.input2 == HIGH;
}

// Send to motor without jumper
void sendAllToMotor(motor motor)
{
   // Set speed
  analogWrite(motor.speedPin, motor.speed);

  // Set direction
  digitalWrite(motor.directionPin1, motor.direction.input1);
  digitalWrite(motor.directionPin2, motor.direction.input2);
}

void sendDirectionToMotor(motor motor)
{
  // Set direction
  digitalWrite(motor.directionPin1, motor.direction.input1);
  digitalWrite(motor.directionPin2, motor.direction.input2);
}

void setMotorSpeed(motor &motor, int speed)
{
  motor.speed = constrain(speed, 50, 255); // Force values between 0-255 (PWM)
    // Set speed
  analogWrite(motor.speedPin, motor.speed);
}

void minMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 50);
    // Set speed
  analogWrite(motor.speedPin, motor.speed);
}

void maxMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 255);
  // Set speed
  analogWrite(motor.speedPin, motor.speed);
}

byte getMotorSpeed(motor &motor)
{
  return motor.speed;
}