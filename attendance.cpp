#include "attendance.h"
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// Parse attendance line
AttendanceRecord parseAttendanceLine(const string& line) {
    AttendanceRecord record;
    stringstream ss(line);
    
    getline(ss, record.date, '|');
    getline(ss, record.rollNumber, '|');
    getline(ss, record.courseCode, '|');
    getline(ss, record.status, '|');
    
    return record;
}

// Calculate attendance percentage
double calculateAttendancePercentage(const string& roll, const string& courseCode) {
    vector<string> lines = readTXT("attendance_log.txt");
    int totalSessions = 0;
    int presentSessions = 0;
    
    for (size_t i = 0; i < lines.size(); i++) {
        AttendanceRecord record = parseAttendanceLine(lines[i]);
        if (record.rollNumber == roll && record.courseCode == courseCode) {
            totalSessions++;
            if (record.status == "Present") {
                presentSessions++;
            }
        }
    }
    
    if (totalSessions == 0) {
        return 0.0;
    }
    
    return (static_cast<double>(presentSessions) / totalSessions) * 100.0;
}

// Mark attendance
void markAttendance() {
    cout << "\n--- MARK ATTENDANCE ---" << endl;
    
    string date;
    cout << "Enter Date (YYYY-MM-DD): ";
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
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    vector<string> enrolledStudents;
    
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string roll, code, status;
        getline(ss, roll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (code == courseCode && status == "A") {
            enrolledStudents.push_back(roll);
        }
    }
    
    if (enrolledStudents.empty()) {
        cout << "No students enrolled in this course!" << endl;
        return;
    }
    
    cout << "\nMarking attendance for " << enrolledStudents.size() << " students..." << endl;
    cout << "Enter 'P' for Present, 'A' for Absent" << endl;
    cout << string(50, '-') << endl;
    
    vector<AttendanceRecord> records;
    
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
            
            if (status != 'P' && status != 'A') {
                cout << "Invalid input! Enter 'P' or 'A': ";
            }
        } while (status != 'P' && status != 'A');
        
        AttendanceRecord record;
        record.date = date;
        record.rollNumber = student.rollNumber;
        record.courseCode = courseCode;
        record.status = (status == 'P') ? "Present" : "Absent";
        records.push_back(record);
    }
    
    // Save all records
    bool success = true;
    for (size_t i = 0; i < records.size(); i++) {
        string line = records[i].date + "|" + 
                     records[i].rollNumber + "|" + 
                     records[i].courseCode + "|" + 
                     records[i].status;
        if (!appendTXT("attendance_log.txt", line)) {
            success = false;
        }
    }
    
    if (success) {
        cout << "\nAttendance marked successfully for " << records.size() << " students!" << endl;
        
        // Display summary
        int presentCount = 0;
        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].status == "Present") {
                presentCount++;
            }
        }
        cout << "Present: " << presentCount << " students" << endl;
        cout << "Absent: " << (records.size() - presentCount) << " students" << endl;
    } else {
        cout << "Error: Failed to save attendance records!" << endl;
    }
}

// View attendance percentage
void viewAttendancePercentage() {
    cout << "\n--- VIEW ATTENDANCE PERCENTAGE ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    // Get all courses the student is enrolled in
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    vector<string> enrolledCourses;
    
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && status == "A") {
            enrolledCourses.push_back(code);
        }
    }
    
    if (enrolledCourses.empty()) {
        cout << "Student is not enrolled in any course!" << endl;
        return;
    }
    
    cout << "\n--- ATTENDANCE PERCENTAGE FOR " << student.name << " ---" << endl;
    cout << left << setw(15) << "Course Code" 
         << setw(35) << "Course Name" 
         << setw(15) << "Percentage" 
         << setw(10) << "Status" << endl;
    cout << string(75, '-') << endl;
    
    bool hasShortage = false;
    
    for (size_t i = 0; i < enrolledCourses.size(); i++) {
        Course course = findCourseByCode(enrolledCourses[i]);
        if (course.courseCode == "NULL") {
            continue;
        }
        
        double percentage = calculateAttendancePercentage(roll, enrolledCourses[i]);
        string status = (percentage >= 75.0) ? "OK" : "SHORTAGE";
        
        if (percentage < 75.0) {
            hasShortage = true;
        }
        
        cout << left << setw(15) << course.courseCode
             << setw(35) << course.courseName
             << setw(15) << fixed << setprecision(2) << percentage << "%"
             << setw(10) << status << endl;
    }
    
    if (hasShortage) {
        cout << "\nWARNING: You have attendance shortage in some courses!" << endl;
        cout << "Minimum required attendance: 75%" << endl;
    } else {
        cout << "\nGood! You have satisfactory attendance in all courses." << endl;
    }
}

