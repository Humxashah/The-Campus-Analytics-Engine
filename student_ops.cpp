#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

// Helper function to check if a string contains another string (case insensitive)
bool containsIgnoreCase(const string& str, const string& substr) {
    if (substr.empty()) return true;
    
    string strLower = str;
    string subLower = substr;
    
    // Convert both to lowercase
    for (size_t i = 0; i < strLower.length(); i++) {
        strLower[i] = tolower(strLower[i]);
    }
    for (size_t i = 0; i < subLower.length(); i++) {
        subLower[i] = tolower(subLower[i]);
    }
    
    // Manual find without using algorithm library
    for (size_t i = 0; i <= strLower.length() - subLower.length(); i++) {
        bool found = true;
        for (size_t j = 0; j < subLower.length(); j++) {
            if (strLower[i + j] != subLower[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return true;
        }
    }
    return false;
}

// Parse a line from students.txt into a Student struct
Student parseStudentLine(const string& line) {
    Student student;
    vector<string> fields = parseCSVLine(line);
    
    if (fields.size() >= 5) {
        student.rollNumber = fields[0];
        student.name = fields[1];
        student.department = fields[2];
        student.cgpa = atof(fields[3].c_str());
        student.status = fields[4];
    }
    
    return student;
}

// Convert Student struct to string for storage
string studentToString(const Student& student) {
    stringstream ss;
    ss << student.rollNumber << "," 
       << student.name << "," 
       << student.department << "," 
       << student.cgpa << "," 
       << student.status;
    return ss.str();
}

// Validate roll number format: BSAI-YY-XXX
bool validateRollNumber(const string& roll) {
    // Check if format is BSAI-YY-XXX (exactly 10 characters: BSAI-YY-XXX)
    if (roll.length() != 10) {
        return false;
    }
    
    // Check first 4 characters are "BSAI"
    if (roll.substr(0, 4) != "BSAI") {
        return false;
    }
    
    // Check 5th character is '-'
    if (roll[4] != '-') {
        return false;
    }
    
    // Check positions 5-6 are digits (YY)
    if (!isdigit(roll[5]) || !isdigit(roll[6])) {
        return false;
    }
    
    // Check 7th character is '-'
    if (roll[7] != '-') {
        return false;
    }
    
    // Check positions 8-10 are digits (XXX)
    if (!isdigit(roll[8]) || !isdigit(roll[9]) || !isdigit(roll[10])) {
        return false;
    }
    
    return true;
}

// Check for duplicate student
bool isDuplicateStudent(const string& roll) {
    vector<vector<string> > data = readTXT("students.txt");
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][0] == roll && data[i][4] == "active") {
            return true;
        }
    }
    return false;
}

// Validate name (no digits allowed)
bool validateName(const string& name) {
    if (name.empty()) {
        return false;
    }
    
    for (size_t i = 0; i < name.length(); i++) {
        if (isdigit(name[i])) {
            return false; // No digits allowed in name
        }
    }
    return true;
}

// Validate CGPA
bool validateCGPA(double cgpa) {
    return (cgpa >= 0.0 && cgpa <= 4.0);
}

