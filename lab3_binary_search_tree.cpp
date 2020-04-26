#include <iostream>
#include <stack>
#include <string>
#include "lab3_binary_search_tree.hpp"
#include <cmath>

using namespace std;

// PURPOSE: Default/empty constructor
BinarySearchTree::BinarySearchTree() {
	root = NULL;
	size = 0;
}

// PURPOSE: Explicit destructor of the class BinarySearchTree
BinarySearchTree::~BinarySearchTree() {
	delete_tree(root);
}

void BinarySearchTree::delete_tree(TaskItem* start){
	if(start == NULL)
		return;
	delete_tree(start->left);
	delete_tree(start->right);
	delete start;
}

// PURPOSE: Returns the number of nodes in the tree
unsigned int BinarySearchTree::get_size() const {
	return size;
}


// PURPOSE: Returns the maximum value of a node in the tree
// if the tree is empty, it returns (-1, "N/A")
BinarySearchTree::TaskItem BinarySearchTree::max() const {
	if(!root) return BinarySearchTree::TaskItem(-1, "N/A");
	TaskItem* current = root;
	while(current->right)
		current = current->right;
	return *current;
}

// PURPOSE: Returns the minimum value of a node in the tree
// if the tree is empty, it returns (-1, "N/A")
BinarySearchTree::TaskItem BinarySearchTree::min() const{
	if(!root) return BinarySearchTree::TaskItem(-1, "N/A");
	TaskItem* current = root;
	while(current->left)
		current = current->left;
	return *current;
}

// PURPOSE: Returns the tree height
unsigned int BinarySearchTree::height() const {
	if(!root) return 0;
	else {
		stack<TaskItem*> s;
		s.push(root);
		int max_depth = 0;
		while(!s.empty()){
			TaskItem* current = s.top();
			s.pop();
			if(!current->left && !current->right)
				if(get_node_depth(current) > max_depth)
					max_depth = get_node_depth(current);
			if(current->right){
				s.push(current->right);
			}
			if(current->left){
				s.push(current->left);
			}
		}
		return max_depth;
	}
}

// PURPOSE: Prints the contents of the tree; format not specified
void BinarySearchTree::print() const {
	if(root){
		stack<TaskItem*> s;
		s.push(root);
		while(!s.empty()){
			TaskItem* current = s.top();
			s.pop();
			cout << "Priority: " << current->priority << endl << "Desciption: " << current->description << endl << endl;
			if(current->right){
				s.push(current->right);
			}
			if(current->left){
				s.push(current->left);
			}
		}
	}
}

// PURPOSE: Returns true if a node with the value val exists in the tree	
// otherwise, returns false
bool BinarySearchTree::exists( BinarySearchTree::TaskItem val ) const {
	if(!root) return false;
	TaskItem* current = root;
	while(current){
		if(*current == val){
			return true;
		}
		else if (val.priority > current->priority)
			current = current->right;
		else 
			current = current->left;
	}
	return false;
}

// PURPOSE: Optional helper function that returns a pointer to the root node
BinarySearchTree::TaskItem* BinarySearchTree::get_root_node() {
    return root;
}

// PURPOSE: Optional helper function that returns the root node pointer address
BinarySearchTree::TaskItem** BinarySearchTree::get_root_node_address() {
    return &root;
}

// PURPOSE: Optional helper function that gets the maximum depth for a given node
int BinarySearchTree::get_node_depth( BinarySearchTree::TaskItem* n ) const {
	if(!root || !exists(*n)) return -1;
	TaskItem* current = root;
	int count = 0;
	while(current){
		if(n == current) return count;
		++count;
		if(n->priority > current->priority) current = current->right;
		else current = current->left;
	}
	return -1;
}

// PURPOSE: Inserts the value val into the tree if it is unique
// returns true if successful; returns false if val already exists
bool BinarySearchTree::insert( BinarySearchTree::TaskItem val ) {
	if(exists(val)) return false;
	++size;
	if(!root){
		root = new TaskItem(val);
		return true;
	}
	TaskItem* current = root;
	while(current){
		if(val.priority == current->priority) return false;
		if(val.priority < current->priority && !current->left){
			current->left = new TaskItem(val);
			current->left->left = NULL;
			current->left->right = NULL;
			return true;
		}
		else if (val.priority < current->priority){
			current = current->left;
		}
		else if (val.priority > current->priority && !current->right){
			current->right = new TaskItem(val);
			current->right->left = NULL;
			current->right->right = NULL;
			return true;
		}
		else{
			current = current->right;
		}
	}
	return false;
}

// PURPOSE: Removes the node with the value val from the tree
// returns true if successful; returns false otherwise
bool BinarySearchTree::remove( BinarySearchTree::TaskItem val ) {
	if(!root || !exists(val)) return false;
	--size; 
	TaskItem* current = root;
	TaskItem* prev = NULL;
	bool found = false;
	while(current && !found){
		if(*current == val)
			found = true;
		else{
			prev = current;
			if(current->priority < val.priority)
				current = current->right;
			else
				current = current->left;
		}
	}
	if(!found) return false;
	if(!current->left && !current->right){
		if(current == root){
			root = NULL;
		}
		else if(prev->left == current)
			prev->left = NULL;
		else 
			prev->right = NULL;
	}
	else if(!current->left){
		if(current == root){
			root = current->right;
		}
		else if(prev->left == current){
			prev->left = current->right;
		}
		else {
			prev->right = current->right;
		}
	}
	else if(!current->right){
		if(current == root){
			root = current->left;
		}
		else if(prev->left == current){
			prev->left = current->left;
		}
		else {
			prev->right = current->left;
		}
	}
	else{
		TaskItem* placeholder = current;
		prev = current;
		current = current->right;
		while(current->left){
			prev = current;
			current = current->left;
		}
		int prior = placeholder->priority;
		string desc = placeholder->description;
		placeholder->priority = current->priority;
		placeholder->description = current->description;
		current->priority = prior;
		current->description = desc;
		
		if(!current->left && !current->right){
			if(current == root)
				root = NULL;
			else if(prev->left == current)
				prev->left = NULL;
			else 
				prev->right = NULL;
		}
		else if(!current->left){
			if(current == root){
				root = current->right;
			}
			else if(prev->left == current){
				prev->left = current->right;
			}
			else {
				prev->right = current->right;
			}
		}
		else if(!current->right){
			if(current == root){
				root = current->left;
			}
			else if(prev->left == current){
				prev->left = current->left;
			}
			else {
				prev->right = current->left;
			}
		}
	}
	delete current;
	return true;
}
