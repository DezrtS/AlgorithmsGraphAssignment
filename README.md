This is our EV Charging Station Route Optimization Application group project. 
The program is well documented and should be easy to understand.
Refer to the ChargingStationData.txt file in the Data Files folder for the data loaded into the graph.

As for running the program, its extremely easy to get it up and running.
--------------------------------------------------------------------------------
All you need is the main.cpp file, as all classes, methods, and structs are included within it.
Once you have the file downloaded, you need to compile and run it.
The program will ask you to enter a number for the starting node index, which, follows the format of (A = 0, B = 1, C = 2, ..., W = 22).
After the starting index has been entered, the program will ask if you want to display all nodes. Declining to this option will cause the program to only print out the paths and distances to nodes that contain charging stations.
Once you answer ('y' or 'n') to that question, the program will run Dijkstra's Algorithm using the data from the ChargingStationData.txt file and print out its results.
