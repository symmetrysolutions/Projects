#include <Arduino.h>

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

void distanceSensorSetup(int trigPin, int echoPin);
float distanceSensorReadInch(int trigPin, int echoPin);
float distanceSensorReadCM(int trigPin, int echoPin);

void waterSensorSetup(int wsPWR, int wsPIN);
uint16_t waterSensorRead(int wsPWR, int wsPIN);

