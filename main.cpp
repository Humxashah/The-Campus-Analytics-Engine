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
                searchStudentByName();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                softDeleteStudent();
                break;
            case 6:
                viewActiveStudents();
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
            case 1:
                enrollStudent();
                break;
            case 2:
                dropCourse();
                break;
            case 3:
                viewCreditLoad();
                break;
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
            case 2:
                viewAttendancePercentage();
                break;
            case 3:
                viewShortageList();
                break;
            case 4:
                undoLastAttendance();
                break;
            case 5:
                viewDailyAttendance();
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
            case 2:
                viewBestThreeQuizzes();
                break;
            case 3:
                viewWeightedTotal();
                break;
            case 4:
                viewLetterGrade();
                break;
            case 5:
                viewGPA();
                break;
            case 6:
                viewClassStatistics();
                break;
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
            case 2:
                viewOutstandingBalance();
                break;
            case 3:
                viewFeeDefaulters();
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
                generateMeritList();
                break;
            case 2:
                generateAttendanceDefaulters();
                break;
            case 3:
                generateFeeDefaulters();
                break;
            case 4:
                generateSemesterResult();
                break;
            case 5:
                generateDepartmentSummary();
                break;
            case 6:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 6);
}