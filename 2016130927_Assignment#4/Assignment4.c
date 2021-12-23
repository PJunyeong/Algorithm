#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_SIZE 1000

typedef struct node
{
	int from;
	int to;
	int weight;
} Node;

typedef struct adjacentNode
{
	Node node;
	struct adjacentNode * link;
} AdjNode;

typedef struct adjacentList
{
	int numAdjNode;
	AdjNode * head;
} AdjList;

typedef struct graph
{
	int numNode;
	int numEdge;
	AdjList * ListArr;
} Graph;

typedef struct PriorityQueue
{
	Node heap[MAX_SIZE];
	int size;
} PQ;

int Kruskal_parent[MAX_SIZE];

Graph * InitGraph(int numNode, int numEdge);
AdjNode * InitNode(Node node);
Graph * DirGraph();
Graph * UndGraph();
int InsertEdge(AdjNode **head, Node node);
int IsIn(AdjNode **head, Node node);
void PrintGraph(Graph * graph);
int ** GetMatrix(Graph * graph, int flag);
void PrintMatrix(int ** mat, int row);
void FreeGraph(Graph * graph);
void Enqueue(PQ * root, Node node);
Node Dequeue(PQ * root);
PQ * PQenqueue(Graph * graph);
int Kruskal_Union(int node1, int node2);
int Kruskal_Find(int node);
Graph * MakeEdge(Graph * graph);
void Dijkstra_Path (int *path, int from, int numNode, int numEdge);
void Warshall(Graph * graph);
void Floyd(Graph * graph);
void Dijkstra(Graph * graph);
void Kruskal(Graph * graph);
void Prim(Graph * graph);

int main(void)
{
	int sel; Graph * Dirgraph = NULL, * Undgraph = NULL;
	while (1)
	{
		printf("1. Directed graph\n");
		printf("2. Undirected graph\n");
		printf("3. Warshall's algorithm\n");
		printf("4. Floyd's algorithm\n");
		printf("5. Dijkstra's algorithm\n");
		printf("6. Kruskal's algorithm\n");
		printf("7. Prim's algorithm\n");
		printf("8. Exit\n");
		printf(" > Enter sel : ");
		scanf_s("%d", &sel);
		fflush(stdin);
		//user-driven menu for graph algorithm. with (un)directed grahp, 5 algorithms are implemented
		switch (sel)
		{
		case 1: 
			if (Dirgraph)
		{
			printf("Graph is already entered. Overwrite it? \n");
			printf("Yes / No - 1 / any other sel :");
			scanf_s("%d", &sel);
			fflush(stdin);
			if (sel == 1) { FreeGraph(Dirgraph); printf("Graph is deleted\n"); }
			else continue;
		}
			printf("Enter your graph's information\n");
			Dirgraph = DirGraph();
			printf("Print your graph \n");
			PrintGraph(Dirgraph);
			system("pause");
			break;
		case 2:
			if (Undgraph)
			{
				printf("Graph is already entered. Overwrite it? \n");
				printf("Yes / No - 1 / 0 :");
				scanf_s("%d", &sel);
				fflush(stdin);
				if (sel == 1) { FreeGraph(Undgraph); printf("Graph is deleted\n"); }
				else continue;
			}
			printf("Enter your graph's information\n");
			Undgraph = UndGraph();
			PrintGraph(Undgraph);
			system("pause");
			break;
		case 3 :
			if (!Dirgraph)
			{
				printf("You should enter your directed graph\n");
				continue;
			}
			Warshall(Dirgraph);
			system("pause");
			break;
		case 4 :
			if (!Dirgraph)
			{
				printf("You should enter your directed graph\n");
				continue;
			}
			Floyd(Dirgraph);
			system("pause");
			break;
		case 5 :
			if (!Dirgraph && !Undgraph)
			{
				printf("You should enter at least either of them\n");
				continue;
			}
			else if (!Dirgraph && Undgraph)
			{
				printf("With your undirected graph Dijkstra's algorithm is implemented\n");
				Dijkstra(Undgraph);
				system("pause");
				break;
			}
			else if (Dirgraph && !Undgraph)
			{
				printf("With your directed graph Dijkstra's algorithm is implemented\n");
				Dijkstra(Dirgraph);
				system("pause");
				break;
			}
			else
			{
				while (1)
				{
					printf("Select one graph between them.\n");
					printf("With this, Dijkstra's algorithm is implemented\n");
					printf("1 : Directed graph 2 : Undirected graph : ");
					scanf_s("%d", &sel); 
					fflush(stdin);
					if (sel == 1)
					{
						Dijkstra(Dirgraph);
						system("pause");
						break;
					}
					else if (sel == 2)
					{
						Dijkstra(Undgraph);
						system("pause");
						break;
					}
					else
					{
						printf("Select 1 or 2\n");
						continue;
					}
				}
				break;
		case 6 :
			if (!Undgraph)
			{
				printf("You should enter your undirected graph\n");
				continue;
			}
			Kruskal(Undgraph);
			system("pause");
			break;
		case 7 :
			if (!Undgraph)
			{
				printf("You should enter your undirected graph\n");
				continue;
			}
			Prim(Undgraph);
			system("pause");
			break;
			//in case of Prim's algorithm, you should select a certain node as source node.
			//if entered graph is not connected, then total sum might be different. 
		case 8 :
			printf("Exit\n");
			FreeGraph(Dirgraph); FreeGraph(Undgraph);
			return 0;
		default :
			printf("Please select between 1~8");
			}
		}
		system("cls");
	}
	return 0;
}

