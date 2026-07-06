#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>

using namespace std;

// Function declarations for reports

/*
 * Generates merit list of students based on CGPA
 * Uses selection sort
 */
void generateMeritList();

/*
 * Generates list of attendance defaulters
 */
void generateAttendanceDefaulters();

/*
 * Generates list of fee defaulters
 */
void generateFeeDefaulters();

/*
 * Generates semester result for a student
 */
void generateSemesterResult();

/*
 * Generates department summary
 */
void generateDepartmentSummary();

/*
 * Exports a report to a text file
 */
void exportReportToFile(const string& filename, const vector<string>& reportLines);

/*
 * Selection sort for sorting students by CGPA (descending)
 */
void sortByCGPA(vector<Student>& students);

#endif