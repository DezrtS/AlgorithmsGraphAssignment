// ------------------------------------------------------------------------------------ //
// Title: EV Charging Station Finder Project                                            //
// Due Date: 03/30/2024                                                                 //
// Authors:    Denzil Walrond,     Christian Zambri,    Michael Atteh,  Hamzah Warsame  //
// Student#:   100868217,          100787919,           100831528,      100824004       //
// ------------------------------------------------------------------------------------ //

// Headers, Libraries, Namespaces.
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// --------------------------------------------------------------------------------------------------------- //
// Helper Methods                                                                                            //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief Splits a given string into substrings based on a specified delimiter.
 *
 * This static method takes a source string and a delimiter, and breaks the source
 * string into substrings wherever the delimiter occurs. The resulting substrings
 * are stored in a vector, which is then returned.
 *
 * @param str The source string to be split.
 * @param delimiter The string that acts as a delimiter for splitting the source string.
 *
 * @return A vector of strings containing the substrings obtained by splitting the source string.
 */
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

/**
 * @brief Determines if the user answers yes or no.
 *
 * This static method continuously gets the user input until they enter a string starting
 * with 'y' for yes or 'n' for no.
 *
 * @param question The question to be asked.
 *
 * @return A bool for if the user entered yes.
 */
static bool YesOrNo(std::string question) {
    std::string answer;
    // While the answer is invalid, keep asking the player to answer yes or no to the question.
    while (true) {
        std::cout << question << " (Y/N)" << std::endl;
        std::cin >> answer;
        // Check to see if the first character from the user input is equal to y or n.
        if (tolower(answer[0]) == 'y') {
            return true;
        }
        else if (tolower(answer[0]) == 'n') {
            return false;
        }
        // If the answer is not a yes or no, ask the question again.
        std::cout << "Invalid answer, try again." << std::endl << std::endl;
    }
}

/**
 * @brief Gets a number from the user within a certain range.
 *
 * This static method continuously gets the user's input until they enter a number within a range.
 *
 * @param question The question to be asked.
 * @param lowerBound The lower bound for the number the user has to input.
 * @param upperBound The upper bound for the number the user has to input.
 *
 * @return The number the user inputted.
 */
static int GetNumber(std::string question, int lowerBound, int upperBound) {
    std::string answer;
    // While the answer is invalid, keep asking the player to enter a number for the question.
    while (true) {
        std::cout << question << std::endl;
        std::cin >> answer;
        try {
            // Convert the input into a number and check to see if it is within the bounds.
            int num = std::stoi(answer);
            if (num >= lowerBound && num <= upperBound) {
                return num;
            }
            else {
                // If the number is outside the bounds, ask the question again.
                std::cout << "Number is outside range, try again." << std::endl << std::endl;
            }
        }
        catch (...) {
            // If the answer is not a valid number, ask the question again.
            std::cout << "Invalid number, try again." << std::endl << std::endl;
        }
    }
}

// --------------------------------------------------------------------------------------------------------- //
// Classes and Structs                                                                                       //
// --------------------------------------------------------------------------------------------------------- //

// Node Struct declaration for Edge struct.
struct Node;

// Edge Struct.
struct Edge {
    // Destination of edge.
    Node* destination = nullptr;
    // Weight of edge.
    float weight = 0;
};

// Node Struct.
struct Node {
    // Index identifier for node.
    int index = 0;
    // Number of outgoing edges.
    int edgeCount = 0;
    // Array of pointers to outgoing edges.
    Edge** outgoingEdges = nullptr;
    // Label for node.
    std::string label;
    // Boolean for if this node is a charging station.
    bool isChargingStation = false;
};

/**
 * @brief Represents a graph.
 *
 * The Graph class encapsulates all information about a graph, including the
 * number of nodes it contains, and a array of pointers to all of those nodes.
 * It provides a various methods to alter, setup, or retreive its elements.
 */
