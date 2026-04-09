# Line-by-Line Explanation: Smart Morse Network Simulation

This document provides a detailed breakdown of `main.cpp` for the Smart Morse Network Simulation project.

---

## 1. Headers and Includes
```cpp
#include <iostream>   // For input and output (std::cout, std::cin)
#include <memory>     // For std::unique_ptr (Smart Pointers/RAII)
#include <string>     // For handling text strings
#include <vector>     // For dynamic arrays (storing paths)
#include <map>        // For the Adjacency List and Encoding Map
#include <queue>      // For the Priority Queue used in Dijkstra
#include <algorithm>  // For std::reverse (path reconstruction)
#include <limits>     // For std::numeric_limits (setting "Infinity" distances)
#include <set>        // For keeping track of unique node names
```

---

## 2. The MorseSystem Class (Trie + Map)

### The Node Structure
```cpp
struct Node {
    char data;                 // The character (e.g., 'A')
    std::unique_ptr<Node> dot;  // Pointer to the "Left" child (Dot)
    std::unique_ptr<Node> dash; // Pointer to the "Right" child (Dash)
};
```
*   **Line-by-line**: We use `std::unique_ptr` so that modern C++ handles memory automatically. When the `MorseSystem` is destroyed, the entire tree is deleted without memory leaks.

### Encoding Logic (Text -> Morse)
```cpp
std::string encode(const std::string& text) {
    std::string result;
    for (char c : text) {
        char upper_c = std::toupper(c);
        if (upper_c == ' ') {
            result += "   "; // Add 3 spaces for word separation
        } else if (encoding_map.count(upper_c)) {
            result += encoding_map[upper_c] + " "; // Fetch Morse from map
        }
    }
    return result;
}
```
*   **Logic**: Loops through the user's string. If it sees 'H', it looks up the code in the map and adds `.... ` to the result string.

### Decoding Logic (Morse -> Text)
```cpp
char decode_char(const std::string& code) const {
    Node* current = root.get(); // Start at the root of the tree
    for (char symbol : code) {
        if (symbol == '.') current = current->dot.get();
        else if (symbol == '-') current = current->dash.get();
    }
    return current->data; // Return the character found at that leaf
}
```
*   **Logic**: This is a **Trie Traversal**. For every dot or dash in a code, we walk down the tree. If we follow `- .`, we land on node 'N'.

---

## 3. The CampusNetwork Class (Graphs & Dijkstra)

### Adjacency List
```cpp
std::map<std::string, std::vector<Edge>> adj;
```
*   **Explanation**: This is the heart of the graph. It maps a location name (Key) to a list of its neighbors (Value).

### Dijkstra’s Algorithm Implementation
```cpp
std::vector<std::string> findShortestPath(...) {
    std::map<std::string, int> dist; // Stores distance from start to every node
    std::map<std::string, std::string> parent; // Tracks where we came from
    
    // Set all distances to "Infinite" initially
    for (const auto& node : nodes) dist[node] = std::numeric_limits<int>::max();

    // Priority Queue: Always picks the node with the SHORTEST current distance
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        std::string u = pq.top().second; // Get node with smallest distance
        pq.pop();

        for (const auto& edge : adj[u]) {
            // RELAXATION: If we found a cheaper way to get here, update it
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                parent[edge.to] = u; // Remember the path
                pq.push({dist[edge.to], edge.to});
            }
        }
    }
}
```
*   **Viva Note**: Explain that Dijkstra is "Greedy." It always explores the closest node first, guaranteeing the absolute shortest path in a graph with positive weights.

---

## 4. The Simulation Runner (Main Function)

### Default Graph Setup
```cpp
network.addConnection("Hostel", "Lab", 2);
network.addConnection("Hostel", "Library", 5);
```
*   **Explanation**: This populates our "Smart Campus" with the initial 4 buildings and their distances.

### The Live Simulation Display
```cpp
for (size_t i = 0; i < path.size(); ++i) {
    std::cout << "Step " << i+1 << ": Node [" << path[i] << "] ";
    // Logic to label Origin, Relay, or Destination...
    std::cout << "\n   Signal Content: " << encoded_msg << "\n";
}
```
*   **Explanation**: This loop walks through the `path` vector (which Dijkstra filled) and prints the status of the "Signal" at every hop.

---

## Summary for Viva
1.  **Complexity**: Dijkstra is $O(E \log V)$ and Morse decoding is $O(L)$. Both are highly efficient.
2.  **Safety**: We use `std::unique_ptr` and `std::string` instead of raw pointers and `char*` arrays to ensure the program is crash-proof.
3.  **Extensibility**: The user can add new buildings (nodes) at runtime, and Dijkstra will recalculate the paths instantly.
