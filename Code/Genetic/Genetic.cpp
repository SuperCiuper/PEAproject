#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <sstream>
#include <fstream>
#include <limits.h>

using namespace std;
/*
selekcja - wybór genów (turniej)
krzyżowanie - jak się zamienia (losowy przedział, po pół) Pc
mutacja - losowa zamiana miast (inwersja, swap) Pm
sukcesja - usuwanie genów (usuwanie 1/4 genów, usuwanie wszystkich starych genów)
*/
#define MUTATION_PROBABILITY 100
#define TOURNAMENT_VERTEXES 20

int n;
int connections[2000][2000];
vector<int> allVertexes;
vector<struct gene> population;
int bestCost;
int result;
int gene1;
int gene2;

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

struct gene {
    vector<int> gnome;
    int fitness;
};

bool lessthanSelect(pair<gene, int> t1, pair<gene, int> t2)
{
    return t1.first.fitness < t2.first.fitness;
}

bool lessthan(gene t1, gene t2)
{
    return t1.fitness < t2.fitness;
}

int getCost(vector<int> gnome)
{
    int cost = connections[0][gnome[0]];
    
    for (int q = 0; q < n - 2; ++q) {
        cost += connections[gnome[q]][gnome[q + 1]];
    }
    cost += connections[gnome[n - 2]][0];

    return cost;
}

void selectGenes() {
    //turniej
    vector<pair<gene, int>> randGenes;
    
    for (int q = 0; q < max(5, n / TOURNAMENT_VERTEXES); ++q) {
        int position = rand() % population.size();
        randGenes.push_back(make_pair(population[position], position));
    }
    sort(randGenes.begin(), randGenes.end(), lessthanSelect);
    
    gene1 = randGenes[0].second;
    gene2 = randGenes[1].second;

}

vector<int> crossGene(vector<int> gnome1, vector<int> gnome2)
{
    //half
    // int breakPoint1 = 0;
    // int breakPoint1 = gnome1.size()/2;
    //random, at least 1
    int breakPoint1 = rand() % (gnome1.size()/2);
    int breakPoint2 = rand() % (gnome1.size()/2 - breakPoint1) + gnome1.size()/2 + breakPoint1;

    vector<int> newGnome = { gnome1.begin() + breakPoint1, gnome1.begin() + breakPoint2 };
    
    for (int q = 0; q < gnome2.size(); ++q) {
        if (!count(newGnome.begin(), newGnome.end(), gnome2[q])) {
            newGnome.push_back(gnome2[q]);
        }
    }

    return newGnome;
}

vector<int> mutateGene(vector<int> gnome)
{
    int mutation = rand() % MUTATION_PROBABILITY;

    if (mutation == 0) {
        
        int gene1 = rand() % (gnome.size() - 1);
        int gene2 = rand() % (gnome.size() - 1);
        while (gene1 == gene2)
            gene2 = rand() % (gnome.size() - 1);

        //swap
        swap(gnome[gene1], gnome[gene2]);

        //invert
        /*
        if (gene2 < gene1)
            swap(gene1, gene2);

        vector<int>::iterator iter = gnome.begin();
        random_shuffle(iter + gene1, iter + gene2);
        */
    }

    return gnome;
}

void succession(vector<gene> new_population) {
    //delete old population
    population = new_population;
    
    //delete 50% population
    /*
    int size = population.size() / 2;
    for (int q = 0; q < size; ++q) {
        population.pop_back();
    }

    copy(new_population.begin(), new_population.end(), back_inserter(population));
    */
    sort(population.begin(), population.end(), lessthan);
}

void findShortestPath()
{
    bestCost = INT_MAX;
    int POP_SIZE = max(n, 100);

    srand(time(0));

    struct gene temp;

    for (int q = 0; q < POP_SIZE; ++q) {
        vector<int>::iterator iter = allVertexes.begin();

        random_shuffle(iter, iter + allVertexes.size());

        temp.gnome = allVertexes;
        temp.fitness = getCost(temp.gnome);
        population.push_back(temp);
    }
    sort(population.begin(), population.end(), lessthan);
    
    auto startTime = chrono::steady_clock::now();
    auto resultTime = chrono::steady_clock::now() - startTime;

    while (chrono::duration <double>(resultTime).count() <= 600 && (bestCost - result) > 0) {
        vector<gene> new_population;

        for (int q = 0; q < POP_SIZE; ++q) {
            //selection
            selectGenes();

            //merge them
            gene newGene = gene();
            newGene.gnome = crossGene(population[gene1].gnome, population[gene2].gnome);

            newGene.gnome = mutateGene(newGene.gnome);
            newGene.fitness = getCost(newGene.gnome);

            new_population.push_back(newGene);
        }

        succession(new_population);
        bestCost = min(population[0].fitness, bestCost);

        resultTime = chrono::steady_clock::now() - startTime;
    }
}

int main()
{
    /*
    cout << "Data file should not have any \' \' before first character in a row" << endl;
    file.open("XD.INI");
    string dataFile = read().str();
    int repeats = 0;
    int result = 0;
    int totalBestCost = 0;

    read() >> repeats;
    file.close();
    file.open(dataFile);
    read() >> n;
    */
    for(int e = 0; e< 9; ++e){
        cout << "Data file should not have any \' \' before first character in a row" << endl;
        string dataFile[9] = {"m15.txt", "gr21.txt", "gr48.txt", "gr96.txt", "lin105.txt", "gr137.txt", "gr202.txt", "lin318.txt", "gr431.txt"};
        int repeats = 1;
        result = 0;
        cout << dataFile[e]<<endl;

        file.open("..\\..\\data\\" + dataFile[e]);
        read() >> n;
        for (int q = 0; q < n; ++q)
        {
            allVertexes.push_back(q);
            for (int w = 0; w < n; ++w)
            {
                read() >> connections[q][w];
            }
        }
        allVertexes.erase(allVertexes.begin());

        cout << "\ngot it\n";
        read() >> result;
        auto startTime = chrono::steady_clock::now();

        for (int q = 0; q < repeats; ++q)
        {
            cout << "Iteration " << q << endl;
            findShortestPath();
            population.clear();
        }
        auto resultTime = chrono::steady_clock::now() - startTime;

        double bestCostDiff = ((double)bestCost - (double)result) * (double)100 / (double)result;
        cout << bestCost << " - expected: " << result << " difference: " << bestCostDiff << endl;
        cout << chrono::duration <double>(resultTime).count() / repeats << "s \n";

        file.close();
        population.clear();
        allVertexes.clear();
    }
    while (1);
}