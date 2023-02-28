#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;
void AddItem(vector<shared_ptr<class Item>>& inventory);

void PrintString(std::string text, long time = 75) // adds each character to the stream for each millisecond

{
	
	for(auto character : text)
		{		std::this_thread::sleep_for(std::chrono::milliseconds(time));
			std::cout <<character<< std::flush;
			}
	
}
 
// Class for inventory items
class Item {
public:
    Item(int id, string name, int quantity, double price) :
        id(id), name(name), quantity(quantity), price(price) {}

    constexpr int GetId() const { return id; }
    string GetName() const { return name; }
    constexpr int GetQuantity() const { return quantity; }
    constexpr double GetPrice() const { return price; }
		void ItemComposition()
		{
			PrintString("Item Name: ");
			GetName();
			"\n";
			PrintString("Item Id: ");
			GetId();
			"\n";
			PrintString("Item Quantity: ");
			GetQuantity();
			"\n";
			PrintString("Item Price: ");
			GetPrice();
			"\n";
		}

private:
    int id;
    string name;
    int quantity;
    double price;
};

class InventoryUI
{
private:
	vector<shared_ptr<Item>> inventory;
public:
	void DisplayUI();
vector<shared_ptr<Item>>& GetInventory(){return inventory;};
void SetInventory(vector<shared_ptr<Item>>& inventory){this->inventory = inventory;};
void DisplayInventory();

};

void InventoryUI::DisplayUI()
{
	char response;
	while(response != 'q' || response != 'Q')
		{
			PrintString("Welcome to the Store Inventory System! \n\n");
			PrintString("1: Add Item\n");
			PrintString("2: Update Item\n");
			PrintString("3: Remove Item\n");
			PrintString("4: View Inventory\n");
			PrintString("Q: Exit Program\n");

			
			PrintString("Enter in a response: ");
			std::cin>>response;
			int intResponse = response - '0';
			switch(response)
				{
					case('1'):
						{
							DisplayInventory();
							AddItem(InventoryUI::GetInventory());
						break;
						}
					case('2'):
						{
							DisplayInventory();
						break;
						}
					case('3'):
						{
							DisplayInventory();
						break;
						}
					case('4'):
						{
							DisplayInventory();
						break;
						}
					default:
						{
							break;
						}
					
				}
			
		}
}

// Exception class for invalid inputs
class InvalidInputException : public exception {
public:
    virtual const char* what() const throw() {
        return "Invalid input";
    }
};

// Exception class for item not found
class ItemNotFoundException : public exception {
public:
    virtual const char* what() const throw() {
        return "Item not found";
    }
};

// Mutex for multi-threading
mutex mtx;

// Function to add a new item to the inventory
void InventoryUI::DisplayInventory()
{
InventoryUI inventoryUI;
auto inventory = inventoryUI.GetInventory();
	if(inventory.size() <= 0)
	{
		PrintString("No items in the inventory! \n");
	}
	else{
		for(int i = 0; i < inventory.size() - 1; i++)
		{
		inventory[i]->ItemComposition();
		}
	}
	
}
void AddItem(vector<shared_ptr<Item>>& inventory) {
    int id, quantity;
    double price;
    string name;

    cout << "Enter item ID: ";
    cin >> id;

    // Check for invalid input
    if (cin.fail() || id < 1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException();
    }

    // Check if item already exists
    for (const auto& item : inventory) {
        if (item->GetId() == id) {
            throw InvalidInputException();
        }
    }

    cout << "Enter item name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter item quantity: ";
    cin >> quantity;

    // Check for invalid input
    if (cin.fail() || quantity < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException();
    }

    cout << "Enter item price: ";
    cin >> price;

    // Check for invalid input
    if (cin.fail() || price < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException();
    }

    // Create new item and add it to the inventory
    shared_ptr<Item> newItem = make_shared<Item>(id, name, quantity, price);
    inventory.push_back(newItem);
}

// Function to update an existing item in the inventory
void UpdateItem(vector<shared_ptr<Item>>& inventory) {
    int id, quantity;
    double price;
    string name;

    cout << "Enter item ID: ";
    cin >> id;

    // Check for invalid input
    if (cin.fail() || id < 1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException();
    }

    // Find the item in the
}

int main()
{
	InventoryUI UI;

	UI.DisplayUI();
}