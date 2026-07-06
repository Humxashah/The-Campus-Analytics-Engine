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
    string status; // "active" or "inactive"
};

// Function declarations for student operations

/*
 * Adds a new student to the system
 * Validates roll format BSAI-YY-XXX using substr and character checks
 * Checks for no duplicates
 * Validates name has no digits
 * CGPA in [0.0, 4.0]
 * Appends to students.txt
 */
void addStudent();

/*
 * Searches for student by roll number
 * Returns student details or empty if student ID is not available
 */
void searchStudentByRoll();  // Added this function

/*
 * Searches for students by name
 * Returns student details or empty if student ID is not available
 */
void searchByName();

/*
 * Updates student information
 * Loads file, finds row, updates specified field (not roll)
 * Rewrites file
 */
void updateStudent();

/*
 * Soft deletes a student
 * Sets status field to 'inactive'
 * Does not remove the row physically
 */
void softDelete();

/*
 * Returns all active students sorted by roll number using selection sort
 */
void listActiveStudents();

/*
 * Validates roll number format: BSAI-YY-XXX
 * Where YY are digits and XXX are digits
 */
bool validateRollNumber(const string& roll);

/*
 * Checks if a student with given roll number already exists
 */
bool isDuplicateStudent(const string& roll);

/*
 * Validates student name (no digits allowed)
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