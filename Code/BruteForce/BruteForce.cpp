//https://iq.opengenus.org/travelling-salesman-problem-brute-force/
//https://stackoverflow.com/questions/11703827/brute-force-algorithm-for-the-traveling-salesman-problem-in-java
#include <iostream>
#include <chrono>

using namespace std;

int n;
int connections[5000][5000];
int best = INT_MAX;
int act = 0;
int pathLenght = 0;

void bruteForce(int previousPoint)
{

	for (int q = 1; q <= n; ++q)
	{
		if (q != previousPoint)
		{
			act += connections[previousPoint][q];
			pathLenght++;

			cout << previousPoint << " " << q << " " << act << " " << pathLenght << endl;

			if (pathLenght == n)
			{
				cout << act << endl;
				if (act < best)
					best = act;
			}

			else
			{
				bruteForce(q);
			}

			act -= connections[previousPoint][q];
			pathLenght--;
		}
	}
}

int main()
{
	cout << "Gimme data\n";

	cin >> n;

	for (int q = 1; q <= n; ++q)
	{
		for (int w = 1; w <= n; ++w)
		{
			cin >> connections[q][w];
		}
	}

	auto startTime = chrono::steady_clock::now();

	bruteForce(4999);

	auto result = chrono::steady_clock::now() - startTime;

	cout << chrono::duration <double, milli> (result).count() << "ms \n";
	cout << best;
}