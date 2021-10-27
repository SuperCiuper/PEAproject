#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <queue>
#include <sstream>
#include <fstream>

using namespace std;

int n;
int shortest_path;
vector<int> solution_list;
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

struct Node
{
    // stores the reduced matrix
    vector<vector<short int>> connections;
    int lowerBound;
    int vertex;
    int vertexesVisited;
};

// Function to reduce each row in such a way that
// there must be at least one zero in each row
void reduceRow(vector<vector<short int>>& connections, vector<short int>& row)
{
    for (int q = 0; q < n; ++q)
    {
        row.push_back(SHRT_MAX);
        for (int w = 0; w < n; ++w)
            if (connections[q][w] < row[q])
                row[q] = connections[q][w];
    }
    // reduce the minimum value from each element in each row
    for (int q = 0; q < n; ++q)
        for (int w = 0; w < n; ++w)
            if (connections[q][w] != SHRT_MAX && row[q] != SHRT_MAX)
                connections[q][w] -= row[q];
}
// Function to reduce each column in such a way that
// there must be at least one zero in each column
void reduceColumn(vector<vector<short int>>& connections, vector<short int>& col)
{
    for (int q = 0; q < n; ++q)
        col.push_back(SHRT_MAX);

    // col[j] contains minimum in col j
    for (int q = 0; q < n; ++q)
    {
        for (int w = 0; w < n; ++w)
            if (connections[q][w] < col[w])
                col[w] = connections[q][w];
    }
    // reduce the minimum value from each element in each column
    for (int q = 0; q < n; ++q)
        for (int w = 0; w < n; ++w)
            if (connections[q][w] != SHRT_MAX && col[w] != SHRT_MAX)
                connections[q][w] -= col[w];
}

int calculateLowerBound(vector<vector<short int>>& connections)
{
    int lowerBound = 0;

    vector<short int> row;
    reduceRow(connections, row);

    vector<short int> col;
    reduceColumn(connections, col);

    // the total expected cost
    // is the sum of all reductions
    for (int q = 0; q < n; ++q)
        lowerBound += (row[q] != SHRT_MAX) ? row[q] : 0,
        lowerBound += (col[q] != SHRT_MAX) ? col[q] : 0;

    vector<short int>().swap(row);
    vector<short int>().swap(col);
    return lowerBound;
}

Node* newNode(vector<vector<short int>>& parentConnections, int vertexesVisited, int parentVertex, int currentVertex)
{
    Node* node = new Node;

    // copy data from parent node to current node
    node->connections = parentConnections;

    // Change all entries of row i and column j to infinity
    // skip for root node
    for (int q = 0; vertexesVisited != 0 && q < n; ++q)
    {
        node->connections[parentVertex][q] = SHRT_MAX;
        node->connections[q][currentVertex] = SHRT_MAX;
    }

    node->connections[currentVertex][0] = SHRT_MAX;

    // set number of cities visited so far
    node->vertexesVisited = vertexesVisited;
    node->vertex = currentVertex;

    return node;
}

// Orders heap
struct heapComparator {
	bool operator()(const Node* leftNode, const Node* rightNode) const
	{
		return leftNode->lowerBound > rightNode->lowerBound;
	}
};

priority_queue<Node*, std::vector<Node*>, heapComparator> priorityQueue;

// Function to solve Traveling Salesman Problem using Branch and Bound
int findShortestPath(vector<vector<short int>>& connections)
{
    Node* root = newNode(connections, 0, -1, 0);

    // get the lower bound of the path starting at node 0
    root->lowerBound = calculateLowerBound(root->connections);
    priorityQueue.push(root);

    // Finds a live node with least cost, add its children to list of
    // live nodes and finally deletes it from the list
    while (!priorityQueue.empty())
    {
        Node* currentNode = priorityQueue.top();
        priorityQueue.pop();

        // if all vertexes are visited
        if (currentNode->vertexesVisited == n - 1)
            return currentNode->lowerBound;
        
        // do for each childNode of currentNode
        for (int q = 0; q < n; ++q)
        {
            if (currentNode->connections[currentNode->vertex][q] != SHRT_MAX)
            {
                // create a child node and calculate its cost
                Node* child = newNode(currentNode->connections, currentNode->vertexesVisited + 1, currentNode->vertex, q);

                child->lowerBound = currentNode->lowerBound + currentNode->connections[currentNode->vertex][q] + calculateLowerBound(child->connections);

                priorityQueue.push(child);
            }
        }

        //free up some space as all needed values are stored in children
        delete currentNode;
    }
    return SHRT_MAX;
}

int main()
{
    file.open("XD.INI");
    string dataFile = read().str();
    int repeats = 0;
    int result = 0;
    read() >> repeats;
    file.close();
    file.open(dataFile);
    read() >> n;
    vector<vector<short int>> connections(n, vector<short int>(n));

    for (int q = 0; q < n; ++q)
    {
        for (int w = 0; w < n; ++w)
        {
            read() >> connections[q][w];

            if (q == w)
                connections[q][w] = SHRT_MAX;
        }
    }
    cout << "\ngot it\n";
    read() >> result;

    auto startTime = chrono::steady_clock::now();
    auto resultTime = chrono::steady_clock::now() - startTime;

    for (int q = 0; q < repeats; ++q)
    {
        cout << "Iteration " << q << endl;
        shortest_path = SHRT_MAX;
        startTime = chrono::steady_clock::now();
        shortest_path = findShortestPath(connections);
        resultTime = resultTime + chrono::steady_clock::now() - startTime;
        while (!priorityQueue.empty())
        {
            Node* currentNode = priorityQueue.top();
            priorityQueue.pop();
            delete currentNode;
        }
    }
    
    cout << shortest_path << " - expected: " << result << endl;
    cout << chrono::duration <double, milli>(resultTime).count() << "ms \n";

    while (1);
}
