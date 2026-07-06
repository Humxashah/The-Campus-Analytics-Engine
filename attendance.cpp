#include "attendance.h"
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// Global backup variables
vector<vector<string> > attendanceBackup;
bool hasBackup = false;

// Parse attendance line from CSV
AttendanceRecord parseAttendanceLine(const string& line) {
    AttendanceRecord record;
    vector<string> fields = parseCSVLine(line);
    
    if (fields.size() >= 4) {
        record.rollNumber = fields[0];
        record.courseCode = fields[1];
        record.date = fields[2];
        record.status = fields[3];
    }
    
    return record;
}

// Save backup of current attendance data
void saveAttendanceBackup() {
    // Read current attendance data
    attendanceBackup = readTXT("attendance_log.txt");
    hasBackup = true;
}

// Compute attendance percentage: (present + 0.5 * late) / totalSessions * 100.0
double getAttendancePct(const string& roll, const string& courseCode) {
    vector<vector<string> > data = readTXT("attendance_log.txt");
    int totalSessions = 0;
    double weightedPresent = 0.0;
    
    // Accumulator loop
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 4) {
            if (data[i][0] == roll && data[i][1] == courseCode) {
                totalSessions++;
                if (data[i][3] == "P") {
                    weightedPresent += 1.0; // Present counts as 1
                } else if (data[i][3] == "L") {
                    weightedPresent += 0.5; // Late counts as 0.5
                }
                // Absent counts as 0
            }
        }
    }
    
    if (totalSessions == 0) {
        return 0.0;
    }
    
    return (weightedPresent / totalSessions) * 100.0;
}

// Mark attendance - iterates enrolled students, prompts P/A/L for each
void markAttendance() {
    cout << "\n--- MARK ATTENDANCE ---" << endl;
    
    string date;
    cout << "Enter Date (DD-MM-YYYY): ";
    cin >> date;
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
    // Get list of enrolled students
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<string> enrolledStudents;
    
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][1] == courseCode && enrollmentData[i][3] == "enrolled") {
                enrolledStudents.push_back(enrollmentData[i][0]);
            }
        }
    }
    
    if (enrolledStudents.empty()) {
        cout << "No students enrolled in this course!" << endl;
        return;
    }
    
    // Save backup before marking attendance
    saveAttendanceBackup();
    
    cout << "\nMarking attendance for " << enrolledStudents.size() << " students..." << endl;
    cout << "Enter 'P' for Present, 'A' for Absent, 'L' for Late" << endl;
    cout << string(50, '-') << endl;
    
    vector<vector<string> > attendanceRows;
    
    for (size_t i = 0; i < enrolledStudents.size(); i++) {
        Student student = findStudentByRoll(enrolledStudents[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        char status;
        do {
            cout << student.name << " (" << student.rollNumber << "): ";
            cin >> status;
            status = toupper(status);
            
            if (status != 'P' && status != 'A' && status != 'L') {
                cout << "Invalid input! Enter 'P', 'A', or 'L': ";
            }
        } while (status != 'P' && status != 'A' && status != 'L');
        
        vector<string> row;
        row.push_back(student.rollNumber);
        row.push_back(courseCode);
        row.push_back(date);
        row.push_back(string(1, status));
        attendanceRows.push_back(row);
    }
    
    // Append all rows to attendance_log.txt
    bool success = true;
    for (size_t i = 0; i < attendanceRows.size(); i++) {
        if (!appendTXT("attendance_log.txt", attendanceRows[i])) {
            success = false;
        }
    }
    
    if (success) {
        cout << "\nAttendance marked successfully for " << attendanceRows.size() << " students!" << endl;
        
        // Display summary
        int presentCount = 0, lateCount = 0, absentCount = 0;
        for (size_t i = 0; i < attendanceRows.size(); i++) {
            if (attendanceRows[i][3] == "P") presentCount++;
            else if (attendanceRows[i][3] == "L") lateCount++;
            else absentCount++;
        }
        cout << "Present: " << presentCount << " students" << endl;
        cout << "Late: " << lateCount << " students" << endl;
        cout << "Absent: " << absentCount << " students" << endl;
    } else {
        cout << "Error: Failed to save attendance records!" << endl;
    }
}

// Get shortage list - returns students with attendance < 75%
void getShortageList() {
    cout << "\n--- ATTENDANCE SHORTAGE LIST ---" << endl;
    
    vector<vector<string> > attendanceData = readTXT("attendance_log.txt");
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<Student> shortageStudents;
    
    // Get unique student roll numbers from enrollments
    vector<string> studentRolls;
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
    
    // Check each student's attendance in each course
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
        
        bool hasShortage = false;
        for (size_t j = 0; j < studentCourses.size(); j++) {
            double pct = getAttendancePct(studentRolls[i], studentCourses[j]);
            if (pct < 75.0) {
                hasShortage = true;
                break;
            }
        }
        
        if (hasShortage) {
            shortageStudents.push_back(student);
        }
    }
    
    if (shortageStudents.empty()) {
        cout << "No students with attendance shortage found!" << endl;
        return;
    }
    
    cout << "\nStudents with attendance below 75%:" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Department" << endl;
    cout << string(60, '-') << endl;
    
    for (size_t i = 0; i < shortageStudents.size(); i++) {
        cout << left << setw(15) << shortageStudents[i].rollNumber
             << setw(30) << shortageStudents[i].name
             << setw(15) << shortageStudents[i].department << endl;
    }
    
    cout << "\nTotal students with shortage: " << shortageStudents.size() << endl;
}

