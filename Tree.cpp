#include <string>
#include <iostream>

#include "Tree.h"

using namespace std;

Tree::Tree() {
}

Tree::~Tree() {
}

Tree::ItemNode* Tree::CreateNode(string type, string name) {
	ItemNode* n = new ItemNode;
	n->type = type;
	n->name = name;
	n->pIngredients[0] = NULL;
	n->pIngredients[1] = NULL;
	n->pIngredients[2] = NULL;
	n->pIngredients[3] = NULL;
	n->ingredientCounts[0] = 0;
	n->ingredientCounts[1] = 0;
	n->ingredientCounts[2] = 0;
	n->ingredientCounts[3] = 0;
	n->calculated = false;

	return n;
}