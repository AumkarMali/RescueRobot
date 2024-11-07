#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <cfloat>
#include <fstream>

using namespace std;

// Coordinates of each node (using pairs to store (x, y))
unordered_map<string, pair<int, int>> coord = {
    {"A", {0, 0}},
    {"B", {0, 0}},
    {"C", {0, 0}},
    {"D", {0, 0}},
    {"E", {0, 0}},
    {"F", {0, 0}},
};

// Function to calculate Euclidean distance between two points
double calculate_distance(pair<int, int>& node1, pair<int, int>& node2) {
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
vector<string> shortest_path() {
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
        if (p.size() == 6) {  // Check if all nodes are visited
            final_paths.push_back(p);
        }
    }


    // Calculate the total distance for each valid path
    vector<double> distances;
    double min_distance = DBL_MAX;
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

int main() {
    ifstream fileRead;
    ofstream fileWrite;
  
    fileRead.open("coordinates.txt");
    fileWrite.open("shortest_path.txt");
    
    const int NUM_NODES = 6;
  
    string currNode = "";
    float curr_x = 0, curr_y = 0;
  
    if (!fileRead.fail() && !fileWrite.fail()) {
      for (int i = 0; i < NUM_NODES; i++) {
        fileRead >> currNode >> curr_x >> curr_y;
        coord[currNode] = {curr_x, curr_y};
      }
      
      vector<string> sp = shortest_path();

      for (const auto& node : sp) {
        fileWrite << coord[node].first <<" "<<coord[node].second<< endl;
      }
      
    }
    
    fileRead.close();


    return 0;
}