// Undo last session - restores from backup vector
bool undoLastSession() {
    cout << "\n--- UNDO LAST ATTENDANCE SESSION ---" << endl;
    
    if (!hasBackup || attendanceBackup.empty()) {
        cout << "No backup available to restore!" << endl;
        return false;
    }
    
    cout << "Are you sure you want to undo the last attendance session? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) != 'y') {
        cout << "Operation cancelled." << endl;
        return false;
    }
    
    // Get header
    vector<string> header;
    header.push_back("roll");
    header.push_back("course_code");
    header.push_back("date");
    header.push_back("status");
    
    // Rewrite file with backup data
    if (writeTXT("attendance_log.txt", header, attendanceBackup)) {
        cout << "Last attendance session undone successfully!" << endl;
        cout << "Restored " << attendanceBackup.size() << " attendance records." << endl;
        hasBackup = false;
        return true;
    } else {
        cout << "Error: Failed to restore attendance data!" << endl;
        return false;
    }
}

// Print daily sheet - formatted console table of all enrolled students
void printDailySheet() {
    cout << "\n--- DAILY ATTENDANCE SHEET ---" << endl;
    
    string date;
    cout << "Enter Date (DD-MM-YYYY): ";
    cin >> date;
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
    // Get enrolled students
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<string> enrolledStudents;
    
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][1] == courseCode && enrollmentData[i][3] == "enrolled") {
                enrolledStudents.push_back(enrollmentData[i][0]);
            }
        }
    }
    
    if (enrolledStudents.empty()) {
        cout << "No students enrolled in this course!" << endl;
        return;
    }
    
    // Get attendance for this date and course
    vector<vector<string> > attendanceData = readTXT("attendance_log.txt");
    vector<vector<string> > dayAttendance;
    
    for (size_t i = 0; i < attendanceData.size(); i++) {
        if (attendanceData[i].size() >= 4) {
            if (attendanceData[i][2] == date && attendanceData[i][1] == courseCode) {
                dayAttendance.push_back(attendanceData[i]);
            }
        }
    }
    
    cout << "\n--- ATTENDANCE SHEET FOR " << date << " ---" << endl;
    cout << "Course: " << course.courseName << " (" << courseCode << ")" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Status" << endl;
    cout << string(60, '-') << endl;
    
    int presentCount = 0, lateCount = 0, absentCount = 0;
    int markedCount = 0;
    
    for (size_t i = 0; i < enrolledStudents.size(); i++) {
        Student student = findStudentByRoll(enrolledStudents[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        string status = "Not Marked";
        for (size_t j = 0; j < dayAttendance.size(); j++) {
            if (dayAttendance[j][0] == enrolledStudents[i]) {
                if (dayAttendance[j][3] == "P") {
                    status = "Present";
                    presentCount++;
                } else if (dayAttendance[j][3] == "L") {
                    status = "Late";
                    lateCount++;
                } else if (dayAttendance[j][3] == "A") {
                    status = "Absent";
                    absentCount++;
                }
                markedCount++;
                break;
            }
        }
        
        cout << left << setw(15) << student.rollNumber
             << setw(30) << student.name
             << setw(15) << status << endl;
    }
    
    cout << string(60, '-') << endl;
    cout << "Total Students: " << enrolledStudents.size() << endl;
    cout << "Marked: " << markedCount << endl;
    cout << "Present: " << presentCount << endl;
    cout << "Late: " << lateCount << endl;
    cout << "Absent: " << absentCount << endl;
    if (markedCount > 0) {
        double attendancePct = (static_cast<double>(presentCount + 0.5 * lateCount) / markedCount) * 100.0;
        cout << "Attendance Percentage: " << fixed << setprecision(2) << attendancePct << "%" << endl;
    }
}