#include "stdafx.h"
#include "MultiListClass.h"
#include <iostream>
#include <fstream>

// Default Constructor for MultiList
// As defined in MultiList.h the List Count is always set to 0 and the pointer to the Next List to nullptr
MultiList::MultiList() {}

// Destructor for MultiList
// Calls deleteAllLists() to delete every List in the MultiList, if the MultiList isn't empty
MultiList::~MultiList() {
	deleteAllLists();
}

// If the MultiList isn't empty, it deletes every List
// Thus the Destructor of List will be caused to be called
void MultiList::deleteAllLists() {
	if (!isMultiListEmpty()) {
		while (m_multiListHead->m_listID < m_listsCount) {
			m_multiListHead = m_multiListHead->m_listNext;
			delete m_multiListHead->m_listPrev;
		}
		delete m_multiListHead;
	}
}

// Initializes the MultiList to the empty state (that of the Default Constructor)
void MultiList::init() {
	m_listsCount = 0;
	m_multiListHead = nullptr;
}

// Returns the Lists Count of the MultiList
int MultiList::getListsCount() {
	return m_listsCount;
}

// Prints the contents of each List in the MultiList
void MultiList::printMultiList() const {

	cout << endl;

	if (isMultiListEmpty()) {
		cout << "There are no lists!\n\n";
		return;
	}

	List *temp = m_multiListHead;
	while (temp->m_listID <= m_listsCount) {
		cout << "List #" << temp->m_listID << ":" << endl;
		(*temp).printList();
		if (temp->m_listID == m_listsCount)
			break;
		temp = temp->m_listNext;
	}

	cout << endl;
}

// Returns true if the MultiList is empty (it checks if the Lists Count = 0) and false otherwise
bool MultiList::isMultiListEmpty() const {
	if (m_listsCount == 0)
		return true;
	return false;
}

// Calls createNewList() to create a new List and sets the next pointer of the last List to point to the new one.
// Increases by 1 the Lists Count.
// Returns the new List if successful, or nullptr if failure.
List* MultiList::addNewList() {

	List *lastList;

	if (isMultiListEmpty()) {
		lastList = createNewList();
		m_multiListHead = lastList;
	}
	else {
		lastList = traverseListsOfMultiList();
		lastList->m_listNext = createNewList(lastList);
		lastList = lastList->m_listNext;
	}

	if (lastList)
		++m_listsCount;

	return lastList;
}

// Returns a new List if it succeeds or nullptr if it fails.
// The Constructor of the List is called.
// The new List's prev pointer is set to point to the last List of the MultiList.
List* MultiList::createNewList(List *listPrev) {
	return new (nothrow) List(listPrev);
}

// Returns the last List of the MultiList.
// If decreaseListIDsBy1 = true and we have > 1 Lists then it also decreases the List ID of each but the last List's by 1
// The default value is false
List* MultiList::traverseListsOfMultiList(const bool decreaseListIDsBy1) {

	if (m_listsCount == 1)
		return m_multiListHead;

	List *lastList = m_multiListHead;

	while (lastList->m_listID < m_listsCount) {
		if (decreaseListIDsBy1)
			--lastList->m_listID;
		lastList = lastList->m_listNext;
	}

	return lastList;
}

// Searches the MultiList for the given position of an existing List.
// Returns a pointer to that position or nullptr otherwise.
List* MultiList::editList(const int number) {

	List *temp = m_multiListHead;
	while (temp->m_listID < number)
		temp = temp->m_listNext;
	return temp;
}

// If the given position >= 1 and also < Lists Count then it goes there and deletes the List that resides there.
// The Destructor of the List is called
// Decreases by 1 the Lists Count of the MultiList
// Otherwise it prints a message about the wrong position.
void MultiList::removeList(const int number) {

	if (number == 1 && m_listsCount > 1) { // if we want to delete the first list in a multilist of more than one lists
		List *temp = m_multiListHead;
		m_multiListHead = m_multiListHead->m_listNext;
		m_multiListHead->m_listPrev = nullptr;
		List *lastList = traverseListsOfMultiList(true);
		--lastList->m_listID;
		delete temp;
		--m_listsCount;
		return;
	}

	if (number == m_listsCount) { // if we want to delete the last node (including the first if there is only 1)
		popList();
		return;
	}

	List *temp = m_multiListHead;

	while (temp->m_listID < number) { // if we have > 1 nodes and we want to delete one that isn't the 1st or last
		temp = temp->m_listNext;
	}

	List *temp2 = temp->m_listNext;

	temp2->m_listPrev = temp->m_listPrev;
	temp2->m_listPrev->m_listNext = temp2;

	delete temp;

	swap(m_multiListHead, temp2);
	List *lastList = traverseListsOfMultiList(true);
	--lastList->m_listID;
	m_multiListHead = temp2;
	--m_listsCount = List::s_listIDGen - 1;
}

// Removes the last List of the MultiList, as long as the MultiList isn't empty
// The Destructor of the List is called
// Decreases by 1 the List Count of the MultiList
void MultiList::popList() {

	if (isMultiListEmpty()) {
		return;
	}

	if (m_listsCount == 1) { // if we only have 1 list

		delete m_multiListHead;

		m_multiListHead = nullptr;
		List::s_listIDGen = 1; // inits the generator for the m_nodeID
	}
	else { // if we have 2+ nodes
		List *lastList = traverseListsOfMultiList();
		lastList->m_listPrev->m_listNext = nullptr;
		delete lastList;
		lastList = nullptr;
	}

	m_listsCount = List::s_listIDGen - 1;
}

