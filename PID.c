#pragma config(Motor, motorB, leftMotor, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rightMotor, tmotorEV3_Large, PIDControl, encoder)

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

	while(abs(nMotorEncoder[motorC]) < 110) {}

	motor[motorC] = 0;
}

float calculateAngle(float x1, float y1, float x2, float y2, float previous_angle) {
    // Vector from (x1, y1) to (x2, y2)
    float vector_x = x2 - x1;
    float vector_y = y2 - y1;

    // Calculate the angle of this vector relative to the positive x-axis
    float angle_radians = atan2(vector_y, vector_x);
    float angle_degrees = angle_radians * (180.0 / PI);

    // Calculate the smallest turn angle relative to the previous angle
    float turn_angle = angle_degrees - previous_angle;

    // Normalize to be within -180 to 180 degrees for shortest rotation
    if (turn_angle > 180) {
        turn_angle -= 360;
    } else if (turn_angle < -180) {
        turn_angle += 360;
    }

    return turn_angle;
}



void rotate(float deg) {
	resetGyro(S1);
	//CCW
	if (deg >= 0) {
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

void configureAllSensors ()
{
	SensorType[S1] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Gyro_RateAndAngle;
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

void fileRead(TFileHandle read)
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
	//For Angle stuff
	 // Define the x and y coordinates of each waypoint
 // Define the x and y coordinates of each waypoint
    float x_coords[] = {0, -5, 0, -6, 3, 3, 9 , 0};
    float y_coords[] = {0, 0, -1, -3, 3, 5, 4 , 0};
    int num_waypoints = sizeof(x_coords) / sizeof(x_coords[0]);

    // Calculate angles between consecutive vectors and display them
    for (int i = 0; i < num_waypoints - 2; i++) {
        float angle = calculateAngle(
            x_coords[i], y_coords[i],      // (x1, y1)
            x_coords[i + 1], y_coords[i + 1],  // (x2, y2)
            x_coords[i + 2], y_coords[i + 2]   // (x3, y3)
        );

        // Display the angle (smallest possible turning angle)
        displayTextLine(i, "Angle %d -> %d -> %d: %.2f deg", i, i + 1, i + 2, angle);
        wait1Msec(1000);    // Pause briefly to view each angle
    }

    wait1Msec(10000); // Wait to view all results on screen

	
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
		fileRead(read);
	}
	closeFilePC(read);
}
