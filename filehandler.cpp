#include "filehandler.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Read all lines from a text file
vector<string> readTXT(const string& filename) {
    vector<string> lines;
    ifstream file(filename.c_str());
    
    // Check if file exists and can be opened
    if (!file.is_open()) {
        return lines; // Return empty vector if file doesn't exist
    }
    
    string line;
    // Read each line and store in vector
    while (getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    return lines;
}

// Write all lines to a text file (overwrites)
bool writeTXT(const string& filename, const vector<string>& lines) {
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << " for writing!" << endl;
        return false;
    }
    
    // Write each line to the file
    for (size_t i = 0; i < lines.size(); i++) {
        file << lines[i];
        // Add newline after each line except the last one
        if (i < lines.size() - 1) {
            file << endl;
        }
    }
    
    file.close();
    return true;
}

// Append a single line to a text file
bool appendTXT(const string& filename, const string& line) {
    ofstream file(filename.c_str(), ios::app); // Open in append mode
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << " for appending!" << endl;
        return false;
    }
    
    // Check if file already has content (to add newline properly)
    ifstream checkFile(filename.c_str());
    bool hasContent = false;
    string tempLine;
    while (getline(checkFile, tempLine)) {
        hasContent = true;
        break; // Just check if there's at least one line
    }
    checkFile.close();
    
    // Add newline before appending if file has content
    if (hasContent) {
        file << endl;
    }
    
    file << line;
    file.close();
    return true;
}

// Find a row containing the search key
int findRow(const string& filename, const string& searchKey) {
    vector<string> lines = readTXT(filename);
    
    // Search through all lines
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].find(searchKey) != string::npos) {
            return i; // Return index of found row
        }
    }
    
    return -1; // Not found
}

// Check if a row exists
bool rowExists(const string& filename, const string& searchKey) {
    return findRow(filename, searchKey) != -1;
}

// Initialize data files with sample data
void initializeDataFiles() {
    // Check if students.txt exists, if not create with sample data
    if (!rowExists("students.txt", "student")) {
        vector<string> sampleStudents;
        sampleStudents.push_back("2024001|Ali Ahmed|BS Computer Science|3.45|active");
        sampleStudents.push_back("2024002|Fatima Khan|BS Computer Science|3.78|active");
        sampleStudents.push_back("2024003|Muhammad Hassan|BS Software Engineering|3.21|active");
        sampleStudents.push_back("2024004|Ayesha Malik|BS Computer Science|2.95|active");
        sampleStudents.push_back("2024005|Usman Raza|BS Software Engineering|3.56|active");
        sampleStudents.push_back("2024006|Sana Tariq|BS Computer Science|3.89|active");
        sampleStudents.push_back("2024007|Hamza Ali|BS Software Engineering|3.12|active");
        sampleStudents.push_back("2024008|Zara Shah|BS Computer Science|3.67|active");
        sampleStudents.push_back("2024009|Bilal Ahmed|BS Software Engineering|2.88|active");
        sampleStudents.push_back("2024010|Nadia Khan|BS Computer Science|3.94|active");
        writeTXT("students.txt", sampleStudents);
    }
    
    // Check if courses.txt exists
    if (!rowExists("courses.txt", "course")) {
        vector<string> sampleCourses;
        sampleCourses.push_back("CS101|Programming Fundamentals|3|50|none");
        sampleCourses.push_back("CS102|Object Oriented Programming|4|45|CS101");
        sampleCourses.push_back("CS201|Data Structures|3|40|CS102");
        sampleCourses.push_back("CS202|Database Systems|3|40|CS101");
        sampleCourses.push_back("SE101|Software Engineering|3|50|none");
        sampleCourses.push_back("SE102|Web Development|3|35|SE101");
        sampleCourses.push_back("MATH101|Calculus|3|60|none");
        sampleCourses.push_back("MATH102|Linear Algebra|3|50|MATH101");
        sampleCourses.push_back("PHY101|Physics|3|45|none");
        sampleCourses.push_back("ENG101|English Composition|2|50|none");
        writeTXT("courses.txt", sampleCourses);
    }
    
    // Check if enrollments.txt exists
    if (!rowExists("enrollments.txt", "enrollment")) {
        vector<string> sampleEnrollments;
        sampleEnrollments.push_back("2024001|CS101|A");
        sampleEnrollments.push_back("2024001|CS102|A");
        sampleEnrollments.push_back("2024001|MATH101|A");
        sampleEnrollments.push_back("2024002|CS101|A");
        sampleEnrollments.push_back("2024002|CS201|A");
        sampleEnrollments.push_back("2024002|MATH101|A");
        sampleEnrollments.push_back("2024003|CS101|A");
        sampleEnrollments.push_back("2024003|SE101|A");
        sampleEnrollments.push_back("2024004|CS102|A");
        sampleEnrollments.push_back("2024004|CS202|A");
        writeTXT("enrollments.txt", sampleEnrollments);
    }
    
    // Check if attendance_log.txt exists
    if (!rowExists("attendance_log.txt", "attendance")) {
        vector<string> sampleAttendance;
        sampleAttendance.push_back("2024-01-15|2024001|CS101|Present");
        sampleAttendance.push_back("2024-01-15|2024002|CS101|Present");
        sampleAttendance.push_back("2024-01-15|2024003|CS101|Absent");
        sampleAttendance.push_back("2024-01-15|2024004|CS102|Present");
        sampleAttendance.push_back("2024-01-16|2024001|CS102|Present");
        sampleAttendance.push_back("2024-01-16|2024002|CS102|Present");
        sampleAttendance.push_back("2024-01-16|2024003|SE101|Present");
        sampleAttendance.push_back("2024-01-16|2024004|CS202|Absent");
        sampleAttendance.push_back("2024-01-17|2024001|MATH101|Present");
        sampleAttendance.push_back("2024-01-17|2024002|MATH101|Present");
        writeTXT("attendance_log.txt", sampleAttendance);
    }
    
    // Check if fees.txt exists
    if (!rowExists("fees.txt", "fee")) {
        vector<string> sampleFees;
        sampleFees.push_back("2024001|2024-01-10|50000|Paid");
        sampleFees.push_back("2024001|2024-02-10|50000|Paid");
        sampleFees.push_back("2024002|2024-01-10|50000|Paid");
        sampleFees.push_back("2024002|2024-02-10|50000|Unpaid");
        sampleFees.push_back("2024003|2024-01-10|50000|Paid");
        sampleFees.push_back("2024004|2024-01-10|50000|Unpaid");
        sampleFees.push_back("2024005|2024-01-10|50000|Paid");
        sampleFees.push_back("2024006|2024-01-10|50000|Unpaid");
        writeTXT("fees.txt", sampleFees);
    }
}