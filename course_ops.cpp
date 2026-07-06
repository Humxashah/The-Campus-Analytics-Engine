#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// Parse course line
Course parseCourseLine(const string& line) {
    Course course;
    stringstream ss(line);
    
    getline(ss, course.courseCode, '|');
    getline(ss, course.courseName, '|');
    
    string creditStr;
    getline(ss, creditStr, '|');
    course.creditHours = atoi(creditStr.c_str());
    
    string seatsStr;
    getline(ss, seatsStr, '|');
    course.maxSeats = atoi(seatsStr.c_str());
    
    getline(ss, course.prerequisite, '|');
    
    return course;
}

// Find course by code
Course findCourseByCode(const string& courseCode) {
    vector<string> lines = readTXT("courses.txt");
    
    for (size_t i = 0; i < lines.size(); i++) {
        Course course = parseCourseLine(lines[i]);
        if (course.courseCode == courseCode) {
            return course;
        }
    }
    
    // Return empty course
    Course emptyCourse;
    emptyCourse.courseCode = "NULL";
    return emptyCourse;
}

// Check seat availability
bool checkSeatAvailability(const string& courseCode) {
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    int enrolledCount = 0;
    
    // Count active enrollments for this course
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string roll, code, status;
        getline(ss, roll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (code == courseCode && status == "A") {
            enrolledCount++;
        }
    }
    
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        return false;
    }
    
    return enrolledCount < course.maxSeats;
}

// Check duplicate enrollment
bool isDuplicateEnrollment(const string& roll, const string& courseCode) {
    vector<string> lines = readTXT("enrollments.txt");
    
    for (size_t i = 0; i < lines.size(); i++) {
        stringstream ss(lines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && code == courseCode && status == "A") {
            return true;
        }
    }
    
    return false;
}

// Check prerequisite
bool checkPrerequisite(const string& roll, const string& courseCode) {
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        return false;
    }
    
    // If no prerequisite, it's satisfied
    if (course.prerequisite == "none" || course.prerequisite == "") {
        return true;
    }
    
    // Check if student has completed the prerequisite
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && code == course.prerequisite && status == "A") {
            return true;
        }
    }
    
    return false;
}

// Calculate total credits for a student
int calculateTotalCredits(const string& roll) {
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    int totalCredits = 0;
    
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && status == "A") {
            Course course = findCourseByCode(code);
            if (course.courseCode != "NULL") {
                totalCredits += course.creditHours;
            }
        }
    }
    
    return totalCredits;
}

// Enroll student in course
void enrollStudent() {
    cout << "\n--- ENROLL STUDENT IN COURSE ---" << endl;
    
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
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
    // Check duplicate enrollment
    if (isDuplicateEnrollment(roll, courseCode)) {
        cout << "Error: Student is already enrolled in this course!" << endl;
        return;
    }
    
    // Check credit hours limit (max 21)
    int currentCredits = calculateTotalCredits(roll);
    if (currentCredits + course.creditHours > 21) {
        cout << "Error: Credit hours limit exceeded! Current: " << currentCredits 
             << ", Attempting to add: " << course.creditHours 
             << " (Max: 21)" << endl;
        return;
    }
    
    // Check prerequisite
    if (!checkPrerequisite(roll, courseCode)) {
        cout << "Error: Prerequisite " << course.prerequisite << " not completed!" << endl;
        return;
    }
    
    // Check seat availability
    if (!checkSeatAvailability(courseCode)) {
        cout << "Error: No seats available in this course!" << endl;
        return;
    }
    
    // Enroll student
    string enrollmentLine = roll + "|" + courseCode + "|A";
    if (appendTXT("enrollments.txt", enrollmentLine)) {
        cout << "Student enrolled successfully!" << endl;
        cout << "Course: " << course.courseName << " (" << course.creditHours << " credits)" << endl;
        cout << "Total credits after enrollment: " << (currentCredits + course.creditHours) << endl;
    } else {
        cout << "Error: Failed to enroll student!" << endl;
    }
}

// Drop course
void dropCourse() {
    cout << "\n--- DROP COURSE ---" << endl;
    
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
    
    // Check if enrolled
    if (!isDuplicateEnrollment(roll, courseCode)) {
        cout << "Error: Student is not enrolled in this course!" << endl;
        return;
    }
    
    // Read all enrollments
    vector<string> lines = readTXT("enrollments.txt");
    bool found = false;
    
    for (size_t i = 0; i < lines.size(); i++) {
        stringstream ss(lines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && code == courseCode && status == "A") {
            // Update status to "D" (dropped)
            lines[i] = roll + "|" + courseCode + "|D";
            found = true;
            break;
        }
    }
    
    if (found) {
        if (writeTXT("enrollments.txt", lines)) {
            cout << "Course dropped successfully!" << endl;
        } else {
            cout << "Error: Failed to drop course!" << endl;
        }
    } else {
        cout << "Error: Enrollment record not found!" << endl;
    }
}

// View credit load
void viewCreditLoad() {
    cout << "\n--- VIEW CREDIT LOAD ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    int totalCredits = calculateTotalCredits(roll);
    
    cout << "\n--- CREDIT LOAD SUMMARY ---" << endl;
    cout << "Student: " << student.name << " (" << roll << ")" << endl;
    cout << "Total Credit Hours: " << totalCredits << " / 21" << endl;
    
    // List enrolled courses
    vector<string> enrollmentLines = readTXT("enrollments.txt");
    cout << "\nEnrolled Courses:" << endl;
    cout << left << setw(15) << "Course Code" 
         << setw(35) << "Course Name" 
         << setw(15) << "Credits" << endl;
    cout << string(65, '-') << endl;
    
    for (size_t i = 0; i < enrollmentLines.size(); i++) {
        stringstream ss(enrollmentLines[i]);
        string studentRoll, code, status;
        getline(ss, studentRoll, '|');
        getline(ss, code, '|');
        getline(ss, status, '|');
        
        if (studentRoll == roll && status == "A") {
            Course course = findCourseByCode(code);
            if (course.courseCode != "NULL") {
                cout << left << setw(15) << course.courseCode
                     << setw(35) << course.courseName
                     << setw(15) << course.creditHours << endl;
            }
        }
    }
    
    if (totalCredits > 21) {
        cout << "\nWARNING: Credit hours exceed 21 limit!" << endl;
    } else if (21 - totalCredits <= 3) {
        cout << "\nNote: You can add up to " << (21 - totalCredits) << " more credits." << endl;
    }
}

// List enrolled students in a course
void listEnrolledStudents() {
    cout << "\n--- LIST ENROLLED STUDENTS ---" << endl;
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        cout << "Error: Course not found!" << endl;
        return;
    }
    
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
    
    cout << "\n--- STUDENTS ENROLLED IN " << courseCode << " (" << course.courseName << ") ---" << endl;
    cout << "Total Enrolled: " << enrolledStudents.size() << " / " << course.maxSeats << endl;
    cout << "Seats Available: " << (course.maxSeats - enrolledStudents.size()) << endl;
    
    if (enrolledStudents.empty()) {
        cout << "No students enrolled in this course." << endl;
        return;
    }
    
    cout << "\n" << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(25) << "Department" << endl;
    cout << string(70, '-') << endl;
    
    for (size_t i = 0; i < enrolledStudents.size(); i++) {
        Student student = findStudentByRoll(enrolledStudents[i]);
        if (student.rollNumber != "NULL") {
            cout << left << setw(15) << student.rollNumber
                 << setw(30) << student.name
                 << setw(25) << student.department << endl;
        }
    }
}