// View shortage list
void viewShortageList() {
    cout << "\n--- ATTENDANCE SHORTAGE LIST ---" << endl;
    
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
    
    vector<Student> shortageStudents;
    
    for (size_t i = 0; i < studentRolls.size(); i++) {
        Student student = findStudentByRoll(studentRolls[i]);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        // Check all courses for this student
        vector<string> enrollmentLines = readTXT("enrollments.txt");
        bool hasShortage = false;
        
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
                    break;
                }
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
         << setw(25) << "Department" << endl;
    cout << string(70, '-') << endl;
    
    for (size_t i = 0; i < shortageStudents.size(); i++) {
        cout << left << setw(15) << shortageStudents[i].rollNumber
             << setw(30) << shortageStudents[i].name
             << setw(25) << shortageStudents[i].department << endl;
    }
    
    cout << "\nTotal students with shortage: " << shortageStudents.size() << endl;
}

// Undo last attendance session
void undoLastAttendance() {
    cout << "\n--- UNDO LAST ATTENDANCE SESSION ---" << endl;
    
    vector<string> lines = readTXT("attendance_log.txt");
    
    if (lines.empty()) {
        cout << "No attendance records to undo!" << endl;
        return;
    }
    
    // Get the date of the last session (from the last record)
    string lastDate = parseAttendanceLine(lines[lines.size() - 1]).date;
    
    // Find all records with the last date
    int recordsToRemove = 0;
    for (int i = lines.size() - 1; i >= 0; i--) {
        AttendanceRecord record = parseAttendanceLine(lines[i]);
        if (record.date == lastDate) {
            recordsToRemove++;
        } else {
            break; // Stop at first different date
        }
    }
    
    cout << "Last attendance session: " << lastDate << endl;
    cout << "Records to undo: " << recordsToRemove << endl;
    cout << "Are you sure you want to undo this session? (y/n): ";
    
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) != 'y') {
        cout << "Operation cancelled." << endl;
        return;
    }
    
    // Remove the records
    for (int i = 0; i < recordsToRemove; i++) {
        lines.pop_back();
    }
    
    if (writeTXT("attendance_log.txt", lines)) {
        cout << "Last attendance session undone successfully!" << endl;
        cout << "Removed " << recordsToRemove << " attendance records." << endl;
    } else {
        cout << "Error: Failed to undo attendance session!" << endl;
    }
}

// View daily attendance sheet
void viewDailyAttendance() {
    cout << "\n--- DAILY ATTENDANCE SHEET ---" << endl;
    
    string date;
    cout << "Enter Date (YYYY-MM-DD): ";
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
    
    vector<string> lines = readTXT("attendance_log.txt");
    vector<AttendanceRecord> dayRecords;
    
    for (size_t i = 0; i < lines.size(); i++) {
        AttendanceRecord record = parseAttendanceLine(lines[i]);
        if (record.date == date && record.courseCode == courseCode) {
            dayRecords.push_back(record);
        }
    }
    
    if (dayRecords.empty()) {
        cout << "No attendance records found for " << date << " in " << courseCode << endl;
        return;
    }
    
    cout << "\n--- ATTENDANCE SHEET FOR " << date << " ---" << endl;
    cout << "Course: " << course.courseName << " (" << courseCode << ")" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Status" << endl;
    cout << string(60, '-') << endl;
    
    int presentCount = 0;
    
    for (size_t i = 0; i < dayRecords.size(); i++) {
        Student student = findStudentByRoll(dayRecords[i].rollNumber);
        if (student.rollNumber == "NULL") {
            continue;
        }
        
        cout << left << setw(15) << student.rollNumber
             << setw(30) << student.name
             << setw(15) << dayRecords[i].status << endl;
        
        if (dayRecords[i].status == "Present") {
            presentCount++;
        }
    }
    
    cout << string(60, '-') << endl;
    cout << "Total Students: " << dayRecords.size() << endl;
    cout << "Present: " << presentCount << endl;
    cout << "Absent: " << (dayRecords.size() - presentCount) << endl;
    cout << "Attendance Percentage: " << fixed << setprecision(2) 
         << (static_cast<double>(presentCount) / dayRecords.size()) * 100 << "%" << endl;
}