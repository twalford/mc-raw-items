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
void PopulateAllItems(vector<Tree::ItemNode*> &list) {
	string val;
	ifstream myfile("HHITEMS.csv");

	if (myfile.is_open()) {
		int colCount = 0;
		string sName = "";

		// Extract data between each comma and save it according to
		// the colCount. (We only want the 2nd column - the name).
		while (getline(myfile, val, ',')) {
			switch (colCount) {
				case 1: sName = val; 
					break;
				default:
					break;
			}

			// 12 Items in each line, at the end we write data
			// to the node and push it into the list. Reset the count.
			if (colCount == 12) {
				Tree::ItemNode* n = new Tree::ItemNode();
				list.push_back(n);
				n->name = sName;
				colCount = 0;
			}
			else
				colCount++;
		}
		// Close the file when finished.
		myfile.close();
	}

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
	PopulateAllItems(allItems);

	// Setup for file reading.
	string val;
	ifstream myfile("HHITEMS.csv");
	if (myfile.is_open())
	{
		// Have a count to track the commas/column.
		int colCount = 0;

		// Initialise variables to hold the data.
		string sType	= "";
		string sName	= "";
		string sIngA	= "";
		string sIngB	= "";
		string sIngC	= "";
		string sIngD	= "";
		int iIngAqty	= 0;
		int iIngBqty	= 0;
		int iIngCqty	= 0;
		int iIngDqty	= 0;
		int iChildren	= 0;
		int iMakes		= 0;
		int iRequired	= 0;

		// Start reading the file by strings seperated by commas.
		while (getline(myfile, val, ','))
		{
			// The colCount tells us which data is being read in so we
			// can set it to the right variable.
			switch (colCount)
			{
			case 0: sType		= "-"; break; // Ignoring TYPE for now (newline issue).
			case 1: sName		= val; break;
			case 2: sIngA		= val; break;
			case 3: iIngAqty	= (val != "-") ? stoi(val) : 0; break;
			case 4: sIngB		= val; break;
			case 5: iIngBqty	= (val != "-") ? stoi(val) : 0; break;
			case 6: sIngC		= val; break;
			case 7: iIngCqty	= (val != "-") ? stoi(val) : 0; break;
			case 8: sIngD		= val; break;
			case 9: iIngDqty	= (val != "-") ? stoi(val) : 0; break;
			case 10: iChildren  = (val != "-") ? stoi(val) : 0; break;
			case 11: iMakes		= (val != "-") ? stoi(val) : 1; break;
			case 12: iRequired  = (val != "-") ? stoi(val) : 0; break;
			default:
				break;
			}
			
			// At the end of each line, store the data into its corret node.
			if (colCount == 12)
			{
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
								}
							}
						}
					}
					n->ingredientCounts[0] = iIngAqty;
					n->ingredientCounts[1] = iIngBqty;
					n->ingredientCounts[2] = iIngCqty;
					n->ingredientCounts[3] = iIngDqty;
					n->numDependables = iChildren;
					n->makesQty = iMakes;
					n->numRequired = iRequired;
				}
				// Add to the error string if a null node was found.
				else {
					errors += "Error: NULL node (FindNodeByName).\n";
				}
				colCount = 0;
			}
			else
				colCount++;
		}
		// Close the file when finished.
		myfile.close();
	}
	else
	{
		// Report error if file cannot be opened.
		cout << "Error: UNABLE TO OPEN FILE\n";
	}

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
						n->pIngredients[1]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[0]);
						n->pIngredients[1]->numDependables--;
						if (n->pIngredients[2] != NULL) {
							n->pIngredients[2]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[2]);
							n->pIngredients[2]->numDependables--;
							if (n->pIngredients[3] != NULL) {
								n->pIngredients[3]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[3]);
								n->pIngredients[3]->numDependables--;
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
	cout << endl << "FINISHED" << endl;
	char input = '0';
	cin >> input;

	return 0;
}


