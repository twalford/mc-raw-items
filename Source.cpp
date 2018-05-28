#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Tree.h"
#pragma region using ...
using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::left;
using std::vector;
using std::string;
using std::ifstream;
#pragma endregion

// Populates an list of nodes with name data.
void PopulateAllItemNames(vector<Tree::ItemNode*> &list) {
	string val;
	ifstream myfile("HHITEMS.csv");

	if (!myfile.is_open()) {
		cout << "Error opening file in PopulateAllItemNames()" << endl;
	}

	while (myfile.good()) {

		int colCount = 0;
		string sName = "";

		// Extract data between each comma and save it according to
		// the colCount. (We only want the 2nd column for now - the name).
		
		getline(myfile, val, ','); //TYPE
		getline(myfile, sName, ','); //NAME
		getline(myfile, val, ','); //ING1
		getline(myfile, val, ','); //QTY1
		getline(myfile, val, ','); //ING2
		getline(myfile, val, ','); //QTY2
		getline(myfile, val, ','); //ING3
		getline(myfile, val, ','); //QTY3
		getline(myfile, val, ','); //ING4
		getline(myfile, val, ','); //QTY4
		getline(myfile, val, ','); //ING5
		getline(myfile, val, ','); //QTY5
		getline(myfile, val, ','); //CHILDREN
		getline(myfile, val, ','); //MAKES
		getline(myfile, val, '\n'); //REQUIRED

		/*
		while (getline(myfile, val, ',')) {
			cout << "val = " << val << endl;
			switch (colCount) {
				case 1: sName = val; 
					break;
				default:
					break;
			}
			*/
			// 15 Items in each line, at the end we write data
			// to the node and push it into the list. Reset the count.
			//if (colCount == 14) {
				Tree::ItemNode* n = new Tree::ItemNode();
				list.push_back(n);
				n->name = sName;
				//colCount = 0;
			//}
			//else
				//colCount++;
		//}
		// Close the file when finished.
	}
	myfile.close();

}

// Reports a list of nodes with all data.
void DisplayList(vector<Tree::ItemNode*> &list) {
	//Display the whole list
	cout << "T |Item Name                    |Ingredient A                 |Qty|Ingredient B                 |Qty|Ingredient C                 |Qty|D  |M  |R  |C" << endl;
	cout << "==+=============================+=============================+===+=============================+===+=============================+===+===+===+===+=" << endl;
	for each (Tree::ItemNode* n in list) {
		if (n == NULL) return;

		cout << left << setw(2) << n->type << "|" << setw(29) << n->name << "|";

		// First Ingredient and qty.
		if (n->pIngredients[0] != NULL) {
			cout << setw(29) << n->pIngredients[0]->name << "|" << setw(3) << n->ingredientCounts[0] << "|";
			// Second Ingredient and qty.
			if (n->pIngredients[1] != NULL) {
				cout << setw(29) << n->pIngredients[1]->name << "|" << setw(3) << n->ingredientCounts[1] << "|";
				// Third Ingredient and qty. skip the fourth.
				if (n->pIngredients[2] != NULL) {
					cout << setw(29) << n->pIngredients[2]->name << "|" << setw(3) << n->ingredientCounts[2] << "|";
				}
				else {
					cout << "-                            |-  |";
				}
			}
			else {
				cout << "-                            |-  |-                            |-  |";
			}
		}
		else {
			cout << "-                            |-  |-                            |-  |-                            |-  |";
		}
		// Children, MakesQty, and numRequired.
		cout << setw(3) << n->numDependables << "|" << setw(3) << n->makesQty << "|" << setw(3) << n->numRequired << "|";
		cout << setw(3) << ((n->calculated) ? "c" : "u") << endl;

		//delete n;
	}
}

// Reports a list of nodes with only Type, Name, and NumRequired.
void DisplayListNoIngredients(vector<Tree::ItemNode*> &list) {
	//Display the whole list
	cout << "T |Item Name                    |R  " << endl;
	cout << "==+=============================+===" << endl;
	for each (Tree::ItemNode* n in list) {
		if (n == NULL) return;

		// Type, name, and numRequired.
		cout << left << setw(2) << n->type << "|" << setw(29) << n->name << "|" << setw(3) << n->numRequired << endl;
	}
}

