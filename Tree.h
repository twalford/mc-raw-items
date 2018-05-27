#pragma once
class Tree {

public:
	struct ItemNode {
		std::string type;
		std::string name;
		ItemNode* pIngredients[4];
		int ingredientCounts[4];
		int numDependables;
		int makesQty;
		int numRequired;
		bool calculated;
	};
	
	Tree();
	~Tree();
	ItemNode* CreateNode(std::string type, std::string name);

};