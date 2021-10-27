#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <sstream>
#include <fstream>

using namespace std;

struct result {
    int mask;
    int cost;
    short int currentBit;
};

int n;
int connections[5000][5000];
int shortest_path;

long long maxMask;
vector<pair<int, pair<short int, int>>> results[50]; //mask, bit, cost
result res;
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
    //with 0 size sets
    for (int q = 1; q < n; ++q)
    {
        results[0].push_back(make_pair(maxMask, make_pair(q, connections[source][q])));
        maxMask *= 2;
    }
    maxMask--;
    long long currentStartMask = 0;
    //when masks are needed
    for (int bitsOn = 1; bitsOn < n -1; ++bitsOn)
    {
        currentStartMask = currentStartMask * 2 + 1;
        res.currentBit = 0;
        //cout << endl;
        //for (long long q = 0; q < results[bitsOn - 1].size(); ++q)
        //    cout << endl << " " << results[bitsOn - 1][q].first << " " << results[bitsOn - 1][q].second.first << " " << results[bitsOn - 1][q].second.second;

        sort(results[bitsOn - 1].begin(), results[bitsOn - 1].end());
        for (int currentMask = 1; currentMask < maxMask; currentMask*=2)
        {
            res.currentBit++;
            res.cost = INT_MAX;
            for (long long q = 0; q < results[bitsOn - 1].size(); ++q)
            {
                if ((currentMask & results[bitsOn - 1][q].first) != currentMask)
                {
                    res.cost = res.cost = min(res.cost, (results[bitsOn - 1][q].second.second + connections[results[bitsOn - 1][q].second.first][res.currentBit]));
                    res.mask = (currentMask | results[bitsOn - 1][q].first);

                    if (q + 1 < results[bitsOn - 1].size())
                    {
                        if ((currentMask | results[bitsOn - 1][q + 1].first) != res.mask)
                        {
                            results[bitsOn].push_back(make_pair(res.mask, make_pair(res.currentBit, res.cost)));
                            res.cost = INT_MAX;
                        }
                    }
                    else
                    {
                        results[bitsOn].push_back(make_pair(res.mask, make_pair(res.currentBit, res.cost)));
                        res.cost = INT_MAX;
                    }
                }
            }
        }
        vector<pair<int, pair<short int, int>>>().swap(results[bitsOn - 1]);
    }

    //cout << endl;
    //for (long long q = 0; q < results[n - 2].size(); ++q)
    //    cout << endl << " " << results[n - 2][q].first << " " << results[n - 2][q].second.first << " " << results[n - 2][q].second.second;


    //final round
    for (long long w = 0; w < results[n - 2].size(); ++w)
        shortest_path = min(shortest_path, (results[n - 2][w].second.second + connections[results[n - 2][w].second.first][0]));
    
    vector<pair<int, pair<short int, int>>>().swap(results[n - 2]);
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
        maxMask = 1;
        findShortestPath();
    }
    auto resultTime = chrono::steady_clock::now() - startTime;

    cout << shortest_path << " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";

    while (1);
}
