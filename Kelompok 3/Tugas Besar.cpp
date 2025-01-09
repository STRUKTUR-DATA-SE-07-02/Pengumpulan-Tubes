#include <iostream> // Header file untuk input/output
#include <vector> // Header file untuk struktur data vector
#include <queue> // Header file untuk struktur data priority_queue
#include <unordered_map> // Header file untuk struktur data unordered_map
#include <climits> // Header file untuk konstanta INT_MAX
#include <algorithm> // Header file untuk fungsi algoritma (seperti sort, next_permutation)

using namespace std;

// typedef untuk mempermudah penulisan pair<int, string>, digunakan dalam priority_queue
typedef pair<int, string> Pair; 

// Kelas Graph yang digunakan untuk merepresentasikan graf
class Graph {
private:
    // adjacency list untuk menyimpan struktur graf (node dan edge-nya)
    unordered_map<string, vector<pair<string, int>>> adjList;

public:
    // Fungsi untuk menambahkan edge ke graf
    void addEdge(const string &u, const string &v, int weight) {
        adjList[u].push_back({v, weight}); // Tambahkan edge dari node u ke v dengan bobot
        adjList[v].push_back({u, weight}); // Karena graf tidak berarah, tambahkan edge sebaliknya
    }

    // Fungsi untuk menampilkan isi graf dalam bentuk adjacency list
    void displayGraph() {
        cout << "Ilustrasi Graf:\n";
        for (const auto &node : adjList) { // Iterasi setiap node dalam adjacency list
            cout << "Node " << node.first << ": "; // Tampilkan nama node
            for (const auto &edge : node.second) { // Iterasi setiap edge dari node
                cout << " -> " << edge.first << "(" << edge.second << ")"; // Tampilkan node tujuan dan bobot edge
            }
            cout << "\n";
        }
    }

    // Fungsi untuk mencari jalur terpendek dari start ke end menggunakan algoritma Dijkstra
    void dijkstra(const string &start, const string &end) {
        // Periksa apakah node start dan end ada di graf
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            cout << "Gudang atau jalur tidak ada" << endl;
            return;
        }

        // Inisialisasi jarak ke semua node dengan INT_MAX (tidak terhingga)
        unordered_map<string, int> distances;
        unordered_map<string, string> parents; // Untuk melacak jalur
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq; // Priority queue untuk memilih node dengan jarak terpendek

        for (const auto &node : adjList) {
            distances[node.first] = INT_MAX; // Set jarak awal semua node ke tak terhingga
        }

        // Atur jarak node awal menjadi 0 dan masukkan ke priority queue
        distances[start] = 0;
        pq.push({0, start});

        // Proses algoritma Dijkstra
        while (!pq.empty()) {
            auto [currentDistance, currentNode] = pq.top(); // Ambil node dengan jarak terpendek
            pq.pop();

            // Jika jarak yang diambil lebih besar dari jarak yang disimpan, lewati
            if (currentDistance > distances[currentNode]) continue;

            // Periksa semua tetangga dari currentNode
            for (const auto &neighbor : adjList[currentNode]) {
                const string &nextNode = neighbor.first;
                int weight = neighbor.second;

                // Hitung jarak baru ke tetangga
                int newDistance = currentDistance + weight;
                if (newDistance < distances[nextNode]) { // Jika jarak baru lebih kecil, perbarui
                    distances[nextNode] = newDistance;
                    parents[nextNode] = currentNode; // Simpan parent node
                    pq.push({newDistance, nextNode}); // Masukkan ke priority queue
                }
            }
        }

