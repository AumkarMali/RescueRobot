

//ROBOT C
const int FULL_POWER = 100;

void elevate(int increments, string upOrDown) {
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

moveBot(int rescueX, int rescueY, int saveX, int saveY, int counter, float angle){
	if (count == 0){
		//move from (0,0) to first pick up
		
	}else{
		/*
 		Pick up stuff
   		move
     		dropoff
       		end function and repeat 3 times
 		 */
	}

	rotate(angle)
	dist = sqrt(pow((saveX-rescueX),2)+pow((saveY-rescueY)));
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = FULL_POWER;
	while(sensorValue[S2] == (int)colorRed)
		{}
	
	while(abs(nMotorEncoder[motorA]) < dist)
		{}
	motor[motorA] = motor[motorD] = 0;



	// prevCode
	/*
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
	*/
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




task main()
{
	configureSensors();

	//Wait for enter button to start
	while(!getButtonPress(buttonEnter)) {}
	while(getButtonPress(buttonEnter)) {}
	wait1Msec(1000);
  
  
  // //get path
  // double min_distance = DBL_MAX;
    
  // // Get the shortest path
  // vector<string> shortest = shortest_path(min_distance);
  
  // string prevNode = "A";
  // float currDist = 0;


  // for (const auto& node : shortest) {
  //   if (node != prevNode) {
  
  //     currDist = calculate_distance(coord[prevNode], coord[node]);
  //   }
  // }
	displayTextline (5, ("Total Distance: &f", min_distance)); // idk if this works
	
	int pickX = 0, pickY = 0, dropX = 0, dropY = 0, prevX = 0, prevY = 0;
	double angle = 0;
	
	readFile(rescueX,rescueY, saveX, saveY);
	operateClaw("open");
	evevate("up");
	for (int i = 0; i < 3; i++){
		angle = getDegrees(pickX, pickY, dropX, dropY);
		if(i == 0){
			angle = getDegrees(0,0, pickX, pickY)
			moveBot(0, 0, pickX, pickY, i);
			elevate("down");
			operateClaw("close");
			elevate("up");
			moveBot(pickX, pickY, dropX, dropY, i);
			prevX = saveX;
			prevY = saveY;
			operateClaw("open");
			elevate("up");
		}else{
			moveBot(prevX, prevY, pickX, pickY);
			//do the rest of the set up
			moveBot(pickX, pickY, dropX, dropY, i);
			prevX = saveX;
			prevY = saveY;
			operateClaw("close");
			moveBot(saveY = )
		}
	}

}
\

void fileRead(TFileHandle read, float &x2, float &y2)
{
		readFloatPC(read, x2);
		readFloatPC(read, y2);
}


task main()
{
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
