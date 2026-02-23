#include <iostream>
#include <string>
#include <stack>
using namespace std;

//Date Class
class Date {
private:
    int day, month, year;

public:
    // Constructor to initialize with default or specific date
    Date(int d = 1, int m = 1, int y = 2023) : day(d), month(m), year(y) {}

    // Input date from user
    void inputDate() {
        do {
            cout << "\nEnter date (DD MM YYYY): ";
            cin >> day >> month >> year;
            if (!isValidDate(day, month, year)) {
                cout << "\nInvalid date. Please try again.\n";
            }
        } while (!isValidDate(day, month, year));
    }

    // Function to check leap year
    bool isLeapYear(int year) const {
        return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    }

    // Function to get number of days in a month
    int getDaysInMonth(int month, int year) const {
        switch (month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            case 4: case 6: case 9: case 11:
                return 30;
            case 2:
                return isLeapYear(year) ? 29 : 28;
            default:
                return 0; // Invalid month
        }
    }

    // Function to check if the given date is valid
    bool isValidDate(int d, int m, int y) const {
        if (m < 1 || m > 12) return false;
        int daysInMonth = getDaysInMonth(m, y);
        return (d >= 1 && d <= daysInMonth);
    }

    // Display date
    void display() const {
        cout << day << "/" << month << "/" << year;
    }

    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    // Setters
    void setDate(int d, int m, int y) {
        if (isValidDate(d, m, y)) {
            day = d;
            month = m;
            year = y;
        } else {
            cout << "Invalid date! Keeping previous date.\n";
        }
    }
};


// Base class for File and Directory
class FileSystemEntity {
public:
    string name;
    int size;
    Date lastModificationDate;
    // Constructor
    FileSystemEntity(string n, int s, Date date) : name(n), size(s), lastModificationDate(date) {}

    // Getter for name
    string getName() const {
        return name;
    }

    // Setter for name
    void setName(string n) {
        name = n;
    }

    // Getter for size
    int getSize() const {
        return size;
    }

    // Setter for size
    void setSize(int newSize) {
        size = newSize;
    }

    // Getter for last modification date
    Date getLastModificationDate() const {
        return lastModificationDate;
    }

    // Setter for last modification date
    void setLastModificationDate(Date date) {
        lastModificationDate = date;
    }
};


// File class
class File : public FileSystemEntity {
private:
    string extension;

public:
    // Constructor
    File() : FileSystemEntity("", 0, Date()), extension("") {}
    File(string name, int size, string ext, Date date = Date()) : FileSystemEntity(name, size, date), extension(ext) {}

    // Display file information
    void display() const {
        cout << "File Name: " << name << "." << extension << ", Size: " << size << " KB, Last Modified: ";
        lastModificationDate.display();
        cout << endl;
    }
};

// Node for linked list of files
class FileNode {
public:
   //File file;
   File* file;
   FileNode* prev; //left == prev
    FileNode* next; // right == next

    // Constructor
   FileNode(File* f) {
        file = f;
        prev = next = nullptr;
    }
};


//FileBST class == FileList class(DLL)
class FileList {
public:
    FileNode* head;
    FileNode* tail;
    FileList() {
        head = tail = nullptr;
    }

    // Insert a file at the end of the doubly linked list
    void insert(File* file) {
        FileNode* newNode = new FileNode(file);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        cout << "File '" << file->getName() << "' added successfully.\n";
    }

