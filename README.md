# MultiList

## Description
MultiList is a console application written in **C++** which implements a [doubly-linked list](https://en.wikipedia.org/wiki/Doubly_linked_list) (here called **MultiList**) of doubly-linked lists of nodes.

## Fuctionality
Upon launching the program, the user is given the option to load a saved instance of the MultiList from a file.

Afterwards, the user can perform the following operations upon the MultiList:
- The creation of a list.
- The editing of a list (which provides options for creating, editing or deleting a node).
- The deletion of a list.
- The copying of a list.
- The merging of two lists.

Finally, upon choosing to exit the program, the user is given the option to save the current instance of the MultiList to a file.

## Implementation
The program has been implemented as three classes: MultiListClass, ListClass and NodeClass.

ListClass and NodeClass each contain some kind of "ID" integer, which for the first object has the value of 1, for the second 2, and so on.
This way, the addition of a node can be done both at the end of a list but also anywhere in-between two already existing nodes, depending on the ID the user wants the new node to have.

This means that with every addition or deletion to the MultiList, the IDs of ListClass and NodeClass objects update accordingly.

## Compiling
The repository includes the complete Visual Studio 2015 solution. As it's common with VS projects, it uses *#pragma once* and #include(s) the *stadfx.h* file with precompiled headers, so swap those out if using an incompatible compiler.
