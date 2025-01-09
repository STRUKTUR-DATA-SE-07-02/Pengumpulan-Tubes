#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <stack>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>

using namespace std;

// Struktur untuk menyimpan data jarak minimum dan simpul terkait
struct Node {
    string vertex;
    int cost;
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

class Graph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList; // Daftar adjacency
    unordered_map<string, string> prev; // Menyimpan simpul sebelumnya untuk setiap jalur
    vector<string> kecamatanNames; // Nama-nama kecamatan

public:
    // Konstruktor untuk inisialisasi graf
    Graph(vector<string> names) {
        this->kecamatanNames = names;
    }

    // Fungsi untuk menambah edge (jalur kereta) dengan bobot (biaya)
    void addEdge(const string& u, const string& v, int cost) {
        adjList[u].push_back({v, cost});
        adjList[v].push_back({u, cost}); // Karena graf ini tidak berarah
    }

    // Fungsi untuk mencari rute termurah menggunakan Algoritma Dijkstra
    void dijkstra(const string& source, const string& target) {
        unordered_map<string, int> dist;
        for (const auto& pair : adjList) {
            dist[pair.first] = INT_MAX; // Menginisialisasi jarak ke tak terhingga
        }
        dist[source] = 0;

        priority_queue<Node, vector<Node>, greater<Node>> pq; // Priority queue untuk memilih jarak terendah
        pq.push({source, 0});

        while (!pq.empty()) {
            string u = pq.top().vertex;
            pq.pop();

            if (u == target) {
                break;
            }

            for (const auto& neighbor : adjList[u]) {
                string v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({v, dist[v]});
                }
            }
        }

        // Menampilkan hasil jarak ke tujuan dan jalur yang dilewati
        if (dist[target] == INT_MAX) {
            cout << "Tidak ada rute antara kecamatan " << source << " dan " << target << endl;
        } else {
            cout << "Biaya terpendek dari " << source << " ke " << target << " adalah Rp " << dist[target] << endl;

            // Menampilkan kecamatan yang dilewati
            cout << "Kecamatan yang dilewati: ";
            stack<string> path;
            string temp = target;
            while (temp != source) {
                path.push(temp);
                temp = prev[temp];
            }
            path.push(source);

            bool first = true;
            while (!path.empty()) {
                if (!first) cout << ", ";
                cout << path.top();
                path.pop();
                first = false;
            }
            cout << endl;
        }
    }

    // Menampilkan daftar kecamatan
    void displayKecamatan() {
        cout << "Daftar Kecamatan di Banyumas:" << endl;
        for (int i = 0; i < kecamatanNames.size(); ++i) {
            cout << i << ". " << kecamatanNames[i] << endl;
        }
        cout << endl;
    }
};

