#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

// Parse a line from students.txt into a Student struct
Student parseStudentLine(const string& line) {
    Student student;
    stringstream ss(line);
    
    // Extract fields separated by '|'
    getline(ss, student.rollNumber, '|');
    getline(ss, student.name, '|');
    getline(ss, student.department, '|');
    
    string cgpaStr;
    getline(ss, cgpaStr, '|');
    student.cgpa = atof(cgpaStr.c_str());
    
    getline(ss, student.status, '|');
    
    return student;
}

// Convert Student struct to string for storage
string studentToString(const Student& student) {
    stringstream ss;
    ss << student.rollNumber << "|" 
       << student.name << "|" 
       << student.department << "|" 
       << student.cgpa << "|" 
       << student.status;
    return ss.str();
}

// Validate roll number format (7 digits)
bool validateRollNumber(const string& roll) {
    // Check length
    if (roll.length() != 7) {
        return false;
    }
    
    // Check if all characters are digits
    for (size_t i = 0; i < roll.length(); i++) {
        if (!isdigit(roll[i])) {
            return false;
        }
    }
    
    return true;
}

// Check for duplicate student
bool isDuplicateStudent(const string& roll) {
    vector<string> lines = readTXT("students.txt");
    for (size_t i = 0; i < lines.size(); i++) {
        Student student = parseStudentLine(lines[i]);
        if (student.rollNumber == roll && student.status == "active") {
            return true;
        }
    }
    return false;
}

// Validate name (only alphabets and spaces)
bool validateName(const string& name) {
    if (name.empty()) {
        return false;
    }
    
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// Validate CGPA
bool validateCGPA(double cgpa) {
    return (cgpa >= 0.0 && cgpa <= 4.0);
}

// Find student by roll number
Student findStudentByRoll(const string& roll) {
    vector<string> lines = readTXT("students.txt");
    
    for (size_t i = 0; i < lines.size(); i++) {
        Student student = parseStudentLine(lines[i]);
        if (student.rollNumber == roll && student.status == "active") {
            return student;
        }
    }
    
    // Return empty student if not found
    Student emptyStudent;
    emptyStudent.rollNumber = "NULL";
    return emptyStudent;
}

// Selection sort for students by roll number
void selectionSortStudents(vector<Student>& students) {
    int n = students.size();
    
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            // Compare roll numbers as strings
            if (students[j].rollNumber < students[minIndex].rollNumber) {
                minIndex = j;
            }
        }
        // Swap if needed
        if (minIndex != i) {
            Student temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
    }
}

// Add new student
void addStudent() {
    cout << "\n--- ADD NEW STUDENT ---" << endl;
    
    Student newStudent;
    
    // Get and validate roll number
    do {
        cout << "Enter Roll Number (7 digits): ";
        cin >> newStudent.rollNumber;
        
        if (!validateRollNumber(newStudent.rollNumber)) {
            cout << "Error: Roll number must be exactly 7 digits!" << endl;
            continue;
        }
        
        if (isDuplicateStudent(newStudent.rollNumber)) {
            cout << "Error: Student with this roll number already exists!" << endl;
            newStudent.rollNumber = ""; // Reset to repeat loop
            continue;
        }
        
        break;
    } while (true);
    
    cin.ignore(); // Clear input buffer
    
    // Get and validate name
    do {
        cout << "Enter Student Name: ";
        getline(cin, newStudent.name);
        
        if (!validateName(newStudent.name)) {
            cout << "Error: Name can only contain alphabets and spaces!" << endl;
            newStudent.name = "";
        } else {
            break;
        }
    } while (true);
    
    // Get department
    cout << "Enter Department: ";
    getline(cin, newStudent.department);
    
    // Get and validate CGPA
    do {
        cout << "Enter CGPA (0.0 - 4.0): ";
        cin >> newStudent.cgpa;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a valid number!" << endl;
            continue;
        }
        
        if (!validateCGPA(newStudent.cgpa)) {
            cout << "Error: CGPA must be between 0.0 and 4.0!" << endl;
        } else {
            break;
        }
    } while (true);
    
    newStudent.status = "active";
    
    // Append to file
    string line = studentToString(newStudent);
    if (appendTXT("students.txt", line)) {
        cout << "Student added successfully!" << endl;
    } else {
        cout << "Error: Failed to add student!" << endl;
    }
}

