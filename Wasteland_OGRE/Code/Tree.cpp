#include "Tree.h"

Node::Node(std::string& name,Node* parent)
{
	this->parent = parent;

	this->name = name;
	value = ""; //blank value
}

Node::Node(std::string& name,std::string& value,Node* parent)
{
	this->parent = parent;

	this->name = name;
	this->value = value;
}

void Node::setParent(Node* p)
{
	this->parent = p;
}

Node* Node::getParent()
{
	return parent;
}

bool Node::childrenExist()
{
	if(children.size()>0)
	{
		return true;
	}
	else
		return false;
}

void Node::addChild(Node* child)
{
	child->setParent(this);
	children.push_back(child);
}

void Node::addChild(std::string &name,std::string& value)
{
	//create the node, setting the name, value, and parent.
	Node* c = new Node(name,value,this);
	//add it to the list of children.
	children.push_back(c);
}

bool Node::childExist(std::string& name, bool recursive)
{
	bool found = false;

	std::vector<Node*>::iterator itr = children.begin();
	for( ; itr != children.end() && found != true; ++itr)
	{
		if((*itr)->getName() == name)
		{
			found = true;
		}

		//Recursively search for children, if needed.
		if(recursive && found != true)
		{
			found = (*itr)->childExist(name,recursive);
		}
		
	}

	return found;
}