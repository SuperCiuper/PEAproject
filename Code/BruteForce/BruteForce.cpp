#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

int n;
int connections[5000][5000];
int shortest_path = INT_MAX;


void findShortestPath()
{
    int source = 1;
    vector<int> nodes;
    for (int q = 1; q <= n; ++q)
        if (q != source)
            nodes.push_back(q);
        
        
    while (next_permutation(nodes.begin(), nodes.end()))
    {
        int path_weight = 0;

        int w = source;
        for (int q = 0; q < n-1; ++q)
        {
            path_weight += connections[w][nodes[q]];
            w = nodes[q];
        }
        path_weight += connections[w][source];

        shortest_path = min(shortest_path, path_weight);
    }
}

int main()
{
    cout << "Gimme data" << endl;
    cin >> n;

    for (int q = 1; q <= n; ++q)
    {
        for (int w = 1; w <= n; ++w)
        {
            cin >> connections[q][w];
        }
    }

    auto startTime = chrono::steady_clock::now();

    for (int q = 0; q < 3; ++q)
    {
        cout << "XD" << endl;
        int shortest_path = INT_MAX;
        findShortestPath();
    }
    auto result = chrono::steady_clock::now() - startTime;

    cout << shortest_path << endl;
    cout << chrono::duration <double, milli>(result).count() << "ms \n";
    
    return 0;
}