#include <Arduino.h>
#include "sensorUtils.h"

void distanceSensorSetup(int trigPin, int echoPin) {

  // Pin Modes for Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

float distanceSensorReadCM(int trigPin, int echoPin) {
  long duration;
  float distanceCm;

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm = (duration * SOUND_SPEED) / 2;

  return distanceCm;
}

float distanceSensorReadInch(int trigPin, int echoPin) {
   float distanceCm,distanceInch;

   distanceCm = distanceSensorReadCM(trigPin, echoPin);
   distanceInch = distanceCm * CM_TO_INCH;

  return distanceInch;
}

void waterSensorSetup(int wsPWR, int wsPIN) {
  // Pin Modes for Water Sensor
  pinMode(wsPWR, OUTPUT);
  pinMode(wsPIN, INPUT);
  // Power off Water Sensor
  digitalWrite(wsPWR, LOW);
}

uint16_t waterSensorRead(int wsPWR, int wsPIN) {
  uint16_t value = 0;

  digitalWrite(wsPWR, HIGH); // turn the sensor ON
  delay(10);                 // wait 10 milliseconds
  value = analogRead(wsPIN); // read the analog value from sensor
  digitalWrite(wsPWR, LOW);  // turn the sensor OFF

  return value;
}