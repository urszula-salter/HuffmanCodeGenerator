#include <iostream> // Include input/output stream library
#include <string> // Include string library

using namespace std; // not a good practice but a little less tying
const int basicASCIIrange = 128; // Maximum number of characters (ASCII range)

// Node class representing a node in a tree
class Node {
private:
    char character; // Character value of the node
    int frequency; // Frequency of the character
    Node* leftChild; // Left child of the node
    Node* rightChild; // Right child of the node

public:
    // Node constructor
    // Initializes a new Node object with the specified character and frequency
   Node(char c, int f) : character(c), frequency(f), leftChild(nullptr), rightChild(nullptr) {}


    // Get the character value of this Node
    //returns the character value of the node
    char getCharacter() const { return character; }

    // Sets the character value of this Node to the specified value c
    void setCharacter(char c) { character = c; }

    // Get the frequency count of this Node
    //returns the frequency count of the node
    int getFrequency() const { return frequency; }

    // Set the frequency count of this Node to the specified value f
    void setFrequency(int f) { frequency = f; }

    // Get the left child Node of this Node
    //returns a pointer to the left child node of the current node
    Node* getLeftChild() const { return leftChild; }

    // Set the left child Node of this Node to the specified node pointer "left"
    void setLeftChild(Node* left) { leftChild = left; }

    // Get the right child Node of this Node
    //returns a pointer to the right child node of the current node
    Node* getRightChild() const { return rightChild; }

    // Set the right child Node of this Node to the specified node pointer "right"
    void setRightChild(Node* right) { rightChild = right; }
};


// Tree class representing the Huffmann tree
class Tree {
private:
    Node* root; // Root node of the Huffmann tree
    string huffCodesStorageCharArray[basicASCIIrange]; // Array to store Huffmann codes for each character

public:
    // Tree class constructor
    // Initializes a new Tree object with the specified character frequencies
    Tree(const int* frequencies);

    // Get the root Node of the Huffmann tree
    Node* getRoot() const { return root; }

    // Set the root Node of the Huffmann tree
    void setRoot(Node* r) { root = r; }

    // Assign Huffmann codes to each character in the Huffmann tree
    void assignHuffCodes(const Node* node, const string& code);

    // Get the array of Huffmann codes for each character in the Huffmann tree
    const string* getCodes() const { return huffCodesStorageCharArray; }

    // Build the Huffmann tree from an array of Nodes
    //Node** nodes parameter is a pointer to a pointer to a Node object
    //used to pass an array of Node pointers into the buildTreeMethod method
    //nodes is a pointer to the first element of an array of Node pointers
    Node* buildTreeMethod(Node** nodes, int counterOfNodes);
};


//Tree constructor
Tree::Tree(const int* frequencies) {
    Node* nodes[basicASCIIrange] = { 0 }; // Array of Node pointers
    int counterOfNodes = 0; // Number of nodes in the array
    for (int i = 0; i < basicASCIIrange; i++) {
        if (frequencies[i] > 0) {
          // Create a new node for each character with a frequency greater than 0
          //new operator to dynamically allocate memory on the heap 
          //for the new Node object and returns a pointer to that memory
          //This pointer is then assigned to the i-th element of the nodes array using the subscript operator [].
            nodes[i] = new Node(i, frequencies[i]); 
            counterOfNodes++; // Increment the node count
        }
    }
    root = buildTreeMethod(nodes, counterOfNodes); // Build the Huffmann tree
    assignHuffCodes(root, ""); // Assign Huffmann codes to characters in the tree
}

// huffmannMethod class that will do the coding and decoding of inputString using the tree
class huffmannMethod {
private:
    Tree tree; // Tree type object
    int* countCharFreqOfInputString(const string& inputString); // Method to count character frequencies in the input inputString

public:
  //Parameter string passed by reference instead of value
  //No need to copy the string, saves memory, improves performance
    huffmannMethod(const string& inputString); // huffmannMethod constructor
    string encode(const string& inputString); // Method to encode inputString
    string decode(const string& encodedInputString); // Method to decode encoded inputString
};

// huffmannMethod constructor
//initializes the Huffmann tree using the character frequencies from the inputString 
//and passes the resulting array to the Tree constructor
huffmannMethod::huffmannMethod(const string& inputString) : tree(countCharFreqOfInputString(inputString)) {}

// Methods
// Method to count character frequencies in the input inputString
int* huffmannMethod::countCharFreqOfInputString(const string& inputString) {
    static int frequencies[basicASCIIrange] = { 0 }; // Initialize an array to store character frequencies
    for (char currentCharacter : inputString) { // Iterate over each character in the inputString
        frequencies[currentCharacter]++; // Increment the frequency of the current character
    }
    return frequencies; // Return the array of character frequencies
}

