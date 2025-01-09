#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <fstream>
#include <set>
using namespace std;

// Merepresentasikan graf menggunakan adjacency list
class Graph {
public:
    unordered_map<string, vector<pair<string, int>>> adjList;

    void addEdge(string u, string v, int weight) {
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
    }

    // Fungsi untuk menampilkan graf
    void displayGraph() {
        cout << "Graf:" << endl;
        for (auto &node : adjList) {
            cout << node.first << " -> ";
            for (auto &neighbor : node.second) {
                cout << "(" << neighbor.first << ", " << neighbor.second << "m) ";
            }
            cout << endl;
        }
    }

    // Fungsi untuk menulis graf ke file DOT
    void writeDotFile(const string &filename) {
        ofstream file(filename);
        file << "graph TelkomCampus {" << endl;
        for (auto &node : adjList) {
            for (auto &neighbor : node.second) {
                if (node.first < neighbor.first) {
                    file << "  \"" << node.first << "\" -- \"" << neighbor.first
                         << "\" [label=\"" << neighbor.second << "m\"]\n";
                }
            }
        }
        file << "}" << endl;
        file.close();
        cout << "\nFile DOT berhasil dibuat: " << filename << endl;
    }

    // Fungsi untuk menemukan persimpangan dengan cabang terbanyak
    string findNodeWithMostBranches() {
        string mostBranchesNode;
        int maxBranches = 0;

        for (const auto &node : adjList) {
            int branches = node.second.size();
            if (branches > maxBranches) {
                maxBranches = branches;
                mostBranchesNode = node.first;
            }
        }

        return mostBranchesNode;
    }
};

// Fungsi Dijkstra untuk mencari jalur terpendek
pair<vector<string>, int> dijkstra(Graph &graph, string start, string end) {
    unordered_map<string, int> distances;
    unordered_map<string, string> shortestPath;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> minHeap;

    for (auto &node : graph.adjList) {
        distances[node.first] = INT_MAX;
    }
    distances[start] = 0;
    minHeap.push({0, start});

    while (!minHeap.empty()) {
        auto [currentDistance, currentNode] = minHeap.top();
        minHeap.pop();

        if (currentDistance > distances[currentNode]) continue;

        for (auto &neighbor : graph.adjList[currentNode]) {
            string nextNode = neighbor.first;
            int weight = neighbor.second;
            int newDist = currentDistance + weight;

            if (newDist < distances[nextNode]) {
                distances[nextNode] = newDist;
                shortestPath[nextNode] = currentNode;
                minHeap.push({newDist, nextNode});
            }
        }
    }

    vector<string> path;
    string current = end;
    while (shortestPath.find(current) != shortestPath.end()) {
        path.insert(path.begin(), current);
        current = shortestPath[current];
    }

    if (!path.empty()) path.insert(path.begin(), start);
    return {path, distances[end]};
}

int main() {
    Graph graph;

    // Menambahkan edge sesuai pada graf
    graph.addEdge("Landmark Tower", "Rektorat", 500);
    graph.addEdge("Rektorat", "Convention Hall", 96);
    graph.addEdge("Rektorat", "Student Center", 240);
    graph.addEdge("Student Center", "SMK Telkom", 500);
    graph.addEdge("Convention Hall", "Landmark Tower", 400);
    graph.addEdge("Convention Hall", "SMK Telkom", 250);
    graph.addEdge("SMK Telkom", "Rektorat", 200);

    graph.displayGraph();

    // Ekspor graf ke file DOT
    graph.writeDotFile("campus_graph.dot");

    // Mencari jalur terpendek
    vector<vector<string>> paths;
    string start = "Landmark Tower", end = "SMK Telkom";
    auto [shortestPath, distance] = dijkstra(graph, start, end);
    paths.push_back(shortestPath);

    cout << "\nJalur terpendek dari " << start << " ke " << end << ": ";
    for (const auto &node : shortestPath) {
        cout << node << " ";
    }
    cout << "dengan jarak " << distance << " meter\n";

    // Mencari persimpangan dengan banyak cabang
    string mostBranchesNode = graph.findNodeWithMostBranches();
    cout << "\nPersimpangan dengan cabang terbanyak: " << mostBranchesNode << endl;

    return 0;
}

// dot -Tpng campus_graph.dot -o campus_graph.png
