/*
@Author
StudentName :< Abdullah Saatci >
StudentID :< 150210020 >
Date :< 18/12/2023 >
*/

// At first libraries are included
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

void insert(map<int, Employee*> &employee_map, Employee* emp) {
    clock_t start = clock();
    employee_map[emp->get_id()] = emp;
    clock_t end = clock(); // finish measure
    cout << "Add, map solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void remove(map<int, Employee*> &employee_map, int id) { // after erasing employee is also deleted.
    clock_t start = clock();
    if (employee_map.size() != 0) {
        if (employee_map[id] != NULL) {
            delete employee_map[id];
            employee_map.erase(id);
        }
        else {
            cout << "ERROR: An invalid ID to delete\n";
        }
    }
    else {
        cout << "ERROR: There is no Employee\n";
    }
    clock_t end = clock(); // finish measure
    cout << "Delete, map solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void update(map<int, Employee*> &employee_map, int id, int s, int d) {
    clock_t start = clock();
    if (employee_map[id] != NULL){
        employee_map[id]->set_salary(s);
        employee_map[id]->set_department(d);
    }
    else {
        cout << "ERROR: An invalid ID to update\n";
    }
    clock_t end = clock(); // finish measure
    cout << "Update, map solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

void print_to_file(ofstream &output_file, map<int, Employee*> &employee_map) { // we iterate each employee in map
    output_file << "Employee_ID;Salary;Department\n";
    map<int, Employee*>::iterator it = employee_map.begin();
    while (it != employee_map.end()) {
        const pair<const int, Employee*>& pair = *it;
        if (pair.second != nullptr) {
            output_file << pair.second->get_id() << ";" << pair.second->get_salary() << ";" << pair.second->get_department() << "\n";
        }
        ++it;
    }
    output_file.close();
}

int main(int agrc, char** argv) {
    map<int, Employee*> employee_map;
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
            Employee* employee = new Employee(id, salary, department);
            insert(employee_map, employee);
        }
        list_size ++;
    }
    will_be_added_id = list_size;
    file.close();
    int operation_values[3];
    string operation_name;
    ifstream file2(filename_operations);
    while (getline(file2, line2)) { // travel each line of operation file
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
            insert(employee_map, new_employee);
            will_be_added_id++;
        }
        else if (operation_name == "DELETE") {
            remove(employee_map, operation_values[0]);
        }
        else if (operation_name == "UPDATE") {
            update(employee_map, operation_values[0], operation_values[1], operation_values[2]);
        }
    }
    file2.close();
    ofstream outputFile;
    outputFile.open("output.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    print_to_file(outputFile, employee_map);
    // All of the employees are by iterating each index of map. at the end cleared map by may.clear() function.
    map<int, Employee*>::iterator it = employee_map.begin();
    while (it != employee_map.end()) {
        const pair<const int, Employee*>& pair = *it;
        if (pair.second != nullptr) {
            delete pair.second;
        }
        ++it;
    }
    employee_map.clear();
    return 0;
}
