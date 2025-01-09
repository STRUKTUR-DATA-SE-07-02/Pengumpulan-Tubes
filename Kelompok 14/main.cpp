#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int to, weight;
};

// Fungsi untuk menemukan jalur terpendek menggunakan Dijkstra
void findShortestPath(vector<vector<Edge>> &graph, int start, int end, vector<int> &shortestPath, int &totalCost) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int cost = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (cost > dist[current]) continue;

        for (Edge &e : graph[current]) {
            int next = e.to;
            int weight = e.weight;

            if (dist[current] + weight < dist[next]) {
                dist[next] = dist[current] + weight;
                prev[next] = current;
                pq.push({dist[next], next});
            }
        }
    }

    // Menyimpan jalur terpendek
    shortestPath.clear();
    totalCost = dist[end];
    if (dist[end] == INT_MAX) {
        cout << "Tidak ada jalur dari " << start << " ke " << end << endl;
        return;
    }

    for (int at = end; at != -1; at = prev[at]) {
        shortestPath.push_back(at);
    }
    reverse(shortestPath.begin(), shortestPath.end());
}

// Fungsi untuk menemukan jalur alternatif (jalur kedua terbaik)
void findAlternativePath(vector<vector<Edge>> &graph, int start, int end, vector<int> &altPath, int &altCost, vector<int> &shortestPath, int shortestCost) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int cost = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (cost > dist[current]) continue;

        for (Edge &e : graph[current]) {
            int next = e.to;
            int weight = e.weight;

            // Abaikan jalur terpendek
            if (find(shortestPath.begin(), shortestPath.end(), current) != shortestPath.end() &&
                find(shortestPath.begin(), shortestPath.end(), next) != shortestPath.end())
                continue;

            if (dist[current] + weight < dist[next]) {
                dist[next] = dist[current] + weight;
                prev[next] = current;
                pq.push({dist[next], next});
            }
        }
    }

    // Menyimpan jalur alternatif
    altPath.clear();
    altCost = dist[end];
    if (dist[end] == INT_MAX || altCost <= shortestCost) {
        altCost = INT_MAX;
        return;
    }

    for (int at = end; at != -1; at = prev[at]) {
        altPath.push_back(at);
    }
    reverse(altPath.begin(), altPath.end());
}

// Fungsi untuk menampilkan daftar kota
void displayCities() {
    cout << "Daftar Kota:\n";
    cout << "0: Godean\n";
    cout << "1: Sleman\n";
    cout << "2: Depok\n";
    cout << "3: Kalasan\n";
    cout << "4: Bantul\n";
    cout << "5: Imogiri\n";
}

int main() {
    // Graph berdasarkan gambar
    vector<vector<Edge>> graph(6);
    graph[0].push_back({1, 6}); // Godean -> Sleman
    graph[0].push_back({4, 3}); // Godean -> Bantul
    graph[1].push_back({2, 3}); // Sleman -> Depok
    graph[1].push_back({5, 9}); // Sleman -> Imogiri
    graph[2].push_back({0, 9}); // Depok -> Godean
    graph[2].push_back({5, 3}); // Depok -> Imogiri
    graph[3].push_back({2, 4}); // Kalasan -> Depok
    graph[3].push_back({4, 9}); // Kalasan -> Bantul
    graph[4].push_back({2, 5}); // Bantul -> Depok
    graph[4].push_back({5, 2}); // Bantul -> Imogiri
    graph[5].push_back({3, 8}); // Imogiri -> Kalasan

    vector<int> shortestPath, altPath;
    int shortestCost, altCost;
    int choice;

    do {
        cout << "\n=== Menu Utama ===\n";
        cout << "1. Tampilkan Daftar Kota\n";
        cout << "2. Cari Jalur Terpendek\n";
        cout << "3. Cari Jalur Alternatif\n";
        cout << "4. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayCities();
                break;

            case 2: {
                int start, end;
                cout << "Masukkan kota awal (kode): ";
                cin >> start;
                cout << "Masukkan kota tujuan (kode): ";
                cin >> end;

                findShortestPath(graph, start, end, shortestPath, shortestCost);

                if (!shortestPath.empty()) {
                    cout << "Jalur terpendek: ";
                    for (int i = 0; i < shortestPath.size(); ++i) {
                        if (i > 0) cout << " -> ";
                        cout << shortestPath[i];
                    }
                    cout << "\nTotal biaya: " << shortestCost << endl;
                }
                break;
            }

            case 3: {
                int start, end;
                cout << "Masukkan kota awal (kode): ";
                cin >> start;
                cout << "Masukkan kota tujuan (kode): ";
                cin >> end;

                findShortestPath(graph, start, end, shortestPath, shortestCost);
                findAlternativePath(graph, start, end, altPath, altCost, shortestPath, shortestCost);

                if (!altPath.empty() && altCost != INT_MAX) {
                    cout << "Jalur alternatif: ";
                    for (int i = 0; i < altPath.size(); ++i) {
                        if (i > 0) cout << " -> ";
                        cout << altPath[i];
                    }
                    cout << "\nTotal biaya: " << altCost << endl;
                } else {
                    cout << "Tidak ada jalur alternatif yang tersedia.\n";
                }
                break;
            }

            case 4:
                cout << "Keluar dari aplikasi. Terima kasih!\n";
                break;

            default:
                cout << "Pilihan tidak valid. Coba lagi.\n";
                break;
        }
    } while (choice != 4);

    return 0;
}
