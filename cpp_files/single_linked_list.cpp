#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

class Node{
    private:
        int id;
        int salary;
        int department;
        Node* next;
    public:
        Node(int, int, int);
        ~Node();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
        int get_id();
        int get_salary();
        int get_department();
        void set_next(Node*);
        Node* get_next();
};

Node::Node(int id, int salary, int department) {
    this->id = id;
    this->salary = salary;
    this->department = department;
    this->next = NULL;
}

Node::~Node() {
}

void Node::set_id(int data) {
    this->id = data;
}

int Node::get_id(){
    return this->id;
}

void Node::set_salary(int data) {
    this->salary = data;
}

int Node::get_salary(){
    return this->salary;
}
void Node::set_department(int data) {
    this->department = data;
}

int Node::get_department(){
    return this->department;
}

void Node::set_next(Node* next) {
    this->next = next;
}

Node* Node::get_next() {
    return this->next;
}

class SingleLinkedList{
    private:
        Node* head;
        Node* tail;
    public: 
        SingleLinkedList();
        ~SingleLinkedList();
        void add_end(int, int, int);
        Node* search(int data);
        void delete_node(int);
        void update_node(int, int, int);
        Node* get_tail();
        Node* get_head();
}; 

SingleLinkedList::SingleLinkedList(){
    this->head = NULL;
    this->tail = NULL;  
}

SingleLinkedList::~SingleLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->get_next();
        delete current;
        current = next;
    }
}

Node* SingleLinkedList::get_tail() {
    return this->tail;
}

Node* SingleLinkedList::get_head() {
    return this->head;
}

void SingleLinkedList::delete_node(int data) {
    clock_t start = clock ();
    // cout << "at" << endl;
    Node* previous_node = this->search(data);
    cout << "prev" << previous_node->get_next()->get_id() << endl;
    Node* to_delete = previous_node->get_next();
    previous_node->set_next(to_delete->get_next());
    to_delete->set_next(nullptr);
    delete to_delete;
    // if (head == tail && head->get_id() == data) {
    //     delete head;
    //     head = NULL;
    //     tail = NULL;
    // }
    // else if (previous_node == NULL && previous_node->get_next()->get_id() == data) {
    //     Node* old_head = head;
    //     this->head = this->head->get_next();
    //     old_head->set_next(NULL);
    //     delete old_head;
    // }
    // else if (previous_node->get_next() == tail && previous_node->get_next()->get_id() == data) {
    //     tail = previous_node;
    //     delete previous_node->get_next();
    //     tail->set_next(NULL);
    // }
    // else {
    //     if (previous_node->get_next()->get_id() == data) {
    //         Node* will_be_deleted = previous_node->get_next();
    //         previous_node->set_next(previous_node->get_next()->get_next());
    //         will_be_deleted->set_next(NULL);
    //         delete will_be_deleted;
    //     }
    //     else {
    //         cout << "ERROR: An invalid ID to delete";
    //     }
    // }
    clock_t end = clock();
    cout << "Delete, linkedList solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

void SingleLinkedList::update_node(int id, int salary, int department) {
    clock_t start = clock ();
    if (this->search(id)->get_next() && this->search(id)->get_next() != this->get_tail()) {
        cout << this->search(id)->get_next()->get_id() << endl;
        Node* will_be_updated = this->search(id)->get_next();
        will_be_updated->set_salary(salary);
        will_be_updated->set_department(department);
    }
    else {
        cout << "ERROR: An invalid ID to update";
    }
    clock_t end = clock();
    cout << "Update, linkedList solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

Node* SingleLinkedList::search(int data) {
    Node* p = this->head;
    // while (p->get_next()) {
    //     if (p->get_next()->get_id() >= data) {
    //         return p;
    //     }
    //     p = p->get_next();
    // }
    // return p;
    while (p && p->get_next()->get_id() != data) {
        p = p->get_next();
    }
    return p;
}

void SingleLinkedList::add_end(int id, int salary, int department) {
    clock_t start = clock ();
    Node* new_node = new Node(id, salary, department);
    if (this->head == NULL) {
        this->head = new_node;
        this->tail = new_node;
    }
    else {
        tail->set_next(new_node);
        tail = tail->get_next();
    }
    clock_t end = clock();
    cout << "Add, linkedList solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

int main(int agrc, char** argv) {
    SingleLinkedList* single_linked_list = new SingleLinkedList();
    int linked_list_size = 0;
    string filename = argv[1];
    // string filename = "hw2.csv";
    string filename_operations = argv[2];
    // string filename_operations = "deneme.txt";
    string line;
    string line2;
    ifstream file(filename);
    int will_be_added_id;

    if (!file.is_open()) {
        cerr << "File could not be opened\n";
        return 1;
    }
    while (getline(file, line)) {
        if (linked_list_size != 0) {
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
            single_linked_list->add_end(id, salary, department);
        }
        linked_list_size += 1;
    }
    will_be_added_id  = linked_list_size;
    file.close();

    int operation_values[3];
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
            single_linked_list->add_end(will_be_added_id, operation_values[0],operation_values[1]);
            will_be_added_id++;
        }
        else if (operation_name == "DELETE") {
            single_linked_list->delete_node(operation_values[0]);
        }
        else if (operation_name == "UPDATE") {
            single_linked_list->update_node(operation_values[0], operation_values[1], operation_values[2]);
        }
    }
    file2.close();

    ofstream outputFile;
    outputFile.open("linked_list_solution.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    outputFile << "Employee_ID;Salary;Department\n";
    Node* will_be_written_node = single_linked_list->get_head();
    while (will_be_written_node != NULL) {
        outputFile << will_be_written_node->get_id() << ";" << will_be_written_node->get_salary() << ";" << will_be_written_node->get_department() << "\n";
        will_be_written_node = will_be_written_node->get_next();
    }
    outputFile.close();
    delete single_linked_list;
    return 0;
}   