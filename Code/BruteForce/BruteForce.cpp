#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <Windows.h>
#include <fstream>

using namespace std;

int n;
int connections[5000][5000];
int bestCost = INT_MAX;
int path_weight;
int w;
ifstream file;

stringstream read()
{
    string word;
    char x;
    word.clear();
    stringstream ss;
    int tni = 0;
    
    x = file.get();
    while (x == ' ')
        x = file.get();
    
    while (x != ' ' && !file.eof())
    {
        word = word + x;
        x = file.get();
    }
    ss << word;
    word.clear();
    return ss;
}

void findShortestPath()
{
    int source = 0;
    vector<int> nodes;
    for (int q = 0; q < n; ++q)
        if (q != source)
            nodes.push_back(q);
        
    do
    {
        path_weight = 0;
        w = source;
        for (int q = 0; q < n-1; ++q)
        {
            path_weight += connections[w][nodes[q]];
            if (path_weight >= bestCost)
                break;
            
            w = nodes[q];
        }
        path_weight += connections[w][source];

        bestCost = min(bestCost, path_weight);
    } while (next_permutation(nodes.begin(), nodes.end()));
}

int main()
{
    cout << "Data file should not have any empty space before first character in a row" << endl;
    file.open("XD.INI"); //{path to dataFile} {repeat count}
    string dataFile = read().str();
    int repeats = 0;
    int result = 0;
    read() >> repeats;
    file.close();
    file.open(dataFile);
    read() >> n;

    for (int q = 0; q < n; ++q)
    {
        for (int w = 0; w < n; ++w)
        {
            read() >> connections[q][w];
        }
    }
    cout << "\ngot it\n";
    read() >> result;
    auto startTime = chrono::steady_clock::now();

    for (int q = 0; q < repeats; ++q)
    {
        cout << "Iteration " << q << endl;
        int shortest_path = INT_MAX;
        findShortestPath();
    }
    auto resultTime = chrono::steady_clock::now() - startTime;

    cout << bestCost <<  " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";
    
    while (1);
}