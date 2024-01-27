/*
@Author
StudentName :< Abdullah Saatci >
StudentID :< 150210020 >
Date :< 08/01/2024 >
*/

// At first libraries are included
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <time.h>
using namespace std;

// create Employee class
class Employee{ 
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d);
        ~Employee();
        void set_id(int data);
        void set_salary(int data);
        void set_department(int data);
        int get_id();
        int get_salary();
        int get_department();
};
Employee::Employee(int i, int s, int d) {
    this->id = i;
    this->salary = s;
    this->department = d;
}
Employee::~Employee() {
}

void Employee::set_id(int data) {
    this->id = data;
}
void Employee::set_salary(int data) {
    this->salary = data;
}
void Employee::set_department(int data) {
    this->department = data;
}
int Employee::get_id() {
    return this->id;
}
int Employee::get_salary() {
    return this->salary;
}
int Employee::get_department() {
    return this->department;
}

// Create DoublySkipList_Node class
class DoublySkipList_Node{
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        DoublySkipList_Node(Employee *data); 
        ~DoublySkipList_Node();
        Employee* get_employee();
        void set_employee(Employee *employee);
        DoublySkipList_Node* get_next();
        DoublySkipList_Node* get_below();
        void set_next(DoublySkipList_Node*);
        void set_below(DoublySkipList_Node*);
};
DoublySkipList_Node::DoublySkipList_Node(Employee *employee) {
    this->data = employee;
    this->next = nullptr;
    this->below = nullptr;
}
// In destructor of the code, below nodes are deleted recursively
DoublySkipList_Node::~DoublySkipList_Node() {
    if (this->below != nullptr) {
        delete this->below;
    }
}
Employee* DoublySkipList_Node::get_employee() {
    return this->data;
}
void DoublySkipList_Node::set_employee(Employee *employee){
    this->data = employee;
}
DoublySkipList_Node* DoublySkipList_Node::get_next() {
    return this->next;
}
DoublySkipList_Node* DoublySkipList_Node::get_below() {
    return this->below;
}
void DoublySkipList_Node::set_next(DoublySkipList_Node* node){
    this->next = node;
}
void DoublySkipList_Node::set_below(DoublySkipList_Node* node){
    this->below = node;
}

// Creating DoublySkipList class
class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in);
        ~DoublySkipList();
        void insert(Employee* data);
        void remove(int remove_id);
        void update(int id, int department, int salary);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);
        bool isEmpty();
        DoublySkipList_Node* find_node(int id);
        DoublySkipList_Node* find_prev_node(int id);
};

// In constructor of DoublySkipList only head of skiplist is created.
DoublySkipList::DoublySkipList(int height_in){
    this->height = height_in; // parameter assigned in height variable
    Employee* data = new Employee(-1, 0, 0);
    this->head = new DoublySkipList_Node(data);
    DoublySkipList_Node* temp = head;
    for (int i = 0; i < this->height -1; i++) {
        temp->set_below(new DoublySkipList_Node(data));
        temp = temp->get_below();
    }
}

// destructor of DoublySkipList 
DoublySkipList::~DoublySkipList() {
    DoublySkipList_Node* to_be_deleted_row_head = head;
    if (isEmpty()) { // If empty delete only head
        delete head;
    }
    else{ // In each step next node of head will be deleted, when there is no more node at next code will go one step below of head
        while(to_be_deleted_row_head){
            if (to_be_deleted_row_head->get_next()){
                DoublySkipList_Node* to_be_deleted_node = to_be_deleted_row_head->get_next();
                DoublySkipList_Node* to_be_deleted_prev_node = to_be_deleted_row_head;
                DoublySkipList_Node* to_be_deleted_node_head = to_be_deleted_node;
                int remove_id = to_be_deleted_node_head->get_employee()->get_id();
                while(to_be_deleted_node){
                    if (to_be_deleted_prev_node != nullptr && to_be_deleted_node->get_next() != nullptr){
                        to_be_deleted_prev_node->set_next(to_be_deleted_node->get_next());
                        to_be_deleted_node->set_next(nullptr);
                    }
                    else if (to_be_deleted_prev_node && to_be_deleted_node->get_next() == nullptr){
                        to_be_deleted_prev_node->set_next(nullptr);
                        to_be_deleted_node->set_next(nullptr);
                    }
                    else if (to_be_deleted_prev_node == nullptr && to_be_deleted_node->get_next() != nullptr){
                        to_be_deleted_node->set_next(nullptr);
                    }
                    to_be_deleted_prev_node = to_be_deleted_prev_node->get_below();
                    to_be_deleted_node = to_be_deleted_node->get_below();
                    while(to_be_deleted_prev_node && to_be_deleted_prev_node->get_next() && to_be_deleted_prev_node->get_next()->get_employee()->get_id() < remove_id){
                        to_be_deleted_prev_node = to_be_deleted_prev_node->get_next();
                    }
                }
                if(to_be_deleted_node_head){
                    delete to_be_deleted_node_head->get_employee();
                    delete to_be_deleted_node_head;
                }
            } else{
                to_be_deleted_row_head = to_be_deleted_row_head->get_below();
            }
        }
        delete head->get_employee();
        delete head;
    }
}
// Finding head node of the column requested by id
DoublySkipList_Node* DoublySkipList::find_node(int id){
    DoublySkipList_Node* temp = head;
    while(temp != nullptr && temp->get_employee()->get_id() != id){
        while(temp->get_next() != nullptr && temp->get_next()->get_employee()->get_id() <= id){
            temp = temp->get_next();
        }
        if (temp->get_employee()->get_id() != id){
            temp = temp->get_below();
        }
    }
    return temp;
}

