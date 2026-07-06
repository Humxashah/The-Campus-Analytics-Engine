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

// Sort students by CGPA in descending order using selection sort
void sortStudentsByCGPA(vector<Student>& students) {
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

// Print merit list - all active students sorted by CGPA descending
void printMeritList() {
    cout << "\n--- MERIT LIST ---" << endl;
    
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
    
    // Sort by CGPA descending
    sortStudentsByCGPA(activeStudents);
    
    // Display merit list with rank column
    cout << "\n" << left << setw(10) << "Rank" 
         << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Department" 
         << setw(8) << "CGPA" << endl;
    cout << string(78, '-') << endl;
    
    for (size_t i = 0; i < activeStudents.size(); i++) {
        int rank = i + 1;
        cout << left << setw(10) << rank
             << setw(15) << activeStudents[i].rollNumber
             << setw(30) << activeStudents[i].name
             << setw(15) << activeStudents[i].department
             << setw(8) << fixed << setprecision(2) << activeStudents[i].cgpa << endl;
    }
    
    cout << "\nTotal Students: " << activeStudents.size() << endl;
}

// Print attendance defaulters - students with any course attendance < 75%
void printAttendanceDefaulters() {
    cout << "\n--- ATTENDANCE DEFAULTERS REPORT ---" << endl;
    
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<string> studentRolls;
    
    // Get unique student roll numbers
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4 && enrollmentData[i][3] == "enrolled") {
            bool exists = false;
            for (size_t j = 0; j < studentRolls.size(); j++) {
                if (studentRolls[j] == enrollmentData[i][0]) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                studentRolls.push_back(enrollmentData[i][0]);
            }
        }
    }
    
    bool hasDefaulters = false;
    
    cout << "\n" << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Course" 
         << setw(15) << "Attendance %" << endl;
    cout << string(75, '-') << endl;
    
    for (size_t i = 0; i < studentRolls.size(); i++) {
        Student student = findStudentByRoll(studentRolls[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        // Get courses this student is enrolled in
        vector<string> studentCourses;
        for (size_t j = 0; j < enrollmentData.size(); j++) {
            if (enrollmentData[j].size() >= 4) {
                if (enrollmentData[j][0] == studentRolls[i] && enrollmentData[j][3] == "enrolled") {
                    studentCourses.push_back(enrollmentData[j][1]);
                }
            }
        }
        
        for (size_t j = 0; j < studentCourses.size(); j++) {
            double pct = getAttendancePct(studentRolls[i], studentCourses[j]);
            if (pct < 75.0) {
                hasDefaulters = true;
                Course course = findCourseByCode(studentCourses[j]);
                cout << left << setw(15) << student.rollNumber
                     << setw(30) << student.name
                     << setw(15) << (course.courseCode != "NULL" ? course.courseCode : studentCourses[j])
                     << setw(15) << fixed << setprecision(2) << pct << "%" << endl;
            }
        }
    }
    
    if (!hasDefaulters) {
        cout << "No attendance defaulters found!" << endl;
    }
}

// Print fee defaulters - formatted with outstanding amount and weeks overdue
void printFeeDefaulters() {
    cout << "\n--- FEE DEFAULTERS REPORT ---" << endl;
    
    vector<vector<string> > data = readTXT("fees.txt");
    vector<FeeRecord> defaulters;
    
    // Current date for comparison
    string currentDate = "30-01-2024";
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 6) {
            FeeRecord record;
            record.rollNumber = data[i][0];
            record.semester = data[i][1];
            record.amountDue = atof(data[i][2].c_str());
            record.amountPaid = atof(data[i][3].c_str());
            record.dueDate = data[i][4];
            record.paidDate = data[i][5];
            
            double outstanding = record.amountDue - record.amountPaid;
            
            if (outstanding > 0) {
                int daysDiff = daysBetween(record.dueDate, currentDate);
                if (daysDiff > 0) {
                    record.amountDue = outstanding; // Store outstanding as amount due for sorting
                    defaulters.push_back(record);
                }
            }
        }
    }
    
    if (defaulters.empty()) {
        cout << "No fee defaulters found!" << endl;
        return;
    }
    
    // Sort by outstanding amount using bubble sort
    bubbleSortDefaulters(defaulters);
    
    cout << "\n" << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Semester" 
         << setw(15) << "Outstanding" 
         << setw(15) << "Weeks Overdue" << endl;
    cout << string(90, '-') << endl;
    
    for (size_t i = 0; i < defaulters.size(); i++) {
        Student student = findStudentByRoll(defaulters[i].rollNumber);
        if (student.rollNumber != "NULL") {
            double outstanding = defaulters[i].amountDue;
            int weeksOverdue = daysBetween(defaulters[i].dueDate, currentDate) / 7;
            
            cout << left << setw(15) << student.rollNumber
                 << setw(30) << student.name
                 << setw(15) << defaulters[i].semester
                 << setw(15) << fixed << setprecision(2) << outstanding
                 << setw(15) << weeksOverdue << " weeks" << endl;
        }
    }
    
    cout << "\nTotal Defaulters: " << defaulters.size() << endl;
}

