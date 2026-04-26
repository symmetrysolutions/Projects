#include "maincontrols.h"

motor motorA1;
motor motorA2;

motor motorB1;
motor motorB2;

void setupMotorPINS() {
	// This assumes A1 and B1 both control the left motors, 
	//and A2 and B2 control the right motors. Adjust as necessary based on your wiring.   
	setupMotor(motorA1, 0, ENA1_PIN, INA1_PIN, INA2_PIN);
	setupMotor(motorA2, 1, ENA2_PIN, INA3_PIN, INA4_PIN);
	setupMotor(motorB1, 0, ENB1_PIN, INB1_PIN, INB2_PIN);
	setupMotor(motorB2, 1, ENB2_PIN, INB3_PIN, INB4_PIN);
}

void setForwardMotion() {
	Serial.println("Moving Forward");
	digitalWrite(FORWARD_PIN, HIGH);
	digitalWrite(BACKWARD_PIN, LOW);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionForward(motorA1);
	setMotorDirectionForward(motorB1);
	setMotorDirectionForward(motorA2);
	setMotorDirectionForward(motorB2);
}

void setBackwardMotion() {
	Serial.println("Moving Backward");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, HIGH);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionBackward(motorA1);
	setMotorDirectionBackward(motorB1);
	setMotorDirectionBackward(motorA2);
	setMotorDirectionBackward(motorB2);
}

void setRotateRight() {
	Serial.println("Rotating Right");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, HIGH);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionForward(motorA1);
	setMotorDirectionForward(motorB1);
	setLeftMotorsSpeed(230);
	setRightMotorsSpeed(0);
	// setMotorDirectionBackward(motorA2);
	// setMotorDirectionBackward(motorB2);
}

void setRotateLeft() {
	Serial.println("Rotating Left");
	// setMotorDirectionBackward(motorA1);
	// setMotorDirectionBackward(motorB1);
	setMotorDirectionForward(motorA2);
	setMotorDirectionForward(motorB2);
	setRightMotorsSpeed(230);
	setLeftMotorsSpeed(0);
}

void stopMotors() {
	Serial.println("Stopping Motors");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, LOW);
	digitalWrite(STOP_PIN, HIGH);
	setMotorSpeed(motorA1, 0);
	setMotorSpeed(motorB1, 0);
	setMotorSpeed(motorA2, 0);
	setMotorSpeed(motorB2, 0);
}

void setRightMotorsSpeed(int speed) {
	// Serial.print("Setting Right Motors Speed to: ");
	// Serial.println(speed);
	setMotorSpeed(motorA2, speed);
	setMotorSpeed(motorB2, speed);
}

void setLeftMotorsSpeed(int speed) {
	// Serial.print("Setting Left Motors Speed to: ");
	// Serial.println(speed);
	setMotorSpeed(motorA1, speed);
	setMotorSpeed(motorB1, speed);
}

void setRightTurn(int speed, int turnSpeed) {
	setRightMotorsSpeed(turnSpeed);
	setLeftMotorsSpeed(speed);
}

void setLeftTurn(int speed, int turnSpeed) {
	setLeftMotorsSpeed(turnSpeed);
	setRightMotorsSpeed(speed);
}

void setAllMotorSpeed(int speed) {
	//Serial.println("Setting Motor Speed");
	setRightMotorsSpeed(speed);
	setLeftMotorsSpeed(speed);
}

boolean isObstacleDetected(float threshold = 30.0) {
	float distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm

	// digitalWrite(STOP_PIN, HIGH);

	/* Externally make sure we do not call too frequently to avoid sensor issues. 
	If distance is negative, it means the reading was invalid, 
	so we can ignore it and wait for the next valid reading.
	while(distance < 0.10) {
		// Wait for a valid reading
		delay(400);
		distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm
	}
	*/
	// digitalWrite(STOP_PIN, LOW);

	// Serial.print("Distance to Object: ");
	// Serial.print(distance);
	// Serial.println(" cm");

	return (distance > 0.10 && distance < threshold); // Adjust threshold as needed
}

void checkObstacleTest() {

	 if(isObstacleDetected(30.0)) {
	   Serial.println("Object detected within 30 cm! Stopping and reversing...");
	   stopMotors();
	   delay(1000);
	   setBackwardMotion();
	   setAllMotorSpeed(200);
	   delay(2000);
	   setRotateRight();
	   delay(4000);
	 }
	 setForwardMotion();
	 setAllMotorSpeed(250);
	 delay(500); // Wait before next reading
	}

void frequencyTest() {
	setForwardMotion();
	delay(10000);
	 Serial.println("Frequency 100");
	 ledcSetup(motorA1.channel, 100, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 500");
	 ledcSetup(motorA1.channel, 500, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 1000");
	 ledcSetup(motorA1.channel, 1000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 2000");
	 ledcSetup(motorA1.channel, 2000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 3000");
	 ledcSetup(motorA1.channel, 3000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 4000");
	 ledcSetup(motorA1.channel, 4000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 5000");
	 ledcSetup(motorA1.channel, 5000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
}

void motorTest()   
{   
	 Serial.println("Motors are stopped now");
	 stopMotors();
	 Serial.println("Set direction FORWARD");   
	 setForwardMotion();
	 Serial.println("Increasing Speed");   
	 increaseMotorsSpeed();
	 Serial.println("Decreasing Speed");   
	 decreaseMotorsSpeed();
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(4000);
	 Serial.println("Right Motors Only");   
	 setRightMotorsSpeed(150);
	 delay(4000);
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(2000);
	 Serial.println("Left Motors Only");
	 setLeftMotorsSpeed(150);
	 delay(4000);
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(2000);
	 Serial.println("All Forwards");
	 setForwardMotion();
	 setAllMotorSpeed(150);
	 delay(4000);
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(2000);
	 Serial.println("All Backwards");
	 setBackwardMotion();
	 setAllMotorSpeed(150);
	 delay(4000);
	 Serial.println("Stopping Motors");
	 stopMotors();
}

void increaseMotorsSpeed()   
{   
	 for (int speed = MIN_SPEED; speed <= MAX_SPEED; speed+=10) {   
	   Serial.print("Testing Frequency: ");
	   Serial.print(ledcReadFreq(motorA1.channel));
	   Serial.print(", Speed: ");
	   Serial.println(speed);
	   setAllMotorSpeed(speed);
	   delay(500); // Add small delay between changes   
	 }   
}

void decreaseMotorsSpeed()   
{   
	 for (int speed = MAX_SPEED; speed >= MIN_SPEED; speed-=5) {   
	   setAllMotorSpeed(speed);
	   delay(40); // Add small delay between changes   
	 }   
}  