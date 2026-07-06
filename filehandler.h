#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

using namespace std;

// Function declarations for file handling operations

/*
 * Reads all lines from a text file and returns them as a vector of strings
 * Parameters: filename - name of the file to read
 * Returns: vector containing all lines from the file
 */
vector<string> readTXT(const string& filename);

/*
 * Writes all lines to a text file (overwrites existing content)
 * Parameters: filename - name of the file to write
 *             lines - vector of strings to write
 * Returns: true if successful, false otherwise
 */
bool writeTXT(const string& filename, const vector<string>& lines);

/*
 * Appends a single line to the end of a text file
 * Parameters: filename - name of the file
 *             line - string to append
 * Returns: true if successful, false otherwise
 */
bool appendTXT(const string& filename, const string& line);

/*
 * Finds a row that contains a specific search string
 * Parameters: filename - name of the file
 *             searchKey - string to search for
 * Returns: index of the row if found, -1 if not found
 */
int findRow(const string& filename, const string& searchKey);

/*
 * Checks if a row exists in the file
 * Parameters: filename - name of the file
 *             searchKey - string to search for
 * Returns: true if row exists, false otherwise
 */
bool rowExists(const string& filename, const string& searchKey);

/*
 * Initializes all data files with sample data if they don't exist
 */
void initializeDataFiles();

#endif