AdjNode * InitNode(Node node)
{
	AdjNode * temp = (AdjNode *)malloc(sizeof(AdjNode));
	temp->node = node;
	temp->link = NULL;
	return temp;
}

Graph * InitGraph(int numNode, int numEdge)
{
	Graph * graph = (Graph*)malloc(sizeof(Graph));
	graph->numNode = numNode;
	graph->numEdge = numEdge;
	graph->ListArr = (AdjList*)malloc(sizeof(AdjNode)*numNode);

	for (int i = 0; i < numNode; i++)
	{
		graph->ListArr[i].head = NULL;
		graph->ListArr[i].numAdjNode = 0;
	}
	return graph;
}

int InsertEdge(AdjNode ** head, Node node)
{
	if (*head == NULL)
	{
		*head = InitNode(node);
		return 1;
	}
	else if ((*head)->node.to == node.to) return -1;

	return InsertEdge(&(*head)->link, node);
}

int IsIn(AdjNode ** head, Node node)
{
	if (*head == NULL) return 0;
	else if ((*head)->node.to == node.to) return 1;
	return IsIn(&(*head)->link, node);
}

void FreeGraph(Graph * graph)
{
	if (graph == NULL) return;
	int i, j; AdjNode * temp, * temp2;
	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			temp2 = temp;
			temp = temp->link;
			free(temp2);
		}
	}
	free(graph);
	return;
}

Graph * DirGraph()
{
	int numNode, numEdge, weight, i;
	int from, to, ret;
	while (1)
	{
		printf("Enter the number of node and edge : ");
		scanf_s("%d %d", &numNode, &numEdge);
		fflush(stdin);
		if (numNode <= 0 || numEdge > (numNode - 1)*(numNode))
		{
			printf("numNode should be more than 0 and numEdge is at most (numNode -1) * (numNode)\n");
			printf("Please Retry again\n");
		}
		else break;
	}
	Graph * graph = InitGraph(numNode, numEdge);
	for (i = 0; i < numEdge; i++)
	{
		while (1)
		{
			printf("%dth insert :", i + 1);
			printf("Enter from node / to node / weight : ");
			scanf_s("%d %d %d", &from, &to, &weight);
			fflush(stdin);
			if (from < 0 || from >= numNode || to < 0 || to >= numNode || weight < 0 || weight >= MAX_SIZE || from == to)
			{
				printf("Check your boundary and retry\n"); continue;
			}
			//condition for directed graph. self loop, 0 weight and others are prevented in advance.
			Node node;
			node.from = from;
			node.to = to;
			node.weight = weight;
			ret = InsertEdge(&graph->ListArr[from].head, node);
			if (ret == -1)
			{
				printf("Parallel edge occurs!\n");
				continue;
			}
			graph->ListArr[from].numAdjNode++;
			break;
		}
	}
	return graph;
}

Graph * UndGraph()
{
	int numNode, numEdge, weight, i;
	int from, to, ret;
	while (1)
	{
		printf("Enter the number of node and edge : ");
		scanf_s("%d %d", &numNode, &numEdge);
		fflush(stdin);
		if (numNode <= 0 || numEdge > (numNode - 1)*(numNode) / 2)
		{
			printf("numNode should be more than 0 and numEdge is at most (numNode -1) * (numNode) / 2\n");
			printf("Please Retry again\n");
		}
		else break;
	}
	numEdge *= 2; printf("Sicne Undirected Graph, the number of edges is doublely stored!\n");
	Graph * graph = InitGraph(numNode, numEdge);
	for (i = 0; i < numEdge/2 ; i++)
	{
		while (1)
		{
			printf("%dth insert :", i + 1);
			printf("Enter two connected node1 node2 / weight : ");
			scanf_s("%d %d %d", &from, &to, &weight);
			fflush(stdin);
			if (from < 0 || from >= numNode || to < 0 || to >= numNode || weight < 0 || weight >= MAX_SIZE || from == to)
			{
				printf("Check your boundary and retry\n"); continue;
			}
			//condition for undirected graph. same as in directed graph.
			Node node;
			node.from = from; node.to = to; node.weight = weight;
			ret = InsertEdge(&graph->ListArr[from].head, node);
			if (ret == -1) 
			{
				printf("Parallel edge occurs!\n");  
				continue;
			}
			node.to = from; node.from = to;
			InsertEdge(&graph->ListArr[to].head, node);
			graph->ListArr[from].numAdjNode++;
			graph->ListArr[to].numAdjNode++;
			break;
		}
	}
	return graph;
}

