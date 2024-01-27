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

// Create QuadruplySkipList_Node class
class QuadruplySkipList_Node{
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
    public:
        QuadruplySkipList_Node(Employee *data); 
        ~QuadruplySkipList_Node();
        Employee* get_employee();
        void set_employee(Employee *employee);
        QuadruplySkipList_Node* get_next();
        QuadruplySkipList_Node* get_prev();
        QuadruplySkipList_Node* get_below();
        QuadruplySkipList_Node* get_above();
        void set_next(QuadruplySkipList_Node*);
        void set_prev(QuadruplySkipList_Node*);
        void set_below(QuadruplySkipList_Node*);
        void set_above(QuadruplySkipList_Node*);
};
QuadruplySkipList_Node::QuadruplySkipList_Node(Employee *employee) {
    this->data = employee;
    this->next = nullptr;
    this->prev = nullptr;
    this->below = nullptr;
    this->above = nullptr;
}
QuadruplySkipList_Node::~QuadruplySkipList_Node() {
    // In destructor of the code, above nodes are deleted recursively and not to try to delete same employee again it deletes it only when it is at head node of the column
    if (this->above == nullptr) {
        delete this->data;
    }
    if (this->above != nullptr) {
        delete this->above;
    }
    if (this->prev != nullptr && this->next != nullptr){ // previous and next nodes are adjusted in destructor of node
        this->prev->set_next(this->next);
        this->get_next()->set_prev(this->prev);
        this->set_prev(nullptr);
        this->set_next(nullptr);
    }
    else if (this->prev != nullptr && this->next == nullptr){
        this->prev->set_next(nullptr);
        this->set_prev(nullptr);
    }
    else if (this->prev == nullptr && this->next != nullptr){
        this->get_next()->set_prev(this->get_prev());
        this->next->set_prev(nullptr);
        this->set_next(nullptr);
    }
}
Employee* QuadruplySkipList_Node::get_employee() {
    return this->data;
}
void QuadruplySkipList_Node::set_employee(Employee *employee){
    this->data = employee;
}
QuadruplySkipList_Node* QuadruplySkipList_Node::get_next() {
    return this->next;
}
QuadruplySkipList_Node* QuadruplySkipList_Node::get_prev() {
    return this->prev;
}
QuadruplySkipList_Node* QuadruplySkipList_Node::get_below() {
    return this->below;
}
QuadruplySkipList_Node* QuadruplySkipList_Node::get_above() {
    return this->above;
}
void QuadruplySkipList_Node::set_next(QuadruplySkipList_Node* node){
    this->next = node;
}
void QuadruplySkipList_Node::set_prev(QuadruplySkipList_Node* node){
    this->prev = node;
}
void QuadruplySkipList_Node::set_below(QuadruplySkipList_Node* node){
    this->below = node;
}
void QuadruplySkipList_Node::set_above(QuadruplySkipList_Node* node){
    this->above = node;
}

// Creating QuadruplySkipList class
class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList(int height_in);
        ~QuadruplySkipList();
        void insert(Employee* data);
        void remove(int remove_id);
        void update(int id, int department, int salary);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);
        bool isEmpty();
        QuadruplySkipList_Node* find_node(int id);
};
// In constructor of QuadruplySkipList only head of skiplist is created.
QuadruplySkipList::QuadruplySkipList(int height_in){
    this->height = height_in; // parameter assigned in height variable
    Employee* data = new Employee(-1, 0, 0);
    this->head = new QuadruplySkipList_Node(data);
    QuadruplySkipList_Node* temp = head;
    QuadruplySkipList_Node* parent_temp = nullptr; // to assing above pointer parent node also kept
    for (int i = 0; i < this->height -1; i++) {
        temp->set_below(new QuadruplySkipList_Node(data));
        temp->set_above(parent_temp);
        parent_temp = temp;
        temp = temp->get_below();
    }
    temp->set_above(parent_temp);
}

