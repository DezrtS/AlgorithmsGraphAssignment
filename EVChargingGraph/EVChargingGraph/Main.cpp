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

    }

    void SetupNode(int count) {

    }

    void ReadGraphData(std::string path) {

    }

    ~Graph() {
        for (int i = 0; i < nodeCount; i++) {
            delete[] nodes[i]->outgoingEdges;
        }
        delete[] nodes;
    }
};

void DijkstraAlgorithm(int count, Node** nodes) {

}

int main()
{
    std::cout << "Hello World!\n";
}