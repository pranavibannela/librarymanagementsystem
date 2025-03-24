// Function to delete a book from BST
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Book {
    int bookID;
    string title, author;
    Book* left;
    Book* right;

    Book(int id, string t, string a) {
        bookID = id;
        title = t;
        author = a;
        left = right = nullptr;
    }
};

// Function to insert book into BST
Book* insertBook(Book* root, int id, string title, string author) {
    if (root == nullptr)
        return new Book(id, title, author);

    if (id < root->bookID)
        root->left = insertBook(root->left, id, title, author);
    else if (id > root->bookID)
        root->right = insertBook(root->right, id, title, author);
    
    return root;
}

// Function to save BST data to a file (Inorder Traversal)
void saveToFile(Book* root, ofstream& file) {
    if (root == nullptr) return;
    saveToFile(root->left, file);
    file << root->bookID << "," << root->title << "," << root->author << endl;
    saveToFile(root->right, file);
}

// Function to write books to file
void saveBooks(Book* root) {
    ofstream file("books.txt");
    if (!file) {
        cout << "Error opening file for saving!\n";
        return;
    }
    saveToFile(root, file);
    file.close();
}

// Function to load books from file
Book* loadBooks(Book* root) {
    ifstream file("books.txt");
    if (!file) {
        cout << "No previous data found. Starting fresh...\n";
        return nullptr;
    }
    
    int id;
    string title, author, line;
    while (getline(file, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.rfind(",");
        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) continue;

        id = stoi(line.substr(0, pos1));
        title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        author = line.substr(pos2 + 1);

        root = insertBook(root, id, title, author);
    }

    file.close();
    return root;
}

// Function to display books in sorted order
void displayBooks(Book* root) {
    if (root == nullptr) return;
    displayBooks(root->left);
    cout << "Book ID: " << root->bookID << ", Title: " << root->title << ", Author: " << root->author << endl;
    displayBooks(root->right);
}

// Function to search for a book
void searchBook(Book* root, int id) {
    if (root == nullptr) {
        cout << "Book Not Found!\n";
        return;
    }

    if (root->bookID == id) {
        cout << "Book Found!\n";
        cout << "Book ID: " << root->bookID << ", Title: " << root->title << ", Author: " << root->author << endl;
        return;
    }

    if (id < root->bookID)
        searchBook(root->left, id);
    else
        searchBook(root->right, id);
}

// Function to find the minimum value node
Book* findMin(Book* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}

// Function to delete a book
Book* deleteBook(Book* root, int id) {
    if (root == nullptr)
        return root;

    if (id < root->bookID)
        root->left = deleteBook(root->left, id);
    else if (id > root->bookID)
        root->right = deleteBook(root->right, id);
    else {
        if (root->left == nullptr) {
            Book* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Book* temp = root->left;
            delete root;
            return temp;
        }

        Book* temp = findMin(root->right);
        root->bookID = temp->bookID;
        root->title = temp->title;
        root->author = temp->author;
        root->right = deleteBook(root->right, temp->bookID);
    }
    return root;
}

int main() {
    Book* root = nullptr;
    int choice, id;
    string title, author;

    // Load books from file when the program starts
    root = loadBooks(root);

    while (true) {
        cout << "\nLibrary Management System (With File Storage)\n";
        cout << "1. Insert Book\n2. Display Books\n3. Search Book\n4. Delete Book\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Author Name: ";
                getline(cin, author);
                root = insertBook(root, id, title, author);
                saveBooks(root);  // Save updated data to file
                cout << "Book added successfully!\n";
                break;
            case 2:
                cout << "Library Books (Sorted by Book ID):\n";
                displayBooks(root);
                break;
            case 3:
                cout << "Enter Book ID to search: ";
                cin >> id;
                searchBook(root, id);
                break;
            case 4:
                cout << "Enter Book ID to delete: ";
                cin >> id;
                root = deleteBook(root, id);
                saveBooks(root);  // Save updated data to file
                cout << "Book deleted successfully!\n";
                break;
            case 5:
                saveBooks(root);  // Ensure books are saved before exiting
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice, try again!\n";
        }
    }
}
