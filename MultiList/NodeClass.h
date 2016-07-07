#pragma once
#include <string>

using namespace std;

class Node {
	static int s_nodeIDGen; // 1, 2, 3, ...
	int m_nodeID; // 1, 2, 3, ... , takes its value from s_nodeIDGen
	string m_nodeText;

	Node *m_nodePrev;
	Node *m_nodeNext = nullptr;

public:
	Node(); // Default Constructor
	Node(const string text, Node *nodePrev = nullptr); // Custom Constructor
	Node(const Node &sourceNode, Node *nodePrev = nullptr); // Copy Constructor
	~Node(); // Destructor

	Node& operator=(const Node &sourceNode); // Assignment operator

	friend class List; // As List is responsible for several operations regarding Nodes it is being declared a friend class
};
#pragma once
