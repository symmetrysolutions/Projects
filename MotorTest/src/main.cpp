#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "maincontrols.h"
#include "buzzerControl.h"

int throttle = 0;
int lastThrottle = 0;
int direction = 0; // 0 = stopped, 1 = forward, -1 = backward
int steering = 0;
int	lastSteering = 0;
int	crashCount = 0;
unsigned long lastCmd = 0;
unsigned long commandCount = 0;
unsigned long loopCount = 0;

BLEServer *server;
BLECharacteristic *tx;
boolean deviceConnected = false;
boolean isAdvertising = false;

class BTServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
	  Serial.println("Device Connected");
 	  digitalWrite(BACKWARD_PIN, LOW);
	  digitalWrite(FORWARD_PIN, HIGH);
	  commandCount = 0;
	  loopCount = 0;
	  crashCount = 0;
	  // playMelody(pinkPantherMelody, sizeof(pinkPantherMelody) / sizeof(pinkPantherMelody[0]), pinkPantherTempo);
	}

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
	  isAdvertising = false;
	  Serial.println("Device Disconnected");
	  digitalWrite(BACKWARD_PIN, HIGH);
	  digitalWrite(FORWARD_PIN, LOW);
	  digitalWrite(STOP_PIN, LOW);
	  playMelody(pacmanMelody, sizeof(pacmanMelody) / sizeof(pacmanMelody[0]), pacmanTempo);
    }
};

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *c) {
        String cmd = String(c->getValue().c_str());
        cmd.trim();
        
        lastCmd = millis();
        
		if (cmd.startsWith("D:")) {
			float throttlePercent = 0.0;
			int commaIndex = cmd.indexOf(',');
			if (commaIndex == -1) {
				Serial.print("Invalid Combined Command Format: ");
				Serial.println(cmd);
				return;
			}
			throttlePercent = cmd.substring(2, commaIndex).toInt()/100.0;
			throttle = throttlePercent * MAX_SPEED;
            steering = cmd.substring(commaIndex + 1).toInt();

			/*
			Serial.print("Received Combined Command: ");
			Serial.print(throttle);
			Serial.print(",");
			Serial.println(steering);
			*/
        } else if (cmd == "STOP") {
            throttle = 0;
            steering = 0;
			Serial.println("Received STOP Command");
		}
		else {
			Serial.print("Received Unknown Command: ");
			Serial.println(cmd);
        }
		commandCount++;
    }
};

void setupBlueTooth() {
	BLEDevice::init("CAR_GAME");
	server = BLEDevice::createServer();
	BLEService *service = server->createService(SERVICE_UUID);
	
	server->setCallbacks(new BTServerCallbacks());

	tx = service->createCharacteristic(TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
	tx->addDescriptor(new BLE2902());
	
	service->createCharacteristic(RX_UUID, BLECharacteristic::PROPERTY_WRITE)->setCallbacks(new RXCallbacks());
	
	service->start();
	server->getAdvertising()->addServiceUUID(SERVICE_UUID);
	server->getAdvertising()->start();
	Serial.println("Waiting for a client connection to notify...");
	isAdvertising = true;
	digitalWrite(BACKWARD_PIN, HIGH);
}

void setup()   
{   
	 Serial.begin(115200);   
	 
	pinMode(FORWARD_PIN, OUTPUT);
  	pinMode(BACKWARD_PIN, OUTPUT);
  	pinMode(STOP_PIN, OUTPUT);
	pinMode(STBY_PIN, OUTPUT);
	digitalWrite(FORWARD_PIN, LOW);
  	digitalWrite(BACKWARD_PIN, LOW);
  	digitalWrite(STOP_PIN, LOW);
	// Set standby pin HIGH to enable motor driver
	digitalWrite(STBY_PIN, HIGH);

	 // Initialize Motor Structure with Pins   
	setupMotorPINS();

	 // Initialize Distance Sensor
	 distanceSensorSetup(TRIG_PIN, ECHO_PIN);
	 setupBlueTooth();
	 setupBuzzer(BUZZER_PIN);
}   

void sendStatusValues(long v1, long v2) {
    
    String msg = "S:" + String(v1) + "," + String(v2);
    
    tx->setValue(msg.c_str());
    tx->notify();
}

void sendStatusMessage(String msg) {   
    tx->setValue(msg.c_str());
    tx->notify();
}

void notifyStop() {
	if(direction == 1) {
		setBackwardMotion();
	} else if (direction == -1) {
		setForwardMotion();
	}
	String msg = "OD:";
    
	crashCount++;
	msg += String(crashCount);
    sendStatusMessage(msg);
	setAllMotorSpeed(100);
	delay(1000);
	stopMotors();
	playMelody(nokiaMelody, sizeof(nokiaMelody) / sizeof(nokiaMelody[0]), nokiaTempo);
}

void loop()   
{   
	static unsigned long lastCheck = 0;

	// motorTest();
	
	// frequencyTest();
	// checkObstacleTest();
	// delay(500); // Wait before next reading
//
	if(deviceConnected) {
		if((millis() - lastCheck > 200 && throttle != 0)) {
			if(isObstacleDetected(40.0)) {	
				Serial.println("Object detected within 40 cm! Stopping and reversing...");
				stopMotors();
				notifyStop();
				throttle = 0;
				steering = 0;
			}
			lastCheck = millis();
		}

		if (throttle != lastThrottle) {
			Serial.print("Current Throttle: ");
			Serial.println(throttle);
			if (throttle > 0) {
				if(lastThrottle <= 0) {
					setForwardMotion();
					direction = 1;
				}
			} else if (throttle < 0) {
				if(lastThrottle >= 0) {
					setBackwardMotion();
					direction = -1;
				}
			}
			if(throttle == 0) {
				stopMotors();
				direction = 0;
				sendStatusValues(commandCount, loopCount);
			} else {
				setAllMotorSpeed(abs(throttle));
			}
			lastThrottle = throttle;
			loopCount++;
		}
		if(steering != lastSteering) {
			float turnIntensity = abs(steering) / 100.0;
			lastSteering = steering;
			if(throttle != 0) {
				if (steering > 0) {
					Serial.print("Turning Right with Steering: ");
					Serial.println(steering);
					setRightTurn(abs(throttle), abs(throttle) - (abs(throttle) * turnIntensity));
				} else if (steering < 0) {
					Serial.print("Turning Left with Steering: ");
					Serial.println(steering);
					setLeftTurn(abs(throttle), abs(throttle) - (abs(throttle) * turnIntensity));
				} else	 {
					setRightMotorsSpeed(abs(throttle));
					setLeftMotorsSpeed(abs(throttle));
				}
			}
		}
	} else {
		if(!isAdvertising) {
			Serial.println("No device connected. Stopping motors.");
			throttle = 0;
			steering = 0;
			stopMotors();
			server->getAdvertising()->addServiceUUID(SERVICE_UUID);
			server->startAdvertising();
			Serial.println("Waiting for a client connection to notify...");
			isAdvertising = true;
		}
	}
//
    
	/*
 	static unsigned long lastStatus = 0;
   if (millis() - lastStatus > 5000) {
        sendStatus();
        lastStatus = millis();
    }
	*/
} 
