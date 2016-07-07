#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "ListClass.h"

int List::s_listIDGen = 1; // inits the generator for the List's ID

						   // Default Constructor for List (unused in this program)
						   // The pointers to the Next List and the List's Head are always set to nullptr
List::List() : m_listID{ s_listIDGen++ }, m_nodeCount{ 0 }, m_listPrev{ nullptr } {}

// Custom Constructor for List
// Each new List's ID is assigned the value of the generator's, which then is increased by 1
// The pointer to the Previous List has a default value of nullptr unless the constructor is provided with a different one
// The pointers to the Next List and the List's Head are always set to nullptr
List::List(List *listPrev) : m_listID{ s_listIDGen++ }, m_nodeCount{ 0 }, m_listPrev{ listPrev } {}

// Copy Constructor for List
List::List(const List &sourceList) {

	Node *sourceNode = sourceList.m_listHead;

	m_listNext = nullptr;
	m_listID = s_listIDGen++;
	m_nodeCount = sourceList.m_nodeCount;

	if (sourceList.m_nodeCount == 0)
		return;

	Node::s_nodeIDGen = 1;

	m_listHead = new (nothrow) Node(*sourceNode);

	Node *targetNode = m_listHead;

	while (sourceNode->m_nodeID <= sourceList.m_nodeCount) {

		if (sourceNode->m_nodeID == sourceList.m_nodeCount)
			break;
		sourceNode = sourceNode->m_nodeNext;

		targetNode->m_nodeNext = new (nothrow) Node(*sourceNode, targetNode);
		targetNode = targetNode->m_nodeNext;

	}
}

// Destructor for List
// Delete every Node in the List, if the List isn't empty
// Causes the destructor of Node to be called
// Decreases by 1 the generator for each List's ID
List::~List() {
	if (!isListEmpty()) {
		while (m_listHead->m_nodeID < m_nodeCount) {
			m_listHead = m_listHead->m_nodeNext;
			delete m_listHead->m_nodePrev;
		}
		delete m_listHead;
	}
	--List::s_listIDGen;
}

// Prints the contents of each Node in the List
void List::printList() const {

	cout << endl;

	if (isListEmpty()) {
		cout << "The list is empty!\n\n";
		return;
	}

	Node *temp = m_listHead;
	while (temp->m_nodeID <= m_nodeCount) {
		cout << temp->m_nodeID << ". " << temp->m_nodeText << endl;
		if (temp->m_nodeID == m_nodeCount)
			break;
		temp = temp->m_nodeNext;
	}

	cout << endl;
}

// Returns true if the List is empty (it checks if the Node Count = 0) and false otherwise
bool List::isListEmpty() const {
	if (m_nodeCount == 0)
		return true;
	return false;
}

// Calls createNewNode() to create a new Node and sets the next pointer of the last Node to point to the new one.
// Increases by 1 the Node Count.
// Returns the new Node if successful, or nullptr if failure.
Node* List::addNewNode(const string text) {

	Node::s_nodeIDGen = m_nodeCount + 1;

	Node *lastNode;

	if (isListEmpty()) {
		lastNode = createNewNode(text);
		m_listHead = lastNode;
	}
	else {
		lastNode = traverseNodesOfList();
		lastNode->m_nodeNext = createNewNode(text, lastNode);
		lastNode = lastNode->m_nodeNext;
	}

	if (lastNode)
		++m_nodeCount;

	return lastNode;
}

// Returns a new Node if it succeeds or nullptr if it fails.
// The Constructor of the Node is called.
// The new Node's prev pointer is set to point to the last Node of the List.
Node* List::createNewNode(const string text, Node *prev) {
	return new (nothrow) Node(text, prev);
}

// Returns the last Node of the List.
// If decreaseNodeIDsBy1/increaseNodeIDsBy1 = true and we have > 1 Nodes then it also decreases/increases the Node ID of each but the last Node's by 1
// The default value for both is false
Node* List::traverseNodesOfList(const bool decreaseNodeIDsBy1, const bool increaseNodeIDsBy1) {

	if (m_nodeCount == 1)
		return m_listHead;

	Node *lastNode = m_listHead;

	while (lastNode->m_nodeID < m_nodeCount) {
		if (decreaseNodeIDsBy1)
			--lastNode->m_nodeID;
		if (increaseNodeIDsBy1)
			++lastNode->m_nodeID;
		lastNode = lastNode->m_nodeNext;
	}

	return lastNode;
}

