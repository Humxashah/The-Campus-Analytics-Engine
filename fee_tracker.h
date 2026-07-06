#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <string>
#include <vector>

using namespace std;

// Structure to hold fee record
struct FeeRecord {
    string rollNumber;
    string date;
    double amount;
    string status; // "Paid" or "Unpaid"
};

// Function declarations for fee operations

/*
 * Records a fee payment for a student
 * Validates date and amount
 */
void recordPayment();

/*
 * Validates date format (YYYY-MM-DD)
 */
bool validateDate(const string& date);

/*
 * Validates if a date is in the past (manual date calculation)
 */
bool isDateValid(const string& date);

/*
 * Calculates late fine based on days overdue
 */
double calculateLateFine(const string& date);

/*
 * Generates a receipt for a payment
 */
void generateReceipt(const string& roll, double amount, const string& date);

/*
 * Calculates outstanding balance for a student
 */
void viewOutstandingBalance();

/*
 * Lists fee defaulters (students with unpaid fees)
 * Uses bubble sort to sort by amount
 */
void viewFeeDefaulters();

/*
 * Parses a fee line from fees.txt
 */
FeeRecord parseFeeLine(const string& line);

/*
 * Bubble sort implementation for sorting fees
 */
void bubbleSortFees(vector<FeeRecord>& fees);

#endif