int main() {
    vector<string> kecamatanNames = {
        "Ajibarang", "Banyumas", "Baturraden", "Cilongok", "Gumelar", "Jatilawang",
        "Kalibagor", "Karanglewas", "Kebasen", "Kedungbanteng", "Kembaran", "Kemrajen",
        "Lumbir", "Patikraja", "Pekuncen", "Purwojati", "Purwokerto Utara", "Purwokerto Selatan",
        "Purwokerto Barat", "Purwokerto Timur", "Rawalo", "Sokaraja", "Somagede", "Sumbang",
        "Sumpiuh", "Tambak", "Wangon"
    };

    Graph g(kecamatanNames);

    // Menambahkan edges (jalur kereta antar kecamatan)
    g.addEdge("Ajibarang", "Cilongok", 4500);
    g.addEdge("Ajibarang", "Pekuncen", 6000);
    g.addEdge("Ajibarang", "Lumbir", 5000);
    g.addEdge("Ajibarang", "Purwojati", 3000);
    g.addEdge("Banyumas", "Kalibagor", 3000);
    g.addEdge("Banyumas", "Patikraja", 5000);
    g.addEdge("Banyumas", "Sokaraja", 2000);
    g.addEdge("Banyumas", "Kebasen", 7000);
    g.addEdge("Banyumas", "Kemrajen", 6000);
    g.addEdge("Banyumas", "Somagede", 4000);
    g.addEdge("Baturraden", "Purwokerto Utara", 4500);
    g.addEdge("Cilongok", "Patikraja", 5000);
    g.addEdge("Cilongok", "Karanglewas", 2000);
    g.addEdge("Cilongok", "Pekuncen", 5000);
    g.addEdge("Cilongok", "Ajibarang", 3000);
    g.addEdge("Cilongok", "Purwojati", 2000);
    g.addEdge("Gumelar", "Lumbir", 8000);
    g.addEdge("Gumelar", "Pekuncen", 8000);
    g.addEdge("Jatilawang", "Wangon", 3000);
    g.addEdge("Jatilawang", "Purwojati", 4000);
    g.addEdge("Jatilawang", "Rawalo", 5000);
    g.addEdge("Kalibagor", "Sokaraja", 2000);
    g.addEdge("Kalibagor", "Somagede", 3000);
    g.addEdge("Kalibagor", "Banyumas", 2500);
    g.addEdge("Karanglewas", "Purwokerto Selatan", 3000);
    g.addEdge("Karanglewas", "Cilongok", 6000);
    g.addEdge("Karanglewas", "Purwokerto Barat", 8000);
    g.addEdge("Kebasen", "Kemrajen", 10000);
    g.addEdge("Kebasen", "Rawalo", 5500);
    g.addEdge("Kebasen", "Patikraja", 6000);
    g.addEdge("Kebasen", "Banyumas", 5000);
    g.addEdge("Kemrajen", "Sumpiuh", 3500);
    g.addEdge("Kemrajen", "Kebasen", 4500);
    g.addEdge("Kemrajen", "Banyumas", 5000);
    g.addEdge("Kemrajen", "Somagede", 6000);
    g.addEdge("Kedungbanteng", "Purwokerto Utara", 3500);
    g.addEdge("Kedungbanteng", "Pekuncen", 9500);
    g.addEdge("Kedungbanteng", "Purwokerto Barat", 4500);
    g.addEdge("Kembaran", "Purwokerto Timur", 2500);
    g.addEdge("Kembaran", "Sokaraja", 3500);
    g.addEdge("Kembaran", "Sumbang", 5000);
    g.addEdge("Lumbir", "Wangon", 4000);
    g.addEdge("Lumbir", "Gumelar", 6000);
    g.addEdge("Lumbir", "Ajibarang", 6000);
    g.addEdge("Patikraja", "Rawalo", 3000);
    g.addEdge("Patikraja", "Kebasen", 4000);
    g.addEdge("Patikraja", "Banyumas", 3500);
    g.addEdge("Patikraja", "Sokaraja", 5000);
    g.addEdge("Patikraja", "Purwokerto Selatan", 3000);
    g.addEdge("Patikraja", "Cilongok", 7000);
    g.addEdge("Pekuncen", "Gumelar", 4500);
    g.addEdge("Pekuncen", "Ajibarang", 6500);
    g.addEdge("Pekuncen", "Kedungbanteng", 7500);
    g.addEdge("Pekuncen", "Cilongok", 5000);
    g.addEdge("Purwojati", "Rawalo", 5000);
    g.addEdge("Purwojati", "Cilongok", 3000);
    g.addEdge("Purwojati", "Wangon", 7000);
    g.addEdge("Purwojati", "Ajibarang", 4000);
    g.addEdge("Purwojati", "Jatilawang", 7000);
    g.addEdge("Purwokerto Barat", "Purwokerto Selatan", 2500);
    g.addEdge("Purwokerto Barat", "Purwokerto Utara", 2500);
    g.addEdge("Purwokerto Barat", "Purwokerto Timur", 2500);
    g.addEdge("Purwokerto Barat", "Karanglewas", 4000);
    g.addEdge("Purwokerto Barat", "Kedungbanteng", 7000);
    g.addEdge("Purwokerto Timur", "Sokaraja", 3000);
    g.addEdge("Purwokerto Timur", "Barat", 3000);
    g.addEdge("Purwokerto Timur", "Utara", 3000);
    g.addEdge("Purwokerto Timur", "Selatan", 3000);
    g.addEdge("Purwokerto Timur", "Kembaran", 5000);
    g.addEdge("Purwokerto Timur", "Sumbang", 7000);
    g.addEdge("Purwokerto Selatan", "Karanglewas", 6000);
    g.addEdge("Purwokerto Selatan", "Sokaraja", 7000);
    g.addEdge("Purwokerto Selatan", "Patikraja", 7000);
    g.addEdge("Purwokerto Selatan", "Purwokerto Timur", 3000);
    g.addEdge("Purwokerto Utara", "Sumbang", 7000);
    g.addEdge("Purwokerto Utara", "Purwokerto Barat", 3000);
    g.addEdge("Purwokerto Utara", "Purwokerto Timur", 3000);
    g.addEdge("Purwokerto Utara", "Sumbang", 6000);
    g.addEdge("Purwokerto Utara", "Baturaden", 9000);
    g.addEdge("Rawalo", "Kebasen", 5000);
    g.addEdge("Rawalo", "Patikraja", 5000);
    g.addEdge("Rawalo", "Jatilawang", 5000);
    g.addEdge("Rawalo", "Purwojati", 5000);
    g.addEdge("Somagede", "Sumpiuh", 3500);
    g.addEdge("Somagede", "Somagede", 5000);
    g.addEdge("Somagede", "Kalibagor", 4500);
    g.addEdge("Somagede", "Banyumas", 3000);
    g.addEdge("Sumbang", "Purwokerto Utara", 2000);
    g.addEdge("Sumbang", "Purwokerto Timur", 2000);
    g.addEdge("Sumbang", "Kembaran", 7000);
    g.addEdge("Sumpiuh", "Tambak", 5000);
    g.addEdge("Sumpiuh", "Somagede", 5000);
    g.addEdge("Sumpiuh", "Kemrajen", 5000);
    g.addEdge("Tambak", "Sumpiuh", 3500);
    g.addEdge("Wangon", "Lumbir", 5000);
    g.addEdge("Wangon", "Purwojati", 5000);
    g.addEdge("Wangon", "Jatilawang", 5000);

    // Menampilkan daftar kecamatan dengan ID
    g.displayKecamatan();

    while (true) {
        int asalIdx, tujuanIdx;
        cout << "Masukkan nomor kecamatan asal (atau -1 untuk keluar): ";
        cin >> asalIdx;

        if (asalIdx == -1) {
            cout << "Sayonara :3." << endl;
            break;
        }

        cout << "Masukkan nomor kecamatan tujuan (atau -1 untuk keluar): ";
        cin >> tujuanIdx;

        if (tujuanIdx == -1) {
            cout << "Sayonara :3." << endl;
            break;
        }

        if (asalIdx < 0 || asalIdx >= kecamatanNames.size() || tujuanIdx < 0 || tujuanIdx >= kecamatanNames.size()) {
            cout << "Input kecamatan tidak valid! Silakan coba lagi.\n" << endl;
        } else {
            string asal = kecamatanNames[asalIdx];
            string tujuan = kecamatanNames[tujuanIdx];

            // Menentukan rute termurah dari kecamatan asal ke kecamatan tujuan
            g.dijkstra(asal, tujuan);
        }
    }

    return 0;
}
