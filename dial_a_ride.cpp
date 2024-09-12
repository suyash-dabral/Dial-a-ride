#include <bits/stdc++.h>
using namespace std;

typedef struct {
    int start_time;
    int end_time;
    int start_loc;
    int end_loc;
    int status;  
} request;

typedef struct {
    int initial_loc;
    vector<int> route;
    int capacity;
    int revenue;
} vehicle;

void dijkstras(vector<vector<int>> &arr, int n, int s);
bool compareRequests(const request &a, const request &b);
void sortRequests(vector<request> &requests);
int calculateRevenue(const request &req, const vector<vector<int>> &weights);
void assignRequests(vector<request> &requests, vector<vehicle> &vehicles, vector<vector<int>> &weights);

int main() {
    int n_loc, n_veh, n_req;
    
    // Input number of locations, vehicles, requests, and capacity
    cout << "Enter the number of locations: ";
    cin >> n_loc;
    cout << "Enter the number of vehicles: ";
    cin >> n_veh;
    cout << "Enter the number of requests: ";
    cin >> n_req;

    // Initialize vehicle data
    vector<vehicle> vehicles(n_veh);
    for (int i = 0; i < n_veh; i++) {
        cout << "Enter initial location and capacity for vehicle " << i + 1 << ": ";
        cin >> vehicles[i].initial_loc >> vehicles[i].capacity;
        vehicles[i].revenue = 0;  // Initialize revenue
    }

    // Input the weight (distance) matrix
    vector<vector<int>> weight(n_loc, vector<int>(n_loc));
    cout << "Enter the weight matrix (use -1 for no path):\n";
    for (int i = 0; i < n_loc; i++) {
        for (int j = 0; j < n_loc; j++) {
            cin >> weight[i][j];
        }
    }

    // Compute shortest paths using Dijkstra's algorithm
    for (int i = 0; i < n_loc; i++) {
        dijkstras(weight, n_loc, i);
    }

    // Input request data
    vector<request> requests(n_req);
    for (int i = 0; i < n_req; i++) {
        cout << "Enter start time, end time, start location, and end location for request " << i + 1 << ": ";
        cin >> requests[i].start_time >> requests[i].end_time >> requests[i].start_loc >> requests[i].end_loc;
        requests[i].status = 0;  // Initialize status
    }

    // Sort requests by start time
    sortRequests(requests);

    // Assign requests to vehicles
    assignRequests(requests, vehicles, weight);

    // Calculate total revenue
    int totalRevenue = 0;
    for (const auto &veh : vehicles) {
        totalRevenue += veh.revenue;
    }

    cout << "Final Revenue: " << totalRevenue << endl;
    return 0;
}

// Dijkstra's algorithm to find shortest paths
void dijkstras(vector<vector<int>> &arr, int n, int s) {
    int i, j;
    vector<int> visited(n, 0);
    vector<long long> weight(n, INT_MAX);
    weight[s] = 0;
    visited[s] = 1;
    
    for (int index = 0; index < n - 1; index++) {
        for (i = 0; i < n; i++) {
            if (arr[s][i] != -1 && weight[s] + arr[s][i] < weight[i] && visited[i] == 0) {
                weight[i] = weight[s] + arr[s][i];
            }
        }
        
        int min = INT_MAX, mini;
        for (i = 0; i < n; i++) {
            if (weight[i] < min && visited[i] == 0) {
                min = weight[i];
                mini = i;
            }
        }
        s = mini;
        visited[s] = 1;
    }

    for (i = 0; i < n; i++) {
        arr[s][i] = weight[i];
    }
}

// Comparison function for sorting requests
bool compareRequests(const request &a, const request &b) {
    return a.start_time < b.start_time;
}

// Sort requests by start time
void sortRequests(vector<request> &requests) {
    sort(requests.begin(), requests.end(), compareRequests);
}

// Assign requests to vehicles
void assignRequests(vector<request> &requests, vector<vehicle> &vehicles, vector<vector<int>> &weights) {
    sort(vehicles.begin(), vehicles.end(), [](const vehicle &a, const vehicle &b) {
        return a.initial_loc < b.initial_loc;
    });

    for (auto &req : requests) {
        auto minDistVehicle = min_element(vehicles.begin(), vehicles.end(), [&](const vehicle &a, const vehicle &b) {
            int distA = weights[a.initial_loc][req.start_loc];
            int distB = weights[b.initial_loc][req.start_loc];
            return distA < distB;
        });

        if (req.status == 0 && minDistVehicle != vehicles.end() && minDistVehicle->capacity > 1) {
            minDistVehicle->route.push_back(req.start_loc);
            minDistVehicle->route.push_back(req.end_loc);
            minDistVehicle->capacity--;
            req.status = 1;
            minDistVehicle->revenue += calculateRevenue(req, weights);
        }
    }
}

// Calculate revenue based on request
int calculateRevenue(const request &req, const vector<vector<int>> &weights) {
    const int baseRate = 1;
    if (weights[req.start_loc][req.end_loc] == -1) {
        return 0;
    }
    int distance = weights[req.start_loc][req.end_loc];
    int revenue = baseRate * distance;
    return revenue;
}
