#ifndef _TREE_INCLUDE_H_
#define _TREE_INCLUDE_H_

#include <string>
#include <vector>

class Node
{
public:
	Node(std::string& name,Node* parent = NULL);
	Node(std::string& name,std::string& value,Node* parent = NULL);

	void setValue(std::string &value);

	//! Does the node have children?
	bool childrenExist();
	
	//! Add a precreated node to this node's children.
	void addChild(Node* node);
	//! Create and add a node to this node's children.
	void addChild(std::string& name,std::string& value);

	//! Searches for child and returns true if it exists.
	bool childExist(std::string& name, bool recursive = false);
	//! Searches and removes child node from children list, while returning the found child.
	Node* removeChild(std::string& name, bool recursive = false);
	//! Removes all children. Deletes all children.
	void removeAllChildren();

	//! Purely for internal use. Shouldn't be called outside the class.
	void setParent(Node* p);
	Node* getParent();

	//! Gets the node's name.
	std::string getName(){ return name; }
	//! Gets the node's value.
	std::string getValue(){ return value; }

private:
	std::string name;
	std::string value;

	std::vector<Node*> children;
	Node* parent;
}


//do I need this...
class GameTree
{
public:

private:

};

#endif