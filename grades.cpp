#include "grades.h"
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

// Parse grade line from CSV
GradeRecord parseGradeLine(const string& line) {
    GradeRecord record;
    vector<string> fields = parseCSVLine(line);
    
    if (fields.size() >= 11) {
        record.rollNumber = fields[0];
        record.courseCode = fields[1];
        
        // Parse 5 quiz marks
        for (int i = 0; i < 5; i++) {
            record.quizMarks[i] = atof(fields[2 + i].c_str());
        }
        
        record.assignment = atof(fields[7].c_str());
        record.midterm = atof(fields[8].c_str());
        record.finalExam = atof(fields[9].c_str());
        record.weightedTotal = atof(fields[10].c_str());
        
        if (fields.size() >= 12) {
            record.letterGrade = fields[11];
        }
    }
    
    return record;
}

// Find grade record
GradeRecord findGradeRecord(const string& roll, const string& courseCode) {
    vector<vector<string> > data = readTXT("grades.txt");
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 10) {
            if (data[i][0] == roll && data[i][1] == courseCode) {
                GradeRecord record;
                record.rollNumber = data[i][0];
                record.courseCode = data[i][1];
                
                for (int j = 0; j < 5; j++) {
                    record.quizMarks[j] = atof(data[i][2 + j].c_str());
                }
                
                record.assignment = atof(data[i][7].c_str());
                record.midterm = atof(data[i][8].c_str());
                record.finalExam = atof(data[i][9].c_str());
                
                if (data[i].size() >= 11) {
                    record.weightedTotal = atof(data[i][10].c_str());
                }
                
                if (data[i].size() >= 12) {
                    record.letterGrade = data[i][11];
                }
                
                return record;
            }
        }
    }
    
    // Return empty record
    GradeRecord emptyRecord;
    emptyRecord.rollNumber = "NULL";
    return emptyRecord;
}

// Best three of five - finds and excludes two lowest using loop (no sort)
double bestThreeOffFive(double quizzes[5]) {
    // Edge case: if less than 3 quizzes, average what's available
    int validCount = 0;
    for (int i = 0; i < 5; i++) {
        if (quizzes[i] >= 0) {
            validCount++;
        }
    }
    
    if (validCount < 3) {
        // Handle n < 3 edge case: average available quizzes
        double sum = 0;
        for (int i = 0; i < 5; i++) {
            if (quizzes[i] >= 0) {
                sum += quizzes[i];
            }
        }
        return (validCount > 0) ? sum / validCount : 0.0;
    }
    
    // Find two lowest values using manual loop (no sort)
    // Initialize with first valid quiz
    int lowest1 = -1, lowest2 = -1;
    
    for (int i = 0; i < 5; i++) {
        if (quizzes[i] < 0) continue; // Skip invalid quizzes
        
        if (lowest1 == -1 || quizzes[i] < quizzes[lowest1]) {
            lowest2 = lowest1;
            lowest1 = i;
        } else if (lowest2 == -1 || quizzes[i] < quizzes[lowest2]) {
            lowest2 = i;
        }
    }
    
    // Calculate sum of best three
    double sum = 0;
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (quizzes[i] >= 0 && i != lowest1 && i != lowest2) {
            sum += quizzes[i];
            count++;
        }
    }
    
    return (count > 0) ? sum / count : 0.0;
}

// Compute weighted total: quiz*0.10 + asgn*0.10 + mid*0.30 + final*0.50
double computeWeightedTotal(double quizAvg, double assignment, double midterm, double finalExam) {
    return (quizAvg * 0.10) + (assignment * 0.10) + (midterm * 0.30) + (finalExam * 0.50);
}

// Get letter grade: >=85 A, >=80 B+, >=70 B, >=65 C+, >=60 C, >=50 D, else F
string getLetterGrade(double total) {
    if (total >= 85.0) {
        return "A";
    } else if (total >= 80.0) {
        return "B+";
    } else if (total >= 70.0) {
        return "B";
    } else if (total >= 65.0) {
        return "C+";
    } else if (total >= 60.0) {
        return "C";
    } else if (total >= 50.0) {
        return "D";
    } else {
        return "F";
    }
}

// Convert letter grade to GPA points
double gradeToPoints(const string& grade) {
    if (grade == "A") return 4.0;
    else if (grade == "B+") return 3.5;
    else if (grade == "B") return 3.0;
    else if (grade == "C+") return 2.5;
    else if (grade == "C") return 2.0;
    else if (grade == "D") return 1.0;
    else return 0.0; // F
}

// Apply attendance penalty - if attendance < 75, overrides grade to F
string applyAttendancePenalty(const string& roll, const string& courseCode, double total) {
    double attendancePct = getAttendancePct(roll, courseCode);
    
    if (attendancePct < 75.0) {
        return "F"; // Override to F regardless of marks
    }
    
    return getLetterGrade(total);
}

// Credit-weighted average of GPA points across all courses in a semester
double computeGPA(const string& roll, const string& semester) {
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<vector<string> > gradeData = readTXT("grades.txt");
    
    double totalGradePoints = 0.0;
    int totalCredits = 0;
    
    // Manual loop through enrollments
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][0] == roll && 
                enrollmentData[i][2] == semester && 
                enrollmentData[i][3] == "enrolled") {
                
                string courseCode = enrollmentData[i][1];
                Course course = findCourseByCode(courseCode);
                if (course.courseCode == "NULL") {
                    continue;
                }
                
                // Find grade for this course
                GradeRecord grade = findGradeRecord(roll, courseCode);
                if (grade.rollNumber != "NULL") {
                    double points = gradeToPoints(grade.letterGrade);
                    totalGradePoints += points * course.creditHours;
                    totalCredits += course.creditHours;
                }
            }
        }
    }
    
    if (totalCredits == 0) {
        return 0.0;
    }
    
    return totalGradePoints / totalCredits;
}

