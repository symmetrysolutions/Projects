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

void setupMotor(motor &motor, int channel, int speedPin, int directionPin1, int directionPin2)
{
  motor.channel = channel;
  motor.speedPin = speedPin;
  motor.directionPin1 = directionPin1;
  motor.directionPin2 = directionPin2;
  pinMode(motor.speedPin, OUTPUT);
  pinMode(motor.directionPin1, OUTPUT);
  pinMode(motor.directionPin2, OUTPUT);
  ledcSetup(motor.channel, 500, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
  ledcAttachPin(motor.speedPin, motor.channel); // Attach the speed pin to

  // analogWriteFrequency(5000); // Set frequency to 5 kHz for the speed pin
  // analogWriteResolution(8); // Set resolution to 8-bit for the speed pin
  
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
  ledcWrite(motor.channel, motor.speed);
  // analogWrite(motor.speedPin, motor.speed);

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
  motor.speed = constrain(speed, 0, 512); // Force values between 0-512 (PWM)
  // Set speed
  ledcWrite(motor.channel, motor.speed);
  // analogWrite(motor.speedPin, motor.speed);
}

void minMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 50);
    // Set speed
  ledcWrite(motor.channel, motor.speed);
  // analogWrite(motor.speedPin, motor.speed);
}

void maxMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 255);
  // Set speed
  ledcWrite(motor.channel, motor.speed);
  // analogWrite(motor.speedPin, motor.speed);
}

byte getMotorSpeed(motor &motor)
{
  return motor.speed;
}