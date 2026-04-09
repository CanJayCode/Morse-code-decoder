#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>
#include <set>

/**
 * --- MORSE SYSTEM ---
 * Node structure for the Morse code binary tree.
 * Moving left represents a dot (.)
 * Moving right represents a dash (-)
 */
struct Node {
    char data;
    std::unique_ptr<Node> dot;  // left subtree
    std::unique_ptr<Node> dash; // right subtree

    explicit Node(char c = '\0') : data(c), dot(nullptr), dash(nullptr) {}
};

/**
 * MorseSystem class handles both encoding and decoding.
 * Uses a Binary Tree for decoding and a Map for encoding.
 */
class MorseSystem {
public:
    MorseSystem() : root(std::make_unique<Node>()) {
        initialize();
    }

    /**
     * Converts plain text to Morse code string.
     */
    std::string encode(const std::string& text) {
        std::string result;
        for (char c : text) {
            char upper_c = std::toupper(c);
            if (upper_c == ' ') {
                result += "   "; // Conventional 3-space gap between words
            } else if (encoding_map.count(upper_c)) {
                result += encoding_map[upper_c] + " ";
            }
        }
        return result;
    }

    /**
     * Decodes a Morse code string back to plain text.
     */
    std::string decode(const std::string& message) const {
        std::string result;
        std::string current_seq;
        
        for (size_t i = 0; i < message.size(); ++i) {
            char c = message[i];
            
            if (c == '.' || c == '-') {
                current_seq += c;
            } else if (c == ' ') {
                if (!current_seq.empty()) {
                    result += decode_char(current_seq);
                    current_seq.clear();
                }
                
                // Check for multiple spaces (word separation)
                // If it's a gap between words (usually 3 spaces), add a space to output
                if (i + 1 < message.size() && message[i+1] == ' ') {
                    if (result.empty() || result.back() != ' ') {
                        result += ' ';
                    }
                }
            }
        }
        
        if (!current_seq.empty()) {
            result += decode_char(current_seq);
        }
        
        return result;
    }

private:
    std::unique_ptr<Node> root;
    std::map<char, std::string> encoding_map;

    /**
     * Traverses the tree to decode a single Morse sequence.
     */
    char decode_char(const std::string& code) const {
        Node* current = root.get();
        for (char symbol : code) {
            if (symbol == '.') {
                if (!current->dot) return '?';
                current = current->dot.get();
            } else if (symbol == '-') {
                if (!current->dash) return '?';
                current = current->dash.get();
            }
        }
        return (current->data != '\0') ? current->data : '?';
    }

    /**
     * Builds the tree and the encoding map simultaneously.
     */
    void insert(char ch, const std::string& code) {
        Node* current = root.get();
        for (char symbol : code) {
            if (symbol == '.') {
                if (!current->dot) current->dot = std::make_unique<Node>();
                current = current->dot.get();
            } else if (symbol == '-') {
                if (!current->dash) current->dash = std::make_unique<Node>();
                current = current->dash.get();
            }
        }
        current->data = ch;
        encoding_map[ch] = code; // Store for encoding
    }

