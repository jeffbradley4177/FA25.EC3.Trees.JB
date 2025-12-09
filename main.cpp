
//
// Created Originally by Manju Muralidharan on 11/22/25.
//

//
// Finished by Jeffrey Bradley starting on 11/27/25

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "tree.h"
using namespace std;

/*
   Students:
   You will complete the Tree template class.
   main.cpp already:
   1. Runs curl to fetch a story from OpenAI.
   2. Saves output to story.txt.
   3. Parses story.txt into structured nodes.
   4. Calls your Tree methods to build a dynamic tree.

   You must NOT modify this file.
*/

// Structure for raw parsed nodes
struct StoryNodeRaw {
    string id;
    string text;
    vector<string> children;
};

// Step 1. Call OpenAI with curl
string runCurlToOpenAI(const string &apiKey) {
    // 1. Use the modern Responses API endpoint
    string url = "https://api.openai.com/v1/responses";

    // 2. Define the Assignment Prompt
    string prompt = "Generate a choose-your-own-adventure story as a structured list of nodes. "
                    "Each node must have: NODE_ID, NODE_TEXT, CHILD_IDS as a comma-separated list. "
                    "Limit to 5 nodes. The format must be strictly: "
                    "[NODE_ID] TEXT: ... NEXT: child1, child2, ... "
                    "No extra commentary.";

    // 3. Build the command with WINDOWS-SAFE escaping
    //    Outer quotes are " ", inner quotes are \" \"
    string command = "curl " + url + " ";
    command += "-H \"Content-Type: application/json\" ";
    command += "-H \"Authorization: Bearer " + apiKey + "\" ";

    // The Data Block (-d)
    command += "-d \"{";
    command += "\\\"model\\\": \\\"gpt-4o-mini\\\",";  // Using the model from your screenshot
    command += "\\\"input\\\": \\\"" + prompt + "\\\""; // 'input' is the correct field for v1/responses
    command += "}\" ";

    command += " > story.txt";

    cout << "Fetching story from OpenAI (gpt-4o-mini)..." << endl;

    // DEBUG: Print the exact command so we can inspect it
    cout << "-----------------------------------------------------" << endl;
    cout << "DEBUG COMMAND: " << command << endl;
    cout << "-----------------------------------------------------" << endl;

    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Error: Curl command failed. Check your API key and internet." << endl;
    }

    return "story.txt";
}

// Step 2. Parse the story file (Smart Version: Handles JSON & Text)
vector<StoryNodeRaw> parseStoryFile(const string &filename) {
    ifstream fin(filename);
    vector<StoryNodeRaw> nodes;

    if (!fin.is_open()) {
        cerr << "Could not open story file." << endl;
        return nodes;
    }

    // 1. Read the ENTIRE file into a single string buffer
    stringstream buffer;
    buffer << fin.rdbuf();
    string content = buffer.str();

    // 2. PRE-PROCESSING: Fix OpenAI JSON formatting
    // We need to turn literal "\n" (slash-n) into actual newlines
    size_t pos = 0;
    while ((pos = content.find("\\n", pos)) != string::npos) {
        content.replace(pos, 2, "\n"); // Replace 2 chars (\n) with 1 char (newline)
        pos += 1;
    }

    // 3. Now parse the clean content line-by-line
    stringstream ssContent(content);
    string line;
    while (getline(ssContent, line)) {
        if (line.empty()) continue;

        // Skip lines that don't look like nodes (cleaning up JSON wrappers)
        size_t idStart = line.find('[');
        size_t idEnd = line.find(']');
        if (idStart == string::npos || idEnd == string::npos) continue;

        // EXTRACT ID
        string potentialID = line.substr(idStart + 1, idEnd - idStart - 1);

        // SAFETY CHECK: If the ID isn't a number (like "1" or "2"), skip it.
        // This prevents "[Text]" or "[JSON]" from becoming broken nodes.
        if (potentialID.find_first_not_of("0123456789") != string::npos) {
            continue;
        }
        StoryNodeRaw node;
        node.id = line.substr(idStart + 1, idEnd - idStart - 1);

        size_t textPos = line.find("TEXT:");
        size_t nextPos = line.find("NEXT:");
        if (textPos == string::npos || nextPos == string::npos) continue;

        string textPart = line.substr(textPos + 5, nextPos - (textPos + 5));
        node.text = textPart;

        string nextPart = line.substr(nextPos + 5);

        // Clean up any trailing JSON characters (like quotes or brackets)
        size_t jsonEnd = nextPart.find_last_of("0123456789");
        if (jsonEnd != string::npos && jsonEnd < nextPart.size()) {
             nextPart = nextPart.substr(0, jsonEnd + 1);
        }

        stringstream ss(nextPart);
        string temp;
        while (getline(ss, temp, ',')) {
            // Trim spaces
            while (!temp.empty() && temp.front() == ' ') temp.erase(0, 1);
            while (!temp.empty() && temp.back() == ' ') temp.pop_back();

            if (!temp.empty()) node.children.push_back(temp);
        }

        nodes.push_back(node);
    }

    return nodes;
}

// Main Program
int main() {
    cout << "Enter your OpenAI API key: ";
    string apiKey;
    getline(cin, apiKey);

    //COMMENT OUT API CALL FOR NOW
    string filename = runCurlToOpenAI(apiKey);



    vector<StoryNodeRaw> rawNodes = parseStoryFile(filename);
    //Debugging code to verify if the nodes can be found
    cout << "--- RAW NODE CHECK ---" << endl;
    for (const auto& node : rawNodes) {
        cout << "ID: " << node.id << ", Children: ";
        for (const auto& child : node.children) {
            cout << child << ", ";
        }
        cout << endl;
    }
    cout << "----------------------" << endl;

    if (rawNodes.empty()) {
        cerr << "Story was empty or invalid." << endl;
        return 1;
    }

    Tree<string> adventureTree;

    // Students, create the root from rawNodes[0]
    if (!rawNodes.empty()) {
        // The first node in the file is always the root
        adventureTree.createRoot(rawNodes[0].id, rawNodes[0].text);
    }
    // Students, add all remaining nodes
    for (const auto& parentNode : rawNodes) {

        // For this parent, look at all its children IDs
        for (const auto& childID : parentNode.children) {
            // Have the Child ID, but need the Child TEXT to create the node.
            // Must search rawNodes to find the text belonging to childID.
            string childText = "Unknown Text"; // Default fallback

            for (const auto& searchNode : rawNodes) {
                if (searchNode.id == childID) {
                    childText = searchNode.text;
                    break; // Found it, stop searching
                }
            }

            // Now we have everything needed to link them
            adventureTree.addNode(parentNode.id, childID, childText);
        }
    }

    // Students, implement a method in Tree<T> called playGame()
    // See Tree.h file for the requested method


    cout << "Story loaded into your dynamic tree structure." << endl;
    cout << "Implement the Tree class to enable traversal and printing." << endl;

    // Once implemented, uncomment to allow full gameplay.
    adventureTree.playGame();
    return 0;
}