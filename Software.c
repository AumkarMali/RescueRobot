

//ROBOT C
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

	motor[motorA] = motor[motorD] = 0;
}


void readFile(int &rescueX, int &rescueY, int &saveX, int &saveY) {
	ifStream coords;
	coords.open("coordinates.txt");

	if(!coords.fail()){

		while(coords>>rescueX>>rescueY>>saveX>>saveY){

		}

	}else{
		displayText(line5) = "Failed to open Files!";
	}
}


void configureSensors() {
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

openClamp(int rescueX, int rescueY, int saveX, int saveY){
	nMotorEncoder(motorB) = 0;
	motor[motorB] = -25;//assuming positive is closing the clamp
	moveBot(rescueX, rescueY, saveX, saveY, openClose);
	while(getSensorValue[s2] != (int)colorBlue)
		{}
	motor[motorB] = -25;
	wait1Msec(1000);
	motor[motorB] = 0;
}

moveBot(){
	double angle = 0;
	dist = sqrt(pow((saveX-rescueX),2)+pow((saveY-rescueY)));
	//find angle equation
	rotate(angle);
	wait1Msec(1000);
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = FULL_POWER;
	while(abs(nMotorEncoder[motorA]) < dist)
		{}
	motor[motorA] = motor[motorD] = 0;
}

closeClamp(){
	nMotorEncoder(motorB) = 0;
	moveBot(rescueX, rescueY, saveX, saveY, openClose); // makes sure the bot is in the correct location
	motor[motorB] = 25;//assuming positive is closing the clamp
	while(getSensorValue[s2] != (int)colorBlue)
		{}
	motor[motorB] = -25;
	wait1Msec(1000);
	motor[motorB] = 0;
	openClose = "close";
}




task main()
{
	configureSensors();

	//Wait for enter button to start
	while(!getButtonPress(buttonEnter)) {}
	while(getButtonPress(buttonEnter)) {}
	wait1Msec(1000);
  
  
  //get path
  double min_distance = DBL_MAX;
    
  // Get the shortest path
  vector<string> shortest = shortest_path(min_distance);
  
  string prevNode = "A";
  float currDist = 0;


  for (const auto& node : shortest) {
    if (node != prevNode) {
  
      currDist = calculate_distance(coord[prevNode], coord[node]);
    }
  }
  cout << "\nTotal Distance: " << min_distance << endl;


	int rescueX = 0, rescueY = 0, saveX = 0, saveY = 0;
	readFile(rescueX,rescueY, saveX, saveY);
	moveBot();
}
