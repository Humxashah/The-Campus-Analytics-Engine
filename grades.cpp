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

// Parse grade line
GradeRecord parseGradeLine(const string& line) {
    GradeRecord record;
    stringstream ss(line);
    
    getline(ss, record.rollNumber, '|');
    getline(ss, record.courseCode, '|');
    
    string temp;
    getline(ss, temp, '|');
    record.quiz1 = atof(temp.c_str());
    getline(ss, temp, '|');
    record.quiz2 = atof(temp.c_str());
    getline(ss, temp, '|');
    record.quiz3 = atof(temp.c_str());
    getline(ss, temp, '|');
    record.quiz4 = atof(temp.c_str());
    getline(ss, temp, '|');
    record.quiz5 = atof(temp.c_str());
    getline(ss, temp, '|');
    record.assignment = atof(temp.c_str());
    getline(ss, temp, '|');
    record.midterm = atof(temp.c_str());
    getline(ss, temp, '|');
    record.finalExam = atof(temp.c_str());
    
    return record;
}

// Find grade record
GradeRecord findGradeRecord(const string& roll, const string& courseCode) {
    vector<string> lines = readTXT("grades.txt");
    
    for (size_t i = 0; i < lines.size(); i++) {
        GradeRecord record = parseGradeLine(lines[i]);
        if (record.rollNumber == roll && record.courseCode == courseCode) {
            return record;
        }
    }
    
    // Return empty record
    GradeRecord emptyRecord;
    emptyRecord.rollNumber = "NULL";
    return emptyRecord;
}

// Calculate letter grade
char calculateLetterGrade(double total) {
    if (total >= 90.0) return 'A';
    else if (total >= 80.0) return 'B';
    else if (total >= 70.0) return 'C';
    else if (total >= 60.0) return 'D';
    else return 'F';
}

// Apply attendance penalty
double applyAttendancePenalty(const string& roll, const string& courseCode, double total) {
    double attendance = calculateAttendancePercentage(roll, courseCode);
    
    if (attendance < 75.0) {
        // Penalty: 5% reduction for every 10% below 75%
        double shortage = 75.0 - attendance;
        int penaltyLevels = static_cast<int>(shortage / 10.0) + 1;
        double penalty = penaltyLevels * 5.0;
        double penalizedTotal = total - penalty;
        
        // Minimum grade is 0
        if (penalizedTotal < 0) penalizedTotal = 0;
        
        return penalizedTotal;
    }
    
    return total;
}

