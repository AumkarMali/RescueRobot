

//ROBOT C
const int FULL_POWER = 100;
const float WHEEL_RADIUS = 8.16 / 2;
const float CONVERSION = PI * WHEEL_RADIUS / 180;

void elevate(bool upOrDown) {
	nMotorEncoder[motorC] = 0;
	if(upOrDown){
		motor[motorC] = -25;
	}else{
		motor[motorC] = 25;
	}
	
	while(abs(nMotorEncoder[motorC]) < 85) {}

	motor[motorC] = 0;
}


void rotate(float deg) {
	resetGyro(S1);
	//CCW
	if (deg <= 180) {
		motor[motorA] = FULL_POWER;
		motor[motorD] = -1 * FULL_POWER;
	}
	//CW
	else {
		motor[motorA] = -1 * FULL_POWER;
		motor[motorD] = FULL_POWER;
	}
	while (abs(getGyroDegrees(S1)) < deg) {}

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


void configureAllSensors ()
{
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	SensorMode[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);
}

moveBot(int rescueX, int rescueY, int saveX, int saveY, int counter, float angle){
	dist = sqrt(pow((saveX-rescueX),2)+pow((saveY-rescueY)));
	
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = FULL_POWER;
	
	//For loop for slowing down bot
	// while(sensorValue[S2] == (int)colorRed)
	// 	{}
	// for ()
	// {
	// 	}
	
	while(abs(nMotorEncoder[motorA]) * CONVERSION < dist)
		{}
	motor[motorA] = motor[motorD] = 0;
}

operateClaw(){
	motor[motorB] = 25;//assuming positive is closing the clamp
	while(getSensorValue[s2] != (int)colorBlue)
		{}
	motor[motorB] = -25;
	wait1Msec(1000);
	motor[motorB] = 0;
	openClose = "close";
}

void fileRead(TFileHandle read, float &x2, float &y2)
{
	float pickX = 0, pickY = 0, dropX = 0, dropY = 0, prevX = 0, prevY = 0;
	readFloatPC(read, pickX);
	readFloatPC(read, pickY);
	readFloatPC(read, dropX);
	readFloatPC(read, dropY);
	float angle = 0;

	bool up = 0;
	bool down = 1;

	operateClaw("open");
	evevate("up");
	for (int i = 0; i < 3; i++){
			angle = getDegrees(prevX, prevY, pickX, pickY);
			rotate(angle);	
			moveBot(prevX, prevY, pickX, pickY);
			elevate(down);
			operateClaw("close");
			elevate(up);
			angle = getDegrees(pickX, pickY, dropX, dropY);
			rotate(angle);
			moveBot(pickX, pickY, dropX, dropY, i);
			prevX = dropX;
			prevY = dropY;
			elevate(down);
			operateClaw("open");
			elevate(up);
		}
}


task main()
{

	configureSensors();

	//Wait for enter button to start
	while(!getButtonPress(buttonEnter)) 
		{}
	while(getButtonPress(buttonEnter))
		{}
	wait1Msec(1000);
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	string space = " ";

	TFileHandle read;
	bool fileOkay = openReadPC(read, "shortest_path.txt");

	if (!fileOkay)
	{
		displayString(5, "Error!");
		wait1Msec(5000);
	} else {
		for(int i = 0; i<7; i++)
		{
			wait1Msec(1000);
			x1 = x2;
			y1 = y2;
			fileRead(read, x2, y2);
			displayTextLine(4, "%f%s%f", x2, space, y2);
		}
	}
	closeFilePC(read);
}