// Print semester result - full result sheet with borders
void printSemesterResult() {
    cout << "\n--- SEMESTER RESULT ---" << endl;
    
    string semester;
    cout << "Enter Semester: ";
    cin >> semester;
    
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<string> studentRolls;
    
    // Get unique student roll numbers for this semester
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][2] == semester && enrollmentData[i][3] == "enrolled") {
                bool exists = false;
                for (size_t j = 0; j < studentRolls.size(); j++) {
                    if (studentRolls[j] == enrollmentData[i][0]) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    studentRolls.push_back(enrollmentData[i][0]);
                }
            }
        }
    }
    
    if (studentRolls.empty()) {
        cout << "No students found for this semester!" << endl;
        return;
    }
    
    cout << "\n" << string(100, '=') << endl;
    cout << setw(50) << "SEMESTER RESULT SHEET - " << semester << endl;
    cout << string(100, '=') << endl;
    
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "CGPA" 
         << setw(15) << "GPA" 
         << setw(20) << "Attendance Status" << endl;
    cout << string(100, '-') << endl;
    
    for (size_t i = 0; i < studentRolls.size(); i++) {
        Student student = findStudentByRoll(studentRolls[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        double gpa = computeGPA(studentRolls[i], semester);
        
        // Check attendance status for all courses
        bool hasShortage = false;
        vector<vector<string> > enrollments = readTXT("enrollments.txt");
        for (size_t j = 0; j < enrollments.size(); j++) {
            if (enrollments[j].size() >= 4) {
                if (enrollments[j][0] == studentRolls[i] && 
                    enrollments[j][2] == semester && 
                    enrollments[j][3] == "enrolled") {
                    double attPct = getAttendancePct(studentRolls[i], enrollments[j][1]);
                    if (attPct < 75.0) {
                        hasShortage = true;
                        break;
                    }
                }
            }
        }
        
        string attendanceStatus = hasShortage ? "SHORTAGE" : "OK";
        
        cout << left << setw(15) << student.rollNumber
             << setw(30) << student.name
             << setw(15) << fixed << setprecision(2) << student.cgpa
             << setw(15) << fixed << setprecision(2) << gpa
             << setw(20) << attendanceStatus << endl;
    }
    
    cout << string(100, '=') << endl;
}

// Print department summary - group by dept using nested loops
void printDepartmentSummary() {
    cout << "\n--- DEPARTMENT SUMMARY ---" << endl;
    
    vector<vector<string> > data = readTXT("students.txt");
    vector<string> departments;
    vector<int> counts;
    vector<double> totalCGPA;
    vector<int> passCount;
    
    // Collect all departments using nested loops
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][4] == "active") {
            string dept = data[i][2];
            
            // Check if department already exists in parallel arrays
            bool found = false;
            for (size_t j = 0; j < departments.size(); j++) {
                if (departments[j] == dept) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                departments.push_back(dept);
                counts.push_back(0);
                totalCGPA.push_back(0.0);
                passCount.push_back(0);
            }
        }
    }
    
    // Populate data using nested loops
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][4] == "active") {
            string dept = data[i][2];
            double cgpa = atof(data[i][3].c_str());
            
            for (size_t j = 0; j < departments.size(); j++) {
                if (departments[j] == dept) {
                    counts[j]++;
                    totalCGPA[j] += cgpa;
                    if (cgpa >= 2.0) {
                        passCount[j]++;
                    }
                    break;
                }
            }
        }
    }
    
    cout << "\n" << left << setw(20) << "Department" 
         << setw(15) << "Students" 
         << setw(15) << "Avg CGPA" 
         << setw(15) << "Pass Rate" << endl;
    cout << string(65, '-') << endl;
    
    for (size_t i = 0; i < departments.size(); i++) {
        double avgCGPA = (counts[i] > 0) ? totalCGPA[i] / counts[i] : 0.0;
        double passRate = (counts[i] > 0) ? (static_cast<double>(passCount[i]) / counts[i]) * 100.0 : 0.0;
        
        cout << left << setw(20) << departments[i]
             << setw(15) << counts[i]
             << setw(15) << fixed << setprecision(2) << avgCGPA
             << setw(15) << fixed << setprecision(2) << passRate << "%" << endl;
    }
    
    cout << string(65, '-') << endl;
}

// Export report to file - redirects cout to ofstream
void exportReportToFile(const string& filename, void (*reportFunction)()) {
    // Save original cout buffer
    streambuf* originalCoutBuffer = cout.rdbuf();
    
    // Open file for writing
    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "Error: Cannot create file " << filename << "!" << endl;
        return;
    }
    
    // Redirect cout to file
    cout.rdbuf(file.rdbuf());
    
    // Call the report function
    reportFunction();
    
    // Restore original cout buffer
    cout.rdbuf(originalCoutBuffer);
    file.close();
    
    cout << "Report exported to " << filename << " successfully!" << endl;
}