// Does a deep copy of the members of a List to the end of the MultiList
bool MultiList::copyList(const int number) {

	List *sourceList = m_multiListHead;
	while (sourceList->m_listID < number)
		sourceList = sourceList->m_listNext;

	List *lastList = traverseListsOfMultiList();

	lastList->m_listNext = new (nothrow) List(*sourceList);

	if (!lastList->m_listNext)
		return false;

	lastList->m_listNext->m_listPrev = lastList;

	++m_listsCount;

	return true;
}

int MultiList::mergeLists(const int number1, const int number2) {

	List *sourceList = m_multiListHead;
	while (sourceList->m_listID < number1)
		sourceList = sourceList->m_listNext;

	List *targetList = m_multiListHead;
	while (targetList->m_listID < number2)
		targetList = targetList->m_listNext;

	if (sourceList->isListEmpty()) {
		removeList(number1);
		return 1;
	}

	sourceList->mergeFirstNodeWithLast(targetList);
	removeList(number1);

	return 0;
}

// Saves the info of this instance of the MultiList to a file.
// Returns true if it opens the file for writing successfully
// or false otherwise
bool MultiList::save(const string filename) const {

	ofstream outf(filename);

	if (!outf)
		return false;

	if (isMultiListEmpty()) {
		outf << "[0]" << endl;
		return true;
	}

	outf << "[" << m_listsCount << "]" << endl;

	List *tempList = m_multiListHead;

	while (tempList->m_listID <= m_listsCount) {
		bool success = tempList->saveNodesToFile(filename);
		if (!success)
			return false;

		if (tempList->m_listID == m_listsCount)
			break;

		tempList = tempList->m_listNext;
	}
	return true;
}

// Loads from a file a saved instance of the MultiList
// Returns 0 if the file loaded successfully
// 1 if it failed to open
// 2 if it was a corrupt file
int MultiList::load(const string filename) {

	ifstream inf(filename);

	if (!inf)
		return 1;

	char ch;
	inf >> noskipws >> ch;

	if (ch != '[')
		return 2;

	inf >> noskipws >> ch;

	string listsCount = "";
	if (isdigit(ch))
		listsCount.push_back(ch);
	else
		return 2;

	while (inf >> noskipws >> ch) {
		if (isdigit(ch))
			listsCount.push_back(ch);
		else if (ch == ']') {
			inf >> noskipws >> ch;
			if (ch == '\n')
				break;
			else
				return 2;
		}
		else
			return 2;
	}
	int lCount = stoi(listsCount);

	int listFrom0 = 1;

	for (int i = 0; i < lCount; ++i) {
		addNewList();

		string currentListID = "";

		{
			inf >> noskipws >> ch;
			if (ch != '#') {
				return 2;
			}
			inf >> noskipws >> ch;
			if (isdigit(ch)) {

				currentListID.push_back(ch);
				while (inf >> noskipws >> ch) {
					if (isdigit(ch))
						currentListID.push_back(ch);
					else if (ch == ':') {
						inf >> noskipws >> ch;
						if (ch == '\n')
							break;
						else
							return 2;
					}
					else
						return 2;
				}

				if (stoi(currentListID) != listFrom0++)
					return 2;
			}
			else
				return 2;
		}
		// processed the ListID

		string nodesCount = "";
		int nodeFrom0 = 1;

		{
			inf >> noskipws >> ch;
			if (ch != '[')
				return 2;
			inf >> noskipws >> ch;

			if (isdigit(ch)) {
				nodesCount.push_back(ch);
				while (inf >> noskipws >> ch) {
					if (isdigit(ch))
						nodesCount.push_back(ch);
					else if (ch == ']') {
						inf >> noskipws >> ch;
						if (ch == '\n') {
							break;
						}
						else
							return 2;
					}
					else
						return 2;
				}
			}
			else
				return 2;
		}
		// taken the NodesCount

		int nCount = stoi(nodesCount);

		List *temp = traverseListsOfMultiList();

		Node *tempnode = temp->m_listHead;

		for (int i = 0; i < nCount; ++i) {

			string currentNodeID = "";

			inf >> noskipws >> ch;
			if (isdigit(ch)) {
				currentNodeID.push_back(ch);
				while (inf >> noskipws >> ch) {
					if (isdigit(ch))
						currentNodeID.push_back(ch);
					else if (ch == '.') {
						inf >> noskipws >> ch;
						if (ch == ' ')
							break;
						else
							return 2;
					}
					else
						return 2;
				}

				if (stoi(currentNodeID) != nodeFrom0++)
					return 2;

				string text;
				getline(inf, text);

				temp->addNewNode(text);
			}
		}
		temp = temp->m_listNext;
	}

	while (inf >> noskipws >> ch)
		return 2;

	return 0;

}

// If the loading failed because of a corrupt file, this fuctions will delete the half-loaded Lists
// and re-initialize the MultiList
void MultiList::cleanUp() {
	deleteAllLists();
	init();
}
