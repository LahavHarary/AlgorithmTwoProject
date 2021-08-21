#include <iostream>
#include <time.h>
#include <list>
#include <fstream>
using namespace std;

/*Data structure*/
class Graph
{
	/*Public attributes of Graph*/
	public:
	int V = 1000;
	list<int> l[1000];
	int length_from_vertex[1000];
	bool visited[1000];

	/*Ctor of Graph*/
	Graph()
	{
		for (int i = 0; i < V; i++)
		{
			length_from_vertex[i] = 0;
			visited[i] = false;
		}
	}

	/*
	A function that adds an edge to a graph
	NOTE: the function adds x to y and y to x 
	meaning that x is connected to y and y is connected to x
	because the Graph is a Connected graph 
	*/
	void addEdge(int x, int y)
	{
		l[x].push_back(y);
		l[y].push_back(x);
	}
	/*
		Grpah has a function named BFS 
		The function helps us calculate the diam and the connectivity of the graph.
	*/
	int BFS(int source)
	{
		// Create a queue for BFS
		list<int> queue;

		// Mark the current node as visited and enqueue it
		visited[source] = true;
		queue.push_back(source);

		// iterator is used to get all adjacent vertices of a vertex
		list<int>::iterator i;

		while (!queue.empty())
		{
			// Dequeue a vertex from queue and place its value into source
			source = queue.front();
			queue.pop_front();

			/*
			Get all adjacent vertices of the dequeued vertex "source":
			If an adjacent has not been visited, then mark it visited and enqueue it
			later on get the current length from the original source.
			*/
			for (i = l[source].begin(); i != l[source].end(); ++i)
			{
				if (!visited[*i])
				{
					visited[*i] = true;
					queue.push_back(*i);
					length_from_vertex[*i] = length_from_vertex[source] + 1;
				}
			}
		}

		// find max length inside length_from_vertex array.
		int max_length = length_from_vertex[0];
		for (int j = 1; j < V; j++)
		{
			if (max_length < length_from_vertex[j])
			{
				max_length = length_from_vertex[j];
			}
		}

		// clear queue
		queue.clear();

		// return the max length of the original source (meaning it isn't necessarily the diameter)
		return max_length;
	}
};

/*Part A of matala*/
Graph build_random_graph(double p, int V);
int diameter(Graph g);
int is_isolated(Graph g);
int connectivity(Graph g);

/*Part B of matala*/
void write_data_to_file(int* res1, int* res2, int* res3, int res_array_size, int amount_of_graphs);
void mainFlow();

void main()
{
	mainFlow();
	system("pause");
}

/*Part B of matala*/
void mainFlow()
{
	/*
		A function which is responsible of Part B.

		first, we create a srand (seed of random).

		second, we create three arrays which contains values below and above the threshold 
		(for your convenience the values are marked at the end of every thershold).

		third, we create three integer arrays and set them all to 0 (we will use them later as our
		solution arrays).

		fourth, few variables which are needed: (V, first_parameter, second_parameter, amount_of_graphs)
		amount_of_graphs = needless to say more
		first_parameter = is used inside our algo part as a counter in "Conectivity" and "Lonely Vertex"
		first_parameter is used differently in "Diameter" saves the result from diameter function.
		second_parameter = is used as a counter for "Diameter" part.
		"counter" means that an attribute from "matala" is positive.

		fifth, Algorithm part: we ran 1000 graphs for each p in order to check if attributes from 
		"matala" has happend.
		more details on Conectivity, Diameter Lonely and Vertex are inside the functions: 
		"diameter", "is_isolated", "connectivity" so as how we are building our graphs in 
		"build_random_graph".

		sixth, we are creating a csv file with write_data_to_file.
	*/

	srand((unsigned)time(0));

	double Threshold1Array[] = { 0.001,0.002,0.003,0.004,0.0069,0.0071,0.009,0.01,0.5,0.65 }; //0.007
	double Threshold2Array[] = { 0.002,0.006,0.008,0.08,0.1,0.119,0.2,0.21,0.22,0.23 }; //0.118
	double Threshold3Array[] = { 0.0015,0.0025,0.0035,0.0045,0.00695,0.00715,0.0095,0.015,0.55,0.655 }; //0.007

	int V = 1000, first_parameter, second_parameter, amount_of_graphs = 1000;
	double p;

	// res arrays that wil be printed in the end of the program.
	int res1[10] = { 0 };
	int res2[10] = { 0 };
	int res3[10] = { 0 };

	//ALGO
	/*********************Conectivity*****************************/
	for (int i = 0; i < 10; i++)
	{
		first_parameter = 0;
		p = Threshold1Array[i];

		for (int j = 0; j < amount_of_graphs; j++)
		{
			Graph G = build_random_graph(p, V);

			if (connectivity(G))
			{
				first_parameter++;
			}
		}

		cout << "connectivity: " << i << endl;
		res1[i] = first_parameter;
	}

	/*********************Diameter*****************************/
	for (int i = 0; i < 10; i++)
	{
		second_parameter = 0;
		p = Threshold2Array[i];

		for (int j = 0; j < amount_of_graphs; j++)
		{
			Graph G = build_random_graph(p, V);

			first_parameter = diameter(G);
			if (first_parameter < 3)
			{
				second_parameter++;
			}
		}

		cout << "Diameter: " << i << endl;
		res2[i] = second_parameter;
	}

	/*********************Lonely Vertex*****************************/
	for (int i = 0; i < 10; i++)
	{
		first_parameter = 0;
		p = Threshold3Array[i];

		for (int j = 0; j < amount_of_graphs; j++)
		{
			Graph G = build_random_graph(p, V);

			if (is_isolated(G))
			{
				first_parameter++;
			}

			//delete G;
		}

		cout << "Lonely Vertex: " << i << endl;
		res3[i] = first_parameter;
	}

	write_data_to_file(res1, res2, res3, 10, amount_of_graphs);
}
void write_data_to_file(int* res1, int* res2, int* res3, int res_array_size, int amount_of_graphs)
{
	/*
	Printing our thresholds as a permanent strings (because we aren't chaning them)
	using res1 as a result for the first attribute
	res2 as a result for the second attribute
	res3 a result for the third attribute
	*/

	std::ofstream myfile;
	myfile.open("Graphs.csv");

	// first attribute
	myfile << "first attribute:\n" << "p, 0.001,0.002,0.003,0.004,0.0069,0.0071,0.009,0.01,0.5,0.65,\n" << "connected,";
	for (int j = 0; j < 10; j++)
	{
		myfile << res1[j] << ",";
	}
	myfile << "\n" << "not connected,";
	for (int j = 0; j < 10; j++)
	{
		myfile << (amount_of_graphs - res1[j]) << ",";
		//myfile << ",";
	}
	myfile << "\n" << "\n";

	// second attribute
	myfile << "second attribute:\n" << "p, 0.002,0.006,0.008,0.08,0.1,0.119,0.2,0.21,0.22,0.23,\n" << "less or equals two,";
	for (int j = 0; j < 10; j++)
	{
		myfile << res2[j] << ",";
	}
	myfile << "\n" << "greater then two,";
	for (int j = 0; j < 10; j++)
	{
		myfile << (amount_of_graphs - res2[j]) << ",";
		//myfile << ",";
	}
	myfile << "\n" << "\n";

	// third attribute
	myfile << "third attribute:\n" << "p, 0.0015,0.0025,0.0035,0.0045,0.00695,0.00715,0.0095,0.015,0.55,0.655,\n" << "has lonely vertex,";
	for (int j = 0; j < 10; j++)
	{
		myfile << res3[j] << ",";
	}
	myfile << "\n" << "doesn't have a lonely vertex,";
	for (int j = 0; j < 10; j++)
	{
		myfile << (amount_of_graphs - res3[j]) << ",";
		//myfile << ",";
	}
	myfile << "\n";
}