class Graph {
private:
    // Number of nodes in the graph.
    int nodeCount;
    // Array of pointers to the nodes in the graph.
    Node** nodes;

public:
    /**
     * @brief Setups up the graph.
     * 
     * Allocates space for the array of node pointers.
     *
     * @param count The number of nodes this graph contains.
     */
    void SetupGraph(int count) {
        nodeCount = count;
        nodes = new Node * [nodeCount];
    }

    /**
     * @brief Setups up the edges of a node.
     *
     * Allocates space for the array of edge pointers.
     *
     * @param node The node to setup the edges for.
     * @param count The number of outgoing edges this node has.
     */
    void SetupNodeEdges(Node& node, int count) {
        node.edgeCount = count;
        node.outgoingEdges = new Edge * [count];
    }

    /**
     * @brief Getter for the nodeCount variable.
     *
     * @return The amount of nodes this graph contains.
     */
    int GetNodeCount() const {
        return nodeCount;
    }

    /**
     * @brief Getter for the node pointer array.
     *
     * @return A reference to the array of pointers to this graphs nodes.
     */
    Node** GetNodes() {
        return nodes;
    }

    /**
     * @brief Reads graph data from a specified file and sets up the graph, nodes, and edges accordingly.
     *
     * The graph data file follows a specific format:
     * - First line: Total number of nodes in the graph.
     * - Subsequent line for each node:
     *   - Node label and (1 or 0) for if the node is a charging station, seperated by a period: "label.[1 or 0]"
     * - Subsequent line for all outgoing edges of a node:
     *   - Index of node for edge destination and weight of edge, seperated by a colon, 
     *     followed by any other connections, seperating each one by a comma: "destinationIndex:edgeWeight,..."
     *
     * @param path The path to the graph data file.
     */
    void ReadGraphData(std::string path) {
        std::string line;
        // Create a ifstream object.
        std::ifstream dataFile(path);

        // Check to see if the file is open before reading any data.
        if (dataFile.is_open()) {
            // Creates a try catch block to handle any errors while reading the graph data file.
            try {
                // Read the total amount of nodes.
                getline(dataFile, line);
                int nodeCount = stoi(line);
                // Setup the graph with the read node count.
                SetupGraph(nodeCount);

                // Loop through each subsequent node data entry.
                for (int i = 0; i < nodeCount; i++) {
                    // Read the node label and integer for whether or not is a charging station.
                    getline(dataFile, line);
                    
                    // Split up the node label and boolean and store each value in variables.
                    std::vector<std::string> nodeData = SplitString(line, ".");
                    std::string label = nodeData[0];
                    bool isChargingStation = stoi(nodeData[1]);
                    
                    // Create a new node object and assign its variables.
                    Node* node = new Node;
                    node->index = i;
                    node->label = label;
                    node->isChargingStation = isChargingStation;
                    // Store the new node in the node pointer array.
                    nodes[i] = node;
                }

                // Loop through each subsequent collection of edge data entries.
                for (int i = 0; i < nodeCount; i++) {
                    // Read the outgoing edges data.
                    getline(dataFile, line);

                    // Split up the outgoing edges data into each individual edge and store the total number of edges in a variable.
                    std::vector<std::string> outgoingEdgesData = SplitString(line, ",");
                    int outgoingEdgeCount = outgoingEdgesData.size();
                    // Setup the outgoing edges of each node.
                    SetupNodeEdges(*nodes[i], outgoingEdgeCount);
                    
                    // Loop through each individual edge.
                    for (int j = 0; j < outgoingEdgeCount; j++) {
                        // Split up the edge destination node index and weight and store each value in variables. 
                        std::vector<std::string> edgeData = SplitString(outgoingEdgesData[j], ":");
                        int destinationNodeIndex = stoi(edgeData[0]);
                        float edgeWeight = stof(edgeData[1]);

                        // Create a new edge object and assign its variables.
                        Edge* edge = new Edge;
                        edge->destination = nodes[destinationNodeIndex];
                        edge->weight = edgeWeight;
                        // Store the new edge in the outgoing edges array inside each node.
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

    /**
     * @brief Destructor for the Graph class.
     *
     * Releases memory allocated for the node pointers array as well as the edge pointers array inside each node.
     */
    ~Graph() {
        // Loop through each node and deallocate its outgoing edges.
        for (int i = 0; i < nodeCount; i++) {
            delete[] nodes[i]->outgoingEdges;
        }
        delete[] nodes;
    }
};

// --------------------------------------------------------------------------------------------------------- //
// Dijkstra Algorithm Methods                                                                                //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief Gets the index of the node with the minimum path distance.
 *
 * This static method cycles through all the path distances and returns the index of the shortest path. 
 * 
 * @param distances An array of integers that represent each path length.
 * @param distancesCount The number of distances/paths.
 * @param visited An array of booleans that represent whether or not a node/path has already been visited.
 *
 * @return The index of the smalled path distance that has not been visited.
 */
static int GetMinDistanceIndex(int* distances, int distancesCount, bool* visited) {
    // Set the minimum value to the maximum possible value.
    int minimum = INT_MAX;
    int minIndex = 0;

    // Cycle through each distance and keep track of the smalled one that has not been visited.
    for (int i = 0; i < distancesCount; i++) {
        if (visited[i] == false && distances[i] <= minimum) {
            minimum = distances[i];
            minIndex = i;
        }
    }

    // Return the index of the smalled distance.
    return minIndex;
}

/**
 * @brief Prints out the shortest path for a specific node.
 *
 * This static method progresses through the shortest path of a node, printing out each node label as it progresses.
 *
 * @param nodes An array of pointers to all nodes.
 * @param previousNodeIndexes An array of the previous node indexes with the shortest path distance.
 * @param startingNodeIndex The index of the node to print the path for.
 */
static void PrintPath(Node** nodes, int* previousNodeIndexes, int startingNodeIndex) {
    // Assigns the current previous node index to the passed in starting node's previous node index.
    int previousIndex = previousNodeIndexes[startingNodeIndex];
    std::cout << "[Path] : " << nodes[startingNodeIndex]->label;

    // While there is still nodes before the current previous node index, keep progressing though the path.
    while (previousIndex != -1) {
        // Prints out the node the current previous node index is refering too, and progresses one node through the path.
        std::cout << " <- " << nodes[previousIndex]->label;
        previousIndex = previousNodeIndexes[previousIndex];
    }
    std::cout << std::endl;
}

/**
 * @brief Runs the Dijkstra Algorithm on a graph.
 *
 * This static method cycles through each node in a graph and builds a list of the shortest path to each node.
 *
 * @param graph The graph to run the algorithm on.
 * @param startingNodeIndex The index of the node to start on.
 * @param displayAllNodes Whether to display the shortest path to all nodes, or just to charging stations.
 */
static void DijkstraAlgorithm(Graph* graph, int startingNodeIndex, bool displayAllNodes) {
    // Prints out that the algorithm has started.
    std::cout << "Dijkstra Algorithm initiated..." << std::endl;
    
    // Retreive the node count and node pointer array from the provided graph.
    int nodeCount = graph->GetNodeCount();
    Node** nodes = graph->GetNodes();

    // Create arrays for the shortest path distances, the previous path nodes, and whether or not a path/node has been visited.
    int* distances = new int[nodeCount];
    std::fill_n(distances, nodeCount, INT_MAX);
    int* previousNodeIndexes = new int[nodeCount];
    std::fill_n(previousNodeIndexes, nodeCount, -1);
    bool* visited = new bool[nodeCount] {false};
    // Set the starting path/node distance to 0.
    distances[startingNodeIndex] = 0;

    // Loops through each node in the graph.
    for (int i = 0; i < nodeCount; i++) {
        // Find the path/node with the smallest length.
        int minDistanceNodeIndex = GetMinDistanceIndex(distances, nodeCount, visited);
        // Indicate that the path/node is now visited.
        visited[minDistanceNodeIndex] = true;

        // Get a pointer to the node with the smallest path.
        Node* currentNode = nodes[minDistanceNodeIndex];
        // Loop through each outgoing edge in that node.
        for (int j = 0; j < currentNode->edgeCount; j++) {
            // Get a pointer to each edge.
            Edge* edge = currentNode->outgoingEdges[j];
            // Retreive the neighbouring node's index.
            int neighbourNodeIndex = edge->destination->index;

            // If the neighbouring node has not been visited, check to see if we should update its path distance.
            if (!visited[neighbourNodeIndex]) {
                // Calculate the new path distance to this neighbouring node.
                int newDistance = distances[minDistanceNodeIndex] + edge->weight;

                // If this new path distance is less than the node's current path distance, update it.
                if (newDistance < distances[neighbourNodeIndex]) {
                    distances[neighbourNodeIndex] = newDistance;
                    // Assigns the new previous node index of the neighbouring node to the current node index.
                    previousNodeIndexes[neighbourNodeIndex] = currentNode->index;
                }
            }
        }
    }

    // Prints out that the algorithm has ended.
    std::cout << "Dijkstra Algorithm concluded..." << std::endl << std::endl;

    // Sets the shortest charging station distance to the maximum possible value.
    int shortestChargingStationDistance = INT_MAX;
    int shortestIndex = 0;
    
    // Loops through each node and prints out it path distance as well as the path it takes.
    for (int i = 0; i < nodeCount; i++) {
        // If the current node is a charging station, check to see if it has a shorter path distance than the current listed shorted path distance.
        // If so, the shortest path distance and index are updated.
        if (nodes[i]->isChargingStation) {
            if (distances[i] < shortestChargingStationDistance) {
                shortestChargingStationDistance = distances[i];
                shortestIndex = i;
            }
        }
        // If not, check to see if the user chose to skip nodes that are not charging stations, and if so, skip the current node.
        else {
            if (!displayAllNodes) {
                continue;
            }
        }

        // Print out the path distance.
        std::cout << "The distance from the starting node " << nodes[startingNodeIndex]->label << " to " << nodes[i]->label << " is " << distances[i] << std::endl;
        // Print the path.
        PrintPath(nodes, previousNodeIndexes, i);
    }

    // Print out the recommended charging station.
    std::cout << std::endl;
    std::cout << "The recommended charging station based on your current location is " << nodes[shortestIndex]->label << " with a distance of " << shortestChargingStationDistance << std::endl;
    // Print the path.
    PrintPath(nodes, previousNodeIndexes, shortestIndex);

    // Release memory allocated to temporary arrays for the algorithm.
    delete[] distances;
    delete[] previousNodeIndexes;
    delete[] visited;

    // Instead of being released, the distances and previousNodeIndexes arrays could be returned to be used for anything else.
}

// --------------------------------------------------------------------------------------------------------- //
// Main Function                                                                                             //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief The starting function of the project.
 */
int main()
{
    // Creates a new graph object.
    Graph graph;
    // Reads the graph data from the ChargingStationData.txt file.
    graph.ReadGraphData("Data Files\\ChargingStationData.txt");

    // Ask the user for which starting node they would like the Dijkstra Algorithm to start at.
    int startingNodeIndex = GetNumber("Enter the index of the starting node (A = 0, B = 1, ...),", 0, graph.GetNodeCount() - 1);
    std::cout << std::endl;
    // Ask the user for if they would like to see the shortest path to all nodes.
    bool displayAllNodes = YesOrNo("Would you like to display all nodes (Include Non-Charging Stations)?");
    std::cout << std::endl;

    // Run the Dijkstra Algorithm.
    DijkstraAlgorithm(&graph, startingNodeIndex, displayAllNodes);
}