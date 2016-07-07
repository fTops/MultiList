#pragma once
#include "NodeClass.h"

class List {
	static int s_listIDGen; // 1, 2, 3 ...
	int m_listID; // 1, 2, 3, ... , takes its value from s_listIDGen
	int m_nodeCount;

	Node *m_listHead = nullptr;
	List *m_listPrev;
	List *m_listNext = nullptr;

	void mergeFirstNodeWithLast(List *targetList);
	bool saveNodesToFile(const string filename) const;

public:
	List(); // Default Constructor
	List(List *listPrev = nullptr); // Custom Constructor
	List(const List &sourceList); // Copy Constructor
	~List(); // Destructor

	void printList() const;
	bool isListEmpty() const;

	Node* addNewNode(const string text);
	Node* createNewNode(const string text, Node *nodePrev = nullptr);
	Node* traverseNodesOfList(const bool decreaseNodeIDsBy1 = false, const bool increaseNodeIdsBy1 = false);
	bool editNode(const int position, const string text);
	Node* addBetweenNodes(const int position, const string text, int &extraMessage);
	bool removeNode(const int position);
	bool popNode();

	List& List::operator=(const List& sourceList); // Assignment operator

	friend class MultiList; // As MultiList is responsible for several operations regarding Lists it is being declared a friend class
};