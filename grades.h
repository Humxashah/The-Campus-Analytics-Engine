#ifndef GRADES_H
#define GRADES_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold grade information
struct GradeRecord {
    string rollNumber;
    string courseCode;
    double quiz1;
    double quiz2;
    double quiz3;
    double quiz4;
    double quiz5;
    double assignment;
    double midterm;
    double finalExam;
};

// Function declarations for grade operations

/*
 * Enters marks for a student in a course
 */
void enterMarks();

/*
 * Finds and displays the best three quiz scores
 */
void viewBestThreeQuizzes();

void viewLetterGrade();

/*
 * Calculates and displays weighted total
 * Quiz: 20%, Assignment: 10%, Midterm: 30%, Final: 40%
 */
void viewWeightedTotal();

/*
 * Converts weighted total to letter grade
 */
char calculateLetterGrade(double total);

/*
 * Calculates GPA for a student
 * Includes attendance penalty
 */
void viewGPA();

/*
 * Applies attendance penalty (reduces grade for low attendance)
 */
double applyAttendancePenalty(const string& roll, const string& courseCode, double total);

/*
 * Displays class statistics (highest, lowest, mean, median)
 */
void viewClassStatistics();

/*
 * Parses a grade line from grades.txt
 */
GradeRecord parseGradeLine(const string& line);

/*
 * Finds grade record for a student in a course
 */
GradeRecord findGradeRecord(const string& roll, const string& courseCode);

#endif