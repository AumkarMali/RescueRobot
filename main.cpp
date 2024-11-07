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



