/*
@Author
StudentName :< Abdullah Saatci >
StudentID :< 150210020 >
Date :< 18/12/2023 >
*/

// At first libraries are included
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
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
// Create Node class
class Node{
    private:
        Employee *employee; 
        Node *left;
        Node *right;
    public:
        Node(Employee *employee);
        ~Node();
        void set_employee(Employee *employee);
        void set_left(Node* new_node);
        void set_right(Node* new_node);
        Employee* get_employee();
        Node* get_left();
        Node* get_right();
};
Node::Node(Employee *employee) {
    this-> employee = employee;
    this->left = NULL;
    this->right = NULL;
}
// Each node has a destructor that deletes its employee when node is deleted itself.
Node::~Node() {
    if (this->get_employee() != NULL) {
        delete this->get_employee();
    }
}
void Node::set_employee(Employee *employee){
    this->employee = employee;
}
void Node::set_left(Node* new_node){
    this->left = new_node;
}
void Node::set_right(Node* new_node){
    this->right = new_node;
}
Employee* Node::get_employee() {
    return this->employee;
}
Node* Node::get_left(){
    return this->left;
}
Node* Node::get_right(){
    return this->right;
}
// Create BinaryTree class which will hold all nodes
class BinaryTree{
    private:
        Node* root;
    public:
        BinaryTree();
        ~BinaryTree();
        void insert(Employee* employee); 
        void remove(int id);
        Employee* search(int id);
        void printToFile(ofstream &output_file);
        Node* find_parent(int id);
        void update(int id, int s, int d);
        Node* get_root();
};
BinaryTree::BinaryTree() {
    this->root = NULL;
}
// when BinaryTree's destructor is called it creates a queue to hold nodes. At first root is pushed in it and then until queue gets empty it will pop the front element and then it will add front element's left and right child if there exist. Therefore, it will delete all the nodes at the end
BinaryTree::~BinaryTree() {
    if (root != NULL){
        queue<Node*> queue_to_delete;
        queue_to_delete.push(root);
        while (!queue_to_delete.empty()) {
            Node* node_to_delete = queue_to_delete.front();
            queue_to_delete.pop();
            if (node_to_delete->get_left())
                queue_to_delete.push(node_to_delete->get_left());
            if (node_to_delete->get_right())
                queue_to_delete.push(node_to_delete->get_right());
            delete node_to_delete;
        }
    }
}
// according to Employee element's id we will find where to add it. Starting from root, if given employee id is larger it goes right down if lower goes left down and so it will find a parent node for itself.
void BinaryTree::insert(Employee* employee){
    clock_t start = clock();
    Node* new_node = new Node(employee);
    if (this->root == NULL) {
        this->root = new_node;
    }
    else {
        Node* temp = this->root;
        while (true) {
            if (employee->get_id() > temp->get_employee()->get_id()) {
                if (temp->get_right() != NULL) {
                    temp = temp->get_right();
                }
                else{
                    temp->set_right(new_node);
                    break;
                }
            }
            else if (employee->get_id() < temp->get_employee()->get_id()){
                if (temp->get_left() != NULL) {
                    temp = temp->get_left();
                }
                else{
                    temp->set_left(new_node);
                    break;
                }
            } else {
                break;
            }
        }
    }
    clock_t end = clock(); // finish measure
    cout << "Add, hybrid solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}
// if id is larger than roots or current node's employee id it will go right if not it will go left. At the end it will take the parent node  
Node* BinaryTree::find_parent(int id) {
    if (this->root != NULL){
        Node* temp = this->root;
        Node* parent = NULL;
        while (temp != NULL) {
            if (id > temp->get_employee()->get_id()) {
                parent = temp;
                temp = temp->get_right();
            } else if (id < temp->get_employee()->get_id()) {
                parent = temp;
                temp = temp->get_left();
            } else if (id == temp->get_employee()->get_id()){
                return parent;
            } 
        }
        return NULL;
    }
    return NULL;
}
// Since there are lots of adge cases let's go step by step
void BinaryTree::remove(int id) {
    clock_t start = clock();
    if (this->root != NULL){ // Chech if root is NULL
        Node* parent = this->find_parent(id);
        if (parent != NULL) { // Check if parent is null, if it is not null there is an employee which has that id
            if (parent->get_right() != NULL && parent->get_right()->get_employee()->get_id() == id) {// Check if the employee is at right child of parent
                if (parent->get_right()->get_right() == NULL && parent->get_right()->get_left() == NULL) {// If the node we want to delete has no children just delete it.
                    delete parent->get_right();
                    parent->set_right(NULL);
                } else if (parent->get_right()->get_right() != NULL && parent->get_right()->get_left() == NULL) { // If the node we want to delete has only one child and the child is at right, connect parent with the node's child.
                    Node* temp = parent->get_right();
                    parent->set_right(temp->get_right());
                    temp->set_right(NULL);
                    delete temp;
                } else if (parent->get_right()->get_right() == NULL && parent->get_right()->get_left() != NULL) {// If the node we want to delete has only one child and the child is at left, connect parent with the node's child. 
                    Node* temp = parent->get_right();
                    parent->set_right(temp->get_left());
                    temp->set_left(NULL);
                    delete temp;
                } else if (parent->get_right()->get_right() != NULL && parent->get_right()->get_left() != NULL) { // If the node we want to delete has two children go to the node's right once and then go left of it as much as possible then replace that node with the node we want to delete but firstly connect parent of the last node have found with its children if there exist.
                    Node* second_parent = parent->get_right();
                    Node* temp = second_parent->get_right();
                    while (temp->get_left() != NULL) {
                        second_parent = temp;
                        temp = temp->get_left();
                    }
                    if (second_parent == parent->get_right()) { // this is another edge case, because if there was never an iteration for while loop above, second parent itself the node we want to delete
                        temp->set_left(second_parent->get_left());
                        parent->set_right(temp);
                        second_parent->set_right(NULL);
                        second_parent->set_left(NULL);
                        delete second_parent;
                    }
                    else {
                        Node* delete_temp = parent->get_right();
                        second_parent->set_left(temp->get_right());
                        parent->set_right(temp);
                        temp->set_left(delete_temp->get_left());
                        temp->set_right(delete_temp->get_right());
                        delete_temp->set_right(NULL);
                        delete_temp->set_left(NULL);
                        delete delete_temp;
                    }
                }
            }   else if (parent->get_left() != NULL && parent->get_left()->get_employee()->get_id() == id) { // Check if the employee is at left child of parent
                    if (parent->get_left()->get_right() == NULL && parent->get_left()->get_left() == NULL) {// If the node we want to delete has no children just delete it.
                        delete parent->get_left();
                        parent->set_left(NULL);
                }   else if (parent->get_left()->get_right() != NULL && parent->get_left()->get_left() == NULL) { // If the node we want to delete has only one child and the child is at right, connect parent with the node's child.
                        Node* temp = parent->get_left();
                        parent->set_left(temp->get_right());
                        temp->set_right(NULL);
                        delete temp;
                    } else if (parent->get_left()->get_right() == NULL && parent->get_left()->get_left() != NULL) {// If the node we want to delete has only one child and the child is at left, connect parent with the node's child. 
                        Node* temp = parent->get_left();
                        parent->set_left(temp->get_left());
                        temp->set_left(NULL);
                        delete temp;
                    } else if (parent->get_left()->get_right() != NULL && parent->get_left()->get_left() != NULL) {// If the node we want to delete has two children go to the node's right once and then go left of it as much as possible then replace that node with the node we want to delete but firstly connect parent of the last node have found with its children if there exist.
                        Node* second_parent = parent->get_left();
                        Node* temp = second_parent->get_right();
                        while (temp->get_left() != NULL) {
                            second_parent = temp;
                            temp = temp->get_left();
                        }
                        if (second_parent == parent->get_left()) {// this is another edge case, because if there was never an iteration for while loop above, second parent itself the node we want to delete
                            temp->set_left(second_parent->get_left());
                            parent->set_left(temp);
                            second_parent->set_left(NULL);
                            second_parent->set_right(NULL);
                            delete second_parent;
                        }
                        else {
                            Node* delete_temp = parent->get_left();
                            second_parent->set_left(temp->get_right());
                            parent->set_left(temp);
                            temp->set_left(delete_temp->get_left());
                            temp->set_right(delete_temp->get_right());
                            delete_temp->set_right(NULL);
                            delete_temp->set_left(NULL);
                            delete delete_temp;
                        }
                    }
            }
        }
        else if (this->root->get_employee()->get_id() == id){// If parent is empty there might be two reasons: firstly id might be root employee's id, secondly there is no employee with that id.
            if (this->root->get_right() == NULL && this->root->get_left() == NULL) {//If the root has no children just delete it.
                delete this->root;
                this->root = NULL;
            } else if (this->root->get_right() != NULL && this->root->get_left() == NULL) {// If the root has only one child and the child is at right, let root's right child be the new root.
                Node* temp = this->root;
                this->root = this->root->get_right();
                temp->set_right(NULL);
                delete temp;
            } else if (this->root->get_right() == NULL && this->root->get_left() != NULL) {// If the root has only one child and the child is at left, let root's left child be the new root.
                Node* temp = this->root;
                this->root = this->root->get_left();
                temp->set_left(NULL);
                delete temp;
            } else if (this->root->get_right() != NULL && this->root->get_left() != NULL) {// If root has two children go right ones and then go left as much as possible. after that make the node you obtained the new root.
                Node* second_parent = this->root;
                Node* temp = second_parent->get_right();
                while (temp->get_left() != NULL) {
                    second_parent = temp;
                    temp = temp->get_left();
                }
                if (temp->get_right() != NULL) {
                    second_parent->set_left(temp->get_right());
                }
                else {
                    second_parent->set_left(NULL);
                }
                Node* delete_temp = this->root;
                this->root = temp;
                this->root->set_left(delete_temp->get_left());
                this->root->set_right(delete_temp->get_right());
                delete_temp->set_right(NULL);
                delete_temp->set_left(NULL);
                delete delete_temp;
            }
        } else {
            cout << "ERROR: An invalid ID to delete\n";
        }
    }
    clock_t end = clock(); // finish measure
    cout << "Delete, hybrid solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}
// For printing to file a stack is created, at first it keeps a node of root and then in a while loop it creates another while loop to push current elemet to stack and go left as much as possible when inner while loop ends it assigns current node with stack's top and pop the stack once. by this way it is able to print all the elements in a line from lower to greater and at the end it goes riht once to do the same for that line of nodes.
void BinaryTree::printToFile(ofstream &output_file) {
    if (this->root != NULL) {
        stack<Node*> file_node_stack;
        Node* curr_node = this->root;
        while (curr_node != NULL || !file_node_stack.empty()) {
            while (curr_node != NULL) {
                file_node_stack.push(curr_node);
                curr_node = curr_node->get_left();
            }
            curr_node = file_node_stack.top();
            file_node_stack.pop();
            output_file << curr_node->get_employee()->get_id() << ";" << curr_node->get_employee()->get_salary() << ";" << curr_node->get_employee()->get_department() << "\n";
            curr_node = curr_node->get_right();
        }
    }
}
Employee* BinaryTree::search(int id) {// in this function the employee with the given id is trying to be founded by find_parent function. if parent null we should check root too.
    Node* parent = this->find_parent(id);
    if (parent != NULL) {
        if (parent->get_left() != NULL && parent->get_left()->get_employee()->get_id() == id) {
            return parent->get_left()->get_employee();
        } else if (parent->get_right() != NULL && parent->get_right()->get_employee()->get_id() == id) {
            return parent->get_right()->get_employee();
        }
    } else if (parent == NULL && this->root != NULL) {
        return this->root->get_employee();
    }
    return NULL;
}

void BinaryTree::update(int id, int s, int d) {// update employee with search fuction's returned value.
    clock_t start = clock();
    Employee* to_be_changed_employee = this->search(id);
    if (to_be_changed_employee != NULL) {
        to_be_changed_employee->set_department(d);
        to_be_changed_employee->set_salary(s);
    }
    else {
        cout << "ERROR: An invalid ID to update\n";
    }
    clock_t end = clock(); // finish measure
    cout << "Update, hybrid solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

Node* BinaryTree::get_root() {
    return this->root;
}

void add_emp(vector <BinaryTree*> &tree_nodes, Employee* emp) {
    int index_counter = -1;
    // We find the tree in vector which has the range of given id.
    for (uint i = 0; i < tree_nodes.size(); i++) {
        if ((emp->get_id()-1) / 5000 == (tree_nodes[i]->get_root()->get_employee()->get_id()-1) / 5000) {
            index_counter = i;
            break;
        }
    }
    if (index_counter != -1) {
        tree_nodes[index_counter]->insert(emp);
    }
    else { // if there is not such tree we create one
        BinaryTree* new_tree = new BinaryTree();
        tree_nodes.push_back(new_tree);
        new_tree->insert(emp);
    }
}

void delete_emp(vector <BinaryTree*> &tree_nodes, int id) { // We find the tree in vector which has the range of given id and change the index_counter so that it will hold vector's desired index. at the beginning is it assigned by -1 since there cannot be any value like that
    int index_counter = -1;
    if (tree_nodes.size() == 0) {
        cout << "ERROR: There is no Employee\n";
    }
    for (uint i = 0; i < tree_nodes.size(); i++) {
        if ((id-1) / 5000 == (tree_nodes[i]->get_root()->get_employee()->get_id()-1) / 5000) {
            index_counter = i;
            break;
        }
    }
    if (index_counter != -1) {
        tree_nodes[index_counter]->remove(id);
    }
    else {
        cout << "ERROR: An invalid ID to delete\n";
    }
}

void update_emp(vector <BinaryTree*> &tree_nodes, int id, int s, int d) { // // We find the tree in vector which has the range of given id and change the index_counter so that it will hold vector's desired index. at the beginning is it assigned by -1 since there cannot be any value like that. Update the value of given id's employee informations at that binary tree.
    int index_counter = -1;
    for (uint i = 0; i < tree_nodes.size(); i++) {
        if ((id-1) / 5000 == (tree_nodes[i]->get_root()->get_employee()->get_id()-1) / 5000) {
            index_counter = i;
            break;
        }
    }
    if (index_counter != -1) {
        tree_nodes[index_counter]->update(id, s, d);
    }
    else{
        cout << "ERROR: An invalid ID to update\n";
    }
}

void print_to_file(vector <BinaryTree*> &tree_nodes, ofstream &output_file) { // print each binary tree in a sorted way
    for (uint i = 0; i < tree_nodes.size(); i++) {
        for (uint j = 0; j < tree_nodes.size(); j++) {
            if (static_cast<uint>((tree_nodes[j]->get_root()->get_employee()->get_id() - 1) / 5000) == i) {
                tree_nodes[j]->printToFile(output_file);
                j = tree_nodes.size();
            }
        }
    }
}

int main(int agrc, char** argv) {
    vector <BinaryTree*> tree_nodes;
    int list_size = 0;
    string filename = argv[1];
    string filename_operations = argv[2];
    string line;
    string line2;
    ifstream file(filename);
    int will_be_added_id;

    if (!file.is_open()) {
        cerr << "File could not be opened\n";
        return 1;
    }
    while (getline(file, line)) {// by list_size veriable we skip first line, and then seperate all lines by ; 
        if (list_size != 0) {
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
            Employee* new_emp = new Employee(id, salary, department);
            add_emp(tree_nodes, new_emp);
        }
        list_size ++;
    }
    will_be_added_id = list_size;
    file.close();

    int operation_values[3];
    string operation_name;
    ifstream file2(filename_operations);
    while (getline(file2, line2)) { // travel each line of operation file
        bool delete_employee_flag = false;
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
        if (operation_values[0] == 15001) {
            cout << "here" << endl;
        }
        if (operation_name == "ADD") {
            Employee* new_employee = new Employee(will_be_added_id, operation_values[0],operation_values[1]);
            add_emp(tree_nodes, new_employee);
            will_be_added_id++;
        }
        else if (operation_name == "DELETE") { // check if there is any employee, if there is call delete_emp function
            for (uint i = 0; i < tree_nodes.size(); i++) {
                if (tree_nodes[i]->get_root() != NULL) {
                    delete_employee_flag = true;
                    break;
                }
            }
            if (delete_employee_flag) {
                delete_emp(tree_nodes, operation_values[0]);
            }
            else {
                cout << "ERROR: There is no Employee\n";
            }
        }
        else if (operation_name == "UPDATE") {
            update_emp(tree_nodes, operation_values[0], operation_values[1], operation_values[2]);
        }
    }
    file2.close();
    ofstream outputFile;
            outputFile.open("output.csv");
            if (!outputFile.is_open()) {
                cerr << "Failed to open the file." << "\n";
                return 1;
            }
    outputFile << "Employee_ID;Salary;Department\n";
    print_to_file(tree_nodes, outputFile);
    for (uint i = 0; i < tree_nodes.size(); i++) {
        delete tree_nodes[i];        
    }
    tree_nodes.clear();
    return 0;
}