#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "MultiListClass.h"

// Checks if a file exists
// Returns True if it does or False otherwise
bool doesFileExists(const string filename) {
	struct stat buf;

	if (stat(filename.c_str(), &buf) != -1)
		return true;

	return false;
}

// Calls the load function of the MultiList
// If a corrupted file has been half-loaded, it handles the clean up of the MultiList to make it empty again
// Returns:
// 0 if the file was loaded successfully
// 1 if the file could not be opened
// 2 if the file was corrupt
int loadFileClassFunctionCaller(MultiList &myMultiList, const string &filename) {

	const int loadSuccess = myMultiList.load(filename);

	if (loadSuccess == 2) // if the file was corrupt
		myMultiList.cleanUp();

	return loadSuccess;
}

// Returns 'Y' (for Yes) if the input was Y, y, yes or Yes (or any other combination of capital and lower case letters)
// Returns 'N' (for No) if the input was N, n, no or No etc
// Returns 'F' (for Failure) if the input was anything else
char validateLoadAndSaveInput(string input) {

	transform(input.begin(), input.end(), input.begin(), toupper); // make input to all CAPS

	if (input == "Y" || input == "YES")
		return 'Y';
	else if (input == "N" || input == "NO")
		return 'N';

	cout << "\nInvalid input, please try again\n\n";
	return 'F';
}

// Asks the user if they want to load a file
// If they do, it returns the filename
// If they don't, it returns ""
string printLoadFileMenu() {

	string input;
	int loadChoice;

	do {
		cout << "Load a saved instance from a file? (Y / N): ";
		getline(cin, input);
		loadChoice = validateLoadAndSaveInput(input);
	} while (loadChoice == 'F'); // F for failure, if the user selected anything other than Yes or No

	if (loadChoice == 'Y') {
		string filename = "saved.txt";

		while (true) {
			if (doesFileExists(filename)) {
				do {
					cout << "File " << filename << " exists! Load it? (Y / N): ";
					getline(cin, input);
					loadChoice = validateLoadAndSaveInput(input);
				} while (loadChoice == 'F');

				if (loadChoice == 'N') {
					cout << "Enter the name of the file you'd like to load: ";
					getline(cin, filename);
				}
				else if (loadChoice == 'Y')
					return filename;
			}
			else {
				do {
					cout << "File " << filename << " doesn't exist! Load a different file? (Y / N): ";
					getline(cin, input);
					loadChoice = validateLoadAndSaveInput(input);
				} while (loadChoice == 'F');

				if (loadChoice == 'Y') {
					cout << "Enter the name of the file you'd like to load: ";
					getline(cin, filename);
				}
				else
					return "";
			}
		}
	}
	return "";
}

// Calls printLoadFileMenu() to ask the user if they want to load the MultiList from a file and
// in return gets the filename or "" if the answer was negative.
// Then calls loadFileClassFuctionCaller() to attempt to load the file and
// prints an error message if it received an error  code indicating problems
void loadFileOperations(MultiList &myMultiList) {

	const string filename = printLoadFileMenu();

	// if the user chose that Yes, they want to load a file, then filename contains the intended filename to be used for loading
	if (filename != "") {
		const int loadSuccess = loadFileClassFunctionCaller(myMultiList, filename);
		if (loadSuccess == 1)
			cerr << "Error opening " << filename << " for reading!\n";
		else if (loadSuccess == 2)
			cerr << "File " << filename << " is corrupt!\n";
	}
}

// Returns 1 to 6 if the input was 1 to 6
// Returns 0 otherwise
int validateInput(const string input) {

	if (input.length() == 1) {
		stringstream myStream(input);
		int choice;
		if ((myStream >> choice) && (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6)) // convert it to an int and see if it's 1, 2, 3 or 4
			return choice;
	}

	cout << "\nInvalid choice, please try again\n\n\n";
	return 0;
}