// Searches the List for the given position of an existing Node and copies the new text there.
// Returns true if it finds the given position or false otherwise.
bool List::editNode(const int position, const string text) {

	if (position < 1 || position > m_nodeCount)
		return false;

	Node *temp = m_listHead;
	while (temp->m_nodeID < position)
		temp = temp->m_nodeNext;
	temp->m_nodeText = text;
	return true;
}

// Searches the List for the given position and calls addNewNode() to add a Node there.
// If a Node exists there it moves it (and all Nodes that follow it) 1 place to the right.
// Returns the new Node if succesful or nullptr if it failed to create it.
Node* List::addBetweenNodes(const int position, const string text, int &extraMessage) {

	Node::s_nodeIDGen = m_nodeCount + 1;

	// If the List is empty just add it to the beginning
	// eg:
	// List = {}
	// Position = anything > 0
	if (isListEmpty()) {
		extraMessage = 1;
		return addNewNode(text);
	}

	// If the List is not empty and the given position is larger than the Node Count just add it to the end
	// eg:
	// List = {1, 2, 3}
	// Position = 4+
	if (position > m_nodeCount) {
		extraMessage = 2;
		return addNewNode(text);
	}

	// If the List is not empty and the given position is 1
	// eg:
	// List = {1, 2, 3}
	// Position = 1
	if (position == 1) {
		Node *temp = m_listHead;
		m_listHead = nullptr;
		int temp_m_nodeCount = m_nodeCount;
		m_nodeCount = 0;
		Node::s_nodeIDGen = 1; // inits the generator for the m_nodeID
		if (!addNewNode(text)) {
			m_listHead = temp;
			m_nodeCount = temp_m_nodeCount;
			Node::s_nodeIDGen = m_nodeCount + 1;
			return nullptr;
		}
		m_nodeCount = temp_m_nodeCount;
		swap(m_listHead, temp);
		Node *lastNode = traverseNodesOfList(false, true);
		++lastNode->m_nodeID;
		++m_nodeCount;
		Node::s_nodeIDGen = m_nodeCount + 1;
		swap(m_listHead, temp);
		m_listHead->m_nodeNext = temp;
		m_listHead->m_nodeNext->m_nodePrev = m_listHead;
		return m_listHead;
	}

	// If the List is not empty and the given position is where the last node exists (meaning it's the same as the Node Count) just add it to the end as a new Node and swap the text of it and the previous Node
	// eg:
	// List = {1, 2, 3}
	// Position = 3
	if (position == m_nodeCount) {
		Node *lastNode = addNewNode(text);
		if (!lastNode)
			return lastNode;
		swap(lastNode->m_nodeText, lastNode->m_nodePrev->m_nodeText);
		return lastNode;
	}

	// If in the List we have > 1 Nodes and the given position is > 1 and it's not where the last Node resides either
	// eg:
	// List = {1, 2, 3, 4, 5}
	// Position = 2 to 4
	Node *temp = m_listHead;
	do {
		temp = temp->m_nodeNext;
	} while (temp->m_nodeID < position);

	int temp_m_nodeCount = m_nodeCount;
	temp->m_nodePrev->m_nodeNext = nullptr;
	m_nodeCount = position - 1;
	Node::s_nodeIDGen = position;

	Node *temp2 = addNewNode(text);

	if (!temp2) {
		m_nodeCount = temp_m_nodeCount;
		Node::s_nodeIDGen = m_nodeCount + 1;
		temp->m_nodePrev->m_nodeNext = temp;
		return nullptr;
	}

	swap(m_listHead, temp);
	m_nodeCount = temp_m_nodeCount;
	Node::s_nodeIDGen = m_nodeCount + 1;

	Node *lastNode = traverseNodesOfList(false, true);
	++lastNode->m_nodeID;

	swap(m_listHead, temp);

	++m_nodeCount;
	Node::s_nodeIDGen = m_nodeCount + 1;
	temp->m_nodePrev = temp2;
	temp2->m_nodeNext = temp;
	return m_listHead;
}

