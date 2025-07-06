// Complaint Management System
// Developed for Data Structures course, AASTMT-Aswan, Spring 2024
// Author: [Your Name]
// Description: A console-based application to manage customer complaints using
// manually implemented data structures: linked list (employee management),
// queue (complaint storage), stack (problem summaries), and priority queue
// (urgent complaints). Supports Customer, Employee, and Admin roles with
// file-based data persistence.

#include <iostream>
#include <limits>
#include <string>
#include <fstream>

using namespace std;

// Function to handle invalid input
void handleInvalidInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. Please try again.\n";
}

// Class to represent a customer
class Customer {
private:
    string name;
    string phone;
    string email;

public:
    Customer(const string& name, const string& phone, const string& email)
        : name(name), phone(phone), email(email) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
};

// Class to represent an employee
class Employee {
private:
    string name;
    string id;
    string password;

public:
    Employee(const string& name, const string& id, const string& password)
        : name(name), id(id), password(password) {}

    string getName() const { return name; }
    string getId() const { return id; }
    string getPassword() const { return password; }
    bool validatePassword(const string& inputPassword) const {
        return password == inputPassword;
    }
};

// Structure to represent a complaint
struct Complaint {
    static int lastId; // Tracks the last used complaint ID
    int id;
    string content;
    bool replied;
    bool urgent;
    Customer customer;
    string replyDetails;

    Complaint(const string& content, bool replied, bool urgent, const Customer& customer)
        : id(++lastId), content(content), replied(replied), urgent(urgent), customer(customer) {}

    void addReply(const string& reply) {
        replyDetails = reply;
        replied = true;
    }
};
int Complaint::lastId = 0;

// Linked list-based queue for storing complaints
class LinkedQueue {
public:
    struct Node {
        Complaint data;
        Node* next;
        Node(const Complaint& complaint) : data(complaint), next(nullptr) {}
    };

private:
    Node* front;
    Node* rear;
    int size;

public:
    LinkedQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~LinkedQueue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        size = 0;
    }

    bool isEmpty() const { return size == 0; }

    void enqueue(const Complaint& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "No complaints in queue.\n";
            return;
        }
        Node* temp = front;
        front = front->next;
        delete temp;
        size--;
        if (front == nullptr) {
            rear = nullptr;
        }
    }

    Complaint getFront() const {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return Complaint("", false, false, Customer("", "", ""));
        }
        return front->data;
    }

    int getSize() const { return size; }

    // Returns the front node (for traversal)
    Node* getFrontNode() const { return front; }

    // Returns the rear node
    Node* getRearNode() const { return rear; }

    // Sets the rear node
    void setRear(Node* node) { rear = node; }

    // Decrements the size
    void decrementSize() { if (size > 0) size--; }

    void display() const {
        if (isEmpty()) {
            cout << "No complaints to display.\n";
            return;
        }
        Node* current = front;
        cout << "Complaints in queue:\n";
        while (current != nullptr) {
            cout << "-----------------------------------------\n";
            cout << "ID: " << current->data.id << "\n";
            cout << "Content: " << current->data.content << "\n";
            cout << "Customer Name: " << current->data.customer.getName() << "\n";
            cout << "Customer Email: " << current->data.customer.getEmail() << "\n";
            cout << "Replied: " << (current->data.replied ? "Yes" : "No") << "\n";
            if (current->data.replied) {
                cout << "Reply Details: " << current->data.replyDetails << "\n";
            }
            cout << "Urgent: " << (current->data.urgent ? "Yes" : "No") << "\n";
            current = current->next;
        }
        cout << "-----------------------------------------\n";
    }
};
LinkedQueue complaintQueue;

// Stack for storing problem summaries
class Stack {
public:
    struct Node {
        Complaint data;
        Node* next;
        Node(const Complaint& complaint) : data(complaint), next(nullptr) {}
    };

private:
    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }

    bool isEmpty() const { return size == 0; }

    void push(const Complaint& complaint) {
        Node* newNode = new Node(complaint);
        newNode->next = top;
        top = newNode;
        size++;
    }

    void pop() {
        if (isEmpty()) return;
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
    }

    Complaint topComplaint() const {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return Complaint("", false, false, Customer("", "", ""));
        }
        return top->data;
    }

    int getSize() const { return size; }
};
Stack summaryStack;

