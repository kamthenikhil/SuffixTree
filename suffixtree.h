/*
 * suffixtree.h
 *
 *  Created on: 13-Feb-2016
 *      Author: nikhil
 */

#ifndef SUFFIXTREE_H_
#define SUFFIXTREE_H_

#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#define ASCII_CODE_LIMIT 256

using namespace std;

class SuffixTree {

private:
	struct SuffixNode {
		/**
		 * The following attribute stores the child nodes.
		 */
		SuffixNode *childNodes[ASCII_CODE_LIMIT];
		/**
		 * The following attribute stores the suffix link from the current node.
		 */
		SuffixNode *suffixLink;
		/**
		 * The following attribute stores the start index of the text stored on the node.
		 */
		int startIndex;
		/**
		 * The following attribute stores pointer to the end index of the text stored on the node.
		 * As we need to update the value of this attribute using a reference to the global end variable.
		 */
		int *endIndex;
		/**
		 * The following attribute stores the index of the suffix for the leaf nodes.
		 */
		int index;
		/**
		 * The following attribute is used to identify the Maximal Unique Matches (MUMs).
		 */
		bool isMarked = false;
		/**
		 * The following attribute is used to store the height of the current node from the root.
		 * It is then used to find the sequence of characters to the node from the root.
		 */
		int height;
	};
	/**
	 * The following method is used to create a Suffix Node.
	 */
	SuffixNode *createSuffixNode(int start, int *end);
	/**
	 * The following method returns the edge length associated with the given node.
	 */
	int edgeLength(SuffixNode *n);
	/**
	 * The following method marks all the potential Maximal Unique Matches (MUMs).
	 * The marked potential MUMs are then used to identify actual MUMs.
	 */
	void markPotentialMUMs(SuffixNode* node);
	/**
	 * The following method is used to run DFS on the Suffix Tree and mark all the leaf nodes with the suffix index.
	 */
	void runDFSMarkLeafIndex(SuffixNode *n, int height);
	/**
	 * The following method initiates the process of identifying Maximal Unique Matches (MUMs).
	 */
	void fetchMUM();
	/**
	 * The following method creates a suffix link from previously created internal node to the current active node.
	 */
	void updateSuffixLink();
	/**
	 * The following method creates a new internal node and a new leaf node.
	 */
	void createInternalNode(int phase, SuffixNode* next);
	/**
	 * The following method updates the active point information when we move over an internal node.
	 */
	void moveOverTheNextNode(SuffixNode* next);
	/**
	 * The following method contains the logic for the Ukkonen's Algorithm which generates Suffix Tree in O(m) time.
	 */
	void runUkkonensAlgorithm();
	/**
	 * This method initiates the construction of Suffix Tree.
	 */
	void build();
	/**
	 * The following method is used to free the dynamically allocated memory.
	 */
	void freeMemory(SuffixNode *n);

public:
	/**
	 * The following attribute stores the input string which is used to build the generalized suffix tree.
	 * This string contains two strings S1 and S2 concatenated using unique special symbols $ and # (e.g. S1$S2#).
	 */
	char *input;
	/**
	 * The following attribute stores the reference to the root node of the suffix tree.
	 */
	SuffixNode *root = NULL;
	/**
	 * 1. activeNode: This attribute stores the pointer to the active node.
	 * 2. lastNewNode: This attribute stores the pointer to the internal node created during previous extension.
	 */
	SuffixNode *activeNode = NULL, *lastNewNode = NULL;
	/**
	 * 1. minimumMUMLength: This attribute stores the minimum length of the Maximal Unique Matches (MUMs) returned.
	 * 2. remaining: This attribute stores the number of suffixes yet to be created during the current phase.
	 * 3. activeEdge: This attribute stores the index of the character.
	 * 4. activeLength: This attribute stores the distance of the active point from the active node.
	 * 5. size: This attribute stores the size of the input string.
	 * 6. rootEnd: This attribute stores the pointer to the end index of the root.
	 * 7. intermediateNodeEnd: This attribute stores the pointer to the end index of the internal nodes created.
	 * 8. globalEnd: This attribute stores the end index of all the leaf nodes of the suffix tree.
	 */
	int minimumMUMLength = 0, remaining = 0, activeEdge = -1, activeLength = 0,
			size = -1, *rootEnd = NULL, *intermediateNodeEnd = NULL, globalEnd =
					-1;
	/**
	 * The following attribute stores the file stream object.
	 */
	ofstream outputFileStream;
	/**
	 * The following vector is used to store the probable Maximal Unique Matches (MUMs).
	 */
	vector<SuffixNode*> mumList;

	/**
	 * Parametrized Constructor.
	 */
	SuffixTree(char* input, char* outputFilePath, int minimumMUMLength) {
		this->input = input;
		this->minimumMUMLength = minimumMUMLength;
		outputFileStream.open(outputFilePath, ofstream::out);
		build();
		outputFileStream.close();
	}

	/**
	 * Destructor
	 */
	~SuffixTree() {
		if (root != NULL) {
			freeMemory(root);
		}
	}

};

#endif /* SUFFIXTREE_H_ */
