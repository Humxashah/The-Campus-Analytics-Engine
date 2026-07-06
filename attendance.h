#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold attendance record
struct AttendanceRecord {
    string rollNumber;
    string courseCode;
    string date;
    string status; // "P" for Present, "A" for Absent, "L" for Late
};

// Global backup vector for undo functionality
extern vector<vector<string> > attendanceBackup;
extern bool hasBackup;

// Function declarations for attendance operations

/*
 * Iterates enrolled students, prompts P/A/L for each
 * Saves a backup of the current attendance vector before writing
 * Appends rows to attendance_log.txt
 */
void markAttendance();

/*
 * Computes (present + 0.5 * late) / totalSessions * 100.0
 * Uses an accumulator loop
 * Returns double
 */
double getAttendancePct(const string& roll, const string& courseCode);

/*
 * Returns students with attendance < 75%
 */
void getShortageList();

/*
 * Restores from backup vector (pre-session snapshot)
 * Rewrites file
 * Returns false if no backup exists
 */
bool undoLastSession();

/*
 * Formatted console table of all enrolled students and their status for the given date
 */
void printDailySheet();

/*
 * Parses an attendance line from attendance_log.txt
 */
AttendanceRecord parseAttendanceLine(const string& line);

/*
 * Saves backup of current attendance data
 */
void saveAttendanceBackup();

#endif