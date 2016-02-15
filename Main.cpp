// Hoffman encoding

#include <iostream>
#include <string>
#include <cstdio>
#include "Encoder.h"

using namespace std;

int main() {
	// Open input file, get frequency counts for characters
	FILE * input = fopen("sample.input", "r");
	Encoder encode;
	while(true)
	{
		char c = fgetc(input);
		if (c != EOF){
			encode.addChar(c);
		}
		else{
			break;
		}
	}
	fclose(input);
	
	// Build heap, then build trie
	encode.initHeap();
	encode.buildHeap();
	encode.buildTrie();
	
	// Build encodings, then encode and decode files
	encode.buildEncodings();
	encode.encodeDecode();
}