/*Part A of matala*/
Graph build_random_graph(double p, int V)
{
	/*
		A function that builds a graph as following:
		It activates the Graph's constructor
		later on in adds edges if a random number is lower then a variable named p
		it uses a Graph function named addEdge (which adds i and j).
		random is a variable that gets a random value for each iteration of the inside loop.
	*/

	Graph g = Graph();
	float random;

	for (int i = 0; i < V; i++)
	{
		for (int j = i + 1; j < V; j++)
		{
			random = (float)rand() / RAND_MAX;

			if (p >= random)
			{
				g.addEdge(i, j);
			}

		}
	}

	return g;
}
int diameter(Graph g)
{
	/*A function that gets the diameter of a Graph:
		Mathematicly we know that we can detrmine what is the diameter of a graph 
		with the usage of a BFS function.
		If we run BFS function on a certin Graph V times we can get the maximum length
		inside the Graph (named Diameter).
	*/

	/* Create two variables named len and diam.
		len will be a temporary variable which holds the maximum length of a certin vertex.
		diam will be the maximum length between all the "len" variables.
		NOTE: if the Graph is not connected the diam will be set to -1.
	*/
	int len;
	int diam = -1;

	// check if diam is connected.
	if (connectivity(g) == 0)
	{
		return diam;
	}

	// get the diam from a Graph in O(V*(V+E))
	for (int i = 0; i < 1000; i++)
	{
		len = g.BFS(i);

		for (int j = 0; j < 1000; j++)
		{
			g.length_from_vertex[j] = 0;
		}

		if (diam < len)
		{
			diam = len;
		}
	}

	return diam;
}
int is_isolated(Graph g)
{
	/*
		A function that detrmines if a Graph has a "lonely vertex",
		A "lonely vertex" is a vertex without any neighbors.

		in order to determine if a Graph has a "lonely vertex" we run a for loop 
		and check if any vertex is empty -> if so we return 1 (which means we have a lonely vertex)
		else -> we return 0 (which means we don't have a lonely vertex).
	*/

	for (int i = 0; i < 1000; i++)
	{
		if (g.l[i].empty())
		{
			return 1;
		}
	}

	return 0;
}
int connectivity(Graph g)
{
	/*
		A function that checks if a graph is connected.
		a connected graph means that you can get to any vertex from any vertex.
	*/

	// declare a variable named source which is set to 0.
	int source = 0;

	// run a BFS on source
	g.BFS(source);

	/* After BFS check if there is any vertex which hasen't been visited during BFS.
		if so -> return 0 because it means that the graph is not connected.
		else -> return 1 because it means that the graph is connected (we can get to any vertex from any vertex).
	*/
	for (int i = 0; i < 1000; i++)
	{
		if (g.visited[i] == false)
		{
			return 0;
		}
	}

	return 1;
}
