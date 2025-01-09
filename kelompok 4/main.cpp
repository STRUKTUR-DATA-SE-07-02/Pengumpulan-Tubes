#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <string>
#include <set>

using namespace std;

#define INF INT_MAX
#define MAX_GEDUNG 5

// Struct untuk menyimpan informasi Edge
struct Edge {
    int to, weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

const string namaGedung[] = {"Rektorat", "DSP", "DC", "IoT", "TT"};
vector<Edge> adj[MAX_GEDUNG]; // Adjacency list untuk graph

// Fungsi untuk menambahkan edge (dua arah)
void tambahJalan(int from, int to, int weight) {
    adj[from].push_back(Edge(to, weight));
    adj[to].push_back(Edge(from, weight));
}

// Fungsi Dijkstra untuk mencari jarak terpendek dengan opsi mengabaikan edge tertentu
vector<int> dijkstra(int start, vector<int> &parent, set<pair<int, int>> &ignoredEdges) {
    vector<int> dist(MAX_GEDUNG, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    pq.push({0, start});
    dist[start] = 0;
    parent[start] = -1;

    while (!pq.empty()) {
        int currDist = pq.top().first;
        int currNode = pq.top().second;
        pq.pop();

        if (currDist > dist[currNode]) continue;

        for (Edge &edge : adj[currNode]) {
            int nextNode = edge.to;
            int nextDist = currDist + edge.weight;

            // Cek apakah edge termasuk dalam ignoredEdges
            if (ignoredEdges.count({currNode, nextNode}) || ignoredEdges.count({nextNode, currNode})) {
                continue; // Abaikan edge ini
            }

            if (nextDist < dist[nextNode]) {
                dist[nextNode] = nextDist;
                pq.push({nextDist, nextNode});
                parent[nextNode] = currNode;
            }
        }
    }
    return dist;
}

// Fungsi untuk mencetak rute
void printRute(int dest, vector<int> &parent) {
    if (parent[dest] == -1) {
        cout << namaGedung[dest];
        return;
    }
    printRute(parent[dest], parent);
    cout << " -> " << namaGedung[dest];
}

// Fungsi untuk mencari gedung tersibuk (node dengan koneksi terbanyak)
void cariGedungTersibuk() {
    int maxDegree = 0, gedungTersibuk = -1;

    for (int i = 0; i < 5; i++) {
        int degree = adj[i].size();
        if (degree > maxDegree) {
            maxDegree = degree;
            gedungTersibuk = i;
        }
    }

    cout << "\nGedung tersibuk adalah: " << namaGedung[gedungTersibuk] << " dengan " << maxDegree << " koneksi.\n";
}

// Fungsi untuk menampilkan peta (koneksi antar gedung dan jarak)
void tampilkanPeta() {
    cout << "\n=== Peta Koneksi Antar Gedung ===\n";
    for (int i = 0; i < 5; i++) {
        cout << namaGedung[i] << " terhubung dengan:\n";
        for (const Edge &edge : adj[i]) {
            cout << "  - " << namaGedung[edge.to] << " (Jarak: " << edge.weight << " meter)\n";
        }
        cout << endl;
    }
}

int main() {
    // Menambahkan koneksi antar gedung
    tambahJalan(2, 3, 5);  // DC <-> IoT
    tambahJalan(2, 0, 12); // DC <-> Rektorat
    tambahJalan(3, 4, 7);  // IoT <-> TT
    tambahJalan(3, 0, 4);  // IoT <-> Rektorat
    tambahJalan(4, 0, 9);  // TT <-> Rektorat
    tambahJalan(0, 1, 10); // Rektorat <-> DSP

    int pilihan;
    while (true) {
        cout << "\nPilih opsi:\n";
        cout << "1. Mencari gedung tersibuk\n";
        cout << "2. Mencari rute tercepat\n";
        cout << "3. Menampilkan peta\n";
        cout << "4. Keluar\n";
        cin >> pilihan;

        if (pilihan == 1) {
            cariGedungTersibuk();
        }
        else if (pilihan == 2) {
            int start, tujuan;
            cout << "Pilih gedung awal:\n";
            for (int i = 0; i < 5; i++) cout << i << ". " << namaGedung[i] << endl;
            cin >> start;

            cout << "Pilih gedung tujuan:\n";
            for (int i = 0; i < 5; i++) cout << i << ". " << namaGedung[i] << endl;
            cin >> tujuan;

            set<pair<int, int>> ignoredEdges; // Menyimpan edge yang ditolak
            bool ruteDitemukan = false;

            while (!ruteDitemukan) {
                vector<int> parent(MAX_GEDUNG);
                vector<int> jarak = dijkstra(start, parent, ignoredEdges);

                if (jarak[tujuan] == INF) {
                    cout << "\nTidak ada rute yang tersedia!\n";
                    break;
                }

                // Tampilkan rute terpendek
                cout << "\nJarak dari " << namaGedung[start] << " ke " << namaGedung[tujuan] << " adalah " << jarak[tujuan] << " meter.\n";
                cout << "Rute: ";
                printRute(tujuan, parent);
                cout << endl;

                // Tanya user apakah ingin mengambil rute
                char ambilRute;
                cout << "Apakah Anda ingin mengambil rute ini? (y/n): ";
                cin >> ambilRute;

                if (ambilRute == 'y' || ambilRute == 'Y') {
                    cout << "Rute diterima. Selamat sampai tujuan!\n";
                    ruteDitemukan = true;
                } else {
                    // Tandai edge yang sudah dilewati sebagai ignoredEdges
                    cout << "Mencari rute alternatif...\n";
                    int curr = tujuan;
                    while (parent[curr] != -1) {
                        ignoredEdges.insert({parent[curr], curr});
                        ignoredEdges.insert({curr, parent[curr]}); // Edge dua arah
                        curr = parent[curr];
                    }
                }
            }
        }
        else if (pilihan == 3) {
            tampilkanPeta();
        }
        else if (pilihan == 4) {
            cout << "Keluar dari program. Sampai jumpa!\n";
            break;
        }
        else {
            cout << "Pilihan tidak valid. Coba lagi.\n";
        }
    }

    return 0;
}