void PrintGraph(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	AdjNode * temp;
	for (int i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		printf("Node %d : ", i);
		for (int j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			printf("%d (Weight %d) ", temp->node.to, temp->node.weight);
			temp = temp->link;
		}
		printf("\n");
	}
	return;
}

void PrintMatrix(int ** mat, int row)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < row; j++)
			if (mat[i][j] == MAX_SIZE) printf("INF   ");
			else printf("%d   ", mat[i][j]);
			printf("\n");
	}

}

int ** GetMatrix(Graph * graph, int flag)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return NULL;
	}
	int i, j;
	int ** mat = (int **)malloc(sizeof(int*)*graph->numNode);
	for (i = 0; i < graph->numNode; i++)
	{
		mat[i] = (int *)calloc(graph->numNode, sizeof(int));
	}
	AdjNode * temp;
	if (flag == 1)
		for (i = 0; i < graph->numNode; i++)
		{
			temp = graph->ListArr[i].head;
			for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
			{
				mat[i][temp->node.to] = 1;
				temp = temp->link;
			}
		}
	else if (flag == 2)
	{
		for (i = 0; i < graph->numNode; i++)
			for (j = 0; j < graph->numNode; j++)
			{
				if (i == j) mat[i][j] = 0;
				else mat[i][j] = MAX_SIZE;
			}

		for (i = 0; i < graph->numNode; i++)
		{
			temp = graph->ListArr[i].head;
			for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
			{
				mat[i][temp->node.to] = temp->node.weight;
				temp = temp->link;
			}
		}
	}

	return mat;
}

PQ * PQenqueue(Graph * graph)
{
	if (graph == NULL) return NULL;
	int i, j;
	PQ * root = NULL; root = (PQ *)malloc(sizeof(PQ)); root->size = 0;
	AdjNode * temp;

	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			Enqueue(root, temp->node);
			temp = temp->link;
		}
	}
	return root;

}

void Enqueue(PQ * root, Node node)
{
	//enqueue a certain node to this min-priority queue
	if (root->size >= MAX_SIZE) { printf("PQ is full\n"); return; }
	root->heap[++root->size] = node;
	int cur = root->size;
	int par = cur / 2;
	while (cur > 1 && root->heap[cur].weight < root->heap[par].weight)
	{
		Node temp;
		temp = root->heap[cur];
		root->heap[cur] = root->heap[par];
		root->heap[par] = temp;
		cur = par;
		par = cur / 2;
	}
	return;
}

Node Dequeue(PQ * root)
{
	//a certain node is dequeued from this min priority queue
	Node ret;
	ret.from = -1; ret.to = -1; ret.weight = -1;
	if (root->size == 0) return ret;

	ret = root->heap[1];

	root->heap[1] = root->heap[root->size--];
	int cur = 1; int leftchild = cur * 2; int rightchild = cur * 2 + 1; int target = cur;

	while (leftchild < root->size)
	{
		if (root->heap[target].weight > root->heap[leftchild].weight)
			target = leftchild;
		if (rightchild < root->size && root->heap[target].weight > root->heap[rightchild].weight)
			target = rightchild;
		if (target == cur) break;
		else
		{
			Node temp;
			temp = root->heap[cur];
			root->heap[cur] = root->heap[target];
			root->heap[target] = temp;
			cur = target; leftchild = cur * 2; rightchild = cur * 2 + 1;
		}
	}

	return ret;
}

int Kruskal_Union(int node1, int node2)
{
	node1 = Kruskal_Find(node1); node2 = Kruskal_Find(node2);
	if (node1 == node2) return 0;
	Kruskal_parent[node1] = node2;
	return 1;
	//if both of them are already in same set, then just return. otherwise make a union.
}
int Kruskal_Find(int node)
{
	if (node == Kruskal_parent[node]) return node;
	else return Kruskal_parent[node] = Kruskal_Find(Kruskal_parent[node]);
	//find a parent node of a certain node, which indicates that a certain set.
}

