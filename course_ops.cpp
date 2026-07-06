#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// Parse course line from CSV
Course parseCourseLine(const string& line) {
    Course course;
    vector<string> fields = parseCSVLine(line);
    
    if (fields.size() >= 5) {
        course.courseCode = fields[0];
        course.courseName = fields[1];
        course.creditHours = atoi(fields[2].c_str());
        course.maxSeats = atoi(fields[3].c_str());
        course.prerequisite = fields[4];
    }
    
    return course;
}

// Find course by code
Course findCourseByCode(const string& courseCode) {
    vector<vector<string> > data = readTXT("courses.txt");
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 5 && data[i][0] == courseCode) {
            Course course;
            course.courseCode = data[i][0];
            course.courseName = data[i][1];
            course.creditHours = atoi(data[i][2].c_str());
            course.maxSeats = atoi(data[i][3].c_str());
            course.prerequisite = data[i][4];
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
    vector<vector<string> > data = readTXT("enrollments.txt");
    int enrolledCount = 0;
    
    // Count active enrollments for this course
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 4) {
            if (data[i][1] == courseCode && data[i][3] == "enrolled") {
                enrolledCount++;
            }
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
    vector<vector<string> > data = readTXT("enrollments.txt");
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 4) {
            if (data[i][0] == roll && data[i][1] == courseCode && data[i][3] == "enrolled") {
                return true;
            }
        }
    }
    
    return false;
}

// Check prerequisite - looks up prereq field, checks grades file for non-F grade
bool checkPrerequisite(const string& roll, const string& courseCode) {
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        return false;
    }
    
    // If no prerequisite, it's satisfied
    if (course.prerequisite == "NONE" || course.prerequisite == "") {
        return true;
    }
    
    // Check grades file to confirm student has a non-F grade in prerequisite
    vector<vector<string> > gradesData = readTXT("grades.txt");
    for (size_t i = 0; i < gradesData.size(); i++) {
        if (gradesData[i].size() >= 3) {
            if (gradesData[i][0] == roll && gradesData[i][1] == course.prerequisite) {
                // Check if grade is non-F (assuming grade is at position 2 or later)
                if (gradesData[i].size() > 2) {
                    string grade = gradesData[i][gradesData[i].size() - 1];
                    if (grade != "F" && grade != "F") {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

// Get credit load - sums credit hours of all active enrollments for given semester
int getCreditLoad(const string& roll, const string& semester) {
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    int totalCredits = 0;
    
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][0] == roll && 
                enrollmentData[i][2] == semester && 
                enrollmentData[i][3] == "enrolled") {
                
                Course course = findCourseByCode(enrollmentData[i][1]);
                if (course.courseCode != "NULL") {
                    totalCredits += course.creditHours;
                }
            }
        }
    }
    
    return totalCredits;
}

// Convert EnrollResult to string message
string getEnrollResultMessage(EnrollResult result) {
    switch(result) {
        case ENROLL_SUCCESS:
            return "Student enrolled successfully!";
        case ENROLL_STUDENT_NOT_ACTIVE:
            return "Error: Student is not active!";
        case ENROLL_COURSE_NOT_FOUND:
            return "Error: Course not found!";
        case ENROLL_NO_SEATS:
            return "Error: No seats available in this course!";
        case ENROLL_ALREADY_ENROLLED:
            return "Error: Student is already enrolled in this course!";
        case ENROLL_CREDIT_LIMIT_EXCEEDED:
            return "Error: Credit hours limit (21) exceeded!";
        case ENROLL_PREREQUISITE_NOT_MET:
            return "Error: Prerequisite not completed with a passing grade!";
        default:
            return "Unknown error!";
    }
}

// Enroll student - checks all conditions
EnrollResult enrollStudent() {
    cout << "\n--- ENROLL STUDENT IN COURSE ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists and is active
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        return ENROLL_STUDENT_NOT_ACTIVE;
    }
    
    string courseCode;
    cout << "Enter Course Code: ";
    cin >> courseCode;
    
    // Check if course exists
    Course course = findCourseByCode(courseCode);
    if (course.courseCode == "NULL") {
        return ENROLL_COURSE_NOT_FOUND;
    }
    
    // Check duplicate enrollment
    if (isDuplicateEnrollment(roll, courseCode)) {
        return ENROLL_ALREADY_ENROLLED;
    }
    
    // Check credit hours limit (max 21)
    string semester = "Fall2024"; // Current semester - could be made dynamic
    int currentCredits = getCreditLoad(roll, semester);
    if (currentCredits + course.creditHours > 21) {
        return ENROLL_CREDIT_LIMIT_EXCEEDED;
    }
    
    // Check prerequisite
    if (!checkPrerequisite(roll, courseCode)) {
        return ENROLL_PREREQUISITE_NOT_MET;
    }
    
    // Check seat availability
    if (!checkSeatAvailability(courseCode)) {
        return ENROLL_NO_SEATS;
    }
    
    // Enroll student
    vector<string> row;
    row.push_back(roll);
    row.push_back(courseCode);
    row.push_back(semester);
    row.push_back("enrolled");
    
    if (appendTXT("enrollments.txt", row)) {
        cout << "Course: " << course.courseName << " (" << course.creditHours << " credits)" << endl;
        cout << "Total credits after enrollment: " << (currentCredits + course.creditHours) << endl;
        return ENROLL_SUCCESS;
    }
    
    return ENROLL_COURSE_NOT_FOUND; // Fallback error
}

