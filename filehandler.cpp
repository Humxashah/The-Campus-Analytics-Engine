#include "filehandler.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Helper function to parse a CSV line character by character
vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string currentField = "";
    bool insideQuotes = false;
    
    // Iterate through each character in the line
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == '"') {
            // Toggle quote state
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            // End of field (comma outside quotes)
            fields.push_back(currentField);
            currentField = "";
        } else {
            // Add character to current field
            currentField += c;
        }
    }
    
    // Add the last field
    fields.push_back(currentField);
    
    return fields;
}

// Helper function to format a field for CSV
string formatCSVField(const string& field) {
    // Check if field contains comma or quote
    bool needsQuotes = false;
    for (size_t i = 0; i < field.length(); i++) {
        if (field[i] == ',' || field[i] == '"') {
            needsQuotes = true;
            break;
        }
    }
    
    if (needsQuotes) {
        // Escape any existing quotes by doubling them
        string escaped = "";
        for (size_t i = 0; i < field.length(); i++) {
            if (field[i] == '"') {
                escaped += "\"\"";
            } else {
                escaped += field[i];
            }
        }
        return "\"" + escaped + "\"";
    }
    
    return field;
}

// Read CSV file, skip header, parse each line
vector<vector<string> > readTXT(const string& filename) {
    vector<vector<string> > data;
    ifstream file(filename.c_str());
    
    // Check if file exists and can be opened
    if (!file.is_open()) {
        return data; // Return empty vector if file doesn't exist
    }
    
    string line;
    bool isFirstLine = true;
    
    // Read each line from the file
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Skip header row
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        
        // Parse the CSV line character by character
        vector<string> fields = parseCSVLine(line);
        data.push_back(fields);
    }
    
    file.close();
    return data;
}

// Write CSV file with header row
bool writeTXT(const string& filename, const vector<string>& header, const vector<vector<string> >& data) {
    ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << " for writing!" << endl;
        return false;
    }
    
    // Write header row
    for (size_t i = 0; i < header.size(); i++) {
        file << formatCSVField(header[i]);
        if (i < header.size() - 1) {
            file << ",";
        }
    }
    file << endl;
    
    // Write data rows
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            file << formatCSVField(data[i][j]);
            if (j < data[i].size() - 1) {
                file << ",";
            }
        }
        if (i < data.size() - 1) {
            file << endl;
        }
    }
    
    file.close();
    return true;
}

// Append a single row to CSV file
bool appendTXT(const string& filename, const vector<string>& row) {
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
        break;
    }
    checkFile.close();
    
    // Add newline before appending if file has content
    if (hasContent) {
        file << endl;
    }
    
    // Write the row
    for (size_t i = 0; i < row.size(); i++) {
        file << formatCSVField(row[i]);
        if (i < row.size() - 1) {
            file << ",";
        }
    }
    
    file.close();
    return true;
}

// Linear search through file rows
vector<string> findRow(const string& filename, int colIndex, const string& searchValue) {
    vector<vector<string> > data = readTXT(filename);
    
    // Search through all rows
    for (size_t i = 0; i < data.size(); i++) {
        // Check if column index exists in this row
        if (colIndex >= 0 && colIndex < static_cast<int>(data[i].size())) {
            if (data[i][colIndex] == searchValue) {
                return data[i]; // Return the matching row
            }
        }
    }
    
    // Return empty vector if not found
    return vector<string>();
}

// Check if a row exists with specific value at column index
bool rowExists(const string& filename, int colIndex, const string& searchValue) {
    vector<string> row = findRow(filename, colIndex, searchValue);
    return !row.empty();
}

// Initialize data files - simply checks if files exist, does NOT create any data
void initializeDataFiles() {
    // Just check if files exist, don't create them
    // The data files should already exist with the sample data
    ifstream students("students.txt");
    ifstream courses("courses.txt");
    ifstream enrollments("enrollments.txt");
    ifstream attendance("attendance_log.txt");
    ifstream fees("fees.txt");
    
    if (!students.is_open() || !courses.is_open() || 
        !enrollments.is_open() || !attendance.is_open() || 
        !fees.is_open()) {
        cerr << "Warning: Some data files are missing!" << endl;
        cerr << "Please ensure all data files exist in the current directory." << endl;
    }
    
    students.close();
    courses.close();
    enrollments.close();
    attendance.close();
    fees.close();
}