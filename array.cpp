/*
@Author
StudentName :< Abdullah Saatci >
StudentID :< 150210020 >
Date :< 09/11/2023 >
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

class Employee{ 
    private : 
        int id;
        int salary;
        int department;
    public :
        Employee();
         ~Employee();
        void set_salary (int); 
        void set_id(int);
        void set_department(int); 
        int get_id();
        int get_salary();
        int get_department();
        };

Employee::Employee() {
    salary = 0;
    id = 0;
    department = 0;
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

void add_employee(Employee**& array, int salary, int department, int* array_size, int* will_be_added_id);
void delete_employee(Employee**& array, int id, int* array_size);
void update_employee(Employee**& array, int id, int salary, int department, int array_size);
int find_employee(Employee** array, int id, int array_size);

int main(int argc, char** argv) {
    int array_size = -1;
    string filename = argv[1];
    string filename_operations = argv[2];
    string line;
    string line2;
    string line3;
    ifstream file(filename);
    int will_be_added_id;

    if (!file.is_open()) {
        cerr << "File could not be opened\n";
        return 1;
    }
    while (getline(file, line)) {
        array_size += 1;
    }
    will_be_added_id  = array_size + 1;
    file.close();
    // I create a double pointer array by the array size We learned above
    Employee** employees_array = new Employee*[array_size];
    int index_of_line = 0;
    ifstream file2(filename);
    while (getline(file2, line2)) {
        if (index_of_line != 0){
            int index_of_token = 0;
            istringstream ss(line2);
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
            employees_array[index_of_line- 1] = new Employee();
            employees_array[index_of_line- 1]->set_id(id);
            employees_array[index_of_line- 1]->set_salary(salary);
            employees_array[index_of_line- 1]->set_department(department);
        }
        index_of_line += 1;
    }
    file2.close();

    int operation_values[3];
    string operation_name;
    ifstream file3(filename_operations);
    while (getline(file3, line3)) {
        int index_of_token_operations = 0;
        istringstream ss(line3);
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
            add_employee(employees_array, operation_values[0], operation_values[1], &array_size, &will_be_added_id);
        }
        else if (operation_name == "DELETE") {
            delete_employee(employees_array, operation_values[0], &array_size);
        }
        else if (operation_name == "UPDATE") {
            update_employee(employees_array, operation_values[0], operation_values[1], operation_values[2], array_size);
        }
    }
    file3.close();
    ofstream outputFile;
    outputFile.open("array_solution.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file." << "\n";
        return 1;
    }
    outputFile << "Employee_ID;Salary;Department\n";
    for (int i = 0; i < array_size; i++) {
        outputFile << employees_array[i]->get_id() << ";" <<employees_array[i]->get_salary() << ";" << employees_array[i]->get_department() << "\n";
    }
    outputFile.close();
    // I delete all the allocated memory
    for (int i = 0; i < array_size; i++) {
        delete employees_array[i];
    }
    delete[] employees_array;
    return 0;
}
// I send array's address
void add_employee(Employee**& array, int salary, int department, int* array_size, int* will_be_added_id) {
    clock_t start = clock ();
    Employee** tmp_array = new Employee*[*array_size+1];
    for (int i = 0; i < *array_size; i++) {
        tmp_array[i] = new Employee();
        tmp_array[i]->set_id((array)[i]->get_id());
        tmp_array[i]->set_salary((array)[i]->get_salary());
        tmp_array[i]->set_department((array)[i]->get_department());
    }
    tmp_array[*array_size] = new Employee();
    tmp_array[*array_size]->set_id(*will_be_added_id);
    tmp_array[*array_size]->set_salary(salary);
    tmp_array[*array_size]->set_department(department);
    // I delete all the allocated memory
    for (int i = 0; i < *array_size; i++) {
        delete (array)[i];
    }
    delete[] array;
    *array_size += 1;
    *will_be_added_id += 1;
    array = tmp_array;
    clock_t end = clock();
    cout<< "Add, Array solution: "  << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}
void update_employee(Employee**& array, int id, int salary, int department, int array_size) {
    clock_t start = clock ();
    int index_of_id = find_employee((array), id, array_size);
    if (index_of_id != -1) {
        (array)[index_of_id]->set_salary(salary);
        (array)[index_of_id]->set_department(department);
    }
    else {
        cout << "ERROR: An invalid ID to update";
    }
    clock_t end = clock();
    cout<< "Update, Array solution: "  << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}
void delete_employee(Employee**& array, int id, int* array_size) {
    clock_t start = clock ();
    Employee** tmp_array = new Employee*[*array_size-1];
    int flag = 0;
    int index_of_id = find_employee((array), id, *array_size);
    if (index_of_id != -1) {
        for (int i = 0; i < *array_size-1; i++) {
            if (i != index_of_id && flag == 0) {
                tmp_array[i] = new Employee();
                tmp_array[i]->set_id((array)[i]->get_id());
                tmp_array[i]->set_salary((array)[i]->get_salary());
                tmp_array[i]->set_department((array)[i]->get_department());
            }
            else {
                flag = 1;
                tmp_array[i] = new Employee();
                tmp_array[i]->set_id((array)[i+1]->get_id());
                tmp_array[i]->set_salary((array)[i+1]->get_salary());
                tmp_array[i]->set_department((array)[i+1]->get_department());
            }
        }
        // I delete all the allocated memory
        for (int i = 0; i < *array_size; i++) {
            delete (array)[i];
        }
        delete[] array;
        *array_size -= 1;
        array = tmp_array;
    }
    else if (array_size == 0) {
        cout << "ERROR: There is no Employee";
    }
    else {
        cout << "ERROR: An invalid ID to delete";
    }
    clock_t end = clock();
    cout << "Delete, Array solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
}
int find_employee(Employee** array, int id, int array_size) {
    for (int i = 0; i < array_size; i++) {
        if (array[i]->get_id() == id) {
            return i;
        }
    }
    return -1;
}