// Drop course - only permitted if no attendance rows exist
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
    
    string semester;
    cout << "Enter Semester: ";
    cin >> semester;
    
    // Check if enrolled
    if (!isDuplicateEnrollment(roll, courseCode)) {
        cout << "Error: Student is not enrolled in this course!" << endl;
        return;
    }
    
    // Check if attendance rows exist for this student+course+semester
    vector<vector<string> > attendanceData = readTXT("attendance_log.txt");
    bool hasAttendance = false;
    for (size_t i = 0; i < attendanceData.size(); i++) {
        if (attendanceData[i].size() >= 4) {
            // attendance_log format: roll, course_code, date, status
            if (attendanceData[i][0] == roll && attendanceData[i][1] == courseCode) {
                hasAttendance = true;
                break;
            }
        }
    }
    
    if (hasAttendance) {
        cout << "Error: Cannot drop course - attendance records already exist!" << endl;
        cout << "Course can only be dropped before attendance is marked." << endl;
        return;
    }
    
    // Read all enrollments
    vector<vector<string> > data = readTXT("enrollments.txt");
    bool found = false;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 4) {
            if (data[i][0] == roll && data[i][1] == courseCode && 
                data[i][2] == semester && data[i][3] == "enrolled") {
                // Update status to 'dropped'
                data[i][3] = "dropped";
                found = true;
                break;
            }
        }
    }
    
    if (found) {
        // Get header
        vector<string> header;
        header.push_back("roll");
        header.push_back("course_code");
        header.push_back("semester");
        header.push_back("status");
        
        if (writeTXT("enrollments.txt", header, data)) {
            cout << "Course dropped successfully!" << endl;
        } else {
            cout << "Error: Failed to drop course!" << endl;
        }
    } else {
        cout << "Error: Enrollment record not found!" << endl;
    }
}

// List enrolled students - returns list of all active enrolled students in a course
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
    
    vector<vector<string> > enrollmentData = readTXT("enrollments.txt");
    vector<string> enrolledStudents;
    
    for (size_t i = 0; i < enrollmentData.size(); i++) {
        if (enrollmentData[i].size() >= 4) {
            if (enrollmentData[i][1] == courseCode && enrollmentData[i][3] == "enrolled") {
                enrolledStudents.push_back(enrollmentData[i][0]);
            }
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
         << setw(15) << "Department" << endl;
    cout << string(60, '-') << endl;
    
    for (size_t i = 0; i < enrolledStudents.size(); i++) {
        Student student = findStudentByRoll(enrolledStudents[i]);
        if (student.rollNumber != "NULL") {
            cout << left << setw(15) << student.rollNumber
                 << setw(30) << student.name
                 << setw(15) << student.department << endl;
        }
    }
}