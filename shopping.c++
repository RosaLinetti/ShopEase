#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// Function to convert string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to check if a username exists in the file
bool usernameExists(const std::string& username, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (toLowerCase(line) == toLowerCase(username)) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Function to handle user login
void login(std::string& username) {
    std::cout << "Enter your username: ";
    std::cin >> username;

    if (usernameExists(username, "users.txt")) {
        std::cout << "Welcome, " << username << "!\n";
    } else {
        std::cout << "Username not found. Please sign up.\n";
        username = "";
    }
}

// Function to handle user signup
void signup(std::string& username) {
    std::cout << "Enter a new username: ";
    std::cin >> username;

    if (usernameExists(username, "users.txt")) {
        std::cout << "Username already exists. Please login.\n";
        username = "";
    } else {
        std::ofstream outfile("users.txt", std::ios_base::app);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file: users.txt" << std::endl;
            return;
        }
        outfile << username << "\n";
        outfile.close();
        std::cout << "Welcome, " << username << "!\n";
    }
}

// Function to manage inventory
void manageInventory(std::string& username, std::vector<std::string>& inventory) {
    const int ADD_ITEM_CHOICE = 1;
    const int REMOVE_ITEM_CHOICE = 2;
    const int LIST_ITEMS_CHOICE = 3;
    const int EXIT_CHOICE = 4;

    int choice;
    while (true) {
        std::cout << "Inventory Management:\n";
        std::cout << "1. Add item\n";
        std::cout << "2. Remove item\n";
        std::cout << "3. List items\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case ADD_ITEM_CHOICE: {
                std::cout << "Enter item to add: ";
                std::string item;
                std::cin >> item;
                inventory.push_back(item);
                std::cout << "Item added successfully.\n";
                break;
            }
            case REMOVE_ITEM_CHOICE: {
                std::cout << "Enter item to remove: ";
                std::string item;
                std::cin >> item;
                auto it = std::find_if(inventory.begin(), inventory.end(),
                                      [item](const std::string& invItem) { return toLowerCase(invItem) == toLowerCase(item); });
                if (it!= inventory.end()) {
                    inventory.erase(it);
                    std::cout << "Item removed successfully.\n";
                } else {
                    std::cout << "Item not found in inventory.\n";
                }
                break;
            }
            case LIST_ITEMS_CHOICE: {
                std::cout << "Current inventory:\n";
                for (const auto& item : inventory) {
                    std::cout << "- " << item << "\n";
                }
                break;
            }
            case EXIT_CHOICE: {
                return;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
}

// Function to handle shopping process
void processShopping(const std::string& username, const std::vector<std::string>& inventory) {
    std::vector<std::string> shoppingList;

    std::cout << "Enter items for your shopping list (type 'done' to finish, 'remove' to remove last item):\n";
    std::string item;
    while (true) {
        std::cin >> item;
        if (item == "done") break;
        if (item == "remove" &&!shoppingList.empty()) {
            shoppingList.pop_back();
            std::cout << "Last item removed. Current list:\n";
            continue;
        }
        shoppingList.push_back(item);
    }

    std::cout << "\nChecking availability:\n";
    for (const auto& item : shoppingList) {
        bool available = std::find_if(inventory.begin(), inventory.end(),
                                      [item](const std::string& invItem) { return toLowerCase(invItem) == toLowerCase(item); }) != inventory.end();
        std::cout << item << ": " << (available ? "Available" : "Not available") << "\n";
    }

    std::ofstream file(username + "_history.txt", std::ios_base::app);
    if (file.is_open()) {
        file << "Shopping list:\n";
        for (const auto& item : shoppingList) {
            file << "- " << item << "\n";
        }
        file << "\n";
        file.close();
        std::cout << "Shopping list saved successfully.\n";
    } else {
        std::cerr << "Error opening file: " << username + "_history.txt" << std::endl;
    }
}

// Function to view shopping history
void viewHistory(const std::string& username) {
    std::ifstream file(username + "_history.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << "\n";
        }
        file.close();
    } else {
        std::cerr << "Error opening file: " << username + "_history.txt" << std::endl;
        std::cout << "No history found for user " << username << ".\n";
    }
}

int main() {
    const int LOGIN_CHOICE = 1;
    const int SIGNUP_CHOICE = 2;
    const int VIEW_HISTORY_CHOICE = 3;
    const int MANAGE_INVENTORY_CHOICE = 4;
    const int EXIT_CHOICE = 5;

    std::string username;
    std::vector<std::string> inventory = {"apple", "banana", "bread", "milk", "eggs", "cheese", "juice"};

    while (true) {
        std::cout << "Welcome to the Shopping System!\n";
        std::cout << "1. Login\n";
        std::cout << "2. Sign up\n";
        std::cout << "3. View shopping history\n";
        std::cout << "4. Manage inventory\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case LOGIN_CHOICE: {
                login(username);
                if (!username.empty()) {
                    bool exitSubMenu = false;
                    while (!exitSubMenu) {
                        std::cout << "1. Process shopping\n";
                        std::cout << "2. View shopping history\n";
                        std::cout << "3. Manage inventory\n";
                        std::cout << "4. Exit\n";
                        std::cout << "Enter your choice: ";
                        int subChoice;
                        std::cin >> subChoice;

                        switch (subChoice) {
                            case 1: {
                                processShopping(username, inventory);
                                break;
                            }
                            case 2: {
                                viewHistory(username);
                                break;
                            }
                            case 3: {
                                manageInventory(username, inventory);
                                break;
                            }
                            case 4: {
                                exitSubMenu = true;
                                break;
                            }
                            default: {
                                std::cout << "Invalid choice. Please try again.\n";
                            }
                        }
                    }
                }
                break;
            }
            case SIGNUP_CHOICE: {
                signup(username);
                break;
            }
            case VIEW_HISTORY_CHOICE: {
                if (!username.empty()) {
                    viewHistory(username);
                } else {
                    std::cout << "Please login or sign up before viewing shopping history.\n";
                }
                break;
            }
            case MANAGE_INVENTORY_CHOICE: {
                if (!username.empty()) {
                    manageInventory(username, inventory);
                } else {
                    std::cout << "Please login or sign up before managing inventory.\n";
                }
                break;
            }
            case EXIT_CHOICE: {
                std::cout << "Exiting the program. Goodbye!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    return 0;
}