// Enter marks
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
    
    // Enter marks with validation
    cout << "\nEnter marks (0-100):" << endl;
    
    do {
        cout << "Quiz 1: ";
        cin >> record.quiz1;
        if (cin.fail() || record.quiz1 < 0 || record.quiz1 > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Quiz 2: ";
        cin >> record.quiz2;
        if (cin.fail() || record.quiz2 < 0 || record.quiz2 > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Quiz 3: ";
        cin >> record.quiz3;
        if (cin.fail() || record.quiz3 < 0 || record.quiz3 > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Quiz 4: ";
        cin >> record.quiz4;
        if (cin.fail() || record.quiz4 < 0 || record.quiz4 > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Quiz 5: ";
        cin >> record.quiz5;
        if (cin.fail() || record.quiz5 < 0 || record.quiz5 > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Assignment: ";
        cin >> record.assignment;
        if (cin.fail() || record.assignment < 0 || record.assignment > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Midterm: ";
        cin >> record.midterm;
        if (cin.fail() || record.midterm < 0 || record.midterm > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    do {
        cout << "Final Exam: ";
        cin >> record.finalExam;
        if (cin.fail() || record.finalExam < 0 || record.finalExam > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Enter a number between 0 and 100!" << endl;
        } else break;
    } while (true);
    
    // Save grade record
    stringstream ss;
    ss << record.rollNumber << "|" << record.courseCode << "|"
       << record.quiz1 << "|" << record.quiz2 << "|" << record.quiz3 << "|"
       << record.quiz4 << "|" << record.quiz5 << "|" << record.assignment << "|"
       << record.midterm << "|" << record.finalExam;
    
    if (appendTXT("grades.txt", ss.str())) {
        cout << "\nMarks entered successfully!" << endl;
    } else {
        cout << "Error: Failed to save marks!" << endl;
    }
}

// View best three quizzes
void viewBestThreeQuizzes() {
    cout << "\n--- BEST THREE QUIZZES ---" << endl;
    
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
    
    GradeRecord record = findGradeRecord(roll, courseCode);
    if (record.rollNumber == "NULL") {
        cout << "No grade record found for this student in this course!" << endl;
        return;
    }
    
    // Store quiz scores in array
    double quizzes[5] = {record.quiz1, record.quiz2, record.quiz3, record.quiz4, record.quiz5};
    
    // Find best three using selection sort (descending)
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (quizzes[i] < quizzes[j]) {
                double temp = quizzes[i];
                quizzes[i] = quizzes[j];
                quizzes[j] = temp;
            }
        }
    }
    
    cout << "\nBest three quiz scores:" << endl;
    cout << "1st: " << quizzes[0] << endl;
    cout << "2nd: " << quizzes[1] << endl;
    cout << "3rd: " << quizzes[2] << endl;
    cout << "Average of best three: " << fixed << setprecision(2) 
         << (quizzes[0] + quizzes[1] + quizzes[2]) / 3.0 << endl;
}

// View weighted total
void viewWeightedTotal() {
    cout << "\n--- VIEW WEIGHTED TOTAL ---" << endl;
    
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
    
    GradeRecord record = findGradeRecord(roll, courseCode);
    if (record.rollNumber == "NULL") {
        cout << "No grade record found for this student in this course!" << endl;
        return;
    }
    
    // Calculate quiz average (best three)
    double quizzes[5] = {record.quiz1, record.quiz2, record.quiz3, record.quiz4, record.quiz5};
    
    // Sort descending
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (quizzes[i] < quizzes[j]) {
                double temp = quizzes[i];
                quizzes[i] = quizzes[j];
                quizzes[j] = temp;
            }
        }
    }
    
    double quizAverage = (quizzes[0] + quizzes[1] + quizzes[2]) / 3.0;
    double weightedQuiz = quizAverage * 0.20; // 20%
    double weightedAssignment = record.assignment * 0.10; // 10%
    double weightedMidterm = record.midterm * 0.30; // 30%
    double weightedFinal = record.finalExam * 0.40; // 40%
    
    double total = weightedQuiz + weightedAssignment + weightedMidterm + weightedFinal;
    
    cout << "\n--- WEIGHTED TOTAL CALCULATION ---" << endl;
    cout << "Student: " << student.name << " (" << roll << ")" << endl;
    cout << "Course: " << courseCode << endl;
    cout << "\nComponent Weights:" << endl;
    cout << "Quiz (best 3 average): " << fixed << setprecision(2) << quizAverage << " × 0.20 = " << weightedQuiz << endl;
    cout << "Assignment: " << record.assignment << " × 0.10 = " << weightedAssignment << endl;
    cout << "Midterm: " << record.midterm << " × 0.30 = " << weightedMidterm << endl;
    cout << "Final: " << record.finalExam << " × 0.40 = " << weightedFinal << endl;
    cout << string(40, '-') << endl;
    cout << "Total (before penalty): " << total << endl;
    
    // Apply attendance penalty
    double penalizedTotal = applyAttendancePenalty(roll, courseCode, total);
    if (penalizedTotal < total) {
        cout << "Attendance penalty applied!" << endl;
        cout << "Final Total: " << penalizedTotal << endl;
    }
    
    char grade = calculateLetterGrade(penalizedTotal);
    cout << "Letter Grade: " << grade << endl;
}

// View GPA
void viewGPA() {
    cout << "\n--- VIEW GPA ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    vector<string> gradeLines = readTXT("grades.txt");
    vector<GradeRecord> studentGrades;
    
    // Get all grades for this student
    for (size_t i = 0; i < gradeLines.size(); i++) {
        GradeRecord record = parseGradeLine(gradeLines[i]);
        if (record.rollNumber == roll) {
            studentGrades.push_back(record);
        }
    }
    
    if (studentGrades.empty()) {
        cout << "No grade records found for this student!" << endl;
        return;
    }
    
    cout << "\n--- GPA CALCULATION FOR " << student.name << " ---" << endl;
    cout << left << setw(15) << "Course" 
         << setw(15) << "Credits" 
         << setw(15) << "Total" 
         << setw(10) << "Grade" 
         << setw(10) << "Points" << endl;
    cout << string(65, '-') << endl;
    
    double totalPoints = 0;
    int totalCredits = 0;
    double gpa = 0.0;
    
    for (size_t i = 0; i < studentGrades.size(); i++) {
        Course course = findCourseByCode(studentGrades[i].courseCode);
        if (course.courseCode == "NULL") {
            continue;
        }
        
        // Calculate total
        double quizzes[5] = {studentGrades[i].quiz1, studentGrades[i].quiz2, 
                            studentGrades[i].quiz3, studentGrades[i].quiz4, 
                            studentGrades[i].quiz5};
        
        for (int j = 0; j < 5; j++) {
            for (int k = j + 1; k < 5; k++) {
                if (quizzes[j] < quizzes[k]) {
                    double temp = quizzes[j];
                    quizzes[j] = quizzes[k];
                    quizzes[k] = temp;
                }
            }
        }
        
        double quizAverage = (quizzes[0] + quizzes[1] + quizzes[2]) / 3.0;
        double total = (quizAverage * 0.20) + 
                      (studentGrades[i].assignment * 0.10) + 
                      (studentGrades[i].midterm * 0.30) + 
                      (studentGrades[i].finalExam * 0.40);
        
        // Apply attendance penalty
        total = applyAttendancePenalty(roll, studentGrades[i].courseCode, total);
        
        char grade = calculateLetterGrade(total);
        
        // Convert letter grade to grade points
        double gradePoints = 0.0;
        switch(grade) {
            case 'A': gradePoints = 4.0; break;
            case 'B': gradePoints = 3.0; break;
            case 'C': gradePoints = 2.0; break;
            case 'D': gradePoints = 1.0; break;
            case 'F': gradePoints = 0.0; break;
        }
        
        cout << left << setw(15) << course.courseCode
             << setw(15) << course.creditHours
             << setw(15) << fixed << setprecision(2) << total
             << setw(10) << grade
             << setw(10) << fixed << setprecision(2) << gradePoints << endl;
        
        totalPoints += gradePoints * course.creditHours;
        totalCredits += course.creditHours;
    }
    
    if (totalCredits > 0) {
        gpa = totalPoints / totalCredits;
        cout << string(65, '-') << endl;
        cout << "Total Credits: " << totalCredits << endl;
        cout << "Total Grade Points: " << fixed << setprecision(2) << totalPoints << endl;
        cout << "GPA: " << fixed << setprecision(2) << gpa << endl;
    } else {
        cout << "No courses with credit hours found!" << endl;
    }
}

// View class statistics
void viewClassStatistics() {
    cout << "\n--- CLASS STATISTICS ---" << endl;
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
    vector<string> gradeLines = readTXT("grades.txt");
    vector<double> totals;
    
    // Calculate totals for all students in this course
    for (size_t i = 0; i < gradeLines.size(); i++) {
        GradeRecord record = parseGradeLine(gradeLines[i]);
        if (record.courseCode == courseCode) {
            // Calculate total
            double quizzes[5] = {record.quiz1, record.quiz2, record.quiz3, record.quiz4, record.quiz5};
            
            for (int j = 0; j < 5; j++) {
                for (int k = j + 1; k < 5; k++) {
                    if (quizzes[j] < quizzes[k]) {
                        double temp = quizzes[j];
                        quizzes[j] = quizzes[k];
                        quizzes[k] = temp;
                    }
                }
            }
            
            double quizAverage = (quizzes[0] + quizzes[1] + quizzes[2]) / 3.0;
            double total = (quizAverage * 0.20) + 
                          (record.assignment * 0.10) + 
                          (record.midterm * 0.30) + 
                          (record.finalExam * 0.40);
            
            // Apply attendance penalty
            total = applyAttendancePenalty(record.rollNumber, courseCode, total);
            
            totals.push_back(total);
        }
    }
    
    if (totals.empty()) {
        cout << "No grade records found for this course!" << endl;
        return;
    }
    
    // Calculate statistics
    // Find highest and lowest
    double highest = totals[0];
    double lowest = totals[0];
    double sum = 0;
    
    for (size_t i = 0; i < totals.size(); i++) {
        if (totals[i] > highest) highest = totals[i];
        if (totals[i] < lowest) lowest = totals[i];
        sum += totals[i];
    }
    
    double mean = sum / totals.size();
    
    // Calculate median (sort first)
    for (size_t i = 0; i < totals.size(); i++) {
        for (size_t j = i + 1; j < totals.size(); j++) {
            if (totals[i] > totals[j]) {
                double temp = totals[i];
                totals[i] = totals[j];
                totals[j] = temp;
            }
        }
    }
    
    double median;
    if (totals.size() % 2 == 0) {
        median = (totals[totals.size()/2 - 1] + totals[totals.size()/2]) / 2.0;
    } else {
        median = totals[totals.size()/2];
    }
    
    // Display statistics
    cout << "\n--- CLASS STATISTICS FOR " << courseCode << " ---" << endl;
    cout << "Course: " << course.courseName << endl;
    cout << "Total Students: " << totals.size() << endl;
    cout << string(40, '-') << endl;
    cout << "Highest Score: " << fixed << setprecision(2) << highest << endl;
    cout << "Lowest Score: " << fixed << setprecision(2) << lowest << endl;
    cout << "Mean Score: " << fixed << setprecision(2) << mean << endl;
    cout << "Median Score: " << fixed << setprecision(2) << median << endl;
    
    // Grade distribution
    int aCount = 0, bCount = 0, cCount = 0, dCount = 0, fCount = 0;
    for (size_t i = 0; i < totals.size(); i++) {
        char grade = calculateLetterGrade(totals[i]);
        switch(grade) {
            case 'A': aCount++; break;
            case 'B': bCount++; break;
            case 'C': cCount++; break;
            case 'D': dCount++; break;
            case 'F': fCount++; break;
        }
    }
    
    cout << "\nGrade Distribution:" << endl;
    cout << "A: " << aCount << " (" << fixed << setprecision(1) 
         << (static_cast<double>(aCount)/totals.size()*100) << "%)" << endl;
    cout << "B: " << bCount << " (" << fixed << setprecision(1) 
         << (static_cast<double>(bCount)/totals.size()*100) << "%)" << endl;
    cout << "C: " << cCount << " (" << fixed << setprecision(1) 
         << (static_cast<double>(cCount)/totals.size()*100) << "%)" << endl;
    cout << "D: " << dCount << " (" << fixed << setprecision(1) 
         << (static_cast<double>(dCount)/totals.size()*100) << "%)" << endl;
    cout << "F: " << fCount << " (" << fixed << setprecision(1) 
         << (static_cast<double>(fCount)/totals.size()*100) << "%)" << endl;
}