// Compute class statistics: highest, lowest, mean, median
Stats computeClassStats(const string& courseCode) {
    Stats stats;
    stats.highest = 0;
    stats.lowest = 100;
    stats.mean = 0;
    stats.median = 0;
    
    vector<vector<string> > gradeData = readTXT("grades.txt");
    vector<double> totals;
    double sum = 0.0;
    
    // Collect all weighted totals for this course
    for (size_t i = 0; i < gradeData.size(); i++) {
        if (gradeData[i].size() >= 11) {
            if (gradeData[i][1] == courseCode) {
                double total = atof(gradeData[i][10].c_str());
                totals.push_back(total);
                sum += total;
                
                if (total > stats.highest) stats.highest = total;
                if (total < stats.lowest) stats.lowest = total;
            }
        }
    }
    
    if (totals.empty()) {
        stats.highest = 0;
        stats.lowest = 0;
        stats.mean = 0;
        stats.median = 0;
        return stats;
    }
    
    // Calculate mean
    stats.mean = sum / totals.size();
    
    // Calculate median using manual sort (selection sort)
    for (size_t i = 0; i < totals.size(); i++) {
        for (size_t j = i + 1; j < totals.size(); j++) {
            if (totals[i] > totals[j]) {
                double temp = totals[i];
                totals[i] = totals[j];
                totals[j] = temp;
            }
        }
    }
    
    if (totals.size() % 2 == 0) {
        stats.median = (totals[totals.size()/2 - 1] + totals[totals.size()/2]) / 2.0;
    } else {
        stats.median = totals[totals.size()/2];
    }
    
    return stats;
}

// Enter marks - accepts up to 5 quizzes, assignment, mid (40), final (60)
void enterMarks() {
    cout << "\n--- ENTER MARKS ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists and student is enrolled
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
    if (!isDuplicateEnrollment(roll, courseCode)) {
        cout << "Error: Student is not enrolled in this course!" << endl;
        return;
    }
    
    GradeRecord record;
    record.rollNumber = roll;
    record.courseCode = courseCode;
    
    // Enter 5 quiz marks
    cout << "\nEnter Quiz Marks (0-100):" << endl;
    for (int i = 0; i < 5; i++) {
        do {
            cout << "Quiz " << (i + 1) << ": ";
            cin >> record.quizMarks[i];
            if (cin.fail() || record.quizMarks[i] < 0 || record.quizMarks[i] > 100) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Enter a number between 0 and 100!" << endl;
                record.quizMarks[i] = -1; // Invalid
            } else {
                break;
            }
        } while (true);
    }
    
    // Enter assignment marks
    do {
        cout << "Assignment (0-100): ";
        cin >> record.assignment;
        if (cin.fail() || record.assignment < 0 || record.assignment > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    // Enter midterm marks (out of 40)
    do {
        cout << "Midterm (0-40): ";
        cin >> record.midterm;
        if (cin.fail() || record.midterm < 0 || record.midterm > 40) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 40!" << endl;
        } else break;
    } while (true);
    
    // Enter final exam marks (out of 60)
    do {
        cout << "Final Exam (0-60): ";
        cin >> record.finalExam;
        if (cin.fail() || record.finalExam < 0 || record.finalExam > 60) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 60!" << endl;
        } else break;
    } while (true);
    
    // Calculate best three quiz average
    double quizAvg = bestThreeOffFive(record.quizMarks);
    
    // Compute weighted total
    record.weightedTotal = computeWeightedTotal(quizAvg, record.assignment, record.midterm, record.finalExam);
    
    // Apply attendance penalty
    record.letterGrade = applyAttendancePenalty(roll, courseCode, record.weightedTotal);
    
    // If attendance penalty not applied, use computed letter grade
    if (record.letterGrade.empty()) {
        record.letterGrade = getLetterGrade(record.weightedTotal);
    }
    
    // Save grade record
    vector<string> row;
    row.push_back(record.rollNumber);
    row.push_back(record.courseCode);
    
    for (int i = 0; i < 5; i++) {
        stringstream ss;
        ss << record.quizMarks[i];
        row.push_back(ss.str());
    }
    
    stringstream asgn, mid, finalE, total, grade;
    asgn << record.assignment;
    mid << record.midterm;
    finalE << record.finalExam;
    total << record.weightedTotal;
    
    row.push_back(asgn.str());
    row.push_back(mid.str());
    row.push_back(finalE.str());
    row.push_back(total.str());
    row.push_back(record.letterGrade);
    
    if (appendTXT("grades.txt", row)) {
        cout << "\nMarks entered successfully!" << endl;
        cout << "Quiz Average (Best 3): " << fixed << setprecision(2) << quizAvg << endl;
        cout << "Weighted Total: " << fixed << setprecision(2) << record.weightedTotal << endl;
        cout << "Letter Grade: " << record.letterGrade << endl;
        
        if (record.letterGrade == "F") {
            double attPct = getAttendancePct(roll, courseCode);
            if (attPct < 75.0) {
                cout << "Note: Grade set to F due to attendance shortage (" 
                     << fixed << setprecision(2) << attPct << "%)" << endl;
            }
        }
    } else {
        cout << "Error: Failed to save marks!" << endl;
    }
}