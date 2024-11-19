#include "PC_FileIO.c"
#include "EV3Servo-lib-UW.c"

//ROBOT C
const int FULL_POWER = 40;
const float WHEEL_RADIUS = 8.16 / 2;
const float CONVERSION = PI * WHEEL_RADIUS / 180;
const int MAX_POINTS = 8;

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

float getDegrees(float x1, float y1, float x2, float y2, float previous_angle) {
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
	int direction = 0;
	if (deg >= 0) {
		direction = 20;
	}
	//CW
	else {
		direction = -20;
	}
	motor[motorA] = direction;
	motor[motorD] = direction*-1;

	while (abs(getGyroDegrees(S1)) < abs(deg))
	{}

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
	SensorType[S3] = sensorI2CCustom9V;
	wait1Msec(50);
	SensorMode[S4] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Color_Color;
	wait1Msec(50);

}

void adjustBot(float distance)
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = 20;

	wait1Msec(100);

	for(int i = 100 ; i>15 ; i --){
		motor[motorA] = motor[motorD] = i;
	}

	while((abs(nMotorEncoder[motorA]) * CONVERSION) < abs(distance))
	{}

	motor[motorA] = motor[motorD] = 0;
}

bool moveBot(int rescueX, int rescueY, int saveX, int saveY, float adjustDist, int cycle){
	float dist = sqrt(pow((saveX-rescueX),2)+pow((saveY-rescueY),2))*15;

	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = FULL_POWER;

	int colour1 = 0, colour2 = 0, colour3 = 0;
	bool loop1 = true;
	const int wait = 12;
	bool error = false;

	while(loop1 && cycle!=6 && !error)
	{
		if(abs(nMotorEncoder[motorA])*CONVERSION>dist+10){
			error = true;
		}

		if((abs(nMotorEncoder[motorA])*CONVERSION>dist+adjustDist) && !error){
			//	motor[motorA] = motor[motorD] = 50;
			if (SensorValue[S4] == (int)colorBlue || SensorValue[S4] == (int)colorRed){
				colour1 = SensorValue[S4];
				wait1Msec(wait);

				if (SensorValue[S4] == (int)colorBlue || SensorValue[S4] == (int)colorRed){
					colour2 = SensorValue[S4];
					wait1Msec(wait);

					if (SensorValue[S4] == (int)colorBlue || SensorValue[S4] == (int)colorRed){
						colour3 = SensorValue[S4];
						wait1Msec(wait);
						if(colour1 == colour2 && colour1 == colour3){
							loop1 = false;
						}
					}
				}
			}
		}
		colour1 = 0;
		colour2 = 0;
		colour3 = 0;
	}

	if(error){
		motor[motorA] = motor[motorD] = 0;
		rotate(180);
		nMotorEncoder[motorA] = 0;
		motor[motorA] = motor[motorD] = FULL_POWER;
		while(abs(nMotorEncoder[motorA]) * CONVERSION < dist+10)
		{}
		motor[motorA] = motor[motorD] = 0;
		return error;
		}else{
		if(cycle == 6){
			while(abs(nMotorEncoder[motorA]) * CONVERSION < dist) {}
			}else{
			while(abs(nMotorEncoder[motorA]) * CONVERSION < (dist-21.5)) {}
		}
		motor[motorA] = motor[motorD] = 0;
		wait1Msec(1000);
	}
	return error;
}

void operateClaw(bool openOrClose){
	if(openOrClose){
		setServoPosition(S3, 7, 65);
		}else{
		setServoPosition(S3, 7, 5);
	}
}


void fileRead(TFileHandle read)
{
	float prevAngle = 0;
	float angle = 0;
	float driveSum = 0;
	float adjustDist = -21.5;

	float displayAngle = 0;

	float xcoords[MAX_POINTS];
	float ycoords[MAX_POINTS];
	xcoords[0] = 0;
	ycoords[0] = 0;
	xcoords[7] = 0;
	ycoords[7] = 0;

	for (int i = 1; i<8; i++)
	{
		readFloatPC(read, xcoords[i]);
		readFloatPC(read, ycoords[i]);
	}

	bool up = false;
	bool down = true;
	bool Claw = true;
	bool loop2 = false;

	operateClaw(Claw);

	clearTimer(T1);

	for (int i = 0; i<7 && !loop2; i++)
	{

		if (i==0 || i==2 || i==4)
		{
			Claw = false;
		} else if (i==1 || i==3 || i==5)
		{
			Claw = true;
		}

		if (i==6)
		{
			adjustDist = 0;
		}

		angle = getDegrees(xcoords[i], ycoords[i], xcoords[i+1], ycoords[i+1], prevAngle);
		displayTextLine(5, "%f", angle);
		prevAngle += angle;


		wait1Msec(1000);


		if (i==0) {//-
			rotate(angle+3);
			} else if (i==1){//-
			rotate(angle+1);
			} else if (i==2){//+
			rotate(angle+1);
			} else if (i==3){//+
			rotate(angle-4);
			} else if (i==4){//-
			rotate(angle+3);
			} else if (i==5){//+
			rotate(angle-2);
			} else if (i==6){//+
			rotate(angle);
		}


		wait1Msec(1000);
		loop2 = moveBot(xcoords[i], ycoords[i], xcoords[i+1], ycoords[i+1], adjustDist-0.5, i);

		if (!loop2){
			driveSum += abs(nMotorEncoder[motorA])*CONVERSION;
			wait1Msec(1000);
			elevate(down);
			wait1Msec(1000);

			if (i>=0 && i<6)
			{
				operateClaw(Claw);
				if(Claw){
					wait1Msec(2000);
				}
				elevate(up);
				adjustBot(adjustDist);
			}
		}
	}

	if(!loop2){
		float driveTime = time1[T1]/1000.0;
		resetGripper(S3, 7);
		displayTextLine(4, 	"Time of Rescue: %f", driveTime);
		displayTextLine(5, "Total Distance Travelled: %fcm/s", driveSum);
		wait1Msec(10000);
	}	else {
		displayTextLine(4, "ERROR");
		displayTextLine(5, "Rescue Bot returned to previous location.");
		wait1Msec(10000);
	}
}


task main()
{

	configureAllSensors();

	wait1Msec(1000);
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
	string space = " ";

	TFileHandle read;
	bool fileOkay = openReadPC(read, "shortest_path.txt");

	if (!fileOkay)
	{
		displayString(5, "Error!");
		wait1Msec(5000);
		} else {
		while(!getButtonPress(buttonAny)) //Wait for enter button to start
		{}
		while(getButtonPress(buttonAny))
		{}
		fileRead(read);
	}
	closeFilePC(read);
}