        // Jika jarak ke node tujuan tetap INT_MAX, tidak ada jalur
        if (distances[end] == INT_MAX) {
            cout << "Gudang atau jalur tidak ada" << endl;
        } else {
            // Tampilkan jarak terpendek
            cout << "Jarak terpendek dari " << start << " ke " << end << " adalah " << distances[end] << endl;

            // Rekonstruksi jalur
            vector<string> path;
            for (string at = end; at != start; at = parents[at]) { // Backtrack dari node tujuan ke awal
                path.push_back(at);
            }
            path.push_back(start); // Tambahkan node awal
            reverse(path.begin(), path.end()); // Balikkan jalur agar sesuai urutan

            // Tampilkan jalur
            cout << "Jalur: ";
            for (const auto &node : path) {
                cout << node << (node == end ? "\n" : " -> ");
            }
        }
    }

    // Fungsi untuk mencari jalur alternatif jika sebuah node tidak bisa dilewati
    void findAlternateRoute(const string &start, const string &end, const string &blockedNode) {
        // Periksa apakah node start, end, atau blockedNode ada di graf
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end() || adjList.find(blockedNode) == adjList.end()) {
            cout << "Gudang atau jalur tidak ada" << endl;
            return;
        }

        cout << "\nMenghitung jalur alternatif dengan Gudang " << blockedNode << " Yang Tidak Bisa Dilewati:\n";

        // Buat salinan adjacency list tanpa blockedNode
        unordered_map<string, vector<pair<string, int>>> tempAdjList = adjList;
        for (auto &node : tempAdjList) {
            node.second.erase(remove_if(node.second.begin(), node.second.end(), [&blockedNode](pair<string, int> edge) {
                return edge.first == blockedNode; // Hapus edge yang terhubung ke blockedNode
            }), node.second.end());
        }
        tempAdjList.erase(blockedNode); // Hapus blockedNode dari adjacency list

        // Gunakan adjacency list sementara untuk mencari jalur alternatif menggunakan algoritma Dijkstra (sama seperti sebelumnya)
        unordered_map<string, int> distances;
        unordered_map<string, string> parents;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        for (const auto &node : tempAdjList) {
            distances[node.first] = INT_MAX;
        }

        distances[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [currentDistance, currentNode] = pq.top();
            pq.pop();

            if (currentDistance > distances[currentNode]) continue;

            for (const auto &neighbor : tempAdjList[currentNode]) {
                const string &nextNode = neighbor.first;
                int weight = neighbor.second;

                int newDistance = currentDistance + weight;
                if (newDistance < distances[nextNode]) {
                    distances[nextNode] = newDistance;
                    parents[nextNode] = currentNode;
                    pq.push({newDistance, nextNode});
                }
            }
        }

        if (distances[end] == INT_MAX) {
            cout << "Gudang atau jalur tidak ada" << endl;
        } else {
            cout << "Jarak terpendek dari " << start << " ke " << end << " adalah " << distances[end] << endl;

            vector<string> path;
            for (string at = end; at != start; at = parents[at]) {
                path.push_back(at);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());

            cout << "Jalur: ";
            for (const auto &node : path) {
                cout << node << (node == end ? "\n" : " -> ");
            }
        }
    }

    // Fungsi untuk menyelesaikan masalah Traveling Salesman Problem (TSP)
    void solveTSP(const string &start) {
        vector<string> nodes;
        for (const auto &node : adjList) {
            nodes.push_back(node.first);
        }

        vector<string> bestPath;
        int minCost = INT_MAX;

        sort(nodes.begin(), nodes.end()); // Urutkan node untuk iterasi dengan next_permutation

        do {
            if (nodes.front() != start) continue; // Hanya mulai dari node start

            int currentCost = 0;
            bool valid = true;

            for (size_t i = 0; i < nodes.size() - 1; ++i) {
                auto &current = nodes[i];
                auto &next = nodes[i + 1];

                bool found = false;
                for (const auto &neighbor : adjList[current]) {
                    if (neighbor.first == next) {
                        currentCost += neighbor.second; // Tambahkan bobot edge
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    valid = false; // Jika tidak ada edge, jalur tidak valid
                    break;
                }
            }

            if (valid) {
                auto &last = nodes.back();
                auto &first = nodes.front();

                bool found = false;
                for (const auto &neighbor : adjList[last]) {
                    if (neighbor.first == first) {
                        currentCost += neighbor.second; // Tambahkan bobot edge untuk kembali ke awal
                        found = true;
                        break;
                    }
                }

                if (found && currentCost < minCost) { // Perbarui solusi terbaik jika ditemukan
                    minCost = currentCost;
                    bestPath = nodes;
                }
            }

        } while (next_permutation(nodes.begin(), nodes.end())); // Iterasi semua kemungkinan jalur

        if (minCost == INT_MAX) {
            cout << "Gudang Tidak Ada\n";
        } else {
            cout << "Solusi TSP dengan biaya minimum: " << minCost << "\n";
            cout << "Jalur Yang Dilewati: \n";
            for (const auto &node : bestPath) {
                cout << node << " -> ";
            }
            cout << bestPath.front() << "\n"; // Kembali ke node awal
        }
    }
};

// Fungsi utama program
int main() {
    Graph g;

    // Menambahkan edge sesuai dengan graf pada soal
    g.addEdge("A", "B", 10);
    g.addEdge("A", "C", 15);
    g.addEdge("B", "C", 5);
    g.addEdge("B", "D", 20);
    g.addEdge("C", "D", 10);
    g.addEdge("C", "E", 25);
    g.addEdge("D", "E", 5);
    g.addEdge("D", "F", 15);
    g.addEdge("E", "F", 10);

    int choice; // Variabel untuk menyimpan pilihan menu
    string startNode, endNode, blockedNode; // Variabel input pengguna

    do {
        cout << "\nMenu:\n";
        cout << "1. Tampilkan Gudang Yang Ada\n"; // Pilihan untuk menampilkan graf
        cout << "2. Cari Rute Menggunakan Djikstra\n"; // Pilihan untuk mencari jalur terpendek
        cout << "3. Temukan Jalur alternatif jika Gudang (Node) Tidak Bisa Dilewati\n"; // Pilihan untuk mencari jalur alternatif
        cout << "4. Mencari Rute Untuk Melewati Semua Gudang (TSP)\n"; // Pilihan untuk menyelesaikan TSP
        cout << "5. Keluar\n"; // Pilihan untuk keluar
        cout << "Pilih opsi: ";
        cin >> choice;

        switch (choice) {
            case 1:
                g.displayGraph(); // Tampilkan graf
                break;
            case 2:
                cout << "Masukkan Gudang awal: ";
                cin >> startNode;
                cout << "Masukkan Gudang tujuan: ";
                cin >> endNode;
                g.dijkstra(startNode, endNode); // Jalankan Dijkstra
                break;
            case 3:
                cout << "Masukkan Gudang awal: ";
                cin >> startNode;
                cout << "Masukkan Gudang tujuan: ";
                cin >> endNode;
                cout << "Masukkan Gudang yang tidak bisa dilewati: ";
                cin >> blockedNode;
                g.findAlternateRoute(startNode, endNode, blockedNode); // Cari jalur alternatif
                break;
            case 4:
                cout << "Masukkan Gudang awal: ";
                cin >> startNode;
                g.solveTSP(startNode); // Selesaikan TSP
                break;
            case 5:
                cout << "Keluar dari program.\n"; // Keluar dari program
                break;
            default:
                cout << "Opsi tidak valid. Silakan coba lagi.\n"; // Input tidak valid
        }
    } while (choice != 5); // Ulangi hingga pengguna memilih keluar

    return 0;
}
