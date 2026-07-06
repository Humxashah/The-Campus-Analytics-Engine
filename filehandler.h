#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

using namespace std;

// Function declarations for file handling operations

/*
 * Reads a CSV file, skips the header row, and parses each line into a vector of strings
 * Uses character-by-character parsing (no getline split)
 * Handles fields containing commas by wrapping in quotes
 * Parameters: filename - name of the file to read
 * Returns: vector of vectors, each inner vector represents a row's fields
 */
vector<vector<string> > readTXT(const string& filename);

/*
 * Writes data to a CSV file with a header row
 * Each field is separated by a comma
 * Handles fields containing commas by wrapping in quotes
 * Parameters: filename - name of the file to write
 *             header - vector of column names
 *             data - vector of vectors containing the data rows
 * Returns: true if successful, false otherwise
 */
bool writeTXT(const string& filename, const vector<string>& header, const vector<vector<string> >& data);

/*
 * Appends a single row to an existing CSV file without loading it fully into memory
 * Handles fields containing commas by wrapping in quotes
 * Parameters: filename - name of the file
 *             row - vector of strings representing the row to append
 * Returns: true if successful, false otherwise
 */
bool appendTXT(const string& filename, const vector<string>& row);

/*
 * Performs a linear search through file rows and returns the first matching row
 * Parameters: filename - name of the file
 *             colIndex - index of the column to search in
 *             searchValue - value to search for
 * Returns: vector<string> containing the matching row, or empty vector if not found
 */
vector<string> findRow(const string& filename, int colIndex, const string& searchValue);

/*
 * Checks if a row exists with a specific value at a given column index
 * Parameters: filename - name of the file
 *             colIndex - index of the column to check
 *             searchValue - value to search for
 * Returns: true if row exists, false otherwise
 */
bool rowExists(const string& filename, int colIndex, const string& searchValue);

/*
 * Initializes all data files with sample data if they don't exist
 */
void initializeDataFiles();

/*
 * Helper function to parse a CSV line character by character
 * Handles quoted fields containing commas
 */
vector<string> parseCSVLine(const string& line);

/*
 * Helper function to format a field for CSV (add quotes if needed)
 */
string formatCSVField(const string& field);

#endif