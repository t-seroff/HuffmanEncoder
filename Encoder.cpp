#include "Encoder.h"
#include <iostream>
#include <string>

#define SPACE 26
#define COMMA 27
#define NEWLINE 28

#define TRIE -1
#define EMPTY 0

using namespace std;

//*** Character frequency counting methods
Encoder::Encoder(){
	// Initialize frequency array
	for (int i = 0; i < 27; i++){
		frequencies[i] = 0;
	}
}

void Encoder::addChar(char c){
	// Increment frequency count for each character read from the input file
	if (c == ' '){
		frequencies[SPACE]++;	
	}
	else if (c == ','){
		;
	}
	else if (c == '\n'){
		;
	}
	else{
		frequencies[((int) c) - 97]++;
	}
}


//*** Heap management methods
void Encoder::initHeap(){
	// Initialize the heap by adding all characters to it
	nzChars = 0;
	for (int i = 0; i < 27; i++){
		if (frequencies[i] != 0){
			nzChars++;
		}
	}

	heap = new heapNode*[nzChars+1];
	heapNode emptyEntry;
	emptyEntry.value = EMPTY;
	emptyEntry.weight = 0;
	heap[0] = &emptyEntry;

	encodings = new encodingNode*[nzChars];

	int heapCtr = 1;
	for (int i = 0; i < 27; i++){
		if (frequencies[i] != 0){
			char c;
			if (i == 26){
				c = ' ';				
			}
			else{
				c = char(i + 97);
			}
			
			heapNode* entry = new heapNode;
			entry->value = c;
			entry->weight = frequencies[i];
			entry->left = 0;
			entry->right = 0;
			heap[heapCtr] = entry;
			heapCtr++;

			encodingNode* encodingEntry = new encodingNode;
			encodingEntry->value = c;
			encodingEntry->encoding = "";
			encodings[heapCtr-2] = encodingEntry;
		}
	}
	heapEntries = nzChars;
}

void Encoder::buildHeap(){
	
	/*
	for (int i = 1; i < 10; i++){
		for (int i = heapEntries; i > 1; i--){
			percolateUp(i);
		}
	}
	*/

	// Find log(heapEntries)
	int logHE = 0;
	for (int i = heapEntries; i > 0; i = i/2){
		logHE++;
	}

	// Calculate index of first node above leaves
	int index = 1;
	for (int j = 1; j < logHE; j++){
		index *= 2;
	}
	index--;

	// Percolate down all nodes starting from calculated index
	for (int j = index; j >= 1; j--){
		percolateDown(j);
	}
}

void Encoder::buildTrie(){
	// Removes two nodes from heap, adds to a trie, re-inserts to heap, repeat until entire heap is one trie
	while(heapEntries > 1){

		// Remove two nodes from the heap
		heapNode* smallest = remove();
		heapNode* secondSmallest = remove();

		// Add those two nodes to a new trie
		heapNode* trie = new heapNode;
		trie->value = TRIE;
		trie->right = smallest;
		trie->left = secondSmallest;
		trie->weight = getTrieWeight(trie);
		
		// Insert new trie into heap
		insert(trie);
	}
}

int Encoder::getTrieWeight(heapNode* root){
	// Calculate weight of a trie node by summing weights of all children nodes
	if (root == 0){
		// Null pointer has no weight
		return 0;
	}
	else if (root->value != TRIE){
		// Weight of non-trie node is trivial
		return root->weight;
	}
	else{
		// Weight of a trie node is sum of weights of children nodes
		return getTrieWeight(root->left) + getTrieWeight(root->right);
	}
}

void Encoder::insert(heapNode* node){
	// Insert a node at bottom of the heap
	heapEntries++;
	heap[heapEntries] = node;

	// Traverse heap from bottom, percolating up through parents as necessary
	for (int i = heapEntries; i > 1; i = i/2){
		int parent = i/2;
		if (heap[i]->weight < heap[parent]->weight){
			heapNode* temp = heap[parent];
			heap[parent] = heap[i];
			heap[i] = temp;
		}
	}
}

