#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <bitset>
#include <bitset>

using namespace std;

struct result {
    long long mask;
    long long cost;
    long long currentBit;
};

int n;
int connections[5000][5000];
long long shortest_path;

long long maxMask = 1;
vector<long long> masks;
vector<result> results[50];
vector<result> currentMask;
result res;

long long bit_twiddle_permute(long long v) {
    long long t = (v | (v - 1)) + 1;
    long long w = t | ((((t & -t) / (v & -v)) >> 1) - 1);
    return w;
}

void getNewMasks(long long startMask)
{
    masks.clear();
    masks.push_back(startMask);
    for (long long n = bit_twiddle_permute(startMask); n > startMask; startMask = n, n = bit_twiddle_permute(startMask))
    {
        if (n > maxMask)
            break;
        masks.push_back(n);
    }
}

void findShortestPath()
{
    int source = 0;
    //with 0 size sets
    for (int q = 1; q < n; ++q)
    {
        res.cost = connections[source][q];
        res.mask = maxMask;
        res.currentBit = q;
        results[0].push_back(res);
        maxMask *= 2;
    }
    maxMask--;
    long long currentStartMask = 0;
    //when masks are needed
    for (int bitsOn = 1; bitsOn < n -1; ++bitsOn)
    {
        currentStartMask = currentStartMask * 2 + 1;
        getNewMasks(currentStartMask);

        for (int q = 0; q < results[bitsOn-1].size(); ++q)
        {
            cout << endl << results[bitsOn - 1][q].cost << " " << results[bitsOn - 1][q].currentBit << " " << results[bitsOn - 1][q].mask;
        }
        res.currentBit = 0;
        for (long long currentMask = 1; currentMask < maxMask; currentMask*=2)
        {
            res.currentBit++; //1, 2, 3, 4 || 0001, 0010, 0100, 1000
            for (long long q = 0; q < masks.size(); ++q)
            {

                if ((currentMask & masks[q]) != currentMask)
                {
                    res.cost = LLONG_MAX;
                    res.mask = (currentMask | masks[q]);

                    bitset<5> bset(res.mask);
                    bitset<5> bbset(masks[q]);
                    cout << endl << " " << bbset << "           " << "bit " << res.currentBit << " bla " << bset;

                    for (long long w=0; w < results[bitsOn - 1].size(); ++w)
                        if (results[bitsOn - 1][w].mask == masks[q])
                            res.cost = min(res.cost, (results[bitsOn - 1][w].cost + connections[results[bitsOn - 1][w].currentBit][res.currentBit]));
                        
                    results[bitsOn].push_back(res);
                }
            }
        }
        results[bitsOn - 1].clear();
    }
    for (int q = 0; q < results[n - 2].size(); ++q)
    {
        cout << endl << results[n - 2][q].cost << " " << results[n - 2][q].currentBit << " " << results[n - 2][q].mask;
    }
    for (long long w = 0; w < results[n - 2].size(); ++w)
        shortest_path = min(shortest_path, (results[n - 2][w].cost + connections[results[n - 2][w].currentBit][0]));
    //final round
}

int main()
{
    cout << "Gimme data" << endl;
    cin >> n;

    for (int q = 0; q < n; ++q)
    {
        for (int w = 0; w < n; ++w)
        {
            cin >> connections[q][w];
        }
    }

    auto startTime = chrono::steady_clock::now();

    for (int q = 0; q < 1; ++q)
    {
        cout << "XD" << endl;
        shortest_path = LLONG_MAX;
        findShortestPath();
    }
    auto result = chrono::steady_clock::now() - startTime;

    cout << shortest_path << endl;
    cout << chrono::duration <double, milli>(result).count() << "ms \n";

    return 0;
}
