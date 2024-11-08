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
	
	while(abs(nMotorEncoder[motorC]) < 85) {}

	motor[motorC] = 0;
}

float calculateAngle(float x1, float y1, float x2, float y2, float x3, float y3) {
    // Vector 1: from (x1, y1) to (x2, y2)
    float vector1_x = x2 - x1;
    float vector1_y = y2 - y1;
    
    // Vector 2: from (x2, y2) to (x3, y3)
    float vector2_x = x3 - x2;
    float vector2_y = y3 - y2;
    
    // Dot product of vector1 and vector2
    float dot_product = (vector1_x * vector2_x) + (vector1_y * vector2_y);
    
    // Magnitudes of vector1 and vector2
    float magnitude1 = sqrt(vector1_x * vector1_x + vector1_y * vector1_y);
    float magnitude2 = sqrt(vector2_x * vector2_x + vector2_y * vector2_y);
    
    // Calculate the angle in radians
    float angle_radians = acos(dot_product / (magnitude1 * magnitude2));
    
    // Convert angle to degrees
    float angle_degrees = angle_radians * (180.0 / PI);
    
    // Calculate the cross product to determine the direction (clockwise or counterclockwise)
    float cross_product = vector1_x * vector2_y - vector1_y * vector2_x;
    
    // Make angle negative if cross product is negative (indicating clockwise turn)
    if (cross_product < 0) {
        angle_degrees = -angle_degrees;
    }
    
    return angle_degrees;
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
	//For Angle stuff
	 // Define the x and y coordinates of each waypoint
    float x_coords[] = {0, -5, 0, -6, 3, 3, 9};
    float y_coords[] = {0, 0, -1, -3, 3, 5, 4};
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
