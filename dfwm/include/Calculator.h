#ifndef _CALCULATOR_
#define _CALCULATOR_

#include <iostream>
#include <string>
#include <math.h>

class Calculator {
	private:
		class Node {
			public:
				int num;
				char op;

				Node* left;
				Node* right;
				Node* parent;
			
				Node(int num, char op, Node* parent, Node* left = NULL, Node* right = NULL) {
					this->num 	= num;
					this->op 	= op;
					this->parent	= parent;
					this->left 	= left;
					this->right	= right;
				}
				
				void addNode(Node* child) {
					if(this->left == NULL) this->left = child;
					else if(this->right == NULL) this->right = child;
				}
		};

		Node* root = NULL;

		void buildTree(std::string);
		void printTree(Node*);
		void calcTree(Node*);
		void addNode(Node*&, char, std::string);

	public:
		static std::string calculate(std::string);
};

void Calculator::printTree(Node* node) {
	if (node != NULL) {
		if(node->left != NULL) 	printTree(node->left);
		if(node->right != NULL)	printTree(node->right);
	}
}

void Calculator::calcTree(Node* node) {
	if (node != NULL) {
		if(node->left != NULL) 	calcTree(node->left);
		if(node->right != NULL)	calcTree(node->right);

		if(node->op == 'h') {
			throw ""; //Not fully implemented yet";
		}
		if(node->op != 'e') {
			switch(node->op) {
				case '+':
					node->num = node->left->num + node->right->num;
					break;
				case '-':
					node->num = node->left->num - node->right->num;
					break;
				case '*':
					node->num = node->left->num * node->right->num;
					break;
				case '/':
					node->num = node->left->num / node->right->num;
					break;
				case '^':
					node->num = pow(node->left->num, node->right->num);
					break;
			}
		}
	}
}

std::string Calculator::calculate(std::string input) {
	Calculator calc;
	try {
		calc.buildTree(input);
		//calc.printTree(calc.root);
		calc.calcTree(calc.root);
	} catch(const char* e) {
		return e;
	}
	return std::to_string(calc.root->num);
}

void Calculator::addNode(Node*& tree, char op, std::string num) {
	if(tree != NULL && tree->op == 'h' && tree->left != NULL) {
		tree->op = op;
	} else {
		if(num != "") {
			Node* child = new Node(0, op, tree);
			child->left = new Node(std::stod(num), 'e', child);
			if (tree == NULL) {
				tree = child;
				root = tree;
			} else if (tree->left == NULL) {
				tree->left = child;
				tree = tree->left;
			} else {
				tree->right = child;
				tree = tree->right;
			}
		} else throw "";
	}
}

void Calculator::buildTree(std::string input) {
	std::string number 	= "";
	std::string characters 	= "";
	bool pow 		= false;
	Node* tree 		= root;

	for(int i = 0; i < input.length(); i++) {
		switch (input[i]) {
			case '(':
				{
					Node* node = new Node(0, 'h', tree);
					if (tree == NULL) {
						tree = node;
						root = tree;
					} else if (tree->left == NULL) {
						tree->left = node;
						tree = tree->left;
					} else {
						tree->right = node;
						tree = tree->right;
					}
				
					number 		= "";
					characters 	= "";
				}
				break;
			case ')':
				{
					if(tree->left != NULL && tree->op == 'h') {
						Node* tmp = tree->left;

						tree->op = tree->left->op;
						tree->left = tmp->left;
						tree->right = tmp->right;
						
						tmp->left = NULL;
						tmp->right = NULL;
						delete tmp;
					} else if(number != "") {
						tree->right = new Node(std::stod(number), 'e', tree);
					}

					Node* walker = tree;				
					while(walker->parent != NULL && walker->op != 'h') walker = walker->parent;
					tree = walker;

					number 		= "";
					characters 	= "";
				}
				break;
			case '+':
				{
					addNode(tree, '+', number);
					number 		= "";
					characters 	= "";
				}
				break;
			case '-':
				{
					addNode(tree, '-', number);
					number 		= "";
					characters 	= "";
				}
				break;
			case '/':
				{
					addNode(tree, '/', number);
					number 		= "";
					characters 	= "";
				}
				break;
			case '*':
				{
					addNode(tree, '*', number);
					number 		= "";
					characters 	= "";
				}
				break;
			case '^':
				{
					addNode(tree, '^', number);
					number 		= "";
					characters 	= "";
				}
				break;
			default:
				if(std::string("1234567890").find(input[i]) != std::string::npos) {
					number += input[i];
				} else if(std::string("abcdefghifklmnopqrstuvwxyz").find(input[i]) != -1) {
					characters += input[i];
				}

				if (characters == "sqrt") {
					characters = "";
					throw "sqrt() not implemented yet";
				}
		}	
	}

	if (root == NULL) throw "";
	if (tree->op == 'h') throw ""; //Something went wrong
	if(number == "") throw "";
	tree->right = new Node(stod(number), 'e', tree);
}

#endif //_CALCULATOR_
