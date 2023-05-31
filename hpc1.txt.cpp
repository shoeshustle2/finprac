#include<bits/stdc++.h>
#include <omp.h>
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std;


int bench_traverse(std::function < void() > traverse_fn) {
        auto start = high_resolution_clock::now();
        traverse_fn();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast < milliseconds > (stop - start);
        return duration.count();
}
// Function to perform Parallel Breadth First Search
void parallelBFS(vector<vector<int>>& graph, int startVertex) {

    int numVertices = graph.size();
    vector<bool> visited(numVertices, false);
    queue<int> bfsQueue;

    visited[startVertex] = true;
    bfsQueue.push(startVertex);

    while (!bfsQueue.empty()) {
        #pragma omp parallel
        {
            queue<int> localQueue;

            #pragma omp for
            for (int i = 0; i < bfsQueue.size(); ++i) {
                int currentVertex;
                #pragma omp critical
                {
                    currentVertex = bfsQueue.front();
                    cout << currentVertex << " ";
                    bfsQueue.pop();
                }



                for (int neighbor : graph[currentVertex]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        localQueue.push(neighbor);
                    }
                }
            }

            #pragma omp critical
            {
                while (!localQueue.empty()) {
                    bfsQueue.push(localQueue.front());
                    localQueue.pop();
                }
            }
        }
    }

}

// Function to perform Parallel Depth First Search
void parallelDFS(vector<vector<int>>& graph, int startVertex) {
    int numVertices = graph.size();
    vector<bool> visited(numVertices, false);
    stack<int> dfsStack;

    dfsStack.push(startVertex);

    while (!dfsStack.empty()) {
        #pragma omp parallel
        {
            stack<int> localStack;

            #pragma omp for
            for (int i = 0; i < dfsStack.size(); ++i) {
                int currentVertex;
                #pragma omp critical
                {
                    currentVertex = dfsStack.top();
                    cout << currentVertex << " ";
                    dfsStack.pop();
                }

                if (!visited[currentVertex]) {
                    visited[currentVertex] = true;


                    for (int neighbor : graph[currentVertex]) {
                        if (!visited[neighbor]) {
                            localStack.push(neighbor);
                        }
                    }
                }
            }

            #pragma omp critical
            {
                while (!localStack.empty()) {
                    dfsStack.push(localStack.top());
                    localStack.pop();
                }
            }
        }
    }
}

// Test the algorithms
int main() {
    vector<vector<int>> graph = {
        {1, 2},       // Node 0 is connected to 1 and 2
        {0, 3, 4},    // Node 1 is connected to 0, 3, and 4
        {0, 5, 6},    // Node 2 is connected to 0, 5, and 6
        {1},          // Node 3 is connected to 1
        {1},          // Node 4 is connected to 1
        {2},          // Node 5 is connected to 2
        {2}           // Node 6 is connected to 2
    };

    cout << "Parallel BFS: ";
    int BFSParallel = bench_traverse([ & ] {
                parallelBFS(graph, 0);
    });
    cout << "\nParallel BFS:" << BFSParallel << "ms\n";
    cout << endl;

    cout << "Parallel DFS: ";
    int DFSParallel = bench_traverse([ & ] {
                parallelDFS(graph, 0);
    });
    cout << "\nParallel DFS:" << DFSParallel << "ms\n";
    cout << endl;

    return 0;
}
