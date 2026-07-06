#ifndef GRADES_H
#define GRADES_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold grade information
struct GradeRecord {
    string rollNumber;
    string courseCode;
    double quizMarks[5];      // Up to 5 quiz marks
    double assignment;         // Assignment marks
    double midterm;           // Midterm marks (out of 40)
    double finalExam;         // Final exam marks (out of 60)
    double weightedTotal;     // Computed weighted total
    string letterGrade;       // Computed letter grade
};

// Structure for class statistics
struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

// Function declarations for grade operations

/*
 * Accepts up to 5 quiz marks (uses bestThreeOffFive),
 * assignment array, mid (40), final (60)
 * Validates ranges
 * Stores computed totals
 */
void enterMarks();

/*
 * Finds and excludes the two lowest values using a loop (no sort)
 * Returns the average of the remaining
 * Handles n < 3 edge case
 */
double bestThreeOffFive(double quizzes[5]);

/*
 * Applies: quiz*0.10 + asgn*0.10 + mid*0.30 + final*0.50
 * Returns a double out of 100
 */
double computeWeightedTotal(double quizAvg, double assignment, double midterm, double finalExam);

/*
 * Maps: >=85 A, >=80 B+, >=70 B, >=65 C+, >=60 C, >=50 D, else F
 * Returns string
 */
string getLetterGrade(double total);

/*
 * Credit-weighted average of GPA points across all courses in a semester
 * Manual loop — no algorithm library
 */
double computeGPA(const string& roll, const string& semester);

/*
 * Returns struct Stats (highest, lowest, mean, median)
 */
Stats computeClassStats(const string& courseCode);

/*
 * If getAttendancePct < 75, overrides grade to F regardless of marks
 */
string applyAttendancePenalty(const string& roll, const string& courseCode, double total);

/*
 * Parses a grade line from grades.txt
 */
GradeRecord parseGradeLine(const string& line);

/*
 * Finds grade record for a student in a course
 */
GradeRecord findGradeRecord(const string& roll, const string& courseCode);

/*
 * Converts letter grade to GPA points
 */
double gradeToPoints(const string& grade);

#endif