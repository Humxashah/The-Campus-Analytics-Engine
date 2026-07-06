#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>

using namespace std;

// Structure for department summary
struct DepartmentSummary {
    string departmentName;
    int studentCount;
    double averageCGPA;
    double passRate; // Percentage of students with CGPA >= 2.0
};

// Function declarations for reports

/*
 * All active students are sorted by CGPA in descending order
 * Formatted table with rank column
 */
void printMeritList();

/*
 * Students with any course attendance < 75%
 * Shows roll, name, course, and percentage
 */
void printAttendanceDefaulters();

/*
 * Call all fee defaulters
 * Formatted with outstanding amount and weeks overdue
 */
void printFeeDefaulters();

/*
 * Full result sheet with borders
 * Grade, GPA, and attendance status per student
 */
void printSemesterResult();

/*
 * Group students by dept field using nested loops and parallel arrays
 * Shows count, avg CGPA, pass rate per dept
 */
void printDepartmentSummary();

/*
 * Redirects cout to ofstream for chosen report
 * Restores cout after
 * Saves .txt file
 */
void exportReportToFile(const string& filename, void (*reportFunction)());

/*
 * Sorts students by CGPA in descending order using selection sort
 */
void sortStudentsByCGPA(vector<Student>& students);

#endif