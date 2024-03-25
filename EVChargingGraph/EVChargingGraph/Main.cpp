#include <iostream>

struct Node;

struct Edge {
    Node* destination;
    float weight = 0;
};

struct Node {
    int edgeCount = 0;
    Edge** outgoingEdges;
    std::string label;
    bool isChargingStation = false;
};

class Graph {
private:
    int nodeCount;
    Node** nodes;

public:
    void SetupGraph(int count) {
        // Allocate Space For Nodes
    }

    void SetupNode(int count, std::string label, bool isChargingStation) {
        // Assign Variables and Allocate Space For Edges
    }

    int GetNodeCount() {
        return nodeCount;
    }

    Node** GetNodes() {
        return nodes;
    }

    void ReadGraphData(std::string path) {
        // Read Graph Data From File
    }

    ~Graph() {
        for (int i = 0; i < nodeCount; i++) {
            delete[] nodes[i]->outgoingEdges;
        }
        delete[] nodes;
    }
};

void DijkstraAlgorithm(Graph* graph) {
    int nodeCount = graph->GetNodeCount();
    Node** nodes = graph->GetNodes();

    // Do Algorithm
}

int main()
{
    Graph graph;
    graph.ReadGraphData("...");
    DijkstraAlgorithm(&graph);
}