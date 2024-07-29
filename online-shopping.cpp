#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

// Base User class
class User {
protected:
    string username;
    string password;
public:
    User(string uname, string pwd) : username(uname), password(pwd) {}
    virtual void displayRole() = 0; // Pure virtual function
    string getUsername() { return username; }
    string getPassword() { return password; }
};

// Admin class derived from User
class Admin : public User {
public:
    Admin(string uname, string pwd) : User(uname, pwd) {}
    void displayRole() override {
        cout << "Role: Admin" << endl;
    }
    void addProduct();
    void removeProduct();
};

// Customer class derived from User
class Customer : public User {
public:
    Customer(string uname, string pwd) : User(uname, pwd) {}
    void displayRole() override {
        cout << "Role: Customer" << endl;
    }
    void viewProducts();
    void addToCart();
};

// UserManager class to manage users
class UserManager {
    unordered_map<string, User*> users;
public:
    void registerUser(User* user) {
        users[user->getUsername()] = user;
    }
    User* loginUser(string username, string password) {
        if(users.find(username) != users.end() && users[username]->getPassword() == password) {
            return users[username];
        }
        return nullptr;
    }
};

// Base Product class
class Product {
protected:
    string name;
    double price;
public:
    Product(string n, double p) : name(n), price(p) {}
    virtual void display() {
        cout << "Product: " << name << ", Price: $" << price << endl;
    }
    double getPrice() const { return price; }
    string getName() const { return name; }
    virtual ~Product() = default; // Virtual destructor
};

// Electronics class derived from Product
class Electronics : public Product {
private:
    int warranty; // in years
public:
    Electronics(string n, double p, int w) : Product(n, p), warranty(w) {}
    void display() override {
        cout << "Electronics: " << name << ", Price: $" << price << ", Warranty: " << warranty << " years" << endl;
    }
};

// Clothing class derived from Product
class Clothing : public Product {
private:
    string size;
public:
    Clothing(string n, double p, string s) : Product(n, p), size(s) {}
    void display() override {
        cout << "Clothing: " << name << ", Price: $" << price << ", Size: " << size << endl;
    }
};

// ProductManager class to manage products
class ProductManager {
    vector<Product*> products;
public:
    void addProduct(Product* product) {
        products.push_back(product);
    }
    void displayProducts() {
        for (auto product : products) {
            product->display();
        }
    }
    ~ProductManager() {
        for (auto product : products) {
            delete product;
        }
    }
};

// ShoppingCart class to manage shopping cart
class ShoppingCart {
    vector<Product*> cart;
public:
    void addProduct(Product* product) {
        cart.push_back(product);
    }
    void removeProduct(Product* product) {
        cart.erase(remove(cart.begin(), cart.end(), product), cart.end());
    }
    void showCart() {
        for (auto product : cart) {
            product->display();
        }
    }
    double getTotal() {
        double total = 0;
        for (auto product : cart) {
            total += product->getPrice();
        }
        return total;
    }
};

// Order class to manage orders
class Order {
    ShoppingCart cart;
    double totalAmount;
public:
    Order(ShoppingCart& c) : cart(c), totalAmount(c.getTotal()) {}
    void placeOrder() {
        cout << "Order placed. Total amount: $" << totalAmount << endl;
    }
    void viewOrder() {
        cout << "Order details: " << endl;
        cart.showCart();
        cout << "Total amount: $" << totalAmount << endl;
    }
};

// Main function to demonstrate the system
int main() {
    // User management
    UserManager userManager;
    User* admin = new Admin("admin", "admin123");
    User* customer = new Customer("john", "password");
    userManager.registerUser(admin);
    userManager.registerUser(customer);

    // Product management
    ProductManager productManager;
    Product* laptop = new Electronics("Laptop", 1000.0, 2);
    Product* tshirt = new Clothing("T-Shirt", 20.0, "L");
    productManager.addProduct(laptop);
    productManager.addProduct(tshirt);

    // Customer login and operations
    User* loggedInUser = userManager.loginUser("john", "password");
    if (loggedInUser) {
        loggedInUser->displayRole();
        ShoppingCart cart;
        cart.addProduct(laptop);
        cart.addProduct(tshirt);
        cart.showCart();

        Order order(cart);
        order.placeOrder();
        order.viewOrder();
    }

    delete admin;
    delete customer;

    return 0;
}

