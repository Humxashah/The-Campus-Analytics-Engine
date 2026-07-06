#include "reports.h"
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

// Sort students by CGPA (descending)
void sortByCGPA(vector<Student>& students) {
    int n = students.size();
    
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (students[j].cgpa > students[maxIndex].cgpa) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            Student temp = students[i];
            students[i] = students[maxIndex];
            students[maxIndex] = temp;
        }
    }
}

// Export report to file
void exportReportToFile(const string& filename, const vector<string>& reportLines) {
    if (writeTXT(filename, reportLines)) {
        cout << "Report exported to " << filename << " successfully!" << endl;
    } else {
        cout << "Error: Failed to export report!" << endl;
    }
}

// Generate merit list
void generateMeritList() {
    cout << "\n--- MERIT LIST ---" << endl;
    
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
    
    // Sort by CGPA
    sortByCGPA(activeStudents);
    
    // Display merit list
    cout << "\n" << left << setw(10) << "Rank" 
         << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(25) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(88, '-') << endl;
    
    vector<string> reportLines;
    reportLines.push_back("========================================");
    reportLines.push_back("           MERIT LIST                  ");
    reportLines.push_back("========================================");
    reportLines.push_back("");
    
    for (size_t i = 0; i < activeStudents.size(); i++) {
        int rank = i + 1;
        cout << left << setw(10) << rank
             << setw(15) << activeStudents[i].rollNumber
             << setw(30) << activeStudents[i].name
             << setw(25) << activeStudents[i].department
             << setw(8) << fixed << setprecision(2) << activeStudents[i].cgpa << endl;
        
        stringstream ss;
        ss << "Rank " << rank << ": " << activeStudents[i].name << " (" 
           << activeStudents[i].rollNumber << ") - CGPA: " << fixed << setprecision(2) 
           << activeStudents[i].cgpa;
        reportLines.push_back(ss.str());
    }
    
    cout << "\nTotal Students: " << activeStudents.size() << endl;
    
    // Export to file
    reportLines.push_back("");
    reportLines.push_back("Total Students: " + to_string(activeStudents.size()));
    
    exportReportToFile("merit_list.txt", reportLines);
}

