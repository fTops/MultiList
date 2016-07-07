#pragma once
#include "ListClass.h"

class MultiList {
	int m_listsCount = 0;

	List *m_multiListHead = nullptr;

	void deleteAllLists();
	void init();
	bool isMultiListEmpty() const;
	List* createNewList(List *listPrev = nullptr);
	List* traverseListsOfMultiList(const bool decreaseListIDsBy1 = false);

public:
	MultiList(); // Default Constructor
	~MultiList(); // Destructor
	int getListsCount();

	void printMultiList() const;

	List* addNewList();
	List* editList(const int number);
	void removeList(const int number);
	void popList();
	bool copyList(const int number);
	int mergeLists(const int number1, const int number2);

	bool save(const string filename) const;
	int load(const string filename);
	void cleanUp();
};