// Find student by roll number - supports partial matching
Student findStudentByRoll(const string& roll) {
    vector<vector<string> > data = readTXT("students.txt");
    vector<Student> matches;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][4] == "active") {
            // Check if roll contains the search string (partial match)
            if (containsIgnoreCase(data[i][0], roll)) {
                Student student;
                student.rollNumber = data[i][0];
                student.name = data[i][1];
                student.department = data[i][2];
                student.cgpa = atof(data[i][3].c_str());
                student.status = data[i][4];
                matches.push_back(student);
            }
        }
    }
    
    // If multiple matches found, display them and let user choose
    if (matches.size() > 1) {
        cout << "\nMultiple students found with matching roll numbers:" << endl;
        cout << left << setw(5) << "No." 
             << setw(15) << "Roll Number" 
             << setw(30) << "Name" 
             << setw(15) << "Department" << endl;
        cout << string(65, '-') << endl;
        
        for (size_t i = 0; i < matches.size(); i++) {
            cout << left << setw(5) << (i + 1)
                 << setw(15) << matches[i].rollNumber
                 << setw(30) << matches[i].name
                 << setw(15) << matches[i].department << endl;
        }
        
        int choice;
        cout << "\nEnter the number of the student you want to select (0 to cancel): ";
        cin >> choice;
        
        if (choice > 0 && choice <= static_cast<int>(matches.size())) {
            return matches[choice - 1];
        } else {
            Student emptyStudent;
            emptyStudent.rollNumber = "NULL";
            return emptyStudent;
        }
    }
    // If exactly one match found, return it
    else if (matches.size() == 1) {
        return matches[0];
    }
    // No matches found
    else {
        Student emptyStudent;
        emptyStudent.rollNumber = "NULL";
        return emptyStudent;
    }
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
    
    // Get and validate roll number (Format: BSAI-YY-XXX)
    do {
        cout << "Enter Roll Number (Format: BSAI-YY-XXX, e.g., BSAI-24-001): ";
        cin >> newStudent.rollNumber;
        
        if (!validateRollNumber(newStudent.rollNumber)) {
            cout << "Error: Roll number must be in format BSAI-YY-XXX!" << endl;
            cout << "Example: BSAI-24-001" << endl;
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
    
    // Get and validate name (no digits allowed)
    do {
        cout << "Enter Student Name: ";
        getline(cin, newStudent.name);
        
        if (!validateName(newStudent.name)) {
            cout << "Error: Name cannot contain digits!" << endl;
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
    
    // Convert to CSV row
    vector<string> row;
    row.push_back(newStudent.rollNumber);
    row.push_back(newStudent.name);
    row.push_back(newStudent.department);
    stringstream cgpaStr;
    cgpaStr << newStudent.cgpa;
    row.push_back(cgpaStr.str());
    row.push_back(newStudent.status);
    
    // Append to file
    if (appendTXT("students.txt", row)) {
        cout << "Student added successfully!" << endl;
    } else {
        cout << "Error: Failed to add student!" << endl;
    }
}

// Search by roll number - supports partial matching
void searchStudentByRoll() {
    cout << "\n--- SEARCH STUDENT BY ROLL NUMBER ---" << endl;
    cout << "Enter Roll Number (partial or full): ";
    
    string roll;
    cin >> roll;
    
    Student student = findStudentByRoll(roll);
    
    if (student.rollNumber == "NULL") {
        cout << "No student found with that roll number!" << endl;
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

// Search by name - returns student details or empty if not available
void searchByName() {
    cout << "\n--- SEARCH STUDENT BY NAME ---" << endl;
    
    string searchName;
    cin.ignore();
    cout << "Enter Name (or part of name): ";
    getline(cin, searchName);
    
    vector<vector<string> > data = readTXT("students.txt");
    vector<Student> foundStudents;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5) {
            if (data[i][4] != "active") {
                continue; // Skip inactive students
            }
            
            // Check if name contains the search string
            if (containsIgnoreCase(data[i][1], searchName)) {
                Student student;
                student.rollNumber = data[i][0];
                student.name = data[i][1];
                student.department = data[i][2];
                student.cgpa = atof(data[i][3].c_str());
                student.status = data[i][4];
                foundStudents.push_back(student);
            }
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
         << setw(15) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(68, '-') << endl;
    
    for (size_t i = 0; i < foundStudents.size(); i++) {
        cout << left << setw(15) << foundStudents[i].rollNumber
             << setw(30) << foundStudents[i].name
             << setw(15) << foundStudents[i].department
             << setw(8) << fixed << setprecision(2) << foundStudents[i].cgpa << endl;
    }
    cout << "Total students found: " << foundStudents.size() << endl;
}

// Update student - loads file, finds row, updates specified field (not roll), rewrites file
void updateStudent() {
    cout << "\n--- UPDATE STUDENT INFORMATION ---" << endl;
    
    string roll;
    cout << "Enter Roll Number of student to update: ";
    cin >> roll;
    
    vector<vector<string> > data = readTXT("students.txt");
    bool found = false;
    int index = -1;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][0] == roll && data[i][4] == "active") {
            found = true;
            index = i;
            break;
        }
    }
    
    if (!found) {
        cout << "Student not found!" << endl;
        return;
    }
    
    Student student;
    student.rollNumber = data[index][0];
    student.name = data[index][1];
    student.department = data[index][2];
    student.cgpa = atof(data[index][3].c_str());
    student.status = data[index][4];
    
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
                    cout << "Error: Name cannot contain digits!" << endl;
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
            string newName;
            do {
                cout << "Enter New Name: ";
                getline(cin, newName);
                if (!validateName(newName)) {
                    cout << "Error: Name cannot contain digits!" << endl;
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
    
    // Update the row (roll number is not updated)
    vector<string> updatedRow;
    updatedRow.push_back(student.rollNumber);
    updatedRow.push_back(student.name);
    updatedRow.push_back(student.department);
    stringstream cgpaStr;
    cgpaStr << student.cgpa;
    updatedRow.push_back(cgpaStr.str());
    updatedRow.push_back(student.status);
    
    data[index] = updatedRow;
    
    // Get header
    vector<string> header;
    header.push_back("roll");
    header.push_back("name");
    header.push_back("dept");
    header.push_back("cgpa");
    header.push_back("status");
    
    if (writeTXT("students.txt", header, data)) {
        cout << "Student information updated successfully!" << endl;
    } else {
        cout << "Error: Failed to update student!" << endl;
    }
}

// Soft delete - sets status field to 'inactive', does not remove row
void softDelete() {
    cout << "\n--- SOFT DELETE STUDENT ---" << endl;
    
    string roll;
    cout << "Enter Roll Number of student to delete: ";
    cin >> roll;
    
    vector<vector<string> > data = readTXT("students.txt");
    bool found = false;
    int index = -1;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][0] == roll && data[i][4] == "active") {
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
    cout << "Are you sure you want to delete student: " << data[index][1] << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled." << endl;
        return;
    }
    
    // Set status to 'inactive'
    data[index][4] = "inactive";
    
    // Get header
    vector<string> header;
    header.push_back("roll");
    header.push_back("name");
    header.push_back("dept");
    header.push_back("cgpa");
    header.push_back("status");
    
    if (writeTXT("students.txt", header, data)) {
        cout << "Student marked as inactive successfully!" << endl;
    } else {
        cout << "Error: Failed to delete student!" << endl;
    }
}

// List active students - returns all active students sorted by roll number using selection sort
void listActiveStudents() {
    cout << "\n--- ACTIVE STUDENTS LIST ---" << endl;
    
    vector<vector<string> > data = readTXT("students.txt");
    vector<Student> activeStudents;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][4] == "active") {
            Student student;
            student.rollNumber = data[i][0];
            student.name = data[i][1];
            student.department = data[i][2];
            student.cgpa = atof(data[i][3].c_str());
            student.status = data[i][4];
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
         << setw(15) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(68, '-') << endl;
    
    for (size_t i = 0; i < activeStudents.size(); i++) {
        cout << left << setw(15) << activeStudents[i].rollNumber
             << setw(30) << activeStudents[i].name
             << setw(15) << activeStudents[i].department
             << setw(8) << fixed << setprecision(2) << activeStudents[i].cgpa << endl;
    }
    cout << "\nTotal active students: " << activeStudents.size() << endl;
}