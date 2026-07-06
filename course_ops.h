#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold course information
struct Course {
    string courseCode;
    string courseName;
    int creditHours;
    int maxSeats;
    string prerequisite;
};

// Structure to hold enrollment information
struct Enrollment {
    string rollNumber;
    string courseCode;
    string status; // "A" for active, "D" for dropped
};

// Function declarations for course operations

/*
 * Enrolls a student in a course
 * Checks prerequisites, seat availability, credit hours limit
 * Prevents duplicate enrollment
 */
void enrollStudent();

/*
 * Drops a course for a student
 * Updates enrollment status to "D"
 */
void dropCourse();

/*
 * Calculates and displays total credit hours for a student
 * Checks if within 21 credit hours limit
 */
void viewCreditLoad();

/*
 * Validates if a course has available seats
 */
bool checkSeatAvailability(const string& courseCode);

/*
 * Checks if a student is already enrolled in a course
 */
bool isDuplicateEnrollment(const string& roll, const string& courseCode);

/*
 * Checks if a student has completed the prerequisite
 */
bool checkPrerequisite(const string& roll, const string& courseCode);

/*
 * Lists all students enrolled in a specific course
 */
void listEnrolledStudents();

/*
 * Parses a course line from courses.txt
 */
Course parseCourseLine(const string& line);

/*
 * Finds a course by course code
 */
Course findCourseByCode(const string& courseCode);

/*
 * Calculates total credit hours for a student
 */
int calculateTotalCredits(const string& roll);

#endif