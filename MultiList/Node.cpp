#include "stdafx.h"
#include "NodeClass.h"

int Node::s_nodeIDGen = 1; // inits the generator for the Node's ID

						   // Default Constructor for Node (unused in this program)
						   // As defined in Node.h the pointer to the Next Node is always set to nullptr
Node::Node() : m_nodeID{ s_nodeIDGen++ }, m_nodeText{ "" }, m_nodePrev{ nullptr } {}

// Custom Constructor for Node
// Each new Node's ID is assigned the value of the generator's, which is then increased by 1
// As defined in Node.h:
// - The pointer to the Previous Node has a default value of nullptr unless the constructor is provided with a different one
// - The pointer to the Next Node is always set to nullptr
Node::Node(const string text, Node *nodePrev) : m_nodeID{ s_nodeIDGen++ }, m_nodeText{ text }, m_nodePrev{ nodePrev } {}

// Copy Constructor for Node
// As defined in Node.h:
// - The pointer to the Previous Node has a default value of nullptr unless the constructor is provided with a different one
// - The pointer to the Next Node is always set to nullptr
Node::Node(const Node &sourceNode, Node *nodePrev) {
	m_nodeID = sourceNode.m_nodeID;
	m_nodeText = sourceNode.m_nodeText;

	m_nodePrev = nodePrev;
}

// Destructor for Node
// Decreases by 1 the generator for the Node's ID
Node::~Node() {
	--Node::s_nodeIDGen;
}

// Assignment operator that copies one Node to another (unused in this program)
// The target Node should be the last Node of the List and be empty
Node& Node::operator=(const Node &sourceNode) {
	if (this == &sourceNode)
		return *this;

	m_nodeID = sourceNode.m_nodeID;
	m_nodeText = sourceNode.m_nodeText;

	return *this;
}