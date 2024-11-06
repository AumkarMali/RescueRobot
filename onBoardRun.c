const int FULL_POWER = 100;

void elevate(int increments) {
	nMotorEncoder[motorC] = 0;

	if (increments > 0 {
		motor[motorC] = 10;
	}
	else {
		motor[motorC] = -10;
	}

	while(abs(nMotorEncoder[motorC]) < increments) {}

	motor[motorC] = 0;
}

void rotate(float deg) {
	resetGyro(S1);
	//CCW
	if (deg > 0) {
		motor[motorA] = FULL_POWER;
		motor[motorD] = -1 * FULL_POWER;
	}
	//CW
	else if (deg < 0) {
		motor[motorA] = -1 * FULL_POWER;
		motor[motorD] = FULL_POWER;
	}
	while (abs(getGyroDegrees(S1)) < deg) {}

	motor[motorA] = motor[motorD] = 0;
}

void driveLen(float dist) {
  //Gear ratio is 36 tooth : 12 tooth
  //Wheel spins 3 times for 1 motor spin, torque / 3
	nMotorEncoder[motorA] = 0;
	const float WHEEL_RADIUS = 8.16 / 2;
	const float CONVERSION = PI * WHEEL_RADIUS / 180;

	motor[motorA] = motor[motorD] = FULL_POWER;

	while (abs(nMotorEncoder[motorA]) * CONVERSION < dist) {}

	motor[motorA] = motor[motorD] = FULL_POWER;
}

void readFile() {
  
}

void configureSensors() {
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}


task main()
{
	configureSensors();

	//Wait for enter button to start
	while(!getButtonPress(buttonEnter)) {}
	while(getButtonPress(buttonEnter)) {}
	wait1Msec(1000);


  while (!bBTSkipStartup){}
  
  // Send a test message to the computer
  string msgOut = "Hello from EV3!";
  sendMessageWithParm(1, msgOut, 0);

  // Receive messages from the computer
  while (true)
  {
      if (bQueuedMsgAvailable())
      {
          string msgIn;
          msgIn = readMsg();
          nxtDisplayString(1, "Received: %s", msgIn);
      }
      wait1Msec(50); // Small delay to avoid flooding
  }


}