// Finding previous node of the head of the column requested by id
DoublySkipList_Node* DoublySkipList::find_prev_node(int id){
    DoublySkipList_Node* temp = head;
    while(temp){
        if (temp->get_next() && temp->get_next()->get_employee()->get_id() < id){
            temp = temp->get_next();
        } else if (temp->get_next() && temp->get_next()->get_employee()->get_id() == id){
            return temp;
        } else if (temp->get_below() && temp->get_below()){
            temp = temp->get_below();
        } else {
            break;
        }
    }
    return temp;
}

// in head node firstly goes to the below as flong as it can go and then checks whether there is node next to it. If there is not skiplist is empty
bool DoublySkipList::isEmpty() {
    DoublySkipList_Node* temp = head;
    for (int i = 0; i < this->height -1; i++) {
        temp = temp->get_below();
    }
    if (temp->get_next() != nullptr) {
        return false;
    } else {
        return true;
    }
}

void DoublySkipList::insert(Employee* data){
    clock_t start = clock();
    int random_number = 1;
    while (random_number < height && (rand() % 2) == 1) // creating a rendom number to assign as level of new node
        random_number++;
    DoublySkipList_Node* will_be_inserted_node = new DoublySkipList_Node(data);
    DoublySkipList_Node* will_be_inserted_node_head = will_be_inserted_node;

    for (int i = 0; i < random_number -1; i++) { // Firstly, new skiplist nodes are connected with each other
        will_be_inserted_node->set_below(new DoublySkipList_Node(data));
        will_be_inserted_node = will_be_inserted_node->get_below();
    }

    DoublySkipList_Node* temp = head;
    int id = data->get_id();
    for (int i = 0; i < height-random_number; i++){ // to start defining from correct level, temp goes below until the same level with new node's head
        temp = temp->get_below();
    }
    for (int j = 0; j < random_number; j++){ // At each column temp goes next to it until it finds appropriate place for new node
        while (temp->get_next() != nullptr && temp->get_next()->get_employee()->get_id() < id){
            temp = temp->get_next();
        }
        will_be_inserted_node_head->set_next(temp->get_next());
        temp->set_next(will_be_inserted_node_head);
        temp = temp->get_below();
        will_be_inserted_node_head = will_be_inserted_node_head->get_below();
    }
    clock_t end = clock(); // finish measure
    cout << "Add, DoublySkiplist solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void DoublySkipList::remove(int remove_id) {
    clock_t start = clock();
    DoublySkipList_Node* to_be_deleted_prev_node = find_prev_node(remove_id); // previous node is found by the function
    DoublySkipList_Node* to_be_deleted_node = to_be_deleted_prev_node->get_next(); // next of previous node
    DoublySkipList_Node* to_be_deleted_node_head = to_be_deleted_node; // since destructor deletes nodes from above to below we should keep head node to delete it at the end
    while(to_be_deleted_node){ // setting previous nodes' nexts and next nodes' previous if they are not nullptr
        if (to_be_deleted_prev_node != nullptr && to_be_deleted_node->get_next() != nullptr){
            to_be_deleted_prev_node->set_next(to_be_deleted_node->get_next());
            to_be_deleted_node->set_next(nullptr);
        }
        else if (to_be_deleted_prev_node && to_be_deleted_node->get_next() == nullptr){
            to_be_deleted_prev_node->set_next(nullptr);
            to_be_deleted_node->set_next(nullptr);
        }
        else if (to_be_deleted_prev_node == nullptr && to_be_deleted_node->get_next() != nullptr){
            to_be_deleted_node->set_next(nullptr);
        }
        to_be_deleted_prev_node = to_be_deleted_prev_node->get_below(); // goes on step below each step
        to_be_deleted_node = to_be_deleted_node->get_below();
        while(to_be_deleted_prev_node && to_be_deleted_prev_node->get_next() && to_be_deleted_prev_node->get_next()->get_employee()->get_id() < remove_id){
            to_be_deleted_prev_node = to_be_deleted_prev_node->get_next(); // Since this is a skiplist previous node's below may not be again previous of the other node's below, so it goes next to it until it finds the node by same id
        }
    }
    if(to_be_deleted_node_head){
        delete to_be_deleted_node_head->get_employee(); // deleting employee 
        delete to_be_deleted_node_head; // deleting head we defined at the beginning of function
    }
    clock_t end = clock(); // finish measure
    cout << "Delete, DoublySkiplist solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

Employee* DoublySkipList::search(int search_id) { // in search function after checking if skiplist is empty or not, finds the node by find_node function and returns it's employee if there is
    if (isEmpty()) {
        return nullptr;
    }
    DoublySkipList_Node* to_be_searched_node = this->find_node(search_id);
    if (to_be_searched_node == nullptr){
        return nullptr;
    }
    else {
        return to_be_searched_node->get_employee();
    }
}

void DoublySkipList::update(int id, int salary, int department){ // desired employee data found by search is being updated if it is not nullptr
    clock_t start = clock();
    Employee* to_be_updated_employee = search(id);
    if (to_be_updated_employee != nullptr){
        to_be_updated_employee->set_department(department);
        to_be_updated_employee->set_salary(salary);
    }
    else {
        cout << "ERROR: An invalid ID to update\n";
    }
    clock_t end = clock(); // finish measure
    cout << "Update, DoublySkiplist solution: "  << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void DoublySkipList::dumpToFile(ofstream& out_file) { // firstly from head node goes below as much as it can and then prints each value at next nodes.
    out_file << "Employee_ID;Salary;Department\n";
    DoublySkipList_Node* temp = head;
    for (int i = 0; i < this->height -1; i++) {
        temp = temp->get_below();
    }
    while (temp->get_next() != nullptr) {
        temp = temp->get_next();
        out_file << temp->get_employee()->get_id() << ";" << temp->get_employee()->get_salary() << ";" << temp->get_employee()->get_department()  << "\n";
    }
    out_file.close();
}

int main(int agrc, char** argv) {
    DoublySkipList* doubly_skip_list = new DoublySkipList(15); // height is given as parameter
    int will_be_added_id = 0; // to know which id will be added in insert function
    string filename = argv[1];
    string filename_operations = argv[2];
    string line;
    string line2;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "File could not be opened\n";
        return 1;
    }
    while (getline(file, line)) {
        if (will_be_added_id != 0) {
            int index_of_token = 0;
            istringstream ss(line);
            string token;
            int id, salary, department;
            while (getline(ss, token, ';')) {
                if (index_of_token == 0) {
                    id = stoi(token);
                } else if (index_of_token == 1) {
                    salary = stoi(token);
                } else if (index_of_token == 2) {
                    department = stoi(token);
                }
                index_of_token++;
            }
            doubly_skip_list->insert(new Employee(id, salary, department));
        }
        will_be_added_id ++;
    }
    file.close();
    int operation_values[3]; // From operations file each value assigned in this list
    string operation_name;
    ifstream file2(filename_operations);
    while (getline(file2, line2)) {
        int index_of_token_operations = 0;
        istringstream ss(line2);
        string token;
        while (getline(ss, token, ';')) {
            if (index_of_token_operations == 0) {
                operation_name = token;
            } else {
                operation_values[index_of_token_operations -1] = stoi(token);
            }
            index_of_token_operations++;
        }
        if (operation_name == "ADD") {
            Employee* new_employee = new Employee(will_be_added_id, operation_values[0],operation_values[1]);
            doubly_skip_list->insert(new_employee);
            will_be_added_id++;
        }
        else if (operation_name == "DELETE") { // checking the list if it is empty or contains desired value
            if (doubly_skip_list->isEmpty()){
                cout << "ERROR: There is no Employee\n";
            }else if (doubly_skip_list->find_node(operation_values[0]) == nullptr) {
                cout << "ERROR: An invalid ID to delete\n";
            }else{
                doubly_skip_list->remove(operation_values[0]);
            }
        }
        else if (operation_name == "UPDATE") {
            doubly_skip_list->update(operation_values[0], operation_values[1], operation_values[2]);
        }
    }
    file2.close();
    ofstream outputFile;
    outputFile.open("doubly_output.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    doubly_skip_list->dumpToFile(outputFile); // prints the new csv file
    delete doubly_skip_list; // deleting skiplist
    return 0;
}
