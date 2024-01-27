#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <time.h>

using namespace std;

class Employee{ 
    private : 
        int id;
        int salary;
        int department;
    public :
        Employee(int, int, int);
         ~Employee();
        void set_salary (int); 
        void set_id(int);
        void set_department(int); 
        int get_id();
        int get_salary();
        int get_department();
        };

Employee::Employee(int id, int salary, int department) {
    this->id = id;
    this->salary = salary;
    this->department = department;
}
Employee::~Employee() {
}
void Employee::set_salary (int salary) {
    this->salary = salary;
}
void Employee::set_id(int id) {
    this -> id = id;
}
void Employee::set_department(int department) {
    this -> department = department;
} 
int Employee::get_id() {
    return id;
}
int Employee::get_salary() {
    return salary;
}
int Employee::get_department() {
    return department;
}

int search(list<Employee>& tmp_list, int targetID) {
    int index = 0;
    for (list<Employee>::iterator it = tmp_list.begin(); it != tmp_list.end(); ++it) {
        if (it->get_id() == targetID) {
            return index;
        }
        index++;
    }
    return -1;
}

// Adding id to the end of list
void add_employee(list<Employee>& tmp_list, int* will_be_added_id, int salary, int department, int* size) {
    clock_t start = clock ();
    tmp_list.push_back(Employee(*will_be_added_id, salary, department));
    *will_be_added_id += 1;
    *size += 1;
    clock_t end = clock();
    cout << "Add, list solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

// Erasing an index which contains the given id
void erase_employee(list<Employee>& tmp_list, int id, int* size) {
    clock_t start = clock ();
    if (*size == 0) {
        cout << "ERROR: There is no Employee";
    }
    else {
        int index = search(tmp_list, id);
        if (index >= 0 && index < *size) {
            list<Employee>::iterator beginning = tmp_list.begin();
            advance(beginning, index);
            tmp_list.erase(beginning);
            *size -= 1;
        } else { 
            cout << "ERROR: An invalid ID to delete";
        }
    }
    clock_t end = clock();
    cout << "Delete, list solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

// Update employee information by searching for the id
void update_employee(list<Employee>& tmp_list, int id, int salary, int department, int* size) {
    clock_t start = clock ();
    int index = search(tmp_list, id);
    if (index >= 0 && index < *size) {
        list<Employee>::iterator it = tmp_list.begin();
        advance(it, index);
        it->set_salary(salary);
        it->set_department(department);
    } else { 
        cout << "ERROR: An invalid ID to update";
    }
    clock_t end = clock();
    cout << "Update, list solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

int main(int agrc, char** argv) {
    list <Employee> employees;
    int list_size = 0;
    string filename = argv[1];
    // string filename = "hw2.csv";
    string filename_operations = argv[2];
    // string filename_operations = "operations.txt";
    string line;
    string line2;
    ifstream file(filename);
    int will_be_added_id;

    if (!file.is_open()) {
        cerr << "File could not be opened\n";
        return 1;
    }
    // At first I add the informations to list
    while (getline(file, line)) {
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
            employees.push_back(Employee(id, salary, department));
        }
        list_size += 1;
    }
    will_be_added_id  = list_size--;
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
            add_employee(employees, &will_be_added_id, operation_values[0], operation_values[1], &list_size);
        }
        else if (operation_name == "DELETE") {
            erase_employee(employees, operation_values[0], &list_size);
        }
        else if (operation_name == "UPDATE") {
            update_employee(employees, operation_values[0], operation_values[1], operation_values[2], &list_size);
        }
    }
    file2.close();

    ofstream outputFile;
    outputFile.open("list_solution.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    outputFile << "Employee_ID;Salary;Department\n";
    for (list<Employee>::iterator it = employees.begin(); it != employees.end(); ++it) {
        outputFile << it->get_id() << ";" << it->get_salary() << ";" << it->get_department() << "\n";
    }
    outputFile.close();
    employees.clear();

    return 0;
}