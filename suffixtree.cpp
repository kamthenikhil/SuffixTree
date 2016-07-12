/*
 * suffixtree.cpp
 *
 *  Created on: 13-Feb-2016
 *      Author: nikhil
 */

#include "suffixtree.h"

SuffixTree::SuffixNode* SuffixTree::createSuffixNode(int start, int *end) {
	SuffixNode *node = (SuffixNode*) malloc(sizeof(SuffixNode));
	node->startIndex = start;
	node->endIndex = end;
	for (int i = 0; i < ASCII_CODE_LIMIT; i++) {
		node->childNodes[i] = NULL;
	}
	node->suffixLink = root;
	node->index = -1;
	node->height = 0;
	return node;
}

int SuffixTree::edgeLength(SuffixNode *n) {
	return *(n->endIndex) - (n->startIndex) + 1;
}

void SuffixTree::markPotentialMUMs(SuffixNode* node) {
	if (node != root && node->suffixLink != NULL) {
		node->suffixLink->isMarked = false;
		markPotentialMUMs(node->suffixLink);
	}
}

void SuffixTree::runDFSMarkLeafIndex(SuffixNode *n, int height) {

	if (n != NULL) {
		bool isLeaf = true;
		int childNodeCount = 0;
		for (int i = 0; i < ASCII_CODE_LIMIT; i++) {
			if (n->childNodes[i] != NULL) {
				n->childNodes[i]->height = height;
				isLeaf = false;
				runDFSMarkLeafIndex(n->childNodes[i],
						height + edgeLength(n->childNodes[i]));
				childNodeCount = childNodeCount + 1;
			}
		}
		/**
		 * The following block is used to check if the internal node has 2 children such that
		 * one belongs to the first string and the other one belongs to the second string.
		 */
		if (childNodeCount == 2 && n->startIndex != -1) {
			int numberOfHashes = 0;
			int numberOfDollars = 0;
			for (int i = 0; i < ASCII_CODE_LIMIT; i++) {
				if (n->childNodes[i] != NULL) {
					for (int j = n->childNodes[i]->startIndex;
							j <= *(n->childNodes[i]->endIndex); j++) {
						if (input[j] == '#') {
							numberOfHashes = numberOfHashes + 1;
						}
						if (input[j] == '$') {
							numberOfDollars = numberOfDollars + 1;
						}
					}
				}
			}
			/**
			 * The following block of code checks if the two child node correspond to different strings.
			 */
			if (numberOfDollars == 1 && numberOfHashes == 2) {
				n->isMarked = true;
				if ((*(n->endIndex) - n->startIndex + n->height + 1)
						>= minimumMUMLength) {
					mumList.push_back(n);
				}
			}
		}
		if (isLeaf) {
			n->index = size - height;
		}
	}
}

void SuffixTree::fetchMUM() {
	int height = 0;
	runDFSMarkLeafIndex(root, height);
	for (unsigned int i = 0; i < mumList.size(); i++) {
		markPotentialMUMs(mumList[i]);
	}
	outputFileStream << "MUM:\n";
	for (unsigned int i = 0; i < mumList.size(); i++) {
		if (mumList[i]->isMarked) {
			outputFileStream << "[";
			for (int j = mumList[i]->startIndex - mumList[i]->height;
					j <= *(mumList[i]->endIndex); j++) {
				outputFileStream << input[j];
			}
			outputFileStream << "]\n";
		}
	}
}

void SuffixTree::updateSuffixLink() {
	if (lastNewNode != NULL) {
		lastNewNode->suffixLink = activeNode;
		lastNewNode = NULL;
	}
}