// If the given position >= 1 and also < Node Count then it goes there and deletes the Node that resides there.
// The Destructor of the Node is called
// Decreases by 1 the Node Count of the List
// Otherwise it prints a message about the wrong position.
bool List::removeNode(const int position) {

	Node::s_nodeIDGen = m_nodeCount + 1;

	if (position < 1 || position > m_nodeCount)
		return false;

	if (position == 1 && m_nodeCount > 1) { // if we want to delete the first node in a list of more than one nodes
		Node *temp = m_listHead;
		m_listHead = m_listHead->m_nodeNext;
		m_listHead->m_nodePrev = nullptr;
		Node *lastNode = traverseNodesOfList(true);
		--lastNode->m_nodeID;
		delete temp;
		m_nodeCount = Node::s_nodeIDGen - 1;
		return true;
	}

	if (position == m_nodeCount) { // if we want to delete the last node (including the first if there is only 1)
		popNode();
		return true;
	}

	Node *temp = m_listHead;

	while (temp->m_nodeID < position) { // if we have > 1 nodes and we want to delete one that isn't the 1st or last
		temp = temp->m_nodeNext;
	}

	Node *temp2 = temp->m_nodeNext;

	temp2->m_nodePrev = temp->m_nodePrev;
	temp2->m_nodePrev->m_nodeNext = temp2;

	delete temp;

	swap(m_listHead, temp2);
	Node *lastNode = traverseNodesOfList(true);
	--lastNode->m_nodeID;
	m_listHead = temp2;
	m_nodeCount = Node::s_nodeIDGen - 1;
	return true;
}

// The Destructor of the Node is called
// Decreases by 1 the Node Count of the List
bool List::popNode() {

	Node::s_nodeIDGen = m_nodeCount + 1;

	if (isListEmpty()) {
		return false;
	}

	if (m_nodeCount == 1) { // if we only have 1 node
		delete m_listHead;
		m_listHead = nullptr;
	}
	else { // if we have 2+ nodes
		Node *lastNode = traverseNodesOfList();
		lastNode->m_nodePrev->m_nodeNext = nullptr;
		delete lastNode;
	}
	m_nodeCount = Node::s_nodeIDGen - 1;
	return true;
}

// Assignment operator that copies one List to another (unused in this program)
// The target List should be the last List of the MultiList and be empty
List& List::operator=(const List& sourceList) {

	if (this == &sourceList)
		return *this;

	if (sourceList.m_nodeCount == 0)
		return *this;

	Node::s_nodeIDGen = 1;
	m_nodeCount = sourceList.m_nodeCount;

	Node *sourceNode = sourceList.m_listHead;
	m_listHead = new (nothrow) Node(*sourceNode);
	Node *targetNode = m_listHead;

	while (sourceNode->m_nodeID <= sourceList.m_nodeCount) {

		if (sourceNode->m_nodeID == sourceList.m_nodeCount)
			break;
		sourceNode = sourceNode->m_nodeNext;

		targetNode->m_nodeNext = new (nothrow) Node(*sourceNode, targetNode);
		targetNode = targetNode->m_nodeNext;
	}

	return *this;
}

// Makes the last Node of a List point to the first of another (and vice versa) so that those two Lists are merged
void List::mergeFirstNodeWithLast(List *targetList) {

	if (isListEmpty())
		return;

	Node *sourceNode = m_listHead;

	if (targetList->m_nodeCount != 0) {
		Node *targetNode = targetList->traverseNodesOfList();
		targetNode->m_nodeNext = sourceNode;
		sourceNode->m_nodePrev = targetNode;
	}
	else
		targetList->m_listHead = sourceNode;

	int temp_nodeCount = targetList->m_nodeCount;
	targetList->m_nodeCount = targetList->m_nodeCount + m_nodeCount;

	if (temp_nodeCount != 0) {

		for (int i = temp_nodeCount; i < targetList->m_nodeCount; ++i) {
			sourceNode->m_nodeID = sourceNode->m_nodePrev->m_nodeID + 1;
			sourceNode = sourceNode->m_nodeNext;
		}
	}

	m_listHead = nullptr;
	m_nodeCount = 0;
}

// Saves each Node's info to the file
// Return true if it opens the file for writing successfully
// Returns false otherwise
bool List::saveNodesToFile(const string filename) const {

	ofstream outf(filename, ios::app);

	if (!outf)
		return false;

	outf << "#" << m_listID << ":" << endl;

	if (isListEmpty()) {
		outf << "[0]" << endl;
		return true;
	}

	outf << "[" << m_nodeCount << "]" << endl;

	Node *tempNode = m_listHead;
	while (tempNode->m_nodeID <= m_nodeCount) {
		outf << tempNode->m_nodeID << ". " << tempNode->m_nodeText << endl;
		if (tempNode->m_nodeID == m_nodeCount)
			break;
		tempNode = tempNode->m_nodeNext;
	}

	return true;
}
