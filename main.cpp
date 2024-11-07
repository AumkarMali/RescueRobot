#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <cfloat>

using namespace std;

// Coordinates of each node (using pairs to store (x, y))
unordered_map<string, pair<int, int>> coord = {
    {"A", {-5, 0}},
    {"B", {3, 5}},
    {"C", {9, 4}},
    {"D", {0, -1}},
    {"E", {-6, -3}},
    {"F", {3, 3}}
};
    

// Function to calculate Euclidean distance between two points
double calculate_distance(pair<int, int>& node1, pair<int, int>& node2) {
    int x1 = node1.first, y1 = node1.second;
    int x2 = node2.first, y2 = node2.second;
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double calculate_angle(pair<int, int>& node1, pair<int, int>& node2) {
    int x1 = node1.first, y1 = node1.second;
    int x2 = node2.first, y2 = node2.second;
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


// DFS function to find all paths from 'curr' to 'target'
void dfs(const unordered_map<string, vector<string>>& graph, const string& curr, const string& target, 
         unordered_map<string, bool>& visited, vector<string>& path, vector<vector<string>>& paths) {
    
    visited[curr] = true;
    path.push_back(curr);

    if (curr == target) {
        paths.push_back(path);
    } else {
        for (const string& neighbor : graph.at(curr)) {
            if (!visited[neighbor]) {
                dfs(graph, neighbor, target, visited, path, paths);
            }
        }
    }

    path.pop_back();
    visited[curr] = false;
}

// Function to find the shortest path and return the best path
vector<string> shortest_path(double &min_distance) {
    // Graph definition (adjacency list)
    unordered_map<string, vector<string>> graph = {
        {"A", {"C", "D", "F"}},  // pick up
        {"B", {"C", "D", "F"}},  // pick up
        {"C", {"A", "B", "E"}},  // drop off
        {"D", {"A", "B", "E"}},  // drop off
        {"E", {"C", "D", "F"}},  // pick up
        {"F", {"A", "B", "E"}}   // drop off
    };


    // Run DFS on the graph to find all paths from 'A' to 'F', 'C', and 'D'
    vector<vector<string>> paths;
    unordered_map<string, bool> visited;
    vector<string> path;
    
    // Find all paths from 'A' to 'F', 'C', and 'D'
    dfs(graph, "A", "F", visited, path, paths);
    dfs(graph, "A", "C", visited, path, paths);
    dfs(graph, "A", "D", visited, path, paths);

    // Filter out incomplete paths (those that don't visit all nodes)
    vector<vector<string>> final_paths;
    for (const auto& p : paths) {
        if (p.size() == 6) {
            final_paths.push_back(p);
        }
    }

    // Add 'A' to the end of each valid path as a return point
    for (auto& p : final_paths) {
        p.push_back("A");
    }

    // Calculate the total distance for each valid path
    vector<double> distances;
    int index = -1;
    int ctr = 0;

    for (const auto& p : final_paths) {
        double total_distance = 0.0;
        for (size_t i = 0; i < p.size() - 1; ++i) {
            total_distance += calculate_distance(coord[p[i]], coord[p[i + 1]]);
        }
        distances.push_back(total_distance);

        if (total_distance < min_distance) {
            min_distance = total_distance;
            index = ctr;
        }
        ctr++;
    }

    // Return the path with the minimum distance
    return final_paths[index];
}


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
