#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold attendance record
struct AttendanceRecord {
    string date;
    string rollNumber;
    string courseCode;
    string status; // "Present" or "Absent"
};

// Function declarations for attendance operations

/*
 * Marks attendance for a student in a course
 * Validates student and course existence
 */
void markAttendance();

/*
 * Calculates and displays attendance percentage for a student
 * Shows total sessions and present days
 */
void viewAttendancePercentage();

/*
 * Lists students with attendance below required threshold (75%)
 */
void viewShortageList();

/*
 * Undoes the last attendance marking session
 * Removes the most recent attendance records
 */
void undoLastAttendance();

/*
 * Displays today's attendance sheet for a course
 */
void viewDailyAttendance();

/*
 * Parses an attendance line from attendance_log.txt
 */
AttendanceRecord parseAttendanceLine(const string& line);

/*
 * Calculates attendance percentage for a student in a course
 */
double calculateAttendancePercentage(const string& roll, const string& courseCode);

#endif