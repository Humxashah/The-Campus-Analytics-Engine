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
    string semester;
    string status; // "enrolled" or "dropped"
};

// Enum for enrollment result
enum EnrollResult {
    ENROLL_SUCCESS,
    ENROLL_STUDENT_NOT_ACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_NO_SEATS,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_CREDIT_LIMIT_EXCEEDED,
    ENROLL_PREREQUISITE_NOT_MET
};

// Function declarations for course operations

/*
 * Enrolls a student in a course
 * Checks: student active, course exists, seats > enrolled count,
 * student not already enrolled, credit load <= 21, prerequisite passed
 * Returns EnrollResult with success/failure reason
 */
EnrollResult enrollStudent();

/*
 * Drops a course for a student
 * Only permitted if no attendance rows exist for this student+course+semester
 * Updates enrollment status to 'dropped'
 */
void dropCourse();

/*
 * Sums credit hours of all active enrollments for the given semester
 * Uses nested loop over enrollments and courses files
 */
int getCreditLoad(const string& roll, const string& semester);

/*
 * Looks up the prereq field of the course
 * If not NONE, checks the grades file to confirm the student has a non-F grade
 */
bool checkPrerequisite(const string& roll, const string& courseCode);

/*
 * Returns list of all active enrolled students in a course
 */
void listEnrolledStudents();

/*
 * Checks if a course has available seats
 */
bool checkSeatAvailability(const string& courseCode);

/*
 * Checks if a student is already enrolled in a course
 */
bool isDuplicateEnrollment(const string& roll, const string& courseCode);

/*
 * Parses a course line from courses.txt
 */
Course parseCourseLine(const string& line);

/*
 * Finds a course by course code
 */
Course findCourseByCode(const string& courseCode);

/*
 * Converts EnrollResult enum to string message
 */
string getEnrollResultMessage(EnrollResult result);

#endif