// Search student by roll number
void searchStudentByRoll() {
    cout << "\n--- SEARCH STUDENT BY ROLL NUMBER ---" << endl;
    
    string roll;
    cout << "Enter Roll Number: ";
    cin >> roll;
    
    Student student = findStudentByRoll(roll);
    
    if (student.rollNumber == "NULL") {
        cout << "Student not found!" << endl;
        return;
    }
    
    // Display student information
    cout << "\n--- STUDENT INFORMATION ---" << endl;
    cout << "Roll Number: " << student.rollNumber << endl;
    cout << "Name: " << student.name << endl;
    cout << "Department: " << student.department << endl;
    cout << "CGPA: " << fixed << setprecision(2) << student.cgpa << endl;
    cout << "Status: " << student.status << endl;
}

// Search student by name (partial match)
void searchStudentByName() {
    cout << "\n--- SEARCH STUDENT BY NAME ---" << endl;
    
    string searchName;
    cin.ignore();
    cout << "Enter Name (or part of name): ";
    getline(cin, searchName);
    
    vector<string> lines = readTXT("students.txt");
    vector<Student> foundStudents;
    
    // Convert search name to lowercase for case-insensitive comparison
    string searchLower = searchName;
    for (size_t i = 0; i < searchLower.length(); i++) {
        searchLower[i] = tolower(searchLower[i]);
    }
    
    for (size_t i = 0; i < lines.size(); i++) {
        Student student = parseStudentLine(lines[i]);
        if (student.status != "active") {
            continue; // Skip deleted students
        }
        
        // Convert student name to lowercase for comparison
        string studentNameLower = student.name;
        for (size_t j = 0; j < studentNameLower.length(); j++) {
            studentNameLower[j] = tolower(studentNameLower[j]);
        }
        
        if (studentNameLower.find(searchLower) != string::npos) {
            foundStudents.push_back(student);
        }
    }
    
    if (foundStudents.empty()) {
        cout << "No students found with that name!" << endl;
        return;
    }
    
    // Display results in a table
    cout << "\n--- SEARCH RESULTS ---" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(25) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(78, '-') << endl;
    
    for (size_t i = 0; i < foundStudents.size(); i++) {
        cout << left << setw(15) << foundStudents[i].rollNumber
             << setw(30) << foundStudents[i].name
             << setw(25) << foundStudents[i].department
             << setw(8) << fixed << setprecision(2) << foundStudents[i].cgpa << endl;
    }
    cout << "Total students found: " << foundStudents.size() << endl;
}

