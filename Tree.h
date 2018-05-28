#pragma once
class Tree {

public:
	struct ItemNode {
		std::string type;
		std::string name;
		ItemNode* pIngredients[5];
		int ingredientCounts[5];
		int numDependables;
		int makesQty;
		int numRequired;
		bool calculated;
	};
	
	Tree();
	~Tree();
	ItemNode* CreateNode(std::string type, std::string name);

};