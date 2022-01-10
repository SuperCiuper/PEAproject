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
selekcja - wybór genów (turniej, ruletka++)
krzyżowanie - jak się zamienia (losowy przedział, po pół) Pc
mutacja - losowa zamiana miast (inwersja, swap) Pm
sukcesja - usuwanie genów (usuwanie 1/4 genów, usuwanie wszystkich starych genów)
*/
#define MUTATION_PROBABILITY = 100

int n;
int connections[2000][2000];
vector<int> allVertexes;
vector<struct gene> population;

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

string mutatedGene(vector<int> gnome1, vector<int> gnome2)
{
    //random, at least 1


}

int getCost(vector<int> gnome)
{
    int cost = connections[0][gnome[0]];

    for (int q = 0; q < n - 2; ++q)
        cost += connections[gnome[q]][gnome[q + 1]];

    cost += connections[gnome[n - 2]][0];

    return cost;
}

bool lessthan(gene t1, gene t2)
{
    return t1.fitness < t2.fitness;
}

void findShortestPath()
{
    int POP_SIZE = n;
    int generation = 1;
    int depth = n;

    srand(time(0));

    struct gene temp;

    for (int q = 0; q < POP_SIZE; ++q) {
        vector<int>::iterator iter = allVertexes.begin();

        random_shuffle(iter, iter + allVertexes.size());

        temp.gnome = allVertexes;
        temp.fitness = getCost(temp.gnome);
        population.push_back(temp);
    }
    
    while (generation <= depth) {
        sort(population.begin(), population.end(), lessthan);
        cout << "\nCurrent depth: " << generation << "\n";
        vector<struct gene> new_population;

        for (int i = 0; i < POP_SIZE; i++) {
            //rand 2 genes
            
            int gene1 = rand() % (population.size() - 1);
            int gene2 = rand() % (population.size() - 1);
            while (gene1 == gene2)
                gene2 = rand() % (population.size() - 1);


            //merge them

            while (true) {
                string newGene = mutatedGene(population[gene1].gnome, population[gene2].gnome);
                struct gene new_gnome;
                new_gnome.gnome = new_g;
                new_gnome.fitness = getCost(new_gnome.gnome);

                if (new_gnome.fitness <= population[i].fitness) {
                    new_population.push_back(new_gnome);
                    break;
                }
                else {

                    // Accepting the rejected children at
                    // a possible probability above threshold.
                    float prob = pow(2.7,
                        -1 * ((float)(new_gnome.fitness
                            - population[i].fitness)
                            / temperature));
                    if (prob > 0.5) {
                        new_population.push_back(new_gnome);
                        break;
                    }
                }
            }
        }

        population = new_population;
        cout << "Generation " << generation << " \n";
        cout << "GNOME     FITNESS VALUE\n";

        for (int i = 0; i < POP_SIZE; i++)
            cout << population[i].gnome << " "
            << population[i].fitness << endl;
        generation++;
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
        allVertexes.push_back(q);
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
        findShortestPath();
    }
    auto resultTime = chrono::steady_clock::now() - startTime;

    //cout << bestCost << " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";

    while (1);
}