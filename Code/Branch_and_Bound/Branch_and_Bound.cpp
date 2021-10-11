#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <utility>
#include <queue>

using namespace std;

int n;
int shortest_path;
vector<int> solution_list;

struct Node
{
    // stores the reduced matrix
    vector<vector<int>> oldConnections;
    int lowerBound;
    int vertex;
    int visitedSoFar;
};

// Function to reduce each row in such a way that
// there must be at least one zero in each row
void reduce_row(vector<vector<int>>& connections, vector<int>& row)
{
    for (int q = 0; q < n; ++q)
    {
        row.push_back(INT_MAX);
        for (int w = 0; w < n; ++w)
            if (connections[q][w] < row[q])
                row[q] = connections[q][w];
    }
    // reduce the minimum value from each element in each row
    for (int q = 0; q < n; ++q)
        for (int w = 0; w < n; ++w)
            if (connections[q][w] != INT_MAX && row[q] != INT_MAX)
                connections[q][w] -= row[q];
}
// Function to reduce each column in such a way that
// there must be at least one zero in each column
void reduce_column(vector<vector<int>>& connections, vector<int>& col)
{
    for (int q = 0; q < n; ++q)
        col.push_back(INT_MAX);

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
            if (connections[q][w] != INT_MAX && col[w] != INT_MAX)
                connections[q][w] -= col[w];
}

Node* newNode(vector<vector<int>>& oldConnections, int vertexesVisited, int parentVertex, int currentVertex)
{
    Node* node = new Node;

    // copy data from parent node to current node
    node->oldConnections = oldConnections;

    // Change all entries of row i and column j to infinity
    // skip for root node
    for (int q = 0; vertexesVisited != 0 && q < n; ++q)
    {
        node->oldConnections[parentVertex][q] = INT_MAX;
        node->oldConnections[q][currentVertex] = INT_MAX;
    }

    node->oldConnections[currentVertex].at(0) = INT_MAX;

    // set number of cities visited so far
    node->visitedSoFar = vertexesVisited;
    node->vertex = currentVertex;

    return node;
}

int calculateLowerBound(vector<vector<int>>& connections)
{
    int lowerBound = 0;

    vector<int> row;
    reduce_row(connections, row);

    vector<int> col;
    reduce_column(connections, col);

    // the total expected cost
    // is the sum of all reductions
    for (int q = 0; q < n; ++q)
        lowerBound += (row[q] != INT_MAX) ? row[q] : 0,
        lowerBound += (col[q] != INT_MAX) ? col[q] : 0;

    return lowerBound;
}

// Orders heap
struct heapComparator {
	bool operator()(const Node* leftNode, const Node* rightNode) const
	{
		return leftNode->lowerBound > rightNode->lowerBound;
	}
};

// Function to solve Traveling Salesman Problem using Branch and Bound
int findShortestPath(vector<vector<int>>& connections)
{
    // Create a priority queue to store live nodes of search tree;
    priority_queue<Node*, std::vector<Node*>, heapComparator> priorityQueue;

    Node* root = newNode(connections, 0, -1, 0);

    // get the lower bound of the path starting at node 0
    root->lowerBound = calculateLowerBound(root->oldConnections);
    priorityQueue.push(root);

    // Finds a live node with least cost, add its children to list of
    // live nodes and finally deletes it from the list
    while (!priorityQueue.empty())
    {
        Node* currentNode = priorityQueue.top();
        priorityQueue.pop();

        // if all vertexes are visited
        if (currentNode->visitedSoFar == n - 1)
            return currentNode->lowerBound;
        
        // do for each childNode of currentNode
        for (int q = 0; q < n; ++q)
        {
            if (currentNode->oldConnections[currentNode->vertex][q] != INT_MAX)
            {
                // create a child node and calculate its cost
                Node* child = newNode(currentNode->oldConnections, currentNode->visitedSoFar + 1, currentNode->vertex, q);

                child->lowerBound = currentNode->lowerBound + currentNode->oldConnections[currentNode->vertex][q] + calculateLowerBound(child->oldConnections);

                priorityQueue.push(child);
            }
        }

        //free up some space as all needed values are stored in children
        delete currentNode;
    }
    return 0;
}

int main()
{
    cout << "Gimme data" << endl;
    cin >> n;

    vector<vector<int>> connections(n, vector<int>(n));

    for (int q = 0; q < n; ++q)
    {
        for (int w = 0; w < n; ++w)
        {
            cin >> connections[q][w];

            if (q == w)
                connections[q][w] = INT_MAX;
        }
    }

    auto startTime = chrono::steady_clock::now();

    for (int q = 0; q < 1; ++q)
    {
        cout << "XD" << endl;
        shortest_path = INT_MAX;
        shortest_path = findShortestPath(connections);
    }
    auto result = chrono::steady_clock::now() - startTime;

    cout << shortest_path << endl;
    cout << chrono::duration <double, milli>(result).count() << "ms \n";

    return 0;
}
