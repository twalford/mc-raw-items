#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "Tree.h"
#pragma region using ...
using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::left;
using std::reverse;
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

		Tree::ItemNode* n = new Tree::ItemNode();
		list.push_back(n);
		n->name = sName;
	}
	// Close the file when finished.
	myfile.close();
}

// Reports a list of nodes with all data.
void DisplayList(vector<Tree::ItemNode*> &list) {
	int counter = 0;
	//Display the whole list
	cout << "T |Item Name                    |Ingredient A                 |Qty|Ingredient B                 |Qty|Ingredient C                 |Qty|D  |M  |R   |C" << endl;
	cout << "==+=============================+=============================+===+=============================+===+=============================+===+===+===+====+=" << endl;
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
		cout << setw(3) << n->numDependables << "|" << setw(3) << n->makesQty << "|" << setw(4) << n->numRequired << "|";
		cout << setw(3) << ((n->calculated) ? "c" : "u") << endl;

		counter++;
		if (counter == 9) {
			counter = 0;
			cout << "==+=============================+=============================+===+=============================+===+=============================+===+===+===+====+=" << endl;
		}
	}
}

// Reports a list of nodes with only Type, Name, and NumRequired.
void DisplayListNoIngredients(vector<Tree::ItemNode*> &list) {
	//Display the whole list
	cout << "T |Item Name                    |R   " << endl;
	cout << "==+=============================+====" << endl;
	for each (Tree::ItemNode* n in list) {
		if (n == NULL) return;

		// Type, name, and numRequired.
		cout << left << setw(2) << n->type << "|" << setw(29) << n->name << "|" << setw(4) << n->numRequired << endl;
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

	string errors = "ERRORS:\n"; // Errors added to this string.

	vector<Tree::ItemNode*> allItemsList; // Original list of our items.
	vector<Tree::ItemNode*> progList; // For crafting progression.
	vector<Tree::ItemNode*> uncalcList; // In case of endless loop.
	vector<Tree::ItemNode*> minimalList; // Hold all the minimal level items.
	allItemsList.clear();
	progList.clear();
	uncalcList.clear();
	minimalList.clear();
	
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

	// Add all the Names only to the list.
	PopulateAllItemNames(allItemsList);

	// Setup for file reading.	
	ifstream myfile("HHITEMS.csv");
	if (!myfile.is_open()) {
		cout << "Error: UNABLE TO OPEN FILE in MAIN" << endl;
	}

	while (myfile.good())
	{
		string val; // Temp string for file reading.

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
		Tree::ItemNode* n = FindNodeByName(sName, allItemsList);
				
		if (n != NULL)
		{
			n->type = sType;

			//if the first ingredient is empty, then so should the others.
			if (sIngA != "-") {
				n->pIngredients[0] = FindNodeByName(sIngA, allItemsList);
				if (sIngB != "-") {
					n->pIngredients[1] = FindNodeByName(sIngB, allItemsList);
					if (sIngC != "-") {
						n->pIngredients[2] = FindNodeByName(sIngC, allItemsList);
						if (sIngD != "-") {
							n->pIngredients[3] = FindNodeByName(sIngD, allItemsList);
							if (sIngE != "-") {
								n->pIngredients[4] = FindNodeByName(sIngE, allItemsList);
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
			n->numDependables = iChildren; // This to stay unchanged.
			n->makesQty = iMakes;
			n->numRequired = iRequired;
			n->numWorkingDependables = iChildren; // This to be used in calculation.
		}
		// Add to the error string if a null node was found.
		else {
			errors += "Error: NULL node (FindNodeByName).\n";
		}
	}
	// Close the file when finished.
	myfile.close();

	// Number of uncalculated items.
	int count = allItemsList.size();
	int prevCount = count + 1;

	// Display all the items with their initial values.
	DisplayList(allItemsList);
	cout << "uncounted:" << count << endl << endl;

////// doing the thing
	cout << "calculating..." << endl;

	// While there are still uncalculated items...
	while (count > 0) {

		// Go through the whole list.
		for each (Tree::ItemNode* n in allItemsList) {

			// Find one that has is not used any uncalculated items. (has zero dependables)
			if (n->numWorkingDependables == 0 && n->calculated == false) {
				if (n->pIngredients[0] != NULL) {
					// required += roundup(R / M) * QTY 
					n->pIngredients[0]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[0]);
					n->pIngredients[0]->numWorkingDependables--;
					if (n->pIngredients[1] != NULL) {
						n->pIngredients[1]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[1]);
						n->pIngredients[1]->numWorkingDependables--;
						if (n->pIngredients[2] != NULL) {
							n->pIngredients[2]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[2]);
							n->pIngredients[2]->numWorkingDependables--;
							if (n->pIngredients[3] != NULL) {
								n->pIngredients[3]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[3]);
								n->pIngredients[3]->numWorkingDependables--;
								if (n->pIngredients[4] != NULL) {
									n->pIngredients[4]->numRequired += (ceil((float)n->numRequired / n->makesQty) * n->ingredientCounts[4]);
									n->pIngredients[4]->numWorkingDependables--;
								}
							}
						}
					}
				}
				n->calculated = true;
				count--;
				progList.push_back(n);
			}
		}

		// Check for a endless loop.
		if (count == prevCount) {
			for each (Tree::ItemNode* n in allItemsList) {
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

	// Populate the minimal list with items that dont have a crafting recipe.
	for each (Tree::ItemNode* n in allItemsList) {
		if (n->pIngredients[0] == NULL) {
			minimalList.push_back(n);
		}
	}

	// Display the minimal list.
	cout << endl << "MINIMAL LIST" << endl;
	DisplayListNoIngredients(minimalList);
	
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

		// Display a "helpful" item crafting progression list.
		if (input == "prog") {
			reverse(progList.begin(), progList.end());
			DisplayList(progList);
		}
		else {

			bool itemFound = true;
			// Search for matching ingredients.
			// calculate the requirements again....
			//    This might be unnecessary. Does it always equal numRequired - 1 ?
			vector<Tree::ItemNode*> tempList;
			tempList.clear();
			int tempCount = 0;
			int tempReq = 0;
			for each (Tree::ItemNode* n in allItemsList) {
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
				n = FindNodeByName(input, allItemsList);
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
		}
		
		cout << endl << ">";
		cin >> input;
	}

	return 0;
}