// Method to build the Huffmann tree
Node* Tree::buildTreeMethod(Node** nodes, int counterOfNodes) {
    while (counterOfNodes > 1) { // Continue until only one node is left (the root node)
        int minimumFreqNode1 = -1, minimumFreqNode2 = -1; // Initialize the indices of the two nodes with the smallest frequencies

        // Iterate over the array of nodes to find the two nodes with the smallest frequencies
        for (int i = 0; i < basicASCIIrange; i++) {
            if (nodes[i]) { // Check if the node exists
                // If minimumFreqNode1 is not set or the current node's frequency is smaller than the minimumFreqNode1 node's frequency
                if (minimumFreqNode1 == -1 || nodes[i]->getFrequency() < nodes[minimumFreqNode1]->getFrequency()) {
                    minimumFreqNode2 = minimumFreqNode1; // Move the minimumFreqNode1 index to minimumFreqNode2
                    minimumFreqNode1 = i; // Set the new minimumFreqNode1 index to the current index
                }
                // If minimumFreqNode2 is not set or the current node's frequency is smaller than the minimumFreqNode2 node's frequency
                else if (minimumFreqNode2 == -1 || nodes[i]->getFrequency() < nodes[minimumFreqNode2]->getFrequency()) {
                    minimumFreqNode2 = i; // Set the new minimumFreqNode2 index to the current index
                }
            }
        }

        // Create a new combined node, a parent, with the sum of the frequencies of the two smallest nodes
        Node* parentNode = new Node(0, nodes[minimumFreqNode1]->getFrequency() + nodes[minimumFreqNode2]->getFrequency());
        parentNode->setLeftChild(nodes[minimumFreqNode1]); // Set the left child to the minimumFreqNode1 node
        parentNode->setRightChild(nodes[minimumFreqNode2]); // Set the right child to the minimumFreqNode2 node

        nodes[minimumFreqNode1] = parentNode; // Replace the minimumFreqNode1 node with the combined node in the array
        nodes[minimumFreqNode2] = nullptr; // Remove the minimumFreqNode2 node from the array

        counterOfNodes--; // -- node count
    }

    // Return the last remaining node (the root node of the Huffmann tree)
    for (int i = 0; i < basicASCIIrange; i++) {
        if (nodes[i]) {
            return nodes[i];
        }
    }

    return nullptr; // Return nullptr if no nodes are found (should not happen)
}


// Method to compute Huffmann code values to each character in the tree
void Tree::assignHuffCodes(const Node* node, const string& huffCodeValue) {
    if (node->getCharacter()) { // If the node contains a character
        huffCodesStorageCharArray[node->getCharacter()] = huffCodeValue; // Assign the current code to the character
    }
    else { // If the node is not a leaf node (does not contain a character)
        assignHuffCodes(node->getLeftChild(), huffCodeValue + "0"); // Traverse the left subtree with code + "0"
        assignHuffCodes(node->getRightChild(), huffCodeValue + "1"); // Traverse the right subtree with code + "1"
    }
}

// Method to encode input text using the Huffman tree
string huffmannMethod::encode(const string& inputString) {
    const string* huffmannCodes = tree.getCodes(); // Retrieve Huffman codes for each character

    string encodedString; // Initialize the encoded text string
    for (char character : inputString) { // Iterate over each character in the input text
        encodedString += huffmannCodes[character]; // Append the Huffman code for the current character to the encoded text
        cout << "Character: " << character << ", Huffmann code: " << huffmannCodes[character] << endl; // Print the character and its Huffman code
    }
    return encodedString; // Return the encoded text
}

// Method to decode encoded inputString using the Huffmann tree
string huffmannMethod::decode(const string& encodedInputString) {
    string decodedInputString; // Initialize the decoded inputString string
    const Node* currentNode = tree.getRoot(); // Start at the root of the Huffmann tree

    for (char c : encodedInputString) { // Iterate over each character in the encoded inputString
        if (c == '0') {
            currentNode = currentNode->getLeftChild(); // Move to the left child if the current character is '0'
        }
        else {
            currentNode = currentNode->getRightChild(); // Move to the right child if the current character is '1'
        }

        if (currentNode->getCharacter()) { // If the current node has a character
            decodedInputString += currentNode->getCharacter(); // Append the character to the decoded inputString
            currentNode = tree.getRoot(); // Reset the current node back to the root
        }
    }

    return decodedInputString; // Return the decoded inputString
}


// Main function
int main() {
    string inputString; // Initialize a string to store the input inputString
    cout << "Please enter a string (128 ASCII range): "; // Prompt the user to enter a string of inputString
    getline(cin, inputString); // Read the input inputString

    huffmannMethod hmObject(inputString); // Initialize a huffmannMethod object with the input inputString

    string encodedInputString = hmObject.encode(inputString); // Encode the input inputString
    cout << "Complete Huffmann code: " << encodedInputString << endl; // Print the encoded inputString

    string decodedInputString = hmObject.decode(encodedInputString); // Decode the encoded inputString
    cout << "Decoded text: " << decodedInputString << endl; // Print the decoded inputString

    return 0; // Return 0 to indicate successful execution
}