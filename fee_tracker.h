#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold fee record
struct FeeRecord {
    string rollNumber;
    string semester;
    double amountDue;
    double amountPaid;
    string dueDate;
    string paidDate;
};

// Function declarations for fee operations

/*
 * Records a payment for a student
 * Validates date format DD-MM-YYYY with string checks
 * Updates fees.txt paid amount
 */
void recordPayment();

/*
 * Validates date format DD-MM-YYYY with string checks
 */
bool validateDate(const string& date);

/*
 * Computes late fine based on due_date and paid_date
 * Calculates 2% per complete week
 */
double computeLateFine(const string& dueDate, const string& paidDate, double amountDue);

/*
 * Parses two DD-MM-YYYY strings, converts to total day count
 * Uses manual month-length array, no time library
 * Returns integer difference
 */
int daysBetween(const string& date1, const string& date2);

/*
 * Prints formatted receipt: tuition breakdown, late fine if any,
 * total due, paid, balance
 * Uses setw from iomanip
 */
void generateReceipt(const string& roll, const string& semester);

/*
 * Returns students with a balance > 0 past due date
 * Sorted by outstanding amount using bubble sort
 */
void getDefaulters();  // Added this function

/*
 * Parses a fee line from fees.txt
 */
FeeRecord parseFeeLine(const string& line);

/*
 * Converts FeeRecord to string for storage
 */
string feeToString(const FeeRecord& fee);

/*
 * Calculates outstanding balance for a student
 */
double getOutstandingBalance(const string& roll, const string& semester);

/*
 * Bubble sort implementation for sorting defaulters by outstanding amount
 */
void bubbleSortDefaulters(vector<FeeRecord>& defaulters);

#endif