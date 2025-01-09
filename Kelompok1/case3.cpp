#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

const int INF = numeric_limits<int>::max();
const int MAX_NODES = 100;

class Graph 
{
private:
  int V;
  vector<vector<pair<int, int>>> adj;

public:
  Graph(int vertices)
  {
    V = vertices;
    adj.resize(V);
  }

  void addEdge(int from, int to, int cost)
  {

    adj[from].push_back(make_pair(to, cost));
    adj[to].push_back(make_pair(from, cost));
  }

  vector<int> shortestPath(int start)
  {
    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[start] = 0;
    pq.push(make_pair(0, start));
    while (!pq.empty())
    {
      int u = pq.top().second;
      pq.pop();

      for (auto &neighbor : adj[u])
      {
        int v = neighbor.first;
        int weight = neighbor.second;

        if (dist[v] > dist[u] + weight)
        {
          dist[v] = dist[u] + weight;
          pq.push(make_pair(dist[v], v));
        }
      }
    }

    return dist;
  }

  vector<int> calculateNodeTraffic()
  {
    vector<int> traffic(V, 0);

    for (int i = 0; i < V; i++)
    {
      traffic[i] = adj[i].size();
    }

    return traffic;
  }

  vector<int> alternativeRoute(int start, int avoid)
  {
    vector<vector<pair<int, int>>> temp_adj = adj;

    for (int i = 0; i < V; i++)
    {

      for (auto it = temp_adj[i].begin(); it != temp_adj[i].end();)
      {
        if (it->first == avoid)
        {
          it = temp_adj[i].erase(it);
        }
        else
        {
          ++it;
        }
      }
    }

    vector<int> dist(V, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    if (start != avoid)
    {
      dist[start] = 0;
      pq.push(make_pair(0, start));
    }

    while (!pq.empty())
    {
      int u = pq.top().second;
      pq.pop();

      for (auto &neighbor : temp_adj[u])
      {
        int v = neighbor.first;
        int weight = neighbor.second;

        if (v != avoid && dist[v] > dist[u] + weight)
        {
          dist[v] = dist[u] + weight;
          pq.push(make_pair(dist[v], v));
        }
      }
    }

    return dist;
  }
};

class LogisticsSystem
{
private:
  Graph *graph;
  vector<string> cityNames;

public:
  LogisticsSystem(int numCities)
  {
    graph = new Graph(numCities);
    cityNames.resize(numCities);
  }

  ~LogisticsSystem()
  {
    delete graph;
  }

  void setCityName(int index, string name)
  {
    cityNames[index] = name;
  }

  void addRoute(int from, int to, int cost)
  {
    graph->addEdge(from, to, cost);
  }

  void displayCityList()
  {
    cout << "\nDaftar Kota:" << endl;
    for (int i = 0; i < cityNames.size(); i++)
    {
      cout << (i + 1) << ". " << cityNames[i] << endl;
    }
  }

  void displayMenu()
  {
    cout << "\n=== Sistem Manajemen Rute Pengiriman ===" << endl;
    cout << "1. Tampilkan semua rute terpendek dari satu kota" << endl;
    cout << "2. Analisis traffic gudang" << endl;
    cout << "3. Cari rute alternatif (kondisi darurat)" << endl;
    cout << "4. Keluar" << endl;
    cout << "Pilihan: ";
  }

  void run()
  {
    int choice;
    do
    {
      displayMenu();
      cin >> choice;

      switch (choice)
      {
      case 1:
      {
        displayCityList();
        cout << "\nMasukkan nomor kota asal (1-" << cityNames.size() << "): ";
        int start;
        cin >> start;
        showAllRoutes(start - 1);
        break;
      }
      case 2:
        analyzeTraffic();
        break;
      case 3:
      {
        displayCityList();
        cout << "\nMasukkan nomor kota asal (1-" << cityNames.size() << "): ";
        int start;
        cin >> start;
        cout << "Masukkan nomor kota yang dihindari (1-" << cityNames.size() << "): ";
        int avoid;
        cin >> avoid;
        findAlternativeRoutes(start - 1, avoid - 1);
        break;
      }
      case 4:
        cout << "Terima kasih telah menggunakan sistem ini!" << endl;
        break;
      default:
        cout << "Pilihan tidak valid!" << endl;
      }
    } while (choice != 4);
  }

private:
  void showAllRoutes(int start)
  {
    vector<int> costs = graph->shortestPath(start);
    cout << "\nRute termurah dari " << cityNames[start] << ":" << endl;
    for (int i = 0; i < costs.size(); i++)
    {
      if (i != start)
      {
        cout << "Ke " << cityNames[i] << ": ";
        if (costs[i] == INF)
          cout << "Tidak terjangkau";
        else
          cout << costs[i];
        cout << endl;
      }
    }
  }

  void analyzeTraffic()
  {
    vector<int> traffic = graph->calculateNodeTraffic();
    cout << "\nAnalisis Traffic Gudang:" << endl;
    for (int i = 0; i < traffic.size(); i++)
    {
      cout << cityNames[i] << ": " << traffic[i] << " rute" << endl;
    }
  }

  void findAlternativeRoutes(int start, int avoid)
  {
    vector<int> altRoutes = graph->alternativeRoute(start, avoid);
    cout << "\nRute alternatif dari " << cityNames[start]
         << " (menghindari " << cityNames[avoid] << "):" << endl;

    for (int i = 0; i < altRoutes.size(); i++)
    {
      if (i != start && i != avoid)
      {
        cout << "Ke " << cityNames[i] << ": ";
        if (altRoutes[i] == INF)
          cout << "Tidak ada rute";
        else
          cout << altRoutes[i];
        cout << endl;
      }
    }
  }
};

int main()
{
  LogisticsSystem system(5);
  system.setCityName(0, "Jakarta");
  system.setCityName(1, "Bandung");
  system.setCityName(2, "Semarang");
  system.setCityName(3, "Surabaya");
  system.setCityName(4, "Malang");

  system.addRoute(0, 1, 10);
  system.addRoute(0, 2, 15);
  system.addRoute(1, 2, 5);
  system.addRoute(1, 3, 12);
  system.addRoute(2, 3, 8);
  system.addRoute(2, 4, 20);
  system.addRoute(3, 4, 7);

  system.run();

  return 0;
}
