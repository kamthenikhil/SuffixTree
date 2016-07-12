#include <string.h>
#include <fstream>
#include <time.h>
#include <iostream>

#include "suffixtree.h"

using namespace std;

char* readFile(string filename) {
	ifstream fin(filename);
	filebuf* pbuf = fin.rdbuf();
	size_t size = pbuf->pubseekoff(0, fin.end, fin.in);
	pbuf->pubseekpos(0, fin.in);
	char* inputString = new char[size];
	pbuf->sgetn(inputString, size);
	fin.close();
	return inputString;
}

int main(int argc, char *argv[]) {

	clock_t startTime = clock();
	char* inputString1 = readFile(argv[1]);
	char* inputString2 = readFile(argv[2]);

	int size = strlen(inputString1) + strlen(inputString2) + 2;

	cout << "Input Size: " << size - 2 << endl;

	char* input = new char[size];
	input[0] = {0};
	strcat(input, inputString1);
	strcat(input, "$");
	strcat(input, inputString2);
	strcat(input, "#");

	SuffixTree* suffixTree = new SuffixTree(input, argv[3], atoi(argv[4]));
	delete (suffixTree);
	cout << "Time taken: " << (double) (clock() - startTime) / CLOCKS_PER_SEC
			<< "s" << endl;
	return 0;
}