    // Search for a file by name
    FileNode* search(string name) {
        FileNode* current = head;
        while (current) {
            if (current->file->getName() == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Delete a file by name
    void deleteFile(string name) {
        FileNode* fileNode = search(name);
        if (!fileNode) {
            cout << "File '" << name << "' not found.\n";
            return;
        }

        if (fileNode == head) head = fileNode->next;
        if (fileNode == tail) tail = fileNode->prev;
        if (fileNode->prev) fileNode->prev->next = fileNode->next;
        if (fileNode->next) fileNode->next->prev = fileNode->prev;

        delete fileNode->file; // Free memory of the file
        delete fileNode; // Free memory of the node
        cout << "File '" << name << "' deleted successfully.\n";
    }

    // Display all files in the list
    void displayFiles() {
        FileNode* current = head;
        while (current) {
            cout << current->file->getName() << " (" << current->file->size << " KB)\n";
            current = current->next;
        }
        cout << endl;
    }
};

//Files to store in Stack in LIFO order(class)
// Stack Node for storing recently opened files
class FileStackNode {
public:
    File* file;
    FileStackNode* next;

    FileStackNode(File* f) : file(f), next(nullptr) {}
};

// Stack class for recently opened files
class FileStack {
private:
    FileStackNode* topNode;

public:
    FileStack() : topNode(nullptr) {}

    // Push a file onto the stack
    void push(File* file) {
        FileStackNode* newNode = new FileStackNode(file);
        newNode->next = topNode;
        topNode = newNode;
    }

    // Pop the most recently opened file
    void pop() {
        if (topNode == nullptr) {
            cout << "No recently opened files." << endl;
            return;
        }
        FileStackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    // Display the recently opened files
    void display() {
        if (topNode == nullptr) {
            cout << "No recently opened files." << endl;
            return;
        }
        cout << "Recently Opened Files (LIFO order):" << endl;
        FileStackNode* temp = topNode;
        while (temp != nullptr) {
            cout << temp->file->getName() << endl;
            temp = temp->next;
        }
    }

    // Destructor to clean up memory
    ~FileStack() {
        while (topNode != nullptr) {
            pop();
        }
    }
};

// Directory class
class Directory : public FileSystemEntity {
private:
    FileNode* fileList; // Linked list of files
    FileStack recentFiles; // Stack for recently opened files
    FileList files; // Each directory contains a doubly linked list for files
    //File file;

public:
    File file;
    // Constructor
    Directory(string name, int size = 0, Date date = Date()) : FileSystemEntity(name, size, date), fileList(nullptr) {}

    // Add a file to the directory
       void addFile(File file) {
        File* newFile = new File(file);
        files.insert(newFile);
        size += file.getSize(); // Update directory size
    }
    // Search for a file
    bool searchFile(string name) {
        return files.search(name);
    }

     // Open a file (record in recent files stack)
    void openFile(string name) {
        if (files.search(name)) {
            File* openedFile = new File(name, 0, "txt"); // Dummy file (retrieve actual data if needed)
            recentFiles.push(openedFile);
            cout << "File '" << name << "' opened and recorded in history." << endl;
        } else {
            cout << "File '" << name << "' not found!" << endl;
        }
    }

    // Display recently opened files
    void showRecentFiles() {
        recentFiles.display();
    }

    // Delete a file
    void deleteFile(string name) {
        files.deleteFile(name);
    }
      // Display all files in sorted order
    void listFiles() {
        files.displayFiles();
    }

    // Display directory information
    void display() {
        cout << "Directory Name: " << name << ", Size: " << size << " KB, Last Modified: ";
        lastModificationDate.display();
        cout << endl;

        if (!files.search("")) { // Check if at least one file exists
        cout << "Files in this directory (LIFO order):" << endl;

        // Using a stack to store files in LIFO order
        //stack<int> myStack;
        stack<FileNode*> fileStack;
        stack<FileNode*> traversalStack;
        FileNode* current = files.head; // Start from root of BST

        // Perform reverse in-order traversal (Right -> Root -> Left)
        while (!traversalStack.empty() || current != nullptr) {
            while (current != nullptr) {
                traversalStack.push(current);
                current = current->next; // Move to right for reverse in-order
            }

            current = traversalStack.top();
            traversalStack.pop();

            fileStack.push(current); // Push into stack for LIFO order

            current = current->prev;
        }

        // Pop from the stack to print in LIFO order
        while (!fileStack.empty()) {
            File* f = fileStack.top()->file;
            cout << f->getName() << "." << " (" << f->getSize() << " KB)" << endl;
            fileStack.pop();
        }
    }

        else {
            cout << "This directory is empty." << endl;
        }
    }
};

// BST Node class
class BSTNode {
public:
    Directory dir;
    BSTNode* left;
    BSTNode* right;

    // Constructor
    BSTNode(Directory d) : dir(d), left(nullptr), right(nullptr) {}
};

// BST class == DirectoryBST
class BST {
private:
    BSTNode* root;

    // Helper function to insert a directory
    BSTNode* insert(BSTNode* node, Directory dir) {
        if (node == nullptr) {
            return new BSTNode(dir);
        }

        if (dir.getName() < node->dir.getName()) {
            node->left = insert(node->left, dir);
        } else if (dir.getName() > node->dir.getName()) {
            node->right = insert(node->right, dir);
        } else {
            cout << "Directory with name '" << dir.getName() << "' already exists!" << endl;
        }
        return node;
    }

    // Helper function to search for a directory by name
    BSTNode* searchByName(BSTNode* node, string name) {
        if (node == nullptr || node->dir.getName() == name) {
            return node;
        }

        if (name < node->dir.getName()) {
            return searchByName(node->left, name);
        } else {
            return searchByName(node->right, name);
        }
    }

    // Helper function to delete a directory by name
    BSTNode* deleteByName(BSTNode* node, string name) {
        if (node == nullptr) {
                cout << "Directory '" << name << "' not found." << endl;
            return node;
        }

        if (name < node->dir.getName()) {
            node->left = deleteByName(node->left, name);
        } else if (name > node->dir.getName()) {
            node->right = deleteByName(node->right, name);
        } else {
            // Node to be deleted found
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                cout << "Directory '" << name << "' deleted." << endl;
                return temp;

            } else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                cout << "Directory '" << name << "' deleted." << endl;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            BSTNode* temp = minValueNode(node->right);
            node->dir = temp->dir;
            node->right = deleteByName(node->right, temp->dir.getName());
        }
        return node;
    }

    // Helper function to find the node with the minimum value
    BSTNode* minValueNode(BSTNode* node) {
        BSTNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Helper function to display all directories (in-order traversal)
    void displayAll(BSTNode* node) {
        if (node == nullptr) {
            return;
        }
        displayAll(node->left);
        node->dir.display();
        displayAll(node->right);
    }

public:
    // Constructor
    BST() : root(nullptr) {}

    // Add a directory to the BST
    void addDirectory(string name, int size = 0) {
        Directory newDir(name, size, Date()); // Initialize with current date
        root = insert(root, newDir);
    }

    // Search for a directory by name
    Directory* searchDirectory(string name) {
        BSTNode* result = searchByName(root, name);
        if (result) {
            return &(result->dir);
        } else {
            cout << "Directory not found!" << endl;
            return nullptr;
        }
    }

    // Delete a directory by name
    void deleteDirectory(string name) {
        root = deleteByName(root, name);
    }

    // Display all directories
    void displayAllDirectories() {
        if (root == nullptr) {
            cout << "No directories found!" << endl;
        } else {
            displayAll(root);
        }
    }
};

// Function to handle user commands
void handleCommand(BST& bst) {
    string command;
    while (true) {
        cout<<"\n\t------------------------\n";
        cout << "\tcr: Create Directory command"<< endl;
        cout << "\tcd: Search Directory by Name command"<< endl;
        cout << "\topen: Open Directory"<< endl;
        cout << "\tfree: Delete Directory by Name"<< endl;
        cout << "\tdir: Show All Directories"<< endl;
        cout<<"\t------------------------\n";
        cout << "\nEnter command (cr, cd, open, free, dir, exit): \n"<< endl;
        cin >> command;

 if (command == "cr") {
            string name;
            cout << "Enter directory name: ";
            cin >> name;
            bst.addDirectory(name);
            cout << "Directory '" << name << "' created." << endl;
        }
         else if (command == "cd") {
            string name;
            cout << "Enter directory name: ";
            cin >> name;
            Directory* dir = bst.searchDirectory(name);
            if (dir) {
                dir->display();
            }
        }


        else if (command == "open") {
            string name;
            int size;
            string ext,d;
            cout << "Enter directory name: ";
            cin >> name;
            Directory* dir = bst.searchDirectory(name);
            if (!dir) {
                cout << "Directory not found!" << endl;
                continue;
            }
            else if(dir){
                 cout << "Directory found!" << endl;
                  while (true) {
        cout<<"\n\t-----------------------------\n";
        cout << "\tadd: Add File to the Directory"<< endl;
        cout << "\tsearch: Search File by Name"<< endl;
        cout << "\tdel: Delete File"<< endl;
        cout << "\tlist: "<< endl;
        cout << "\texit: Exit"<< endl;
        cout<<"\t-------------------------------\n";
        cout << "\nEnter command: add, search, del, list, exit"<<endl;

                cout << "Inside " << name << " > "<<endl;
                cin >> command;

               if (command == "add") {
                    cout << "Enter file name (without extension): ";
                    cin >> name;
                    cout << "Enter file extension(.txt): ";
                    cin >> ext;
                    if (ext != ".txt") {
                        ext = ".txt"; // Ensure the extension is .txt
                    }
                    cout<<"Edit File:\n";
                    cin.ignore();
                    getline(cin, d);
                    // file size (in KB)
                    size = d.size();
                    cout << "\nEnter last modification date (day month year): "<< endl;
                    Date d;
                    d.inputDate();
                    File* newFile = new File(name, size, ext, d);


                    dir->addFile(*newFile); // Passing the actual object
                    cout << "File '" << name << "." << ext << "' added successfully." << endl;
                }

                else if (command == "search") {
                        cout<<"Enter File Name:"<<endl;
                    cin >> name;
                    cout << (dir->searchFile(name) ? "Found" : "Not Found") << endl;
                }
                else if (command == "del") {
                    cout<<"Enter File Name:"<<endl;
                    cin >> name;
                    dir->deleteFile(name);
                }
                else if (command == "list") {
                    dir->listFiles();
                }
                else if (command == "exit") break;
            }
        }
        else
            cout<<"invalid input"<<endl;
    }
        //free == delete
        else if (command == "free") {
            string name;
            cout << "Enter directory name: ";
            cin >> name;
            bst.deleteDirectory(name);
            //cout << "Directory '" << name << "' deleted." << endl;
        }
        //dir == display
        else if (command == "dir") {
            bst.displayAllDirectories();
        } else if (command == "exit") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid command. Please try again." << endl;
        }
    }
}




int main(){
     BST bst;
    handleCommand(bst);
    return 0;

}



















