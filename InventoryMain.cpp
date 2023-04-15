#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm>

using namespace std;
void AddItem(vector<shared_ptr<class Item>>& inventory);
void RemoveItem(int itemIdToRemove, vector<shared_ptr<Item>>& inventory );
void UpdateItem(vector<shared_ptr<Item>>& inventory);

void PrintString(std::string text, long time = 5) // adds each character to the stream for each millisecond

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
		
		Item& SetId(int newId) { id = newId; return *this; }
		inline void SetPrice(double newPrice) noexcept { price = newPrice; }
		inline void SetQuantity(int newQuantity) noexcept { quantity = newQuantity; }
		inline void SetName(std::string newName) noexcept { name = newName; }





		

private:
    int id;
    string name;
    int quantity;
    double price;
};

class InventoryUI
{
public:
	vector<shared_ptr<Item>> inventory;
public:
static InventoryUI* inventoryInstance;
static InventoryUI* Get() // getting the singleton ref
	{
		if (inventoryInstance == NULL)
		{
			inventoryInstance = new InventoryUI();
		}
		return inventoryInstance;
	}

	void DisplayUI();
vector<shared_ptr<Item>>& GetInventory(){return inventory;};
void SetInventory(vector<shared_ptr<Item>>& inventory){this->inventory = inventory;};
void DisplayInventory(const vector<shared_ptr<Item>>& inventory);

};
InventoryUI* InventoryUI::inventoryInstance = NULL;

void InventoryUI::DisplayUI()
{
	InventoryUI* UI = InventoryUI::Get();
	char response;
	while(response != 'q' &&  response != 'Q')
		{
			PrintString("Welcome to the Store Inventory System! \n\n");
			PrintString("1: Add Item\n");
			PrintString("2: Update Item\n");
			PrintString("3: Remove Item\n");
			PrintString("4: View Inventory\n");
			PrintString("Q: Exit Program\n");

			
			PrintString("Enter in a response: \n");
			std::cin>>response;
			switch(response)
				{
					case('1'):
						{
							std::cout<<UI->inventory.size();
							DisplayInventory(inventory);
							
							AddItem(InventoryUI::GetInventory());
							
						break;
						}
					case('2'):
						{
							DisplayInventory(UI->inventory);
							UpdateItem(InventoryUI::GetInventory());
						break;
						}
					case('3'):
						{
							DisplayInventory(UI->inventory);
							int numResponse;
							PrintString("Remove item with valid id: ");
							std::cin>>numResponse;
							RemoveItem(numResponse,UI->inventory);
						break;
						}
					case('4'):
						{
							DisplayInventory(UI->inventory);
						break;
						}
					default:
						{
							break;
						}
					
				}
			
		}
}
void PushItemToFile(Item& item)
{
	ofstream outputStoreFile("storeInventory.txt");
	auto inventory = InventoryUI::Get()->GetInventory();
	if(outputStoreFile.is_open())
	{
		for(auto& item: inventory)
				outputStoreFile << item->GetId() << " " << item->GetName() << " " << item->GetQuantity() << " " << item->GetPrice() << std::endl;
		outputStoreFile.flush();
		outputStoreFile.close();
			
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


void InventoryUI::DisplayInventory(const vector<shared_ptr<Item>>& inventory)
{
		PrintString("Current items in Inventory:\n");
	std::cout<<inventory.size()<<std::endl;
    for (const auto& item : inventory) 
		{
        
			PrintString("Item Name: ");
			std::cout<< item->GetName() << std::endl;
			
			PrintString("Item Id: ");
			std::cout<< item->GetId() << std::endl;;
			
			PrintString("Item Quantity: ");
			std::cout<< item->GetQuantity() << std::endl;;
			
			PrintString("Item Price: ");
			std::cout<< item->GetPrice() << std::endl;
    }
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

    // Find the item in the inventory
    auto itr = find_if(inventory.begin(), inventory.end(), [=](auto item) {
        return item->GetId() == id;
    });

    if (itr == inventory.end()) {
        throw ItemNotFoundException();
    }

    shared_ptr<Item>& item = *itr;

    cout << "Enter new item name (leave blank to keep current): ";
    cin.ignore();
    getline(cin, name);

    if (!name.empty()) {
        item->SetName(name);
    }

    cout << "Enter new item quantity (negative value to keep current): ";
    cin >> quantity;

    if (quantity >= 0) {
        item->SetQuantity(quantity);
    }

    cout << "Enter new item price (negative value to keep current): ";
    cin >> price;

    if (price >= 0) {
        item->SetPrice(price);
    }

    PushItemToFile(*item.get());
}

	
void RemoveItem(int itemIdToRemove, vector<shared_ptr<Item>>& inventory) {
    auto itr = find_if(inventory.begin(), inventory.end(), [=](auto item) {
        return item->GetId() == itemIdToRemove;
    });
    if (itr != inventory.end()) {
        int removedIndex = std::distance(inventory.begin(), itr);
        inventory.erase(itr);
        inventory[removedIndex] = nullptr;
    }
}
	


	
	




void AddItem(vector<shared_ptr<Item>>& inventoryRef) {
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
    for (const auto& item : inventoryRef) {
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
    inventoryRef.push_back(newItem);
		std::cout<<inventoryRef.size();
		PushItemToFile(*newItem);
	
}

int main()
{
InventoryUI* UI = InventoryUI::Get();

	
	ifstream storeItems("storeInventory.txt");
	if(storeItems.is_open())
	{
		int id;
    string name;
    int quantity;
    double price;
		while(storeItems >> id >> name >> quantity >> price)
			{
				shared_ptr<Item> item = make_shared<Item>(id, name, quantity, price);
				UI->inventory.push_back(item);
				
			}
		storeItems.close();
	}
	
	std::cout<< UI->GetInventory().size();
	UI->DisplayUI();
}