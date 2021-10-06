//https://iq.opengenus.org/travelling-salesman-problem-brute-force/
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

class brute_force
{
public:

    int shortest_path_sum(int** edges_list, int num_nodes)
    {
        int source = 0;
        vector<int> nodes;
        for (int i = 0; i < num_nodes; i++)
        {
            if (i != source)
            {
                nodes.push_back(i);
            }
        }
        int n = nodes.size();
        int shortest_path = INT_MAX;
        while (next_permutation(nodes.begin(), nodes.end()))
        {
            int path_weight = 0;

            int j = source;
            for (int i = 0; i < n; i++)
            {
                path_weight += edges_list[j][nodes[i]];
                j = nodes[i];
            }
            path_weight += edges_list[j][source];

            shortest_path = min(shortest_path, path_weight);
        }
        return shortest_path;
    }
};

int main()
{
    /// Getting the number of nodes and number of edges as input
    int num_nodes;
    cin >> num_nodes;

    /// creating a multi-dimensional array
    int** edges_list = new int* [num_nodes];
    for (int i = 0; i < num_nodes; i++)
    {
        edges_list[i] = new int[num_nodes];
        for (int j = 0; j < num_nodes; j++)
        {
            cin >> edges_list[i][j];
        }
    }

    cout << "XD";
    brute_force approach1;

    auto startTime = chrono::steady_clock::now();

    cout << approach1.shortest_path_sum(edges_list, num_nodes) << endl;

    auto result = chrono::steady_clock::now() - startTime;

    cout << chrono::duration <double, milli>(result).count() << "ms \n";

    return 0;
}