// Calls printMultiList() to print the entire MultiList
// Prints a menu of 6 possible operations on the MultiList
// Returns 1 to 6 depending on the choice made
int printMenu(const MultiList &myMultiList) {
	int choice;
	do {
		myMultiList.printMultiList();
		cout << "1. Create a new list.\n";
		cout << "2. Edit the nodes of an existing list.\n";
		cout << "3. Delete a list.\n";
		cout << "4. Copy a list.\n";
		cout << "5. Merge two lists.\n";
		cout << "6. Exit.\n\n";
		cout << "Make your choice..: ";

		string input;
		getline(cin, input);
		choice = validateInput(input);
	} while (choice == 0);

	return choice;
}

// Calls printList() to print the entire given List
// Prints a menu of 6 possible operations on the List
// Returns 1 to 6 depending on the choice made
int printMenu(const List &myList) {
	int choice;
	do {
		myList.printList();
		cout << "1. Push a new node to the end of the list.\n";
		cout << "2. Edit the text of a node in a specific position in the list.\n";
		cout << "3. Add a new node to a specific position.\n";
		cout << "4. Pop the node from the end of the list.\n";
		cout << "5. Delete a node from a specific position in the list.\n";
		cout << "6. Return to the first menu.\n\n";
		cout << "Make your choice..: ";

		string input;
		getline(cin, input);
		choice = validateInput(input);
	} while (choice == 0);

	return choice;
}

// Executes one of the 6 operations possible on a List
void listFunctions(List &myList) {

	// Loop.
	// Calls printMenu() to print the contents of the List and the menu of operations
	// The returning int from printMenu specifies what will happen:
	// 1 to 5 executes a function on the given List.
	// If something goes wrong it prints an error message
	// 6 exits the loop and returns to the MultiList Menu.
	while (true) {

		const int choice = printMenu(myList);

		switch (choice) {

			// Push a new Node
		case 1: {
			cout << "Type the text you'd like to add to the new node: ";
			string text;
			getline(cin, text);
			const Node *success = myList.addNewNode(text);
			if (!success)
				cerr << "Failure to add a new node!\n";
			break;
		}

				// Edit the text of a specific Node
		case 2: {
			cout << "Enter the number of the node you'd like to edit: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number) {
				cout << "Type the text you'd like to edit in: ";
				string text;
				getline(cin, text);
				const bool success = myList.editNode(number, text);
				if (!success)
					cout << "Wrong number!\n";
			}
			else
				cout << "\nInput must be a number!\n";
			break;
		}

				// Add a new Node to a specific position
		case 3: {
			cout << "Enter the number of the position you'd like to add to the new node: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number) {
				// If the given number is negative or 0
				if (number < 1)
					break;
				cout << "Type the text you'd like to add to the new node: ";
				string text;
				getline(cin, text);
				int extraMessage = 0;
				const Node *success = myList.addBetweenNodes(number, text, extraMessage);
				if (!success)
					cerr << "Failure to add a new node!\n";
				else {
					if (extraMessage == 1)
						cout << "The list was empty thus the node was added to position 1\n";
					else if (extraMessage == 2)
						cout << "The position exceeded the number of nodes in the list, thus the node was added to the end of it\n";
				}
			}
			else
				cout << "\nInput must be a number!\n";
			break;
		}

				//pop the last Node
		case 4: {
			if (!myList.popNode())
				cout << "There's nothing to pop!\n";
			break;
		}

				// Delete a specific Node
		case 5: {
			cout << "Enter the number of the node you'd like to delete: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number)
				if (!myList.removeNode(number))
					cout << "\nWrong position!\n";
				else
					cout << "\nInput must be a number!\n";
			break;
		}

		case 6:
			return;
		}
	}
}

// Calls the Save member function of the MultiList
// Returns true if the file was saved successfully
// Returns false if the file couldn't be opened
bool saveFileClassFunctionCaller(const MultiList &myMultiList, const string &filename) {
	return myMultiList.save(filename);
}