// destructor of QuadruplySkipList; since node destructor deletes recursively to the above nodes, skiplist destructor goes to below as long as it can and then deletes next to it as long as it can and at the end it deletes head column
QuadruplySkipList::~QuadruplySkipList() {
    QuadruplySkipList_Node* temp = head;
    while(temp->get_below()){
        temp = temp->get_below();
    }
    while(temp->get_next()){
        delete temp->get_next();
    }
    delete temp;
}
// Finding head node of the column requested by id
QuadruplySkipList_Node* QuadruplySkipList::find_node(int id){
    QuadruplySkipList_Node* temp = head;
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
// in head node firstly goes to the below as flong as it can go and then checks whether there is node next to it. If there is not skiplist is empty
bool QuadruplySkipList::isEmpty() {
    QuadruplySkipList_Node* temp = head;
    for (int i = 0; i < this->height -1; i++) {
        temp = temp->get_below();
    }
    if (temp->get_next() != nullptr) {
        return false;
    } else {
        return true;
    }
}

void QuadruplySkipList::insert(Employee* data){
    clock_t start = clock();
    int random_number = 1;
    while (random_number < height && (rand() % 2) == 1) // creating a random number to assign as level of new node
        random_number++;
    QuadruplySkipList_Node* will_be_inserted_node = new QuadruplySkipList_Node(data);
    QuadruplySkipList_Node* will_be_inserted_node_head = will_be_inserted_node;
    QuadruplySkipList_Node* will_be_inserted_parent_node = nullptr;

    for (int i = 0; i < random_number -1; i++) { // Firstly, new skiplist nodes are connected with each other
        will_be_inserted_node->set_below(new QuadruplySkipList_Node(data));
        will_be_inserted_node->set_above(will_be_inserted_parent_node);
        will_be_inserted_parent_node = will_be_inserted_node;
        will_be_inserted_node = will_be_inserted_node->get_below();
    }
    will_be_inserted_node->set_above(will_be_inserted_parent_node);

    QuadruplySkipList_Node* temp = head;
    int id = data->get_id();
    for (int i = 0; i < height-random_number; i++){ // to start defining from correct level, temp goes below until the same level with new node's head
        temp = temp->get_below();
    }
    for (int j = 0; j < random_number; j++){ // At each column temp goes next to it until it finds appropriate place for new node
        while (temp->get_next() != nullptr && temp->get_next()->get_employee()->get_id() < id){
            temp = temp->get_next();
        }
        if(temp->get_next() != nullptr) {
            temp->get_next()->set_prev(will_be_inserted_node_head);
        }
        will_be_inserted_node_head->set_next(temp->get_next());
        will_be_inserted_node_head->set_prev(temp);
        temp->set_next(will_be_inserted_node_head);
        temp = temp->get_below();
        will_be_inserted_node_head = will_be_inserted_node_head->get_below();
    }
    clock_t end = clock(); // finish measure
    cout << "Add, quadrupSkiplist solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void QuadruplySkipList::remove(int remove_id) {
    clock_t start = clock();
    QuadruplySkipList_Node* temp = find_node(remove_id); // desired node found by find_node function
    while(temp->get_below()){ // since destructor deletes recursively to above it, temp firstly goes below as long as it can
        temp = temp->get_below();
    }
    delete temp; // all of new connections are done in destructor of node
    clock_t end = clock(); // finish measure
    cout << "Delete, quadrupSkiplist solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

Employee* QuadruplySkipList::search(int search_id) { // in search function after checking if skiplist is empty or not, finds the node by find_node function and returns it's employee if there is
    if (isEmpty()) {
        return nullptr;
    }
    QuadruplySkipList_Node* to_be_searched_node = this->find_node(search_id);
    if (to_be_searched_node == nullptr){
        return nullptr;
    }
    else {
        return to_be_searched_node->get_employee();
    }
}
void QuadruplySkipList::update(int id, int salary, int department){ // desired employee data found by search is being updated if it is not nullptr
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
    cout << "Update, quadrupSkiplist solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void QuadruplySkipList::dumpToFile(ofstream& out_file) { // firstly from head node goes below as much as it can and then prints each value at next nodes.
    out_file << "Employee_ID;Salary;Department\n";
    QuadruplySkipList_Node* temp = head;
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
    QuadruplySkipList* quadruply_skip_list = new QuadruplySkipList(100); // height is given as parameter
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
            quadruply_skip_list->insert(new Employee(id, salary, department));
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
            quadruply_skip_list->insert(new_employee);
            will_be_added_id++;
        }
        else if (operation_name == "DELETE") { // checking the list if it is empty or contains desired value
            if (quadruply_skip_list->isEmpty()){
                cout << "ERROR: There is no Employee\n";
            }else if (quadruply_skip_list->find_node(operation_values[0]) == nullptr) {
                cout << "ERROR: An invalid ID to delete\n";
            }else{
                quadruply_skip_list->remove(operation_values[0]);
            }
        }
        else if (operation_name == "UPDATE") {
            quadruply_skip_list->update(operation_values[0], operation_values[1], operation_values[2]);
        }
    }
    file2.close();
    ofstream outputFile;
    outputFile.open("quadruply_output.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    quadruply_skip_list->dumpToFile(outputFile); // prints the new csv file
    delete quadruply_skip_list; // deleting skiplist
    return 0;
}
