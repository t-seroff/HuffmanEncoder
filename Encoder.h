#ifndef ENCODER_H
#define ENCODER_H

#include <string>

using namespace std;

struct heapNode{
	char value;
	int weight;
	
	// Children pointers - used when heapNode is a trie
	heapNode* left;
	heapNode* right;
};

struct encodingNode{
	char value;
	string encoding;
};

class Encoder{
	public:
		Encoder();
		void addChar(char);

		void initHeap();
		void buildHeap();
		void buildTrie();
		int getTrieWeight(heapNode*);

		void buildEncodings();
		void encodeDecode();

	private:
		int frequencies[27];
		int nzChars;

		heapNode** heap;
		int heapEntries;

		void insert(heapNode*);
		heapNode* remove();
		void percolateDown(int);
		void percolateUp(int);

		encodingNode** encodings;
		string search(char, heapNode*);
};

#endif