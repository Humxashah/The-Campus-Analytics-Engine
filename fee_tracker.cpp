#include "fee_tracker.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

// Parse fee line
FeeRecord parseFeeLine(const string& line) {
    FeeRecord record;
    stringstream ss(line);
    
    getline(ss, record.rollNumber, '|');
    getline(ss, record.date, '|');
    
    string amountStr;
    getline(ss, amountStr, '|');
    record.amount = atof(amountStr.c_str());
    
    getline(ss, record.status, '|');
    
    return record;
}

// Validate date format
bool validateDate(const string& date) {
    // Check length
    if (date.length() != 10) {
        return false;
    }
    
    // Check format YYYY-MM-DD
    if (date[4] != '-' || date[7] != '-') {
        return false;
    }
    
    // Check if all characters are digits or hyphens
    for (size_t i = 0; i < date.length(); i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }
    
    // Extract parts
    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());
    
    // Basic validation
    if (year < 2000 || year > 2099) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Month-specific day validation
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    } else if (month == 2) {
        // Leap year check
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    
    return true;
}

// Check if date is valid (not in the future)
bool isDateValid(const string& date) {
    if (!validateDate(date)) {
        return false;
    }
    
    // Current date (hardcoded for demonstration)
    string currentDate = "2024-03-15";
    
    // Simple comparison - convert to comparable format
    return date <= currentDate;
}

// Calculate late fine
double calculateLateFine(const string& date) {
    // Current date (hardcoded)
    string currentDate = "2024-03-15";
    
    // Extract parts
    int payYear = atoi(date.substr(0, 4).c_str());
    int payMonth = atoi(date.substr(5, 2).c_str());
    int payDay = atoi(date.substr(8, 2).c_str());
    
    int curYear = atoi(currentDate.substr(0, 4).c_str());
    int curMonth = atoi(currentDate.substr(5, 2).c_str());
    int curDay = atoi(currentDate.substr(8, 2).c_str());
    
    // Calculate days difference (simplified)
    int days = (curYear - payYear) * 365 + (curMonth - payMonth) * 30 + (curDay - payDay);
    
    // Fine: Rs. 100 per day late after 30 days grace period
    if (days > 30) {
        int lateDays = days - 30;
        return lateDays * 100.0;
    }
    
    return 0.0;
}

// Generate receipt
void generateReceipt(const string& roll, double amount, const string& date) {
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    string receiptFile = "receipt_" + roll + "_" + date + ".txt";
    
    vector<string> receipt;
    receipt.push_back("============================================");
    receipt.push_back("          FEE PAYMENT RECEIPT              ");
    receipt.push_back("============================================");
    receipt.push_back("Receipt Date: " + date);
    receipt.push_back("Student Roll: " + roll);
    receipt.push_back("Student Name: " + student.name);
    receipt.push_back("Department: " + student.department);
    receipt.push_back("--------------------------------------------");
    receipt.push_back("Amount Paid: Rs. " + to_string(static_cast<int>(amount)));
    
    double fine = calculateLateFine(date);
    if (fine > 0) {
        receipt.push_back("Late Fine: Rs. " + to_string(static_cast<int>(fine)));
        receipt.push_back("Total Paid: Rs. " + to_string(static_cast<int>(amount + fine)));
    }
    
    receipt.push_back("--------------------------------------------");
    receipt.push_back("Status: PAID");
    receipt.push_back("Thank you for your payment!");
    receipt.push_back("============================================");
    
    if (writeTXT(receiptFile, receipt)) {
        cout << "Receipt generated successfully: " << receiptFile << endl;
    } else {
        cout << "Error: Failed to generate receipt!" << endl;
    }
}

// Record payment
void recordPayment() {
    cout << "\n--- RECORD FEE PAYMENT ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    string date;
    do {
        cout << "Enter Payment Date (YYYY-MM-DD): ";
        cin >> date;
        
        if (!validateDate(date)) {
            cout << "Error: Invalid date format! Use YYYY-MM-DD" << endl;
            continue;
        }
        
        if (!isDateValid(date)) {
            cout << "Error: Payment date cannot be in the future!" << endl;
            continue;
        }
        
        break;
    } while (true);
    
    double amount;
    do {
        cout << "Enter Amount (Rs.): ";
        cin >> amount;
        
        if (cin.fail() || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a valid positive amount!" << endl;
        } else {
            break;
        }
    } while (true);
    
    // Calculate late fine
    double fine = calculateLateFine(date);
    if (fine > 0) {
        cout << "Late Fine: Rs. " << fine << endl;
        cout << "Total Amount: Rs. " << (amount + fine) << endl;
    }
    
    // Confirm payment
    cout << "Confirm payment? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) != 'y') {
        cout << "Payment cancelled." << endl;
        return;
    }
    
    // Record payment
    FeeRecord record;
    record.rollNumber = roll;
    record.date = date;
    record.amount = amount;
    record.status = "Paid";
    
    stringstream ss;
    ss << record.rollNumber << "|" << record.date << "|" 
       << record.amount << "|" << record.status;
    
    if (appendTXT("fees.txt", ss.str())) {
        cout << "Payment recorded successfully!" << endl;
        generateReceipt(roll, amount, date);
    } else {
        cout << "Error: Failed to record payment!" << endl;
    }
}

