#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold student information
struct Student {
    string rollNumber;
    string name;
    string department;
    double cgpa;
    string status; // "active" or "deleted"
};

// Function declarations for student operations

/*
 * Adds a new student to the system
 * Validates roll number, name, and CGPA
 * Checks for duplicates
 */
void addStudent();

/*
 * Searches for a student by roll number
 * Displays student information if found
 */
void searchStudentByRoll();

/*
 * Searches for students by name (partial match)
 * Displays all matching students
 */
void searchStudentByName();

/*
 * Updates student information (name, department, CGPA)
 * Requires roll number to identify the student
 */
void updateStudent();

/*
 * Soft deletes a student (marks status as "deleted")
 * The student record remains in the file but is not shown in active listings
 */
void softDeleteStudent();

/*
 * Displays all active students in a formatted table
 * Uses selection sort to sort by roll number
 */
void viewActiveStudents();

/*
 * Validates if a roll number is in correct format (7 digits)
 */
bool validateRollNumber(const string& roll);

/*
 * Checks if a student with given roll number already exists
 */
bool isDuplicateStudent(const string& roll);

/*
 * Validates student name (only alphabets and spaces)
 */
bool validateName(const string& name);

/*
 * Validates CGPA (between 0.0 and 4.0)
 */
bool validateCGPA(double cgpa);

/*
 * Selection sort implementation for sorting students by roll number
 */
void selectionSortStudents(vector<Student>& students);

/*
 * Converts a string line from students.txt to Student struct
 */
Student parseStudentLine(const string& line);

/*
 * Converts Student struct to string line for storage
 */
string studentToString(const Student& student);

/*
 * Finds a student by roll number and returns the Student struct
 * Returns empty Student with rollNumber "NULL" if not found
 */
Student findStudentByRoll(const string& roll);

#endif