// Returns a node matching the given name from the list.
Tree::ItemNode* FindNodeByName(string s, vector<Tree::ItemNode*> &list) {
	for each(Tree::ItemNode* n in list) {
		if (n->name == s)
			return n;
	}
	return NULL;
}

int main() {

	// Any errors will be added to this string;
	string errors = "ERRORS:\n";

	// Create a list for our items;
	vector<Tree::ItemNode*> allItems;
	allItems.clear();

	// Add all the Names only to the list.
	PopulateAllItemNames(allItems);

	// Setup for file reading.
	string val;
	ifstream myfile("HHITEMS.csv");
	if (!myfile.is_open()) {
		cout << "Error: UNABLE TO OPEN FILE in MAIN" << endl;
	}
	
	// Initialise variables to hold the data.
	string sType = "";
	string sName = "";
	string sIngA = "";
	string sIngB = "";
	string sIngC = "";
	string sIngD = "";
	string sIngE = "";
	int iIngAqty = 0;
	int iIngBqty = 0;
	int iIngCqty = 0;
	int iIngDqty = 0;
	int iIngEqty = 0;
	int iChildren = 0;
	int iMakes = 0;
	int iRequired = 0;

	while (myfile.good())
	{
		// Start reading the file by strings seperated by commas.
		getline(myfile, sType, ','); //TYPE
		getline(myfile, sName, ','); //NAME

		getline(myfile, sIngA, ','); //ING1
		getline(myfile, val, ','); //QTY1
		iIngAqty = (val != "-") ? stoi(val) : 0;

		getline(myfile, sIngB, ','); //ING2
		getline(myfile, val, ','); //QTY2
		iIngBqty = (val != "-") ? stoi(val) : 0;

		getline(myfile, sIngC, ','); //ING3
		getline(myfile, val, ','); //QTY3
		iIngCqty = (val != "-") ? stoi(val) : 0;

		getline(myfile, sIngD, ','); //ING4
		getline(myfile, val, ','); //QTY4
		iIngDqty = (val != "-") ? stoi(val) : 0;

		getline(myfile, sIngE, ','); //ING5
		getline(myfile, val, ','); //QTY5
		iIngEqty = (val != "-") ? stoi(val) : 0;

		getline(myfile, val, ','); //CHILDREN
		iChildren = stoi(val);
		getline(myfile, val, ','); //MAKES
		iMakes = stoi(val);
		getline(myfile, val, '\n'); //REQUIRED
		iRequired = stoi(val);

		// Search the list for the correct node by name and
		// assign all the data to it.
		Tree::ItemNode* n = FindNodeByName(sName, allItems);
				
		if (n != NULL)
		{
			n->type = sType;

			//if the first ingredient is empty, then so should the others.
			if (sIngA != "-") {
				n->pIngredients[0] = FindNodeByName(sIngA, allItems);
				if (sIngB != "-") {
					n->pIngredients[1] = FindNodeByName(sIngB, allItems);
					if (sIngC != "-") {
						n->pIngredients[2] = FindNodeByName(sIngC, allItems);
						if (sIngD != "-") {
							n->pIngredients[3] = FindNodeByName(sIngD, allItems);
							if (sIngE != "-") {
								n->pIngredients[4] = FindNodeByName(sIngE, allItems);
							}
						}
					}
				}
			}
			n->ingredientCounts[0] = iIngAqty;
			n->ingredientCounts[1] = iIngBqty;
			n->ingredientCounts[2] = iIngCqty;
			n->ingredientCounts[3] = iIngDqty;
			n->ingredientCounts[4] = iIngEqty;
			n->numDependables = iChildren;
			n->makesQty = iMakes;
			n->numRequired = iRequired;
		}
		// Add to the error string if a null node was found.
		else {
			errors += "Error: NULL node (FindNodeByName).\n";
		}
	}
	// Close the file when finished.
	myfile.close();
	
	// Create a list for all the uncalcuated items, in case of a endless loop.
	vector<Tree::ItemNode*> uncalcList;
	uncalcList.clear();

	// Number of uncalculated items.
	int count = allItems.size();
	int prevCount = count + 1;

	// Display all the items with their initial values.
	DisplayList(allItems);
	cout << "uncounted:" << count << endl << endl;

////// doing the thing
	cout << "calculating..." << endl;

	// While there are still uncalculated items...
	while (count > 0) {

		// Go through the whole list.
		for each (Tree::ItemNode* n in allItems) {

			// Find one that has is not used any uncalculated items. (has zero dependables)
			if (n->numDependables == 0 && n->calculated == false) {
				if (n->pIngredients[0] != NULL) {
					// required += roundup(R / M) * QTY 
					n->pIngredients[0]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[0]);
					n->pIngredients[0]->numDependables--;
					if (n->pIngredients[1] != NULL) {
						n->pIngredients[1]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[1]);
						n->pIngredients[1]->numDependables--;
						if (n->pIngredients[2] != NULL) {
							n->pIngredients[2]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[2]);
							n->pIngredients[2]->numDependables--;
							if (n->pIngredients[3] != NULL) {
								n->pIngredients[3]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[3]);
								n->pIngredients[3]->numDependables--;
								if (n->pIngredients[4] != NULL) {
									n->pIngredients[4]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[4]);
									n->pIngredients[4]->numDependables--;
								}
							}
						}
					}
				}
				n->calculated = true;
				count--;
			}
		}

		// Check for a endless loop.
		if (count == prevCount) {
			for each (Tree::ItemNode* n in allItems) {
				if (n->calculated == false) {
					uncalcList.push_back(n);
				}
			}
			cout << "uncalclist:" << endl;
			DisplayList(uncalcList);
			errors += "Error: Endless loop: see uncalclist.\n";
			break;
		}
		prevCount = count;
	}

	// Create a list to hold all the raw level items.
	vector<Tree::ItemNode*> rawList;
	rawList.clear();

	// Populate the raw list with items that dont have a crafting recipe.
	for each (Tree::ItemNode* n in allItems) {
		if (n->pIngredients[0] == NULL) {
			rawList.push_back(n);
		}
	}

	// Display the raw list.
	cout << endl << "RAW LIST" << endl;
	DisplayListNoIngredients(rawList);
	
	// Program finished, wait for input.
	cout << "=========================" << endl;
	cout << errors;
	cout << "=========================" << endl;
	cout << "FINISHED" << endl << endl;

	// Let the user enter an item to view its usages.
	cout << "Enter item name to view breakdown or 'q' to quit\n>";
	string input = "0";
	cin >> input;
	while (input != "q") {

		bool itemFound = true;
		// Search for matching ingredients.
		// calculate the requirements again....
		//    This might be unnecessary. Does it always equal numRequired - 1 ?
		vector<Tree::ItemNode*> tempList;
		tempList.clear();
		int tempCount = 0;
		int tempReq = 0;
		for each (Tree::ItemNode* n in allItems) {
			if (n != NULL) {
				if (n->pIngredients[0] != NULL) {
					if (n->pIngredients[0]->name == input) {
						tempList.push_back(n);
						tempCount += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[0]);
					}
					else if (n->pIngredients[1] != NULL) {
						if (n->pIngredients[1]->name == input) {
							tempList.push_back(n);
							tempCount += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[1]);
						}
						else if (n->pIngredients[2] != NULL) {
							if (n->pIngredients[2]->name == input) {
								tempList.push_back(n);
								tempCount += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[2]);
							}
							else if (n->pIngredients[3] != NULL) {
								if (n->pIngredients[3]->name == input) {
									tempList.push_back(n);
									tempCount += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[3]);
								}
								else if (n->pIngredients[4] != NULL) {
									if (n->pIngredients[4]->name == input) {
										tempList.push_back(n);
										tempCount += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[4]);
									}
								}
							}
						}
					}
				}
			}

			// Check if the item exists.
			n = FindNodeByName(input, allItems);
			if (n == NULL) {
				cout << "CANNOT FIND '" << input << endl;
				itemFound = false;
				break;
			}
			else
				// If it exists, grab the total required value.
				tempReq = n->numRequired;
		}

		// Display the list if found.
		if (itemFound) {
			cout << "CRAFTING RECIPES WITH '" << input << "'" << endl;
			DisplayList(tempList);
			// tempCount is the total in the list.
			// tempReq is n->numRequired
			cout << "TOTAL '" << input << "':" << tempCount << " (" << tempReq << ")" << endl;
		}
		
		cout << endl << ">";
		cin >> input;
	}

	return 0;
}