// Asks the user if they want to load a file
// If they do, it returns the filename
// If they don't, it returns ""
string printSaveFileMenu() {

	string input;
	int saveChoice;

	do {
		cout << "Save everything to a file? (Y / N): ";
		getline(cin, input);
		saveChoice = validateLoadAndSaveInput(input);
	} while (saveChoice == 'F');

	if (saveChoice == 'Y') {
		string filename = "saved.txt";
		while (true) {
			if (doesFileExists(filename)) {
				do {
					cout << "File " << filename << " already exists! Overwrite it? (Y / N): ";
					getline(cin, input);
					saveChoice = validateLoadAndSaveInput(input);
				} while (saveChoice == 'F');

				if (saveChoice == 'N') {
					cout << "Enter the filename you'd like to use: ";
					getline(cin, filename);
				}
				else if (saveChoice == 'Y')
					return filename;
			}
			else
				return filename;
		}
	}
	return "";
}

// Provides the option to the user to save the current instance of the MultiList to a file
void saveFileOperations(const MultiList &myMultiList) {

	const string filename = printSaveFileMenu();

	// if the user chose that Yes, they want to save a file, then filename contains the intended filename to be used for loading
	if (filename != "") {
		const bool saveSuccess = saveFileClassFunctionCaller(myMultiList, filename);
		if (saveSuccess)
			cout << "\nSave completed!\n";
		else if (!saveSuccess)
			cerr << "Error opening file: " << filename << " for saving!\n";
	}
}

int main() {

	MultiList myMultiList;

	loadFileOperations(myMultiList);

	// Main loop.
	// Calls printMenu() to print the contents of the MultiList and the menu of operations
	// The returning int from printMenu specifies what will happen:
	// 1 to 5 executes a function on the MultiList.
	// If something goes wrong it prints an error message
	// 6 exits the program.
	while (true) {

		const int choice = printMenu(myMultiList);

		switch (choice) {

			// Add a new List to the MultiList
		case 1: {
			const List *success = myMultiList.addNewList();
			if (!success)
				cerr << "Failure to add a new list!\n";
			break;
		}

				// Edit a specific List
		case 2: {
			cout << "Enter the number of the list you'd like to edit: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number)
				if (number < 1 || number > myMultiList.getListsCount())
					cout << "There's no list with that number!\n";
				else {
					List *list = myMultiList.editList(number);
					if (list)
						listFunctions(*list);
				}
			else
				cout << "\nInput must be a number!\n";
			break;
		}

				// Delete a specific List
		case 3: {
			cout << "Enter the number of the list you'd like to delete: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number)
				if (number < 1 || number > myMultiList.getListsCount())
					cout << "\nThere's no list with that number!\n";
				else
					myMultiList.removeList(number);
			else
				cout << "\nInput must be a number!\n";
			break;
		}

				// Copy a specific List to a new one
		case 4: {
			cout << "Enter the number of the list you'd like to copy to the end of the MultiList: ";
			string input;
			getline(cin, input);
			stringstream myStream2(input);
			int number;

			if (myStream2 >> number)
				if (number < 1 || number > myMultiList.getListsCount())
					cout << "There's no list with that number!\n";
				else {
					const bool success = myMultiList.copyList(number);
					if (!success)
						cerr << "Failure to copy the list!\n";
				}
			else
				cout << "\nInput must be a number!\n";
			break;
		}

				// Merge two Lists together
		case 5: {
			cout << "Enter the first number of the list you'd like to merge: ";
			string input;
			getline(cin, input);
			stringstream myStream(input);
			int number1;

			if (myStream >> number1)
				if ((number1 > 0) && (number1 <= myMultiList.getListsCount())) {
					cout << "Enter the number of the second list you'd like to merge: ";
					getline(cin, input);
					stringstream myStream(input);
					int number2;
					if (myStream >> number2)
						if ((number2 > 0) && (number2 <= myMultiList.getListsCount()))
							if (number1 != number2) {
								const int success = myMultiList.mergeLists(number1, number2);
								if (success == 1)
									cout << "List #" << number1 << " is empty so it was just removed\n";
							}
							else
								cout << "Cannot merge List with itself!\n";
						else
							cout << "\nThere's no list with number " << number2 << "!\n";
					else
						cout << "\nInput must be a number!\n";
				}
				else
					cout << "\nThere's no list with number " << number1 << "!\n";
			else
				cout << "\nInput must be a number!\n";

			break;
		}

				// Exits the program after providing the opportunity to save to a file the current instance of the MultiList
		case 6: {
			saveFileOperations(myMultiList);
			return 0;
		}
		}
	}
}
