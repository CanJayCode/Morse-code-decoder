# Technical Explanation: Smart Morse Network Simulation

## 📖 Overview
This project is a high-performance C++ simulation that combines **Graph Theory** (for physical routing) and **Tree Data Structures** (for communication protocols). It models a real-world scenario where university campus devices communicate over a weighted network using Morse signals.

---

## 🏗️ System Components

### 1. Morse Code Subsystem (`MorseSystem`)
**Approach:** Hybrid Binary Tree (Trie) and Hash Map.

*   **Decoding (Morse to Text):**
    *   **Logic:** Uses a binary tree where every "Dot" is a move to the left child and every "Dash" is a move to the right child.
    *   **Performance:** $O(L)$ where $L$ is the length of the Morse code sequence.
    *   **Benefit:** Extremely memory efficient and follow safe memory patterns using `std::unique_ptr`.
*   **Encoding (Text to Morse):**
    *   **Logic:** Uses a `std::map<char, std::string>` for $O(1)$ average-time lookup per character.
    *   **Constraint:** Automatically handles Uppercase/Lowercase conversion via `std::toupper`.

### 2. Network Routing Subsystem (`CampusNetwork`)
**Approach:** Adjacency List with Dijkstra's Shortest Path.

*   **Data Representation:** 
    *   Uses a `std::map<std::string, std::vector<Edge>>` to store connections. 
    *   This is the **Adjacency List** pattern, which is optimal for sparse graphs like a campus layout.
*   **Dijkstra's Algorithm Implementation:**
    ```cpp
    using NodeDist = std::pair<int, std::string>;
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;
    ```
    *   **Greedy Selection:** We use a `std::priority_queue` to always select the node with the absolute minimum distance from the current pool.
    *   **Path Reconstruction:** A `parent` map stores the "best previous hop" for every node. Once the destination is reached, we trace back from `Destination -> Source` and reverse the list to get the final path.

---

## ⚙️ Detailed Algorithm Walkthrough

### Sending a Morse Message: Step-by-Step

| Step | Action | Logic Applied |
|:---|:---|:---|
| **1** | **Input Message** | User enters "LAB" |
| **2** | **Encoding** | `MorseSystem` converts "LAB" into `.-.. .- -...` |
| **3** | **Initialize Dijkstra** | `dist["Hostel"] = 0`, all others = $\infty$ |
| **4** | **Relaxation** | The algorithm "relaxes" edges. If `dist[u] + weight < dist[v]`, update `dist[v]`. |
| **5** | **Path Found** | Finds Shortest Path: `Hostel -> Lab -> Admin` (Cost: 6) |
| **6** | **Simulation** | The loop prints the "Content: `.-.. .- -...`" at each node in the path. |
| **7** | **Final Decoding** | At destination (Admin), the Morse string is passed to the Binary Tree to get "LAB". |

---

## 🛠️ Technical Decisions & Trade-offs

### Decision 1: Adjacency List vs. Adjacency Matrix
*   **Chosen:** Adjacency List.
*   **Reason:** Campus networks don't have every building connected to every other building. A list saves significant memory ($O(V+E)$) compared to a matrix ($O(V^2)$).

### Decision 2: Smart Pointers (`std::unique_ptr`)
*   **Chosen:** `std::unique_ptr` for MorseTree nodes.
*   **Reason:** Prevents memory leaks. In a viva, this shows mastery of **RAII (Resource Acquisition Is Initialization)**—the tree manages its own memory without needing a destructor.

### Decision 3: Priority Queue in Dijkstra
*   **Chosen:** `std::priority_queue` with `std::greater`.
*   **Reason:** Without a priority queue, Dijkstra would be $O(V^2)$. With it, it becomes $O(E \log V)$, which is professional-grade optimization.

---

## 🚀 How to Run & Verify
1. **Compile:** `g++ -std=c++17 main.cpp -o main`
2. **Run:** `./main`
3. **Test Case:**
   *   Source: `Hostel`, Dest: `Admin`, Msg: `HELLO`
   *   **Expectation:** Path: `Hostel -> Lab -> Admin`, Cost: 6.

---

## 👨‍🏫 Viva Questions (Predictor)
1. **Why use a Binary Tree for Morse?** Because Morse is a prefix code. A tree allows us to decode as we read each symbol without looking ahead.
2. **What happens if a node has no path?** Dijkstra will leave its distance as `std::numeric_limits<int>::max()`, and the `findShortestPath` function will return an empty vector.
3. **Is the graph directed?** No, in this simulation, it's undirected (if A connects to B, B connects to A).
