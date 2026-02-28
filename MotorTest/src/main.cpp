#include <Arduino.h>
#include "motorControl.h"
#include "sensorUtils.h"

// Distance sensor
#define TRIG_PIN 4 
#define ECHO_PIN 5

// Motor A   
#define ENA_PIN 13 //PWM   
#define IN1_PIN 12   
#define IN2_PIN 14   
#define MIN_SPEED 50
#define MAX_SPEED 255

motor motorA;
void motorTest();
void increaseMotorsSpeed();   
void decreaseMotorsSpeed();

void setup()   
{   
	 Serial.begin(115200);   
	 
	 // Initialize Motor Structure with Pins   
	 // setupMotor(motorA, ENA_PIN, IN1_PIN, IN2_PIN);

	 // Initialize Distance Sensor
	 distanceSensorSetup(TRIG_PIN, ECHO_PIN);
}   
void loop()   
{   
	// motorTest();
/* */
	 Serial.println("Reading Distance Sensor");
	 float distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN);
	 Serial.print("Distance: ");
	 Serial.print(distance);
	 Serial.println(" cm");

/*
	 if(distance < 30.0) {
	   Serial.println("Object detected within 30 cm! Stopping motors.");
	   stopMotor(motorA);
	 } else {
		int speed = 200; // Decrease speed as distance decreases
	   	 
		setMotorDirectionForward(motorA);
		setMotorSpeed(motorA, speed); // Set speed to 200 (out of 255)
		Serial.println("No object nearby. Motors running forward at speed 200.");
	 }
*/
	 delay(1000); // Wait before next reading
} 

void motorTest()   
{   
	 Serial.println("Motors are stopped now");   
	 Serial.println("Set direction FORWARD");   
	 setMotorDirectionForward(motorA);   
	 Serial.println("Gradually increase motors speed to max");   
	 increaseMotorsSpeed();   
	 Serial.println("Motors are on full speed now");   
	 delay(2000);   
	 Serial.println("Gradually decrease motors speed to min");   
	 decreaseMotorsSpeed();   
	 Serial.println("Motors are stopped now");   
	 Serial.println("Set direction BACKWARD");   
	 delay(2000);   
	 setMotorDirectionBackward(motorA);   
	 Serial.println("Gradually increase motors speed to max");   
	 increaseMotorsSpeed();   
	 Serial.println("Motors are on full speed now");   
	 delay(2000);   
	 Serial.println("Gradually decrease motors speed to min");   
	 decreaseMotorsSpeed();
	 Serial.println("Motors are stopped now");   
	 delay(2000); 
} 

void increaseMotorsSpeed()   
{   
	 for (int speed = MIN_SPEED; speed <= MAX_SPEED; speed++) {   
	   setMotorSpeed(motorA, speed);
	   delay(40); // Add small delay between changes   
	 }   
}

void decreaseMotorsSpeed()   
{   
	 for (int speed = MAX_SPEED; speed >= MIN_SPEED; speed--) {   
	   setMotorSpeed(motorA, speed); 
	   delay(40); // Add small delay between changes   
	 }   
}  