heapNode* Encoder::remove(){
	// Remove node from top of heap and swap last node to top
	heapNode* minNode = heap[1];
	heap[1] = heap[heapEntries];
	heapEntries--;

	// Percolate down to satisfy invariant as necessary
	percolateDown(1);

	return minNode;
}

void Encoder::percolateDown(int i){
	// Determine less of two children, then swap with parent

	// Find indexes of children nodes
		int leftChild = i*2;
		if (leftChild > heapEntries){
			leftChild = 0;
		}
		int rightChild = (i*2)+1;
		if (rightChild > heapEntries){
			rightChild = 0;
		}

	// Determine if/where to swap
	if (rightChild == 0){
		// No right child, only left
		if (leftChild == 0){
			// No left child either, do nothing
			return;
		}
		else if (heap[leftChild]->weight < heap[i]->weight){
			// Left child is less than parent, swap with left child
			heapNode* temp = heap[i];
			heap[i] = heap[leftChild];
			heap[leftChild] = temp;
			percolateDown(leftChild);
		}
	}	
	else{
		// Both children present
		if ( (heap[leftChild]->weight < heap[rightChild]->weight) && (heap[leftChild]->weight < heap[i]->weight) ){
			// Left child is less than right and parent, swap with left child
			heapNode* temp = heap[i];
			heap[i] = heap[leftChild];
			heap[leftChild] = temp;
			percolateDown(leftChild);
		}
		else if ( (heap[rightChild]->weight <= heap[leftChild]->weight) && (heap[rightChild]->weight < heap[i]->weight) ){
			// Right child is less than or equal to left and less than parent, swap with right child
			heapNode* temp = heap[i];
			heap[i] = heap[rightChild];
			heap[rightChild] = temp;
			percolateDown(rightChild);
		}
		else{
			// Neither child is smaller than parent, do nothing
			return;
		}
	}
	
}

void Encoder::percolateUp(int i){
	// Traverse heap from bottom, percolating up through parents as necessary

	// Percolation is done if the root node is reached
	if (i == 1){
		return;
	}

	// Find parent node's index
	int parent = i/2;

	// Percolate up if necessary
	if (heap[parent]->weight > heap[i]->weight){
		heapNode* temp = heap[parent];
		heap[parent] = heap[i];
		heap[i] = temp;
		// Percolate new parent upwards, if necessary
		percolateUp(parent);
	}
}


//*** Final encoding methods

void Encoder::encodeDecode(){

	FILE * sample = fopen("sample.input", "r");
	while(true){
		char c = fgetc(sample);
		if (c != EOF){
			for (int i = 0; i < nzChars; i++){
				if (encodings[i]->value == c){
					cout << encodings[i]->encoding;
				}
			}
		}
		else{
			cout << endl;
			break;
		}
	}

	FILE * encoded = fopen("encoded.input", "r");
	string code = "";
	while(true){ // Each letter in the file
		char c = fgetc(encoded);
		if (c == EOF){
			//cout << endl;
			break;
		}
		code = code + c;

		for (int i = 0; i < nzChars; i++){
			if (encodings[i]->encoding.compare(code) == 0){
				cout << encodings[i]->value;
				code = "";
			}
		}
	}
}


void Encoder::buildEncodings(){
	// Search for each character in the trie to find its encoding
	for (int i = 0; i < nzChars; i++){
		encodings[i]->encoding = search(encodings[i]->value, heap[1]);
	}
}

string Encoder::search(char c, heapNode* triePtr){
	// Searches trie for specified character and returns its encoding

	if (triePtr == 0){
		// Null pointer, the character is not found on this path
		return "-1"; 
	}
	else if (triePtr->value == c){
		// If current node holds desired character, return but do not append to encoding
		return "";
	}
	else{
		string leftCode = search(c, triePtr->left);
		string rightCode = search(c, triePtr->right);
		if (leftCode.compare("-1") != 0){
			// Add a 1 in the character is found from the left child of current node
			return "1" + leftCode;
		}
		else if (rightCode.compare("-1") != 0){
			// Add a 0 if the character is found from the right child of the current node
			return "0" + rightCode;
		}
		// The current node has no children
		return "-1";
	}
}