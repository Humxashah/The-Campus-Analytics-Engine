#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <limits>

#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"

using namespace std;

// Function prototypes for menu system
void showMainMenu();
void showStudentMenu();
void showCourseMenu();
void showAttendanceMenu();
void showGradesMenu();
void showFeeMenu();
void showReportsMenu();

int main() {
    // Ensure data files exist
    initializeDataFiles();
    
    int choice;
    do {
        showMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Validate input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                showStudentMenu();
                break;
            case 2:
                showCourseMenu();
                break;
            case 3:
                showAttendanceMenu();
                break;
            case 4:
                showGradesMenu();
                break;
            case 5:
                showFeeMenu();
                break;
            case 6:
                showReportsMenu();
                break;
            case 7:
                cout << "Thank you for using the Student Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 7);
    
    return 0;
}

void showMainMenu() {
    cout << "\n=================================================" << endl;
    cout << "    STUDENT MANAGEMENT SYSTEM - MAIN MENU      " << endl;
    cout << "=================================================" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Attendance Management" << endl;
    cout << "4. Grades Management" << endl;
    cout << "5. Fee Tracking" << endl;
    cout << "6. Reports" << endl;
    cout << "7. Exit" << endl;
    cout << "=================================================" << endl;
}

void showStudentMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "          STUDENT MANAGEMENT MENU              " << endl;
        cout << "=================================================" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Search Student by Roll Number" << endl;
        cout << "3. Search Student by Name" << endl;
        cout << "4. Update Student Information" << endl;
        cout << "5. Soft Delete Student" << endl;
        cout << "6. View Active Students" << endl;
        cout << "7. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                searchStudentByRoll();
                break;
            case 3:
                searchByName();  // Fixed: Changed from searchStudentByName to searchByName
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                softDelete();  // Fixed: Changed from softDeleteStudent to softDelete
                break;
            case 6:
                listActiveStudents();  // Fixed: Changed from viewActiveStudents to listActiveStudents
                break;
            case 7:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 7);
}

void showCourseMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "          COURSE MANAGEMENT MENU               " << endl;
        cout << "=================================================" << endl;
        cout << "1. Enroll Student in Course" << endl;
        cout << "2. Drop Course" << endl;
        cout << "3. View Credit Load" << endl;
        cout << "4. List Enrolled Students" << endl;
        cout << "5. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1: {
                EnrollResult result = enrollStudent();
                cout << getEnrollResultMessage(result) << endl;
                break;
            }
            case 2:
                dropCourse();
                break;
            case 3: {
                string roll, semester;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Semester: ";
                cin >> semester;
                int credits = getCreditLoad(roll, semester);
                cout << "Total Credit Hours: " << credits << " / 21" << endl;
                break;
            }
            case 4:
                listEnrolledStudents();
                break;
            case 5:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 5);
}

void showAttendanceMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "         ATTENDANCE MANAGEMENT MENU            " << endl;
        cout << "=================================================" << endl;
        cout << "1. Mark Attendance" << endl;
        cout << "2. View Attendance Percentage" << endl;
        cout << "3. View Shortage List" << endl;
        cout << "4. Undo Last Attendance Session" << endl;
        cout << "5. View Daily Attendance Sheet" << endl;
        cout << "6. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                markAttendance();
                break;
            case 2: {
                string roll, courseCode;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Course Code: ";
                cin >> courseCode;
                double pct = getAttendancePct(roll, courseCode);
                cout << "Attendance Percentage: " << fixed << setprecision(2) << pct << "%" << endl;
                break;
            }
            case 3:
                getShortageList();  // Fixed: Changed from viewShortageList to getShortageList
                break;
            case 4:
                undoLastSession();  // Fixed: Changed from undoLastAttendance to undoLastSession
                break;
            case 5:
                printDailySheet();  // Fixed: Changed from viewDailyAttendance to printDailySheet
                break;
            case 6:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 6);
}

void showGradesMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "           GRADES MANAGEMENT MENU              " << endl;
        cout << "=================================================" << endl;
        cout << "1. Enter Marks" << endl;
        cout << "2. View Best Three Quizzes" << endl;
        cout << "3. View Weighted Total" << endl;
        cout << "4. View Letter Grade" << endl;
        cout << "5. View GPA" << endl;
        cout << "6. View Class Statistics" << endl;
        cout << "7. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                enterMarks();
                break;
            case 2: {
                string roll, courseCode;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Course Code: ";
                cin >> courseCode;
                GradeRecord record = findGradeRecord(roll, courseCode);
                if (record.rollNumber != "NULL") {
                    double bestAvg = bestThreeOffFive(record.quizMarks);
                    cout << "Best Three Quiz Average: " << fixed << setprecision(2) << bestAvg << endl;
                } else {
                    cout << "No grade record found!" << endl;
                }
                break;
            }
            case 3: {
                string roll, courseCode;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Course Code: ";
                cin >> courseCode;
                GradeRecord record = findGradeRecord(roll, courseCode);
                if (record.rollNumber != "NULL") {
                    double quizAvg = bestThreeOffFive(record.quizMarks);
                    double total = computeWeightedTotal(quizAvg, record.assignment, record.midterm, record.finalExam);
                    cout << "Weighted Total: " << fixed << setprecision(2) << total << endl;
                } else {
                    cout << "No grade record found!" << endl;
                }
                break;
            }
            case 4: {
                string roll, courseCode;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Course Code: ";
                cin >> courseCode;
                GradeRecord record = findGradeRecord(roll, courseCode);
                if (record.rollNumber != "NULL") {
                    cout << "Letter Grade: " << record.letterGrade << endl;
                } else {
                    cout << "No grade record found!" << endl;
                }
                break;
            }
            case 5: {
                string roll, semester;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Semester: ";
                cin >> semester;
                double gpa = computeGPA(roll, semester);
                cout << "GPA: " << fixed << setprecision(2) << gpa << endl;
                break;
            }
            case 6: {
                string courseCode;
                cout << "Enter Course Code: ";
                cin >> courseCode;
                Stats stats = computeClassStats(courseCode);
                cout << "\n--- CLASS STATISTICS ---" << endl;
                cout << "Highest: " << fixed << setprecision(2) << stats.highest << endl;
                cout << "Lowest: " << fixed << setprecision(2) << stats.lowest << endl;
                cout << "Mean: " << fixed << setprecision(2) << stats.mean << endl;
                cout << "Median: " << fixed << setprecision(2) << stats.median << endl;
                break;
            }
            case 7:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 7);
}

void showFeeMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "           FEE TRACKING MENU                   " << endl;
        cout << "=================================================" << endl;
        cout << "1. Record Payment" << endl;
        cout << "2. View Outstanding Balance" << endl;
        cout << "3. View Fee Defaulters" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                recordPayment();
                break;
            case 2: {
                string roll, semester;
                cout << "Enter Student Roll Number: ";
                cin >> roll;
                cout << "Enter Semester: ";
                cin >> semester;
                double balance = getOutstandingBalance(roll, semester);
                cout << "Outstanding Balance: Rs. " << fixed << setprecision(2) << balance << endl;
                break;
            }
            case 3:
                getDefaulters();  // Fixed: Changed from viewFeeDefaulters to getDefaulters
                break;
            case 4:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 4);
}

void showReportsMenu() {
    int choice;
    do {
        cout << "\n=================================================" << endl;
        cout << "             REPORTS MENU                      " << endl;
        cout << "=================================================" << endl;
        cout << "1. Merit List" << endl;
        cout << "2. Attendance Defaulters" << endl;
        cout << "3. Fee Defaulters" << endl;
        cout << "4. Semester Result" << endl;
        cout << "5. Department Summary" << endl;
        cout << "6. Return to Main Menu" << endl;
        cout << "=================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        switch(choice) {
            case 1:
                printMeritList();  // Fixed: Changed from generateMeritList to printMeritList
                break;
            case 2:
                printAttendanceDefaulters();  // Fixed: Changed from generateAttendanceDefaulters to printAttendanceDefaulters
                break;
            case 3:
                printFeeDefaulters();  // Fixed: Changed from generateFeeDefaulters to printFeeDefaulters
                break;
            case 4:
                printSemesterResult();  // Fixed: Changed from generateSemesterResult to printSemesterResult
                break;
            case 5:
                printDepartmentSummary();  // Fixed: Changed from generateDepartmentSummary to printDepartmentSummary
                break;
            case 6:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 6);
}