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
int connections[2000][2000];
int bestCost;
int currentCost;
ifstream file;
vector<int> bestPath;
vector<int> currentPath;
vector<int> nextPath;
pair<int,int>  bestChange;
queue<pair<int, int>> tabuList;
int tabuConnections[2000][2000];
int tabuMaxSize;
int nextCost;

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

void insertTabu()
{
	if (tabuList.size() == tabuMaxSize)
	{
		tabuConnections[tabuList.front().first][tabuList.front().second] = 0;
		tabuConnections[tabuList.front().second][tabuList.front().first] = 0;
		tabuList.pop();
	}
	tabuConnections[bestChange.first][bestChange.second] = 1;
	tabuConnections[bestChange.second][bestChange.first] = 1;
	tabuList.push(bestChange);
}

void clearTabu()
{
	for (int q = 0; q < 2000; ++q)
		for (int w = 0; w < 2000; ++w)
			tabuConnections[q][w] = 0;

	while (!tabuList.empty())
		tabuList.pop();
}

void randomSwap()
{
	int vertex1 = rand() % (n - 1);
	int vertex2 = rand() % (n - 1);
	while (vertex1 == vertex2)
		vertex2 = rand() % (n - 1);

	swap(currentPath[vertex1], currentPath[vertex2]);
}

void getBestNeighbour()
{
	int repeats = 0;

	for (int i = 0; i < sqrt(n); i++)
	{
		nextPath.clear();
		for (int q = 0; q < currentPath.size(); ++q)
			nextPath.push_back(currentPath[q]);

		int vertex1 = rand() % (n - 1);
		int vertex2 = rand() % (n - 1);
		while (vertex1 == vertex2)
			vertex2 = rand() % (n - 1);

		swap(nextPath[vertex1], nextPath[vertex2]);
		nextCost = getCost();
		if ((!tabuConnections[vertex1][vertex2] && !tabuConnections[vertex2][vertex1]) && nextCost < currentCost)
		{
			currentCost = nextCost;

			currentPath.clear();
			for (int q = 0; q < nextPath.size(); ++q)
				currentPath.push_back(nextPath[q]);

			bestChange = make_pair(vertex1, vertex2);
		}
	}
}

void findShortestPath()
{
	tabuMaxSize = (n * (n - 1))/ 2;
	clearTabu();
	srand(time(0));

	double totalTime = 0;
	int pathNotChanged = 0;

	for (int q = 0; q < n - 1; ++q)
		currentPath.push_back(q + 1);

	bestPath.clear();
	for (int q = 0; q < currentPath.size(); ++q)
		bestPath.push_back(currentPath[q]);

	nextPath.clear();
	for (int q = 0; q < currentPath.size(); ++q)
		nextPath.push_back(currentPath[q]);

	currentCost = getCost();
	bestCost = currentCost;

	clock_t begin = clock();
	while (totalTime < n)
	{
		getBestNeighbour();

		//for (int q = 0; q < currentPath.size(); ++q)
		//	cout << currentPath[q] << " ";
		//cout << "    " << currentCost << endl;

		if (currentCost < bestCost)
		{
			bestCost = currentCost;
			bestPath.clear();
			for (int q = 0; q < currentPath.size(); ++q)
				bestPath.push_back(currentPath[q]);

			pathNotChanged = 0;
		}
		else
		{
			pathNotChanged++;
			insertTabu();
		}

		if (pathNotChanged == 100)
		{
			for (int i = 0; i < 100; i++) 
				randomSwap();

			pathNotChanged = 0;
			clearTabu();
		}

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		totalTime = elapsed_secs;
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
        bestCost = INT_MAX;
        findShortestPath();
    }
    auto resultTime = chrono::steady_clock::now() - startTime;

    cout << bestCost << " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";

    while (1);
}