Graph * MakeEdge(Graph * graph)
{
	int numNode = graph->numNode, numEdge = (graph->numEdge / 2), i, j; 
	Node node, reverse; Graph * Kruskal = InitGraph(numNode, numEdge);
	AdjNode * temp;
	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			node = temp->node; temp = temp->link;
			reverse.from = node.to; reverse.to = node.from; reverse.weight = node.weight;
			if (IsIn(&Kruskal->ListArr[reverse.from].head, reverse) == 1) continue;
			else
			{
				InsertEdge(&Kruskal->ListArr[node.from].head, node);
				Kruskal->ListArr[node.from].numAdjNode++;
			}
		}
	}
	PrintGraph(Kruskal);
	return Kruskal;
	//before edges are sorted, check duplicate edges in advance.
}

void Warshall(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	int ** mat, i, j, k;
	mat = GetMatrix(graph, 1);
	printf("R0 stage\n");
	PrintMatrix(mat, graph->numNode);
	for (k = 0; k < graph->numNode; k++)
	{
		for (i = 0; i < graph->numNode; i++)
		{
			for (j = 0; j < graph->numNode; j++)
			{
				mat[i][j] = MAX(mat[i][j], mat[i][k] && mat[k][j]);
				//since all of value is 0 or 1, && indicates that there is a way at stage k.
			}
		}
		printf("R%d stage\n", k + 1);
		PrintMatrix(mat, graph->numNode);
	}

	for (i = 0; i < graph->numNode; i++)
	{
		free(mat[i]);
	}
	free(mat);
	return;
}

void Floyd(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	int ** mat, i, j, k;
	mat = GetMatrix(graph, 2);
	printf("D0 stage\n");
	PrintMatrix(mat, graph->numNode);
	for (k = 0; k < graph->numNode; k++)
	{
		for (i = 0; i < graph->numNode; i++)
		{
			for (j = 0; j < graph->numNode; j++)
			{
				mat[i][j] = MIN(mat[i][j], mat[i][k] + mat[k][j]);
				//since all of weights are initilaized as INF (MAX_SIZE) except for self loop,
				//you can select less weighted edge using MIN opertaion.
			}
		}
		printf("D%d stage\n", k + 1);
		PrintMatrix(mat, graph->numNode);
	}

	for (i = 0; i < graph->numNode; i++)
	{
		free(mat[i]);
	}
	free(mat);
	return;
}

void Dijkstra(Graph * graph)
{
	int numNode = graph->numNode;
	int i, j, k;
	int * D = (int *)malloc(sizeof(int)*numNode);
	PQ * root = (PQ *)malloc(sizeof(PQ));
	Node node, curnode, nextnode; AdjNode * temp;
	for (i = 0; i < numNode; i++)
	{
		int path[MAX_SIZE] = { -1 }; // make a backtracking from destination to starting position
		int stack[MAX_SIZE];
 
		printf("Find a shortest way from node %d\n", i);
		printf("D[n] : from node %d the smallest sum of weights node n\n", i);
		for (k = 0; k < numNode; k++)
		{
			if (k == i) D[k] = 0;
			else D[k] = MAX_SIZE; //except for self loop, all of possible path to other nodes from node i are set as MAX_SIZE
		}
		node.from = i; node.to = i; node.weight = 0; root->size = 0; Enqueue(root, node);
		while (root->size != 0)
		{
			curnode = Dequeue(root); //current node is the least weighted node in priority queue.
			temp = graph->ListArr[curnode.to].head;
			if (graph->ListArr[curnode.to].numAdjNode) printf("Find way from current node %d\n", curnode.to);
			for (j = 0; j < graph->ListArr[curnode.to].numAdjNode; j++)
			{
				nextnode = temp->node; //each node adjacent to this current node is nextnode
				nextnode.weight = temp->node.weight + curnode.weight; //to nextnode via current node
				temp = temp->link;
				if (nextnode.weight < D[nextnode.to]) //compare them and select smaller one
				{
					D[nextnode.to] = nextnode.weight;
					Enqueue(root, nextnode); //if updated, nextnode is also enqueued
					printf("D[%d].weight (%d) > D[%d].weight (%d) + C[%d, %d].weight (%d)\n",
						nextnode.to, nextnode.weight, curnode.to, curnode.weight,
						curnode.to, nextnode.to, nextnode.weight - curnode.weight);
					printf("D[%d].weight is updated\n", nextnode.to);
					printf("path[%d] = %d\n", nextnode.to, nextnode.from);
					path[nextnode.to] = nextnode.from; //path[destination] = stopover
					//path[final dest] = stopover1, path[stopover1] = stopover2, ... path[stopoverlast] = start position
					//to backtrack its shortest path, path is recorded in this array and read by a stack.
				}
				else
				{
					printf("D[%d].weight (%d) < D[%d].weight (%d) + C[%d, %d].weight (%d)\n",
						nextnode.to, nextnode.weight, curnode.to, curnode.weight,
						curnode.to, nextnode.to, nextnode.weight - curnode.weight);
				}
			}
		}

		printf("\n\nThe Smallest weight from node %d \n", i);
		for (k = 0; k < numNode; k++)
		{
			if (D[k] == MAX_SIZE) printf("INF  ");
			else printf("%d   ", D[k]); 
			//the smallest weight to other nodes from source node i
		}
		printf("\n\n");

		int top = 0; int to;
		printf("The Shortest Path from node %d \n", i);
		
		for (k = 0; k < numNode; k++)
		{
			if (k == i || D[k] == MAX_SIZE) continue;
			stack[++top] = k;
			while (1)
			{
				to = path[stack[top]];
				stack[++top] = to;
				if (to == i) break;
			}
			printf("\n\nTo node %d\n", k);
			while (top)
			{
				printf("%d ", stack[top]);
				top--;
			}
			printf("\n");
			//the shortest path (if any, i.e. not 0 or MAX_SIZE) is printed out.
		}
		printf("\n\n");
	}
	free(D); free(root);
	return;
}


