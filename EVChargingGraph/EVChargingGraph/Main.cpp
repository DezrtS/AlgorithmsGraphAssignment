#include <iostream>
#include <vector>
#include <fstream>
#include <string>

static std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter) {
    // Splits a string up into a vector of strings.
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));

    return result;
}

struct Node;

struct Edge {
    Node* destination;
    float weight = 0;
};

struct Node {
    int index;
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
        nodeCount = count;
        nodes = new Node * [nodeCount];
    }

    void SetupNode(Node& node, int index, std::string label, bool isChargingStation) {
        node.index = index;
        node.label = label;
        node.isChargingStation = isChargingStation;
    }

    void SetupNodeEdges(Node& node, int count) {
        node.edgeCount = count;
        node.outgoingEdges = new Edge * [count];
    }

    int GetNodeCount() {
        return nodeCount;
    }

    Node** GetNodes() {
        return nodes;
    }

    void ReadGraphData(std::string path) {
        std::string line;
        std::ifstream dataFile(path);

        if (dataFile.is_open()) {
            try {
                getline(dataFile, line);
                int nodeCount = stoi(line);
                SetupGraph(nodeCount);

                for (int i = 0; i < nodeCount; i++) {
                    getline(dataFile, line);
                    
                    std::vector<std::string> nodeData = SplitString(line, ".");
                    std::string label = nodeData[0];
                    bool isChargingStation = stoi(nodeData[1]);
                    
                    Node* node = new Node;
                    SetupNode(*node, i, label, isChargingStation);
                    nodes[i] = node;
                }

                for (int i = 0; i < nodeCount; i++) {
                    getline(dataFile, line);

                    std::vector<std::string> outgoingEdgeData = SplitString(line, ",");
                    SetupNodeEdges(*nodes[i], outgoingEdgeData.size());
                    
                    for (int j = 0; j < outgoingEdgeData.size(); j++) {
                        std::vector<std::string> edgeData = SplitString(outgoingEdgeData[j], ":");
                        int destinationNodeIndex = stoi(edgeData[0]);
                        int edgeWeight = stoi(edgeData[1]);

                        Edge* edge = new Edge;
                        edge->destination = nodes[destinationNodeIndex];
                        edge->weight = edgeWeight;
                        nodes[i]->outgoingEdges[j] = edge;
                    }
                }
            }
            catch (...) {
                // Print out that there was an error and exit the program.
                std::cout << "Error Reading File" << std::endl;
                exit(1);
            }
        }
    }

    ~Graph() {
        for (int i = 0; i < nodeCount; i++) {
            delete[] nodes[i]->outgoingEdges;
        }
        delete[] nodes;
    }
};

static void DijkstraAlgorithm(Graph* graph, int startingNodeIndex) {
    std::cout << "Dijkstra Algorithm initiated." << std::endl;
    int nodeCount = graph->GetNodeCount();
    Node** nodes = graph->GetNodes();

    int* distances = new int[nodeCount] {INT_MAX};
    bool* visited = new bool[nodeCount] {false};
    distances[startingNodeIndex] = 0;

    // Dijkstra's algorithm.
    for (int i = 0; i < nodeCount; i++) {
        // Find the node with the minimum distance.
        int minDistanceNodeIndex = GetMinDistanceIndex(distances, nodeCount, visited);
        visited[minDistanceNodeIndex] = true;

        // Update distances of neighboring nodes.
        Node* currentNode = nodes[minDistanceNodeIndex];
        for (int j = 0; j < currentNode->edgeCount; j++) {
            Edge* edge = currentNode->outgoingEdges[j];
            int neighborNodeIndex = edge->destination->index;
            int newDistance = distances[minDistanceNodeIndex] + edge->weight;

            if (!visited[neighborNodeIndex] && newDistance < distances[neighborNodeIndex]) {
                distances[neighborNodeIndex] = newDistance;
            }
        }
    }

    std::cout << "Dijkstra Algorithm concluded," << std::endl;
    // Print out distances.
    for (int i = 0; i < nodeCount; i++) {
        std::cout << "The distance from the starting node " << nodes[startingNodeIndex]->label << " to " << nodes[i]->label << " is " << distances[i] << std::endl;
    }

    delete[] distances;
    delete[] visited;
}

static int GetMinDistanceIndex(int* distances, int distancesCount, bool* visited) {

    int minimum = INT_MAX;
    int minIndex = 0;

    for (int i = 0; i < distancesCount; i++) {
        if (visited[i] == false && distances[i] <= minimum) {
            minimum = distances[i];
            minIndex = i;
        }
    }  

    return minIndex;
}

int main()
{
    Graph graph;
    graph.ReadGraphData("Data Files\\ChargingStationData.txt");
    DijkstraAlgorithm(&graph, 0);
}