// Priority queue implemented as a sorted linked list for urgent complaints
class PriorityQueue {
public:
    struct Node {
        int order; // Priority (lower number = higher priority)
        Complaint data;
        Node* next;
        Node(int ord, const Complaint& complaint) : order(ord), data(complaint), next(nullptr) {}
    };

private:
    Node* head;
    int size;

public:
    PriorityQueue() : head(nullptr), size(0) {}

    ~PriorityQueue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    bool isEmpty() const { return size == 0; }

    void insert(int order, const Complaint& complaint) {
        Node* newNode = new Node(order, complaint);
        if (head == nullptr || head->order > order) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr && current->next->order <= order) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }

    void remove(int id) {
        Node* current = head;
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->data.id == id) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                cout << "Complaint ID " << id << " removed from priority queue.\n";
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Complaint ID " << id << " not found in priority queue.\n";
    }

    void display() const {
        if (isEmpty()) {
            cout << "No urgent complaints.\n";
            return;
        }
        Node* current = head;
        cout << "Urgent Complaints:\n";
        while (current != nullptr) {
            cout << "-----------------------------------------\n";
            cout << "Order: " << current->order << "\n";
            cout << "ID: " << current->data.id << "\n";
            cout << "Content: " << current->data.content << "\n";
            cout << "Replied: " << (current->data.replied ? "Yes" : "No") << "\n";
            cout << "Customer Name: " << current->data.customer.getName() << "\n";
            cout << "Customer Email: " << current->data.customer.getEmail() << "\n";
            current = current->next;
        }
        cout << "-----------------------------------------\n";
    }
};
PriorityQueue urgentQueue;

// Linked list for employee management
class EmployeeList {
public:
    struct Node {
        Employee employee;
        Node* next;
        Node(const Employee& emp) : employee(emp), next(nullptr) {}
    };

private:
    Node* head;

public:
    EmployeeList() : head(nullptr) {}

    ~EmployeeList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addEmployee(const Employee& emp) {
        Node* newNode = new Node(emp);
        newNode->next = head;
        head = newNode;
        cout << "Employee added successfully!\n";
    }

    void displayEmployees() const {
        if (head == nullptr) {
            cout << "No employees found.\n";
            return;
        }
        Node* current = head;
        cout << "Employees List\n================\n";
        while (current != nullptr) {
            cout << "Name: " << current->employee.getName() << "\n";
            cout << "ID: " << current->employee.getId() << "\n";
            cout << "Password: " << current->employee.getPassword() << "\n";
            cout << "---------------\n";
            current = current->next;
        }
        cout << "================\n";
    }

    void deleteEmployee(const string& id) {
        Node* current = head;
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->employee.getId() == id) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                cout << "Employee ID " << id << " deleted successfully!\n";
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Employee ID " << id << " not found.\n";
    }
};
EmployeeList empList;

// Customer functions
void addNewComplaint() {
    string name, phone, email, content;
    cout << "Enter your name: ";
    getline(cin >> ws, name);
    cout << "Enter your phone number: ";
    getline(cin, phone);
    cout << "Enter your email: ";
    getline(cin, email);
    if (email.empty() || email.find('@') == string::npos) {
        cout << "Invalid email format!\n";
        return;
    }
    cout << "Enter complaint content: ";
    getline(cin, content);
    if (content.empty()) {
        cout << "Complaint content cannot be empty!\n";
        return;
    }

    Customer customer(name, phone, email);
    Complaint complaint(content, false, false, customer);
    complaintQueue.enqueue(complaint);
    cout << "Complaint ID: " << complaint.id << "\n";
    cout << "Complaint received. We will respond soon.\n";
}