void Kruskal(Graph * graph)
{
	Graph * Kruskal = MakeEdge(graph);
	int numNode = Kruskal->numNode, numEdge = Kruskal->numEdge, i, j = 0, total = 0;
	Node * sorted = (Node *)malloc(sizeof(Node)*(numEdge)); Node edge;
	for (i = 0; i < numNode; i++) Kruskal_parent[i] = i;
	PQ * root = PQenqueue(Kruskal);
	while (root->size != 0) sorted[j++] = Dequeue(root);
	//all of edges are sorted in increasing order in terms of weight value.
	printf("Sorted edge :\n");
	for (i = 0; i < numEdge; i++) printf("node %d - node %d weight %d\n", sorted[i].from, sorted[i].to, sorted[i].weight);
	for (i = 0; i < numEdge; i++)
	{
		if (Kruskal_Union(sorted[i].from, sorted[i].to) == 1)
		{//if this edge makes no cycle, then add.
			edge = sorted[i];
			printf("weight %d between node %d and node %d is summed\n", edge.weight, edge.from, edge.to);
			total += edge.weight;
		}
		else printf("Cycle due to edge between node %d and node %d so do not recept\n", edge.from, edge.to);
	}
	printf("total weight = %d\n", total);
	free(sorted); free(Kruskal); free(root);
	return;
}

void Prim(Graph * graph)
{
	int numNode = graph->numNode;
	int start, i, total = 0, * mark;
	while (1)
	{
		printf("Select your starting node : \n");
		scanf_s("%d", &start);
		fflush(stdin);
		if (start >= 0 && start < numNode) break;
		else { printf("Check your boundary\n"); continue; }
	}
	mark = (int *)calloc(numNode, sizeof(int));
	mark[start] = 1;
	PQ * root = (PQ *)malloc(sizeof(PQ)); root->size = 0;
	Node node, nextnode; AdjNode * temp = graph->ListArr[start].head;
	
	printf("Start from node %d\n", start);
	for (i = 0; i < graph->ListArr[start].numAdjNode; i++)
	{
		Enqueue(root, temp->node);
		temp = temp->link;
		//all of nodes adjacent to start source node are enqueued: they are sorted in increasing order in terms of weight.
	}

	while (root->size != 0)
	{
		node = Dequeue(root); 
		if (mark[node.to] == 1) continue; //if visited, skip in order to prevent cycle.
		mark[node.to] = 1; //if not, mark it as visited.
		printf("node %d is selected amongs nodes adjacent to node %d\n", node.to, node.from);
		total += node.weight; printf("weight %d is summed\n", node.weight);
		temp = graph->ListArr[node.to].head; printf("all of nodes adjacent to node %d are enqueued\n", node.to);
		for (i = 0; i < graph->ListArr[node.to].numAdjNode; i++)
		{
			nextnode = temp->node;
			Enqueue(root, nextnode);
			temp = temp->link;
			//all of nodes adjacent to this node are enqueued. this node makes no duplicate and no cycle.
		}
	}
	printf("total weight : %d\n", total);
	free(mark); free(root);
	return;
}