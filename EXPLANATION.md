# Smart Morse Network Simulation: Comprehensive Explanation

This document provides a high-level summary and a detailed line-by-line breakdown of the **Smart Morse Network Simulation** implemented in `main.cpp`.

---

## 🚀 High-Level Summary

### 1. The Core Concept
The project simulates a communication network where "Smart Campus" locations (Nodes) are connected by physical links (Edges) with specific costs/weights. A user can send a text message from any node to another. The system automatically:
1.  **Encodes** the text into Morse Code.
2.  **Calculates** the shortest physical path between the nodes using **Dijkstra's Algorithm**.
3.  **Simulates** the transmission by showing the signal passing through each relay node.
4.  **Decodes** the Morse signal back to text at the final destination using a **Binary Tree (Trie)**.

### 2. Architecture & Algorithms
*   **Morse Decoding (Binary Tree)**: A `MorseTree` structure handles decoding. It is a prefix tree where `.` means "go left" and `-` means "go right." This ensures $O(L)$ lookup (where $L$ is the length of the signal).
*   **Morse Encoding (Map)**: A `std::map` is used for encoding text to Morse for instant $O(1)$ character lookup.
*   **Pathfinding (Dijkstra)**: A greedy algorithm that uses a **Priority Queue** to explore nodes. It finds the path with the minimum sum of edge weights.
*   **Memory Management**: The project uses **RAII (Resource Acquisition Is Initialization)** with `std::unique_ptr`, ensuring that there are no memory leaks without needing manual `delete` calls.

---

## 📝 Line-by-Line Detailed Explanation

### Part 1: Header Includes & Data Structures (Lines 1–23)

- **Line 1–9**: Standard C++ library headers.
    - `iostream`: Input/Output streams.
    - `memory`: For `std::unique_ptr` (smart pointers).
    - `queue`: For the priority queue used in Dijkstra.
    - `algorithm`: For `std::reverse` to flip the path.
    - `limits`: To use `std::numeric_limits<int>::max()` (representing Infinity).
- **Line 17–23**: `struct Node`
    - This defines a single node in our **Morse Tree**.
    - `data`: Stores the character (like 'A' or 'B').
    - `dot` and `dash`: Pointers to the left and right children.
    - **Line 22**: Constructor initializes the node. `explicit` prevents implicit conversions.

### Part 2: MorseSystem Class (Lines 29–143)

- **Line 31**: Constructor creates the root node using `std::make_unique`.
- **Line 38–49**: `encode(string text)` function.
    - Loops through each character.
    - Converts to uppercase (`std::toupper`).
    - Uses `encoding_map` to get the Morse string.
    - Adds 3 spaces for word separation (Line 43).
- **Line 54–84**: `decode(string message)` function.
    - This is the main "receiving" logic.
    - It builds a `current_seq` until it hits a space, then calls `decode_char`.
    - **Line 71–75**: Checks for word gaps (multiple spaces) to add spaces back into the English text.
- **Line 93–105**: `decode_char(string code)` function.
    - This performs the **Tree Traversal**.
    - It starts at `root` and moves `current = current->dot` or `current = current->dash`.
    - If it hits a null pointer, the Morse sequence is invalid and returns `?`.
- **Line 110–123**: `insert(char ch, string code)` function.
    - This builds the tree during initialization.
    - It creates nodes as it follows the dot/dash path.
    - **Line 122**: Simultaneously populates the `encoding_map` (Map) for the encoder.
- **Line 125–142**: `initialize()` function.
    - Hardcodes the standard Morse Alphabet (A–Z and 0–9) and inserts them into the system.

### Part 3: CampusNetwork System (Lines 149–234)

- **Line 149–152**: `struct Edge`
    - Represents a physical connection to another node (`to`) and its distance (`weight`).
- **Line 159–164**: `addConnection(u, v, w)`
    - Adds a link in the adjacency list (`adj`).
    - Because the campus path is bidirectional, we add `u->v` and `v->u`.
- **Line 169–217**: `findShortestPath(...)` — **The Dijkstra Algorithm**.
    - **Line 170–171**: Maps for `dist` (current shortest distance to a node) and `parent` (to remember the route).
    - **Line 174**: Sets all initial distances to Infinity.
    - **Line 181**: Pushes the starting node into the Priority Queue with distance 0.
    - **Line 183–200**: The **Core Optimization Loop**.
        - It pulls the "cheapest" node `u`.
        - It relaxes its neighbors: if `dist[u] + weight < dist[neighbor]`, it updates the neighbor's distance and current parent.
    - **Line 210–214**: **Path Reconstruction**.
        - Dijkstra only gives distances. To get the actual path (e.g., A -> B -> C), we follow the `parent` pointers backward from the destination.
- **Line 219–234**: `displayNodes()`
    - Simple helper to show the user which campus locations are available.

### Part 4: Simulation Runner (Lines 239–328)

- **Line 244–247**: **Default Scenario Setup**.
    - Pre-loads the "Hostel", "Library", "Lab", and "Admin" setup for the demo.
- **Line 253–324**: **The Main Application Loop**.
    - A standard menu system.
- **Line 270–305**: **Simulation Implementation (Option 1)**.
    - Gets user input for Source/Destination and Message.
    - **Line 278**: Calls Dijkstra to get the path.
    - **Line 283**: Encodes the message into Morse.
    - **Line 289–296**: **Simulation Loop**.
        - Loops through the `path` vector and prints messages to the screen to simulate real-time relaying of the Morse signal.
    - **Line 304**: Finally calls the decoder at the last node to "receive" the message.
- **Line 307–314**: **Dynamic Network Update (Option 2)**.
    - Allows the user to add new nodes (like "Canteen") or change weights between existing ones during runtime.

---

### Conclusion
This code demonstrates a sophisticated integration of **Tree Traversal**, **Graph Theory (Dijkstra)**, and **Modern C++ Memory Safety**. It effectively separates the "protocol" (Morse) from the "transmission medium" (Network), making it a high-quality project for any technical evaluation.