// Generate attendance defaulters
void generateAttendanceDefaulters() {
    cout << "\n--- ATTENDANCE DEFAULTERS REPORT ---" << endl;
    
    vector<string> attendanceLines = readTXT("attendance_log.txt");
    vector<string> studentRolls;
    
    // Get unique student roll numbers
    for (size_t i = 0; i < attendanceLines.size(); i++) {
        AttendanceRecord record = parseAttendanceLine(attendanceLines[i]);
        
        bool exists = false;
        for (size_t j = 0; j < studentRolls.size(); j++) {
            if (studentRolls[j] == record.rollNumber) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            studentRolls.push_back(record.rollNumber);
        }
    }
    
    vector<Student> defaulters;
    vector<string> reportLines;
    reportLines.push_back("========================================");
    reportLines.push_back("     ATTENDANCE DEFAULTERS REPORT       ");
    reportLines.push_back("========================================");
    reportLines.push_back("");
    
    for (size_t i = 0; i < studentRolls.size(); i++) {
        Student student = findStudentByRoll(studentRolls[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        // Check all courses for this student
        vector<string> enrollmentLines = readTXT("enrollments.txt");
        bool hasShortage = false;
        string shortageInfo = "";
        
        for (size_t j = 0; j < enrollmentLines.size(); j++) {
            stringstream ss(enrollmentLines[j]);
            string roll, code, status;
            getline(ss, roll, '|');
            getline(ss, code, '|');
            getline(ss, status, '|');
            
            if (roll == studentRolls[i] && status == "A") {
                double percentage = calculateAttendancePercentage(roll, code);
                if (percentage < 75.0) {
                    hasShortage = true;
                    shortageInfo += code + " (" + to_string(static_cast<int>(percentage)) + "%), ";
                }
            }
        }
        
        if (hasShortage) {
            defaulters.push_back(student);
            stringstream ss;
            ss << student.name << " (" << student.rollNumber << ") - " << shortageInfo;
            reportLines.push_back(ss.str());
        }
    }
    
    if (defaulters.empty()) {
        cout << "No attendance defaulters found!" << endl;
        reportLines.push_back("No attendance defaulters found.");
    } else {
        cout << "\n" << left << setw(15) << "Roll Number" 
             << setw(30) << "Name" 
             << setw(25) << "Department" << endl;
        cout << string(70, '-') << endl;
        
        for (size_t i = 0; i < defaulters.size(); i++) {
            cout << left << setw(15) << defaulters[i].rollNumber
                 << setw(30) << defaulters[i].name
                 << setw(25) << defaulters[i].department << endl;
        }
        
        cout << "\nTotal Attendance Defaulters: " << defaulters.size() << endl;
    }
    
    reportLines.push_back("");
    reportLines.push_back("Total Defaulters: " + to_string(defaulters.size()));
    
    exportReportToFile("attendance_defaulters.txt", reportLines);
}

// Generate fee defaulters
void generateFeeDefaulters() {
    cout << "\n--- FEE DEFAULTERS REPORT ---" << endl;
    
    vector<string> lines = readTXT("fees.txt");
    vector<FeeRecord> allFees;
    
    for (size_t i = 0; i < lines.size(); i++) {
        FeeRecord record = parseFeeLine(lines[i]);
        allFees.push_back(record);
    }
    
    vector<Student> defaulters;
    vector<string> processedRolls;
    vector<string> reportLines;
    reportLines.push_back("========================================");
    reportLines.push_back("       FEE DEFAULTERS REPORT            ");
    reportLines.push_back("========================================");
    reportLines.push_back("");
    
    for (size_t i = 0; i < allFees.size(); i++) {
        if (allFees[i].status == "Unpaid") {
            bool alreadyProcessed = false;
            for (size_t j = 0; j < processedRolls.size(); j++) {
                if (processedRolls[j] == allFees[i].rollNumber) {
                    alreadyProcessed = true;
                    break;
                }
            }
            
            if (!alreadyProcessed) {
                Student student = findStudentByRoll(allFees[i].rollNumber);
                if (student.rollNumber != "NULL") {
                    defaulters.push_back(student);
                    processedRolls.push_back(allFees[i].rollNumber);
                    
                    stringstream ss;
                    ss << student.name << " (" << student.rollNumber << ") - Amount Due: Rs. 50,000";
                    reportLines.push_back(ss.str());
                }
            }
        }
    }
    
    if (defaulters.empty()) {
        cout << "No fee defaulters found!" << endl;
        reportLines.push_back("No fee defaulters found.");
    } else {
        cout << "\n" << left << setw(15) << "Roll Number" 
             << setw(30) << "Name" 
             << setw(25) << "Department" 
             << setw(15) << "Amount Due" << endl;
        cout << string(85, '-') << endl;
        
        for (size_t i = 0; i < defaulters.size(); i++) {
            cout << left << setw(15) << defaulters[i].rollNumber
                 << setw(30) << defaulters[i].name
                 << setw(25) << defaulters[i].department
                 << setw(15) << "Rs. 50,000" << endl;
        }
        
        cout << "\nTotal Fee Defaulters: " << defaulters.size() << endl;
    }
    
    reportLines.push_back("");
    reportLines.push_back("Total Defaulters: " + to_string(defaulters.size()));
    
    exportReportToFile("fee_defaulters.txt", reportLines);
}

// Generate semester result
void generateSemesterResult() {
    cout << "\n--- GENERATE SEMESTER RESULT ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    vector<string> reportLines;
    reportLines.push_back("========================================");
    reportLines.push_back("       SEMESTER RESULT REPORT           ");
    reportLines.push_back("========================================");
    reportLines.push_back("");
    reportLines.push_back("Student Name: " + student.name);
    reportLines.push_back("Roll Number: " + student.rollNumber);
    reportLines.push_back("Department: " + student.department);
    reportLines.push_back("CGPA: " + to_string(student.cgpa));
    reportLines.push_back("");
    reportLines.push_back("----------------------------------------");
    reportLines.push_back("COURSE RESULTS:");
    reportLines.push_back("----------------------------------------");
    
    cout << "\n--- SEMESTER RESULT FOR " << student.name << " ---" << endl;
    cout << "Student: " << student.name << " (" << roll << ")" << endl;
    cout << "Department: " << student.department << endl;
    cout << "\n" << left << setw(15) << "Course" 
         << setw(15) << "Credits" 
         << setw(15) << "Total" 
         << setw(10) << "Grade" << endl;
    cout << string(55, '-') << endl;
    
    vector<string> gradeLines = readTXT("grades.txt");
    double totalPoints = 0;
    int totalCredits = 0;
    
    for (size_t i = 0; i < gradeLines.size(); i++) {
        GradeRecord record = parseGradeLine(gradeLines[i]);
        if (record.rollNumber == roll) {
            Course course = findCourseByCode(record.courseCode);
            if (course.courseCode == "NULL") {
                continue;
            }
            
            // Calculate total
            double quizzes[5] = {record.quiz1, record.quiz2, record.quiz3, 
                                record.quiz4, record.quiz5};
            
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
            
            total = applyAttendancePenalty(roll, record.courseCode, total);
            
            char grade = calculateLetterGrade(total);
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
                 << setw(10) << grade << endl;
            
            stringstream ss;
            ss << course.courseCode << " | Credits: " << course.creditHours 
               << " | Total: " << fixed << setprecision(2) << total 
               << " | Grade: " << grade;
            reportLines.push_back(ss.str());
            
            totalPoints += gradePoints * course.creditHours;
            totalCredits += course.creditHours;
        }
    }
    
    if (totalCredits > 0) {
        double gpa = totalPoints / totalCredits;
        cout << string(55, '-') << endl;
        cout << "Total Credits: " << totalCredits << endl;
        cout << "GPA: " << fixed << setprecision(2) << gpa << endl;
        
        reportLines.push_back("");
        reportLines.push_back("----------------------------------------");
        reportLines.push_back("SUMMARY:");
        reportLines.push_back("Total Credits: " + to_string(totalCredits));
        reportLines.push_back("GPA: " + to_string(gpa));
        reportLines.push_back("========================================");
        
        exportReportToFile("semester_result_" + roll + ".txt", reportLines);
    } else {
        cout << "No courses found for this student!" << endl;
        reportLines.push_back("No courses found for this student.");
        exportReportToFile("semester_result_" + roll + ".txt", reportLines);
    }
}

// Generate department summary
void generateDepartmentSummary() {
    cout << "\n--- DEPARTMENT SUMMARY ---" << endl;
    
    string department;
    cin.ignore();
    cout << "Enter Department Name: ";
    getline(cin, department);
    
    vector<string> lines = readTXT("students.txt");
    vector<Student> deptStudents;
    
    for (size_t i = 0; i < lines.size(); i++) {
        Student student = parseStudentLine(lines[i]);
        if (student.status == "active" && student.department == department) {
            deptStudents.push_back(student);
        }
    }
    
    if (deptStudents.empty()) {
        cout << "No active students found in this department!" << endl;
        return;
    }
    
    vector<string> reportLines;
    reportLines.push_back("========================================");
    reportLines.push_back("       DEPARTMENT SUMMARY REPORT        ");
    reportLines.push_back("========================================");
    reportLines.push_back("");
    reportLines.push_back("Department: " + department);
    reportLines.push_back("Total Students: " + to_string(deptStudents.size()));
    reportLines.push_back("");
    reportLines.push_back("----------------------------------------");
    reportLines.push_back("STUDENT LIST:");
    reportLines.push_back("----------------------------------------");
    
    cout << "\n--- DEPARTMENT SUMMARY FOR " << department << " ---" << endl;
    cout << "Total Students: " << deptStudents.size() << endl;
    cout << "\n" << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(8) << "CGPA" << endl;
    cout << string(53, '-') << endl;
    
    double totalCGPA = 0;
    
    for (size_t i = 0; i < deptStudents.size(); i++) {
        cout << left << setw(15) << deptStudents[i].rollNumber
             << setw(30) << deptStudents[i].name
             << setw(8) << fixed << setprecision(2) << deptStudents[i].cgpa << endl;
        
        stringstream ss;
        ss << deptStudents[i].rollNumber << " | " << deptStudents[i].name 
           << " | CGPA: " << fixed << setprecision(2) << deptStudents[i].cgpa;
        reportLines.push_back(ss.str());
        
        totalCGPA += deptStudents[i].cgpa;
    }
    
    double avgCGPA = totalCGPA / deptStudents.size();
    cout << string(53, '-') << endl;
    cout << "Average CGPA: " << fixed << setprecision(2) << avgCGPA << endl;
    
    reportLines.push_back("");
    reportLines.push_back("----------------------------------------");
    reportLines.push_back("Average CGPA: " + to_string(avgCGPA));
    reportLines.push_back("========================================");
    
    exportReportToFile("dept_summary_" + department + ".txt", reportLines);
}