void deleteComplaint() {
    int complaintId;
    bool found = false;
    cout << "Enter complaint ID to delete: ";
    cin >> complaintId;
    if (cin.fail()) {
        handleInvalidInput();
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    LinkedQueue::Node* prev = nullptr;
    while (current != nullptr) {
        if (current->data.id == complaintId) {
            found = true;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (found) {
        if (prev == nullptr) {
            complaintQueue.dequeue();
        } else {
            prev->next = current->next;
            if (current == complaintQueue.getRearNode()) {
                complaintQueue.setRear(prev);
            }
            delete current;
            complaintQueue.decrementSize();
        }
        cout << "Complaint ID " << complaintId << " deleted successfully!\n";
    } else {
        cout << "Complaint ID " << complaintId << " not found!\n";
    }
}

void showComplaintDetails() {
    int complaintId;
    cout << "Enter complaint ID: ";
    cin >> complaintId;
    if (cin.fail()) {
        handleInvalidInput();
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    bool found = false;
    while (current != nullptr) {
        if (current->data.id == complaintId) {
            found = true;
            cout << "=========================================\n";
            cout << "Complaint Details\n";
            cout << "ID: " << current->data.id << "\n";
            cout << "Customer Name: " << current->data.customer.getName() << "\n";
            cout << "Customer Email: " << current->data.customer.getEmail() << "\n";
            cout << "Content: " << current->data.content << "\n";
            cout << "Replied: " << (current->data.replied ? "Yes" : "No") << "\n";
            if (current->data.replied) {
                cout << "Reply Details: " << current->data.replyDetails << "\n";
            }
            cout << "Urgent: " << (current->data.urgent ? "Yes" : "No") << "\n";
            cout << "=========================================\n";
            break;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Complaint ID " << complaintId << " not found.\n";
    }
}

void searchHistory() {
    string email;
    cout << "Enter email to search complaints: ";
    cin >> email;
    if (complaintQueue.isEmpty()) {
        cout << "No complaints in queue.\n";
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    bool found = false;
    while (current != nullptr) {
        if (current->data.customer.getEmail() == email) {
            found = true;
            cout << "=========================================\n";
            cout << "Complaint ID: " << current->data.id << "\n";
            cout << "Customer Name: " << current->data.customer.getName() << "\n";
            cout << "Content: " << current->data.content << "\n";
            cout << "Replied: " << (current->data.replied ? "Yes" : "No") << "\n";
            if (current->data.replied) {
                cout << "Reply Details: " << current->data.replyDetails << "\n";
            }
            cout << "=========================================\n";
        }
        current = current->next;
    }
    if (!found) {
        cout << "No complaints found for email: " << email << "\n";
    }
}

// Employee functions
void addReply() {
    int complaintId;
    cout << "Enter complaint ID to reply: ";
    cin >> complaintId;
    if (cin.fail()) {
        handleInvalidInput();
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    bool found = false;
    while (current != nullptr) {
        if (current->data.id == complaintId) {
            found = true;
            string reply;
            cout << "Enter reply details: ";
            getline(cin >> ws, reply);
            current->data.addReply(reply);
            cout << "Reply added successfully!\n";
            break;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Complaint ID " << complaintId << " not found.\n";
    }
}

void showUnrepliedComplaints(bool employeeMode = false) {
    if (complaintQueue.isEmpty()) {
        cout << "No complaints found.\n";
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    int count = 0;
    cout << "=========================================\n";
    cout << "Unreplied Complaints\n";
    while (current != nullptr) {
        if (!current->data.replied) {
            count++;
            cout << "ID: " << current->data.id << "\n";
            cout << "Customer Name: " << current->data.customer.getName() << "\n";
            cout << "Content: " << current->data.content << "\n";
            cout << "Urgent: " << (current->data.urgent ? "Yes" : "No") << "\n";
            cout << "-----------------------------------------\n";
        }
        current = current->next;
    }
    cout << "Total unreplied: " << count << "\n";
    cout << "=========================================\n";

    if (employeeMode) {
        int option;
        do {
            cout << "1) Reply to Complaint\n2) Back\nOption: ";
            cin >> option;
            if (cin.fail()) {
                handleInvalidInput();
                continue;
            }
            if (option == 1) addReply();
        } while (option != 2);
    }
}

void addProblemSummary() {
    int complaintId;
    cout << "Enter complaint ID for summary: ";
    cin >> complaintId;
    if (cin.fail()) {
        handleInvalidInput();
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    bool found = false;
    while (current != nullptr) {
        if (current->data.id == complaintId) {
            found = true;
            string summary;
            cout << "Enter problem summary: ";
            getline(cin >> ws, summary);
            current->data.addReply(summary); // Treat summary as reply
            summaryStack.push(current->data);
            cout << "Summary added successfully!\n";
            break;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Complaint ID " << complaintId << " not found.\n";
    }
}

void showComplaintsWithSummary() {
    if (summaryStack.isEmpty()) {
        cout << "No summaries found.\n";
        return;
    }

    Stack tempStack;
    cout << "=========================================\n";
    cout << "Complaints with Summaries\n";
    while (!summaryStack.isEmpty()) {
        Complaint complaint = summaryStack.topComplaint();
        cout << "ID: " << complaint.id << "\n";
        cout << "Content: " << complaint.content << "\n";
        cout << "Summary: " << complaint.replyDetails << "\n";
        cout << "-----------------------------------------\n";
        tempStack.push(complaint);
        summaryStack.pop();
    }
    while (!tempStack.isEmpty()) {
        summaryStack.push(tempStack.topComplaint());
        tempStack.pop();
    }
    cout << "=========================================\n";
}

void searchByComplaintNameWithSummary() {
    string name;
    cout << "Enter complaint content to search: ";
    getline(cin >> ws, name);
    if (summaryStack.isEmpty()) {
        cout << "No summaries found.\n";
        return;
    }

    Stack tempStack;
    cout << "=========================================\n";
    cout << "Search Results\n";
    while (!summaryStack.isEmpty()) {
        Complaint complaint = summaryStack.topComplaint();
        if (complaint.content.find(name) != string::npos) {
            cout << "ID: " << complaint.id << "\n";
            cout << "Content: " << complaint.content << "\n";
            cout << "Summary: " << complaint.replyDetails << "\n";
            cout << "-----------------------------------------\n";
        }
        tempStack.push(complaint);
        summaryStack.pop();
    }
    while (!tempStack.isEmpty()) {
        summaryStack.push(tempStack.topComplaint());
        tempStack.pop();
    }
    cout << "=========================================\n";
}

// Admin functions
void addNewEmployee() {
    string name, id, password;
    cout << "Enter employee name: ";
    getline(cin >> ws, name);
    cout << "Enter employee ID: ";
    getline(cin, id);
    cout << "Enter employee password: ";
    getline(cin, password);
    Employee emp(name, id, password);
    empList.addEmployee(emp);
}

void deleteEmployee() {
    string id;
    cout << "Enter employee ID to delete: ";
    cin >> id;
    empList.deleteEmployee(id);
}

void addUrgentComplaint() {
    int complaintId;
    cout << "Enter complaint ID for urgent: ";
    cin >> complaintId;
    if (cin.fail()) {
        handleInvalidInput();
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    bool found = false;
    while (current != nullptr) {
        if (current->data.id == complaintId) {
            found = true;
            int order;
            cout << "Enter priority order (lower = higher priority): ";
            cin >> order;
            if (cin.fail()) {
                handleInvalidInput();
                return;
            }
            current->data.urgent = true;
            urgentQueue.insert(order, current->data);
            cout << "Complaint ID " << complaintId << " added to urgent queue.\n";
            break;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Complaint ID " << complaintId << " not found.\n";
    }
}

// File I/O functions
void loadComplaintDataFromFile() {
    ifstream inFile("complaint_data.txt");
    if (!inFile.is_open()) {
        cout << "No complaint data file found. Starting empty.\n";
        return;
    }

    int id;
    string content, name, phone, email, replyDetails;
    bool replied, urgent;
    while (inFile >> id) {
        inFile.ignore();
        getline(inFile, content);
        inFile >> replied >> urgent;
        inFile.ignore();
        getline(inFile, name);
        getline(inFile, phone);
        getline(inFile, email);
        getline(inFile, replyDetails);

        Customer customer(name, phone, email);
        Complaint complaint(content, replied, urgent, customer);
        complaint.id = id;
        complaint.replyDetails = replyDetails;
        complaintQueue.enqueue(complaint);
    }
    inFile.close();
    cout << "Complaint data loaded successfully!\n";
}

void saveComplaintDataToFile() {
    ofstream outFile("complaint_data.txt");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open file for writing!\n";
        return;
    }

    LinkedQueue::Node* current = complaintQueue.getFrontNode();
    while (current != nullptr) {
        outFile << current->data.id << "\n";
        outFile << current->data.content << "\n";
        outFile << current->data.replied << "\n";
        outFile << current->data.urgent << "\n";
        outFile << current->data.customer.getName() << "\n";
        outFile << current->data.customer.getPhone() << "\n";
        outFile << current->data.customer.getEmail() << "\n";
        outFile << current->data.replyDetails << "\n";
        current = current->next;
    }
    outFile.close();
    cout << "Complaint data saved successfully!\n";
}

int main() {
    loadComplaintDataFromFile();
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "        Complaint Management System\n";
        cout << "============================================\n\n";
        cout << "1. Customer\n2. Employee\n3. Admin\n0. Exit\n";
        cout << "Option: ";
        cin >> choice;
        if (cin.fail()) {
            handleInvalidInput();
            continue;
        }

        switch (choice) {
        case 1: {
            int option;
            do {
                cout << "\n======================\n";
                cout << "    Customer Menu\n";
                cout << "1) Add Complaint\n2) Delete Complaint\n3) View Details\n";
                cout << "4) View History\n0) Back\nOption: ";
                cin >> option;
                if (cin.fail()) {
                    handleInvalidInput();
                    continue;
                }
                switch (option) {
                case 1: addNewComplaint(); break;
                case 2: deleteComplaint(); break;
                case 3: showComplaintDetails(); break;
                case 4: searchHistory(); break;
                case 0: saveComplaintDataToFile(); break;
                default: cout << "Invalid option.\n";
                }
            } while (option != 0);
            break;
        }
        case 2: {
            int option;
            do {
                cout << "\n======================\n";
                cout << "    Employee Menu\n";
                cout << "1) View Urgent\n2) View Unreplied\n3) Reply\n";
                cout << "4) Add Summary\n5) View Summaries\n6) Search Summaries\n";
                cout << "0) Back\nOption: ";
                cin >> option;
                if (cin.fail()) {
                    handleInvalidInput();
                    continue;
                }
                switch (option) {
                case 1: urgentQueue.display(); break;
                case 2: showUnrepliedComplaints(true); break;
                case 3: addReply(); break;
                case 4: addProblemSummary(); break;
                case 5: showComplaintsWithSummary(); break;
                case 6: searchByComplaintNameWithSummary(); break;
                case 0: break;
                default: cout << "Invalid option.\n";
                }
            } while (option != 0);
            break;
        }
        case 3: {
            int option;
            do {
                cout << "\n======================\n";
                cout << "     Admin Menu\n";
                cout << "1) Employee List\n2) Complaint List\n0) Back\nOption: ";
                cin >> option;
                if (cin.fail()) {
                    handleInvalidInput();
                    continue;
                }
                switch (option) {
                case 1: {
                    int subOption;
                    do {
                        cout << "\n==============================\n";
                        cout << "    Employee List Menu\n";
                        cout << "1) Add Employee\n2) View Employees\n";
                        cout << "3) Delete Employee\n0) Back\nOption: ";
                        cin >> subOption;
                        if (cin.fail()) {
                            handleInvalidInput();
                            continue;
                        }
                        switch (subOption) {
                        case 1: addNewEmployee(); break;
                        case 2: empList.displayEmployees(); break;
                        case 3: deleteEmployee(); break;
                        case 0: break;
                        default: cout << "Invalid option.\n";
                        }
                    } while (subOption != 0);
                    break;
                }
                case 2: {
                    int subOption;
                    do {
                        cout << "\n==============================\n";
                        cout << "    Complaint List Menu\n";
                        cout << "1) View Count\n2) View Unreplied\n";
                        cout << "3) Add Urgent\n4) View Urgent\n0) Back\nOption: ";
                        cin >> subOption;
                        if (cin.fail()) {
                            handleInvalidInput();
                            continue;
                        }
                        switch (subOption) {
                        case 1: cout << "Total complaints: " << complaintQueue.getSize() << "\n"; break;
                        case 2: showUnrepliedComplaints(); break;
                        case 3: addUrgentComplaint(); break;
                        case 4: urgentQueue.display(); break;
                        case 0: break;
                        default: cout << "Invalid option.\n";
                        }
                    } while (subOption != 0);
                    break;
                }
                case 0: break;
                default: cout << "Invalid option.\n";
                }
            } while (option != 0);
            break;
        }
        case 0: cout << "Thank you, goodbye!\n"; break;
        default: cout << "Invalid option.\n";
        }
    } while (choice != 0);
    return 0;
}