#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <limits>
#include <algorithm> // Header untuk fungsi reverse

using namespace std;

class Graph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList;

public:
    void addEdge(const string &u, const string &v, int weight) {
        adjList[u].emplace_back(v, weight);
    }

    void busiestNodes() {
        unordered_map<string, int> nodeFrequency;

        for (const auto &node : adjList) {
            for (const auto &neighbor : node.second) {
                nodeFrequency[neighbor.first]++;
            }
        }

        cout << "\nGedung tersibuk (paling sering dilewati):\n";
        for (const auto &entry : nodeFrequency) {
            cout << "- " << entry.first << ": " << entry.second << " kali" << endl;
        }
    }

    void shortestPath(const string &start, const string &end) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        for (const auto &node : adjList) {
            distances[node.first] = numeric_limits<int>::max();
        }
        distances[start] = 0;

        auto compare = [](pair<int, string> a, pair<int, string> b) { return a.first > b.first; };
        priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(compare)> pq(compare);

        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [currentDist, currentNode] = pq.top();
            pq.pop();

            if (currentDist > distances[currentNode]) continue;

            for (const auto &neighbor : adjList[currentNode]) {
                int newDist = currentDist + neighbor.second;
                if (newDist < distances[neighbor.first]) {
                    distances[neighbor.first] = newDist;
                    previous[neighbor.first] = currentNode;
                    pq.emplace(newDist, neighbor.first);
                }
            }
        }

        if (distances[end] == numeric_limits<int>::max()) {
            cout << "\nTidak ada rute dari " << start << " ke " << end << "." << endl;
        } else {
            cout << "\nRute terpendek dari " << start << " ke " << end << " adalah: " << endl;
            string current = end;
            vector<string> path;
            while (current != start) {
                path.push_back(current);
                current = previous[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());

            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i];
                if (i != path.size() - 1) cout << " -> ";
            }
            cout << "\nJarak: " << distances[end] << " meter" << endl;
        }
    }

    void displayConnections() {
        cout << "\nKoneksi antar gedung:\n";
        for (const auto &start : adjList) {
            cout << "- Gedung " << start.first << " terhubung dengan:\n";
            for (const auto &neighbor : start.second) {
                cout << "  - Gedung " << neighbor.first << " dengan jarak " << neighbor.second << " meter\n";
            }
            cout << endl;
        }
    }
};

int main() {
    Graph graph;

    // Menambahkan edge sesuai deskripsi hubungan gedung
    graph.addEdge("A", "B", 5);
    graph.addEdge("B", "C", 4);
    graph.addEdge("C", "D", 6);
    graph.addEdge("D", "E", 3);
    graph.addEdge("E", "F", 7);
    graph.addEdge("F", "G", 5);
    graph.addEdge("G", "H", 4);
    graph.addEdge("H", "I", 6);
    graph.addEdge("H", "J", 6);
    graph.addEdge("H", "K", 6);
    graph.addEdge("H", "L", 6);

    int choice;
    do {
        cout << "\n========== Sistem Navigasi Kampus ==========" << endl;
        cout << "1. Cari gedung tersibuk" << endl;
        cout << "2. Cari rute tercepat antar gedung" << endl;
        cout << "3. Tampilkan semua jarak antar gedung" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> choice;

        switch (choice) {
        case 1:
            graph.busiestNodes();
            break;
        case 2: {
            string start, end;
            cout << "\nMasukkan gedung awal: ";
            cin >> start;
            cout << "Masukkan gedung tujuan: ";
            cin >> end;
            graph.shortestPath(start, end);
            break;
        }
        case 3:
            graph.displayConnections();
            break;
        case 0:
            cout << "Keluar dari sistem." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
        }
    } while (choice != 0);

    return 0;
}
