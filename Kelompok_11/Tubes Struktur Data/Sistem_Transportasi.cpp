#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <set>
#include <climits>
using namespace std;

// Struktur untuk merepresentasikan edge dengan bobot waktu
struct Edge {
    string destination;
    int time; 
};

// Graf sebagai adjacency list
unordered_map<string, vector<Edge>> graph;

// Daftar stasiun di Jakarta Selatan
set<string> jakartaSelatanStations = {
    "Manggarai", "Tebet", "Cawang", "Duren Kalibata", "Pasar Minggu",
    "Pasar Minggu Baru", "Tanjung Barat", "Lenteng Agung", 
    "Universitas Pancasila"
};

// Fungsi untuk menambahkan edge ke graf
void addEdge(const string& from, const string& to, int time) {
    graph[from].push_back({to, time});
    graph[to].push_back({from, time}); 
}

// Fungsi untuk mencari waktu perjalanan terpendek menggunakan Dijkstra
void dijkstra(const string& start, const string& end) {
    // Priority queue untuk Dijkstra (min-heap)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    unordered_map<string, int> distances; // Waktu terpendek ke setiap stasiun
    unordered_map<string, string> previous; // Melacak jalur sebelumnya

    // Inisialisasi jarak ke semua stasiun dengan tak terhingga
    for (const auto& station : graph) {
        distances[station.first] = INT_MAX;
    }
    distances[start] = 0;

    // Tambahkan stasiun awal ke priority queue
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        // Jika mencapai tujuan
        if (currentNode == end) {
            break;
        }

        // Periksa semua tetangga
        for (const Edge& neighbor : graph[currentNode]) {
            if (!jakartaSelatanStations.count(neighbor.destination)) continue; // Hanya Jakarta Selatan

            int newDistance = currentDistance + neighbor.time;

            // Jika waktu lebih singkat, perbarui
            if (newDistance < distances[neighbor.destination]) {
                distances[neighbor.destination] = newDistance;
                previous[neighbor.destination] = currentNode;
                pq.push({newDistance, neighbor.destination});
            }
        }
    }

    // Cetak hasil
    if (distances[end] == INT_MAX) {
        cout << "Tidak ada jalur dari " << start << " ke " << end << endl;
    } else {
        cout << "Waktu perjalanan dari " << start << " ke " << end << ": " << distances[end] << " menit" << endl;
        cout << "Jalur: ";
        string current = end;
        vector<string> path;
        while (current != start) {
            path.push_back(current);
            current = previous[current];
        }
        path.push_back(start);
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            cout << *it;
            if (it + 1 != path.rend()) cout << " -> ";
        }
        cout << endl;
    }
}

int main() {
    // Tambahkan edge (waktu perjalanan antar stasiun dalam menit)
    addEdge("Manggarai", "Tebet", 3);
    addEdge("Tebet", "Cawang", 3);
    addEdge("Cawang", "Duren Kalibata", 3);
    addEdge("Duren Kalibata", "Pasar Minggu Baru", 3);
    addEdge("Pasar Minggu Baru", "Pasar Minggu", 3);
    addEdge("Pasar Minggu", "Tanjung Barat", 5);
    addEdge("Tanjung Barat", "Lenteng Agung", 5);
    addEdge("Lenteng Agung", "Universitas Pancasila", 5);


    string start, end;
    cout << "Masukkan stasiun awal: ";
    getline(cin, start);  
    cout << "Masukkan stasiun akhir: ";
    getline(cin, end);  

    // Hitung waktu perjalanan menggunakan Dijkstra
    dijkstra(start, end);

    return 0;
}