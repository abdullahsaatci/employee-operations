#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

int search(vector<Employee>& vec, int data, int vector_size) {
    for (int i = 0; i < vector_size; i++) {
        if (vec[i].get_id() == data) {
            return i;
        }
    }
    
    return -1;
}

// Adding id to the end of vector
void add_employee(vector<Employee>& vec, int* will_be_added_id, int salary, int department, int* size) {
    clock_t start = clock ();
    vec.push_back(Employee(*will_be_added_id, salary, department));
    *will_be_added_id += 1;
    *size += 1;
    clock_t end = clock();
    cout << "Add, vector solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

// Erasing an index which contains the given id
void erase_employee(vector<Employee>& vec, int id, int* size) {
    clock_t start = clock ();
    if (*size == 0) {
        cout << "ERROR: There is no Employee";
    }
    else {
        int index = search(vec, id, *size);
        if (index >= 0 && index < *size) {
            vec.erase(vec.begin() + index);
            *size -= 1;
        } else { 
            cout << "ERROR: An invalid ID to delete";
        }
    }
    clock_t end = clock();
    cout << "Delete, vector solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

// Update employee information by searching for the id
void update_employee(vector<Employee>& vec, int id, int salary, int department, int* size) {
    clock_t start = clock ();
    int index = search(vec, id, *size);
    if (index >= 0 && index < *size) {
        vec[index].set_salary(salary);
        vec[index].set_department(department);
    } else { 
        cout << "ERROR: An invalid ID to update";
    }
    clock_t end = clock();
    cout << "Update, vector solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}

int main(int agrc, char** argv) {
    vector <Employee> employees;
    int vector_size = 0;
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
    // At first I add the informations to vector
    while (getline(file, line)) {
        if (vector_size != 0) {
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
        vector_size += 1;
    }
    will_be_added_id  = vector_size--;
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
            add_employee(employees, &will_be_added_id, operation_values[0], operation_values[1], &vector_size);
        }
        else if (operation_name == "DELETE") {
            erase_employee(employees, operation_values[0], &vector_size);
        }
        else if (operation_name == "UPDATE") {
            update_employee(employees, operation_values[0], operation_values[1], operation_values[2], &vector_size);
        }
    }
    file2.close();

    ofstream outputFile;
    outputFile.open("vector_solution.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    outputFile << "Employee_ID;Salary;Department\n";
    for (int i = 0; i < vector_size; i++) {
        outputFile << employees[i].get_id() << ";" << employees[i].get_salary() << ";" << employees[i].get_department() << "\n";
    }
    outputFile.close();
    employees.clear();
    employees.shrink_to_fit();

    return 0;
}