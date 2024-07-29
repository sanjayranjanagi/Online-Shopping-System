#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
using namespace std;

class Item {
private:
    string name;
    double price;
    int stock;
public:
    Item(string n, double p, int s) : name(n), price(p), stock(s) {}
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    void setStock(int s) { stock = s; }
    void displayDetails() const {
        cout << "Item: " << name << ", Price: " << price << ", Stock: " << stock << endl;
    }
};

class User {
private:
    string username;
    string password;
public:
    User(string u, string p) : username(u), password(p) {}
    string getUsername() const { return username; }
    bool checkPassword(string p) const { return p == password; }
};

class Customer : public User {
private:
    unordered_map<string, int> cart;  // Item name and quantity
public:
    Customer(string u, string p) : User(u, p) {}
    void addToCart(string itemName, int quantity) { cart[itemName] += quantity; }
    void displayCart() const {
        cout << "Cart contents:" << endl;
        for (const auto& item : cart) {
            cout << "Item: " << item.first << ", Quantity: " << item.second << endl;
        }
    }
    void clearCart() { cart.clear(); }
};

class Admin : public User {
private:
    Admin(string u, string p) : User(u, p) {}
    static Admin* instance;
public:
    static Admin* getInstance() {
        if (instance == nullptr) {
            instance = new Admin("admin", "admin123");
        }
        return instance;
    }
    void addItem(vector<Item>& inventory, string name, double price, int stock) {
        inventory.push_back(Item(name, price, stock));
    }
    void removeItem(vector<Item>& inventory, string name) {
        inventory.erase(remove_if(inventory.begin(), inventory.end(), [&](Item& item) { return item.getName() == name; }), inventory.end());
    }
    void updateItemStock(vector<Item>& inventory, string name, int stock) {
        for (auto& item : inventory) {
            if (item.getName() == name) {
                item.setStock(stock);
                break;
            }
        }
    }
};
Admin* Admin::instance = nullptr;

class OnlineShoppingSystem {
private:
    vector<Item> inventory;
    unordered_map<string, shared_ptr<Customer>> customers;
public:
    void addItemToInventory(string name, double price, int stock) {
        Admin* admin = Admin::getInstance();
        admin->addItem(inventory, name, price, stock);
    }
    void removeItemFromInventory(string name) {
        Admin* admin = Admin::getInstance();
        admin->removeItem(inventory, name);
    }
    void updateItemStockInInventory(string name, int stock) {
        Admin* admin = Admin::getInstance();
        admin->updateItemStock(inventory, name, stock);
    }
    void registerCustomer(string username, string password) {
        customers[username] = make_shared<Customer>(username, password);
    }
    shared_ptr<Customer> getCustomer(string username) {
        if (customers.find(username) != customers.end()) {
            return customers[username];
        }
        return nullptr;
    }
    void displayInventory() const {
        cout << "Inventory:" << endl;
        for (const auto& item : inventory) {
            item.displayDetails();
        }
    }
};

int main() {
    OnlineShoppingSystem system;
    Admin* admin = Admin::getInstance();
    int choice;
    string username, password, itemName;
    double itemPrice;
    int itemStock, quantity;

    do {
        cout << "\n===== Online Shopping System =====\n";
        cout << "\n=========== MENU =========\n";
        cout << "1. Register Customer\n";
        cout << "2. Add Item to Inventory\n";
        cout << "3. Remove Item from Inventory\n";
        cout << "4. Update Item Stock\n";
        cout << "5. Display Inventory\n";
        cout << "6. Add Item to Cart\n";
        cout << "7. Display Cart\n";
        cout << "8. Clear Cart\n";
        cout << "9. Exit\n";
        cout << "Enter your choice (1-9): ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            system.registerCustomer(username, password);
            break;
        case 2:
            cout << "Enter item name: ";
            cin.ignore();
            getline(cin, itemName);
            cout << "Enter item price: ";
            cin >> itemPrice;
            cout << "Enter item stock: ";
            cin >> itemStock;
            system.addItemToInventory(itemName, itemPrice, itemStock);
            break;
        case 3:
            cout << "Enter item name to remove: ";
            cin.ignore();
            getline(cin, itemName);
            system.removeItemFromInventory(itemName);
            break;
        case 4:
            cout << "Enter item name to update stock: ";
            cin.ignore();
            getline(cin, itemName);
            cout << "Enter new stock quantity: ";
            cin >> itemStock;
            system.updateItemStockInInventory(itemName, itemStock);
            break;
        case 5:
            system.displayInventory();
            break;
        case 6:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter item name: ";
            cin.ignore();
            getline(cin, itemName);
            cout << "Enter quantity: ";
            cin >> quantity;
            if (auto customer = system.getCustomer(username)) {
                customer->addToCart(itemName, quantity);
            } else {
                cout << "Customer not found!" << endl;
            }
            break;
        case 7:
            cout << "Enter username: ";
            cin >> username;
            if (auto customer = system.getCustomer(username)) {
                customer->displayCart();
            } else {
                cout << "Customer not found!" << endl;
            }
            break;
        case 8:
            cout << "Enter username: ";
            cin >> username;
            if (auto customer = system.getCustomer(username)) {
                customer->clearCart();
            } else {
                cout << "Customer not found!" << endl;
            }
            break;
        case 9:
            cout << "Thank you for using the Online Shopping System!\n";
            break;
        default:
            cout << "Invalid choice! Please enter a number between 1 and 9.\n";
        }
    } while (choice != 9);

    return 0;
}