// Update student information
void updateStudent() {
    cout << "\n--- UPDATE STUDENT INFORMATION ---" << endl;
    
    string roll;
    cout << "Enter Roll Number of student to update: ";
    cin >> roll;
    
    // Read all students
    vector<string> lines = readTXT("students.txt");
    bool found = false;
    int index = -1;
    Student student;
    
    for (size_t i = 0; i < lines.size(); i++) {
        student = parseStudentLine(lines[i]);
        if (student.rollNumber == roll && student.status == "active") {
            found = true;
            index = i;
            break;
        }
    }
    
    if (!found) {
        cout << "Student not found!" << endl;
        return;
    }
    
    cout << "\nCurrent Information:" << endl;
    cout << "Name: " << student.name << endl;
    cout << "Department: " << student.department << endl;
    cout << "CGPA: " << student.cgpa << endl;
    
    cout << "\n--- UPDATE OPTIONS ---" << endl;
    cout << "1. Update Name" << endl;
    cout << "2. Update Department" << endl;
    cout << "3. Update CGPA" << endl;
    cout << "4. Update All" << endl;
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    
    cin.ignore(); // Clear input buffer
    
    switch(choice) {
        case 1: {
            string newName;
            do {
                cout << "Enter New Name: ";
                getline(cin, newName);
                if (!validateName(newName)) {
                    cout << "Error: Name can only contain alphabets and spaces!" << endl;
                } else {
                    student.name = newName;
                    break;
                }
            } while (true);
            break;
        }
        case 2: {
            cout << "Enter New Department: ";
            getline(cin, student.department);
            break;
        }
        case 3: {
            double newCGPA;
            do {
                cout << "Enter New CGPA (0.0 - 4.0): ";
                cin >> newCGPA;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Please enter a valid number!" << endl;
                    continue;
                }
                if (!validateCGPA(newCGPA)) {
                    cout << "Error: CGPA must be between 0.0 and 4.0!" << endl;
                } else {
                    student.cgpa = newCGPA;
                    break;
                }
            } while (true);
            break;
        }
        case 4: {
            // Update all fields
            string newName;
            do {
                cout << "Enter New Name: ";
                getline(cin, newName);
                if (!validateName(newName)) {
                    cout << "Error: Name can only contain alphabets and spaces!" << endl;
                } else {
                    student.name = newName;
                    break;
                }
            } while (true);
            
            cout << "Enter New Department: ";
            getline(cin, student.department);
            
            double newCGPA;
            do {
                cout << "Enter New CGPA (0.0 - 4.0): ";
                cin >> newCGPA;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error: Please enter a valid number!" << endl;
                    continue;
                }
                if (!validateCGPA(newCGPA)) {
                    cout << "Error: CGPA must be between 0.0 and 4.0!" << endl;
                } else {
                    student.cgpa = newCGPA;
                    break;
                }
            } while (true);
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    // Update the line in the vector
    lines[index] = studentToString(student);
    
    // Write back to file
    if (writeTXT("students.txt", lines)) {
        cout << "Student information updated successfully!" << endl;
    } else {
        cout << "Error: Failed to update student!" << endl;
    }
}

// Soft delete student
void softDeleteStudent() {
    cout << "\n--- SOFT DELETE STUDENT ---" << endl;
    
    string roll;
    cout << "Enter Roll Number of student to delete: ";
    cin >> roll;
    
    vector<string> lines = readTXT("students.txt");
    bool found = false;
    int index = -1;
    Student student;
    
    for (size_t i = 0; i < lines.size(); i++) {
        student = parseStudentLine(lines[i]);
        if (student.rollNumber == roll && student.status == "active") {
            found = true;
            index = i;
            break;
        }
    }
    
    if (!found) {
        cout << "Student not found!" << endl;
        return;
    }
    
    // Confirm deletion
    cout << "Are you sure you want to delete student: " << student.name << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled." << endl;
        return;
    }
    
    // Mark as deleted
    student.status = "deleted";
    lines[index] = studentToString(student);
    
    if (writeTXT("students.txt", lines)) {
        cout << "Student marked as deleted successfully!" << endl;
    } else {
        cout << "Error: Failed to delete student!" << endl;
    }
}

// View all active students
void viewActiveStudents() {
    cout << "\n--- ACTIVE STUDENTS LIST ---" << endl;
    
    vector<string> lines = readTXT("students.txt");
    vector<Student> activeStudents;
    
    for (size_t i = 0; i < lines.size(); i++) {
        Student student = parseStudentLine(lines[i]);
        if (student.status == "active") {
            activeStudents.push_back(student);
        }
    }
    
    if (activeStudents.empty()) {
        cout << "No active students found!" << endl;
        return;
    }
    
    // Sort students using selection sort
    selectionSortStudents(activeStudents);
    
    // Display in a formatted table
    cout << "\n" << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(25) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(78, '-') << endl;
    
    for (size_t i = 0; i < activeStudents.size(); i++) {
        cout << left << setw(15) << activeStudents[i].rollNumber
             << setw(30) << activeStudents[i].name
             << setw(25) << activeStudents[i].department
             << setw(8) << fixed << setprecision(2) << activeStudents[i].cgpa << endl;
    }
    cout << "\nTotal active students: " << activeStudents.size() << endl;
}