void SuffixTree::createInternalNode(int phase, SuffixNode* next) {
	intermediateNodeEnd = (int*) (malloc(sizeof(int)));
	*intermediateNodeEnd = next->startIndex + activeLength - 1;
	SuffixNode* intermediateNode = createSuffixNode(next->startIndex,
			intermediateNodeEnd);
	activeNode->childNodes[(int) (input[activeEdge])] = intermediateNode;
	intermediateNode->childNodes[(int) (input[phase])] = createSuffixNode(phase,
			&globalEnd);
	next->startIndex += activeLength;
	intermediateNode->childNodes[(int) (input[next->startIndex])] = next;
	if (lastNewNode != NULL) {
		lastNewNode->suffixLink = intermediateNode;
	}
	lastNewNode = intermediateNode;
}

void SuffixTree::moveOverTheNextNode(SuffixNode* next) {
	activeEdge += edgeLength(next);
	activeLength -= edgeLength(next);
	activeNode = next;
}

void SuffixTree::runUkkonensAlgorithm() {

	rootEnd = (int*) malloc(sizeof(int));
	*rootEnd = -1;
	root = createSuffixNode(-1, rootEnd);
	activeNode = root;
	size = strlen(input);
	for (int phase = 0; phase < size; phase++) {
		/**
		 * [Rule 1]: By updating the global end all the leaf nodes are automatically updated.
		 */
		globalEnd = phase;
		remaining++;
		lastNewNode = NULL;
		while (remaining > 0) {
			/**
			 * If the active length is zero, we set the active edge with the index of current character.
			 * This is the case when we start our search from a node.
			 */
			if (activeLength == 0) {
				activeEdge = phase;
			}
			if (activeNode->childNodes[(int) input[activeEdge]] == NULL) {
				/**
				 * This is the case where a path from the active node does not exist.
				 * [Rule 2]: If the path does not exist, create a path.
				 * As a new node is created, we check if there was an internal node created during previous extension.
				 * If yes, we set the newly created node as the suffix link of the previously created internal node.
				 */
				activeNode->childNodes[(int) input[activeEdge]] =
						createSuffixNode(phase, &globalEnd);
				updateSuffixLink();
			} else {
				/**
				 * If a path exists, we traverse the child node in the direction of the current character.
				 */
				SuffixNode *next =
						activeNode->childNodes[(int) input[activeEdge]];
				/**
				 * [Rule 3]: This is the case where we move over an internal node. In this case we set the values of our active point accordingly.
				 * In such cases we keep moving downward till the edge length is strictly greater than active length
				 * i.e. the character lies on the edge.
				 */
				if (activeLength >= edgeLength(next)) {
					moveOverTheNextNode(next);
					continue;
				}
				/**
				 * [Rule 3]: If the next character in the string is found at the position on the next node, we simply increment the active length.
				 */
				if (input[next->startIndex + activeLength] == input[phase]) {
					if (activeNode != root) {
						updateSuffixLink();
					}
					activeLength++;
					break;
				}
				/**
				 * [Rule 2]: If the next character is not found on the child node, we create a new internal node and a new leaf node.
				 * 1. The next node and the new leaf node are then added as the child nodes of the newly created internal node.
				 * 2. Also if an internal node was created during previous extension, the newly created internal node is added as
				 * 	  the suffix link to the previously created internal node.
				 * 3. We then update the value for last new node.
				 */
				createInternalNode(phase, next);
			}
			/**
			 * If we reach this point, then we have created either a leaf or an internal node.
			 * So we decrement the number of suffixes to be created during the phase.
			 */
			remaining--;
			if (activeNode == root && activeLength > 0) {
				activeLength--;
				activeEdge = phase - remaining + 1;
			} else if (activeNode != root) {
				activeNode = activeNode->suffixLink;
			}
		}
	}
}

void SuffixTree::build() {
	runUkkonensAlgorithm();
	fetchMUM();
}

void SuffixTree::freeMemory(SuffixNode *n) {
	if (n != NULL) {
		for (int i = 0; i < ASCII_CODE_LIMIT; i++) {
			if (n->childNodes[i] != NULL) {
				freeMemory(n->childNodes[i]);
			}
		}
		if (n->index == -1) {
			free(n->endIndex);
		}
		free(n);
	}
}
