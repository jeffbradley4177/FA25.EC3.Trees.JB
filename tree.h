//
// Created by Manju Muralidharan on 11/22/25.
//

#ifndef FA25EC3_TREE_H
#define FA25EC3_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
   TEMPLATE TREE CLASS
   Multiple parents may point to the same child.
   Students must manage storage, pointers, and dynamic allocation.

   Requirements:
   1. Templated Tree class.
   2. Nodes stored dynamically.
   3. Each node has:
      - id
      - data
      - vector of children pointers
   4. Students must prevent memory leaks.
   5. Students must create createRoot, addNode, findNode, print functions, etc.

   DO NOT IMPLEMENT ANYTHING HERE.
   Only placeholders and TODO comments.
*/


template <typename U>
class Node {
public:
    string id;
    U data;
    vector<Node<U>*> children;

    // Write constructor
    Node(const string &nodeID, const U &value) {
        id = nodeID;
        data = value;
    }
};

template <typename T>
class Tree {
private:
    Node<T>* root;
    map<string, Node<T>*> nodeMap; // stores all nodes by ID for memory management

public:
    //Initialize root pointer to nullptr
    Tree() {
        root = nullptr;
    }

    void createRoot(const string &id, const T &value) {
        // Allocate memory, assign id, assign data, set as root
        Node<T>* newNode = new Node<T>(id, value);
        root = newNode;
        nodeMap[id] = newNode; //Add to registry
    }

    void addNode(const string &parentID, const string &childID, const T &value){
        // Find parent, create child, link parent to child
        // Support repeated children under multiple parents

        //Find the parent
        if (nodeMap.find(parentID) == nodeMap.end()) {
            cerr<< "Error: Parent " << parentID << " does not exist" << endl;
            return;
        }
        Node<T>* parent = nodeMap[parentID];

        // Check if child already exists (shared node logic)
        Node<T>* child;
        if (nodeMap.find(childID) != nodeMap.end()) {
            // Child exists, just link it together
            child = nodeMap[childID];
        } else {
            // Child is new, create and register it
            child = new Node<T>(childID, value);
            nodeMap[childID] = child;
        }

        //3. Link Parent to child
        parent->children.push_back(child);
    }

    Node<T>* findNode(const string &id) {
        // Use DFS or BFS to search tree
        if (nodeMap.find(id) != nodeMap.end()) {
            return nodeMap[id];
        }
        return nullptr;
    }

    void printAll() {
        // Print entire structure in readable form

        //Iterate through the map to print the full structure
        for (auto const& pair : nodeMap) {
            string id = pair.first;
            Node<T>* node = pair.second;

            cout << "Node " << id << ": " << node->data << endl;
            for (auto child : node->children) {
                cout << "  Child -> " << child->id << endl;
            }
            cout << endl;
        }
    }

    ~Tree() {
        //Free all allocated memory by iterating the map
        //Handles the "multiple parents" issue
        for (auto const& pair : nodeMap) {
            Node<T>* nodePtr = pair.second;
            delete nodePtr;
        }
    }

    void playGame() {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }
        Node<T>* current = root;

        while (current != nullptr) {
            // 1. Display Current Node
            cout << "---------------------------------" << endl;
            cout << current->data << endl;

            // 2. Check for End of Game (Leaf Node)
            if (current->children.empty()) {
                cout << "---------------------------------" << endl;
                cout << "The story ends here." << endl;
                break;
            }

            // 3. Display Options
            cout << "Choose your path:" << endl;
            int optionIndex = 1;
            for (auto child : current->children) {
                // Take the first 50 characters of the next node's text as a preview
                string preview = child->data;
                if (preview.length() > 50) {
                    preview = preview.substr(0, 50) + "...";
                }

                // Print "1. You follow the path..." instead of "Choose this path"
                cout << optionIndex << ". " << preview << endl;
                optionIndex++;
            }

            // 4. Get User Input
            int choice;
            cout << "> ";
            cin >> choice;

                // Check if input is actually a number
                if (!(cin >> choice)) {
                    cin.clear(); // Clear error flag
                    cin.ignore(10000, '\n'); // Skip bad input
                    cout << "Invalid input. Please enter a number." << endl;
                    continue; // Restart loop
            }

            // 5. Validation and Traversal
            if (choice > 0 && choice <= current->children.size()) {
                current = current->children[choice - 1];
            } else {
                cout << "Invalid choice. Try again." << endl;
            }
        }
    }
};

#endif //FA25EC3_TREE_H
