#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <queue>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

int n;
int connections[200][200];
int shortest_path;
int currentCost;
ifstream file;
vector<int> shortestPath;
vector<int> currentPath;
vector<int> nextPath;
uniform_real_distribution<double> distribution(0.0, 1.0);
default_random_engine generator;

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

int getCost()
{
    int cost = connections[0][nextPath[0]];

    for (int q = 0; q < n - 2; ++q)
        cost += connections[nextPath[q]][nextPath[q + 1]];

    cost += connections[nextPath[n - 2]][0];

    return cost;
}

void invert(int q, int w)
{
    if (w < q)
        swap(q, w);
    
    while (q < w)
    {
        swap(nextPath[q], nextPath[w]);
        q++;
        w--;
    }
}

void shuffle(int q, int w)
{
    if (w < q)
        swap(q, w);

    vector<int>::iterator iter = nextPath.begin();

    random_shuffle(iter + q, iter + w);
}

void findShortestPath()
{
    for (int q = 0; q < n - 1; ++q)
        currentPath.push_back(q + 1);

    nextPath.clear();
    for (int q = 0; q < currentPath.size(); ++q)
        nextPath.push_back(currentPath[q]);

    shortestPath.clear();
    for (int q = 0; q < currentPath.size(); ++q)
        shortestPath.push_back(currentPath[q]);

    shortest_path = getCost();
    currentCost = shortest_path;

    double temperature = pow(n, 2) * 1000.0;
    double startTemperature = temperature;
    double coolingRate = 0.99, absoluteTemperature = 0.00001;
    int vertex1, vertex2;
    int newCost;
    vector<int>::iterator iter;
    srand(time(0));

    while (temperature > absoluteTemperature)
    {
        nextPath.clear();
        for (int q = 0; q < currentPath.size(); ++q)
            nextPath.push_back(currentPath[q]);

        int vertex1 = rand() % (n - 1);
        int vertex2 = rand() % (n - 1);
        while (vertex1 == vertex2) 
            vertex2 = rand() % (n - 1);
        
        //version with single element swap
        //swapPath(vertex1, vertex2);
        
        //version with invert
        invert(vertex1, vertex2);

        //version with random shuffle
        //shuffle(vertex1, vertex2);

        newCost = getCost();
        if (currentCost > newCost || (exp((currentCost - newCost) / temperature)) > distribution(generator))
        {
            currentCost = newCost;
            currentPath.clear();
            for (int q = 0; q < nextPath.size(); ++q)
                currentPath.push_back(nextPath[q]);

            if (currentCost < shortest_path)
            {
                shortest_path = currentCost;
                shortestPath.clear();
                for (int q = 0; q < currentPath.size(); ++q)
                    shortestPath.push_back(currentPath[q]);
            }
        }
        if (currentCost > (shortest_path * 1.5))
        {
            currentCost = shortest_path;
            currentPath.clear();
            for (int q = 0; q < shortestPath.size(); ++q)
                currentPath.push_back(shortestPath[q]);
        }
        //cout << "vertex1 is " << vertex1 << " vertex2 is " << vertex2 << "\n";
        //cout << "new cost is " << newCost << " currentCost is " << currentCost << " shortest_path is " << shortest_path << " " << endl << endl;
        temperature = temperature * coolingRate;
    }
}

int main()
{
    cout << "Data file should not have any \' \' before first character in a row" << endl;
    file.open("XD.INI");
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
        shortest_path = INT_MAX;
        findShortestPath();
    }
    auto resultTime = chrono::steady_clock::now() - startTime;

    cout << shortest_path << " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";

    while (1);
}