#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
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
 * MorseTree class handles the building and decoding logic.
 * Uses std::unique_ptr to manage memory automatically (RAII).
 * This follows modern C++ practices as outlined in the cpp-pro skill.
 */
class MorseTree {
public:
    MorseTree() : root(std::make_unique<Node>()) {
        initialize();
    }

    /**
     * Decodes a full message of Morse code separated by spaces.
     * Convention: 
     * - Single space between characters.
     * - Multiple spaces (e.g., 3 spaces) between words.
     */
    std::string decode_message(const std::string& message) const {
        std::string result;
        std::string current_symbol_seq;
        
        for (size_t i = 0; i < message.size(); ++i) {
            char c = message[i];
            
            if (c == '.' || c == '-') {
                current_symbol_seq += c;
            } else if (c == ' ') {
                // End of a letter
                if (!current_symbol_seq.empty()) {
                    result += decode_char(current_symbol_seq);
                    current_symbol_seq.clear();
                }
                
                // If there are multiple spaces, add a space to the output (word separation)
                if (i + 1 < message.size() && message[i+1] == ' ') {
                    if (result.empty() || result.back() != ' ') {
                        result += ' ';
                    }
                }
            }
        }
        
        // Don't forget the last letter if the string doesn't end in a space
        if (!current_symbol_seq.empty()) {
            result += decode_char(current_symbol_seq);
        }
        
        return result;
    }

private:
    std::unique_ptr<Node> root;

    /**
     * Traverses the tree to decode a single Morse sequence.
     * Returns '?' if the sequence is invalid.
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
     * Internal helper to build the tree.
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
    }

    /**
     * Populates the tree with standard Morse code values.
     */
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

int main() {
    MorseTree decoder;
    
    std::cout << "====================================\n";
    std::cout << "   Morse Code Decoder (Binary Tree) \n";
    std::cout << "====================================\n";
    std::cout << "Rules:\n";
    std::cout << "  - Type dots (.) and dashes (-)\n";
    std::cout << "  - Single space between letters\n";
    std::cout << "  - Triple space between words\n";
    std::cout << "  - Type 'exit' to quit\n\n";
    std::cout << "Example: .... . .-.. .-.. ---   .-- --- .-. .-.. -..\n";
    std::cout << "         (Decodes to: HELLO WORLD)\n\n";

    std::string input;
    while (true) {
        std::cout << "Morse > ";
        if (!std::getline(std::cin, input) || input == "exit") break;
        if (input.empty()) continue;

        std::string result = decoder.decode_message(input);
        std::cout << "Text  > " << result << "\n\n";
    }

    std::cout << "Goodbye!\n";
    return 0;
}
