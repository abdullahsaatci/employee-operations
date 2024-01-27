/*
@Author
StudentName :< Abdullah Saatci >
StudentID :< 150210020 >
Date :< 29/10/2023 >
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

int main(int agrc, char** argv) {
    // string filename = argv[1];
    string filename_operations = argv[2];
    string filename_main = argv[1];
    string line;
    string line_size;
    string line_for_temp;
    string line_for_main;
    int will_be_added_id = 0;
    ifstream file_size(filename_main);
    string operation_name;
    ifstream file(filename_operations);
    int size_of_file = 0;
    int operation_add_salary = 0;
    int operation_add_department = 0;
    int operation_delete_id = 0;
    int operation_update_id = 0;
    int operation_update_salary = 0;
    int operation_update_department = 0;
    while (getline(file_size, line_size)) {
        will_be_added_id += 1;
    }
    size_of_file = will_be_added_id -1;
    file_size.close();
    // at first I take a line from operations file and then I seperate the indexes by ; of the line and according to that i change main file
    while (getline(file, line)) {
        int index_of_token_operations = 0;
        ifstream file_main(filename_main);
        ofstream tempFile("tempfile.csv");
        istringstream ss(line);
        string token_in_operations;
        int index_of_line = 0;
        bool update_flag = false;
        while (getline(ss, token_in_operations, ';')) {
            // bool deletion_flag = false;
            if (index_of_token_operations == 0) {
                operation_name = token_in_operations;
            } 
            else if (index_of_token_operations == 1 && operation_name == "ADD"){
                operation_add_salary = stoi(token_in_operations);
            }
            else if (index_of_token_operations == 2 && operation_name == "ADD"){
                operation_add_department = stoi(token_in_operations);
            }
            else if (index_of_token_operations == 1 && operation_name == "DELETE"){
                operation_delete_id = stoi(token_in_operations);
            }
            else if (index_of_token_operations == 1 && operation_name == "UPDATE"){
                operation_update_id = stoi(token_in_operations);
            }
            else if (index_of_token_operations == 2 && operation_name == "UPDATE"){
                operation_update_salary = stoi(token_in_operations);
            }
            else if (index_of_token_operations == 3 && operation_name == "UPDATE"){
                operation_update_department = stoi(token_in_operations);
            }
            index_of_token_operations++;
        }

        while (getline(file_main, line_for_main)) {
            if (index_of_line != 0){
                int index_of_token = 0;
                istringstream ss(line_for_main);
                string token_in_lines;
                int id;
                while (getline(ss, token_in_lines, ';')) {
                    if (index_of_token == 0) {
                        index_of_token++;
                        // cout << token_in_lines << "\n";
                        id = stoi(token_in_lines);
                        if (operation_name == "ADD") {
                            clock_t start = clock();
                            tempFile << line_for_main << "\n";
                            if (index_of_line == size_of_file) {
                                tempFile << will_be_added_id << ";" << operation_add_salary << ";" << operation_add_department << "\n";
                                will_be_added_id++;
                                size_of_file++;
                                // cout << "ADDED: " << will_be_added_id - 1 << "\n";
                            }
                            clock_t end = clock(); // finish measure
                            cout << "Add, file_io solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
                        }
                        else if (operation_name == "DELETE") {
                            clock_t start = clock();
                            if (operation_delete_id == id) {
                                // deletion_flag = true;
                                size_of_file--;
                                // cout << "DELETED: " << id << "\n";
                                continue;
                            }
                            else {
                                tempFile << line_for_main << "\n";
                            }
                            clock_t end = clock(); // finish measure
                            cout << "Delete, file_io solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
                        }
                        else if (operation_name == "UPDATE") {
                            clock_t start = clock();
                            if (operation_update_id == id) {
                                update_flag = true;
                                tempFile << operation_update_id << ";" << operation_update_salary << ";" << operation_update_department << "\n";
                                // cout << "UPDATED: " << id  << "\n";
                            }
                            else if (index_of_line == size_of_file && !update_flag) {
                                cout << "ERROR: An invalid ID to update";
                                tempFile << line_for_main << "\n";
                            }
                            else {
                                tempFile << line_for_main << "\n";
                            }
                            clock_t end = clock(); // finish measure
                            cout << "Update, file_io solution: " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
                        }
                    }
                }
            }
            else {
                tempFile << line_for_main << "\n";
            }
            index_of_line += 1;
        }
        tempFile.close();
        file_main.close();

        if (remove(filename_main.c_str()) != 0) {
            // cerr << "Error: Unable to remove the original file" << "\n";
            return 1;
        }

        // Rename tempFile to filename_main
        if (rename("tempfile.csv", filename_main.c_str()) != 0) {
            // cerr << "Error: Unable to rename tempfile.csv" << "\n";
            return 1;
        }
        // cout << filename_main.c_str() << "\n";
    }
    file.close();
    return 0;
}