// View outstanding balance
void viewOutstandingBalance() {
    cout << "\n--- VIEW OUTSTANDING BALANCE ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    vector<string> lines = readTXT("fees.txt");
    double totalPaid = 0;
    double totalFees = 50000.0; // Semester fee (fixed for simplicity)
    
    for (size_t i = 0; i < lines.size(); i++) {
        FeeRecord record = parseFeeLine(lines[i]);
        if (record.rollNumber == roll && record.status == "Paid") {
            totalPaid += record.amount;
        }
    }
    
    double outstanding = totalFees - totalPaid;
    if (outstanding < 0) outstanding = 0;
    
    cout << "\n--- OUTSTANDING BALANCE FOR " << student.name << " ---" << endl;
    cout << "Student: " << student.name << " (" << roll << ")" << endl;
    cout << "Total Semester Fee: Rs. " << totalFees << endl;
    cout << "Total Paid: Rs. " << totalPaid << endl;
    cout << "Outstanding Balance: Rs. " << outstanding << endl;
    
    if (outstanding > 0) {
        cout << "\nStatus: UNPAID - Please clear your dues!" << endl;
    } else {
        cout << "\nStatus: PAID - All fees cleared!" << endl;
    }
}

// View fee defaulters (bubble sort)
void viewFeeDefaulters() {
    cout << "\n--- FEE DEFAULTERS LIST ---" << endl;
    
    vector<string> lines = readTXT("fees.txt");
    vector<FeeRecord> allFees;
    
    for (size_t i = 0; i < lines.size(); i++) {
        FeeRecord record = parseFeeLine(lines[i]);
        allFees.push_back(record);
    }
    
    // Get unique students with unpaid fees
    vector<FeeRecord> defaulters;
    vector<string> processedRolls;
    
    for (size_t i = 0; i < allFees.size(); i++) {
        if (allFees[i].status == "Unpaid") {
            bool alreadyProcessed = false;
            for (size_t j = 0; j < processedRolls.size(); j++) {
                if (processedRolls[j] == allFees[i].rollNumber) {
                    alreadyProcessed = true;
                    break;
                }
            }
            
            if (!alreadyProcessed) {
                // Check if student has any paid fees
                bool hasPaid = false;
                for (size_t j = 0; j < allFees.size(); j++) {
                    if (allFees[j].rollNumber == allFees[i].rollNumber && 
                        allFees[j].status == "Paid") {
                        hasPaid = true;
                        break;
                    }
                }
                
                if (!hasPaid) {
                    defaulters.push_back(allFees[i]);
                    processedRolls.push_back(allFees[i].rollNumber);
                }
            }
        }
    }
    
    if (defaulters.empty()) {
        cout << "No fee defaulters found!" << endl;
        return;
    }
    
    // Sort defaulters using bubble sort (by amount descending)
    bubbleSortFees(defaulters);
    
    cout << "\n--- STUDENTS WITH UNPAID FEES ---" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(20) << "Amount Due" << endl;
    cout << string(65, '-') << endl;
    
    for (size_t i = 0; i < defaulters.size(); i++) {
        Student student = findStudentByRoll(defaulters[i].rollNumber);
        if (student.rollNumber != "NULL") {
            cout << left << setw(15) << student.rollNumber
                 << setw(30) << student.name
                 << setw(20) << "Rs. " << fixed << setprecision(2) << 50000.0 << endl;
        }
    }
    
    cout << "\nTotal Defaulters: " << defaulters.size() << endl;
}

// Bubble sort implementation
void bubbleSortFees(vector<FeeRecord>& fees) {
    int n = fees.size();
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Sort by amount descending
            if (fees[j].amount < fees[j + 1].amount) {
                FeeRecord temp = fees[j];
                fees[j] = fees[j + 1];
                fees[j + 1] = temp;
            }
        }
    }
}