    void initialize() {
        static const std::vector<std::pair<char, std::string>> alphabet = {
            {'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
            {'E', "."},    {'F', "..-."}, {'G', "--."},  {'H', "...."},
            {'I', ".."},   {'J', ".---"}, {'K', "-.-"},  {'L', ".-.."},
            {'M', "--"},   {'N', "-."},   {'O', "---"},  {'P', ".--."},
            {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
            {'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"},
            {'Y', "-.--"}, {'Z', "--.."},
            {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
            {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
            {'9', "----."}, {'0', "-----"}
        };

        for (const auto& item : alphabet) {
            insert(item.first, item.second);
        }
    }
};

/**
 * --- NETWORK SYSTEM ---
 * Represents a connection between two campus locations.
 */
struct Edge {
    std::string to;
    int weight;
};

class CampusNetwork {
public:
    /**
     * Adds a bidirectional connection between two nodes.
     */
    void addConnection(const std::string& u, const std::string& v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Campus network is usually undirected
        nodes.insert(u);
        nodes.insert(v);
    }

    /**
     * Dijkstra's Algorithm to find the shortest path.
     */
    std::vector<std::string> findShortestPath(const std::string& start, const std::string& end, int& total_cost) {
        std::map<std::string, int> dist;
        std::map<std::string, std::string> parent;
        
        for (const auto& node : nodes) {
            dist[node] = std::numeric_limits<int>::max();
        }

        using NodeDist = std::pair<int, std::string>;
        std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            std::string u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;
            if (u == end) break;

            if (adj.count(u)) {
                for (const auto& edge : adj.at(u)) {
                    if (dist[u] + edge.weight < dist[edge.to]) {
                        dist[edge.to] = dist[u] + edge.weight;
                        parent[edge.to] = u;
                        pq.push({dist[edge.to], edge.to});
                    }
                }
            }
        }

        total_cost = dist[end];
        std::vector<std::string> path;
        
        if (dist[end] == std::numeric_limits<int>::max()) {
            return path; // No path found
        }

        // Reconstruct the path from end to start
        for (std::string at = end; at != ""; at = parent[at]) {
            path.push_back(at);
            if (at == start) break;
        }
        std::reverse(path.begin(), path.end());
        
        return path;
    }

    void displayNodes() {
        if (nodes.empty()) {
            std::cout << "(No nodes in network)\n";
            return;
        }
        std::cout << "Available Nodes: ";
        for (auto const& n : nodes) {
            std::cout << "[" << n << "] ";
        }
        std::cout << "\n";
    }

private:
    std::map<std::string, std::vector<Edge>> adj;
    std::set<std::string> nodes;
};

/**
 * --- SIMULATION RUNNER ---
 */
int main() {
    MorseSystem morse;
    CampusNetwork network;

    // Default Scenario Setup (Hostel, Library, Lab, Admin)
    network.addConnection("Hostel", "Lab", 2);
    network.addConnection("Hostel", "Library", 5);
    network.addConnection("Lab", "Admin", 4);
    network.addConnection("Library", "Admin", 3);

    std::cout << "========================================\n";
    std::cout << "   SMART MORSE CAMPUS NETWORK SIM      \n";
    std::cout << "========================================\n";

    while (true) {
        std::cout << "\nMENU:\n";
        std::cout << "1. Send Morse Message (Dijkstra Path)\n";
        std::cout << "2. Add/Update Network Connection\n";
        std::cout << "3. View Campus Nodes\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter selection: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(); // flush newline

        if (choice == 1) {
            std::string src, dest, msg;
            network.displayNodes();
            
            std::cout << "Enter Source Location: "; std::getline(std::cin, src);
            std::cout << "Enter Target Location: "; std::getline(std::cin, dest);
            std::cout << "Enter Message Text   : "; std::getline(std::cin, msg);

            int total_weight = 0;
            std::vector<std::string> path = network.findShortestPath(src, dest, total_weight);

            if (path.empty()) {
                std::cout << "\n[!] Transmission Failed: No route found between " << src << " and " << dest << "\n";
            } else {
                std::string encoded_msg = morse.encode(msg);
                
                std::cout << "\n--- STARTING TRANSMISSION ---\n";
                std::cout << "Original Text : " << msg << "\n";
                std::cout << "Morse Signal  : " << encoded_msg << "\n\n";

                for (size_t i = 0; i < path.size(); ++i) {
                    std::cout << "Step " << i+1 << ": Node [" << path[i] << "] ";
                    if (i == 0) std::cout << "(Originating Signal)";
                    else if (i == path.size() - 1) std::cout << "(Final Destination! Signal Received)";
                    else std::cout << "(Relaying Signal...)";
                    
                    std::cout << "\n   Signal Content: " << encoded_msg << "\n";
                }

                std::cout << "\n--- TRANSMISSION COMPLETE ---\n";
                std::cout << "Shortest Path  : ";
                for (size_t i = 0; i < path.size(); ++i) {
                    std::cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
                }
                std::cout << "\nTotal Path Cost: " << total_weight << " units\n";
                std::cout << "Final Decoded Message: " << morse.decode(encoded_msg) << "\n";
            }
        } 
        else if (choice == 2) {
            std::string u, v; int w;
            std::cout << "Add connection from: "; std::cin >> u;
            std::cout << "Add connection to  : "; std::cin >> v;
            std::cout << "Connection weight  : "; std::cin >> w;
            network.addConnection(u, v, w);
            std::cout << "Network updated successfully.\n";
        }
        else if (choice == 3) {
            network.displayNodes();
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    std::cout << "\nStopping simulation. Goodbye!\n";
    return 0;
}
