# The Campus Analytics Engine

## 📊 A Comprehensive Student Management System

---

### 👨‍💻 Created By
**Hamza Ali Bukhari**
- **Program:** BS Artificial Intelligence
- **University:** University of Management and Technology (UMT), Lahore
- **Roll Number:** f2025376441

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Setup Instructions](#setup-instructions)
- [Data Files](#data-files)
- [Module Structure](#module-structure)
- [Sample Run](#sample-run)
- [Technical Specifications](#technical-specifications)
- [Error Handling](#error-handling)
- [Future Enhancements](#future-enhancements)

---

## 🎯 Overview

**The Campus Analytics Engine** is a comprehensive student management system designed to streamline academic operations in educational institutions. Built entirely in **pure C++**, this console-based application provides a complete suite of tools for managing students, courses, attendance, grades, fees, and generating analytical reports.

The system follows **Object-Oriented Programming** principles while maintaining strict adherence to standard C++ without relying on external libraries or advanced STL algorithms. Every sorting, searching, and data processing algorithm has been manually implemented.

---

## ✨ Features

### 📚 Student Management
- ✅ Add new students with validation
- 🔍 Search students by roll number (BSAI-YY-XXX format)
- 🔎 Search students by name (partial match)
- ✏️ Update student information (name, department, CGPA)
- 🗑️ Soft delete (mark as inactive)
- 📋 View all active students sorted by roll number
- ✅ Roll number validation (BSAI-YY-XXX format)
- ✅ Duplicate checking
- ✅ Name validation (no digits allowed)
- ✅ CGPA validation (0.0 - 4.0)

### 📖 Course Management
- 📝 Enroll students with comprehensive validation
- ❌ Drop courses (only before attendance is marked)
- 📊 Credit load calculation (max 21 credit hours)
- 💺 Seat availability validation
- 🔄 Duplicate enrollment check
- 📚 Prerequisite checking (from grades file)
- 📋 List all enrolled students in a course

### 📅 Attendance Management
- ✅ Mark attendance (P/A/L)
- 📊 Attendance percentage calculation `(P + 0.5*L) / Total * 100`
- ⚠️ Shortage list (students with < 75% attendance)
- ↩️ Undo last attendance session (backup restore)
- 📋 Daily attendance sheet

### 📊 Grades Management
- 📝 Enter marks (5 quizzes, assignment, midterm, final)
- 🏆 Best three quizzes (manual selection without sorting)
- 📈 Weighted total calculation (Quiz:10%, Assignment:10%, Midterm:30%, Final:50%)
- 📝 Letter grade mapping (A, B+, B, C+, C, D, F)
- 📊 GPA calculation (credit-weighted average)
- ⚠️ Attendance penalty (grade overridden to F if attendance < 75%)
- 📊 Class statistics (highest, lowest, mean, median)

### 💰 Fee Tracking
- 💳 Record payments with date validation
- 📅 Manual date calculations (DD-MM-YYYY format)
- 💸 Late fine calculation (2% per complete week)
- 🧾 Receipt generation with formatted output
- 💰 Outstanding balance calculation
- 📋 Fee defaulters list with bubble sort

### 📈 Reports
- 🏆 Merit list (sorted by CGPA with ranks)
- ⚠️ Attendance defaulters report
- 💰 Fee defaulters report (with weeks overdue)
- 📊 Semester result sheet
- 🏫 Department summary (count, avg CGPA, pass rate)
- 💾 Export reports to .txt files

---

## 🏗️ System Architecture

### File Structure
```
The Campus Analytics Engine/
├── main.cpp
├── filehandler.h / filehandler.cpp
├── student_ops.h / student_ops.cpp
├── course_ops.h / course_ops.cpp
├── attendance.h / attendance.cpp
├── grades.h / grades.cpp
├── fee_tracker.h / fee_tracker.cpp
├── reports.h / reports.cpp
└── Data Files/
    ├── students.txt
    ├── courses.txt
    ├── enrollments.txt
    ├── attendance_log.txt
    ├── fees.txt
    └── grades.txt
```

### Data File Formats (CSV)

| File | Columns | Format |
|------|---------|--------|
| **students.txt** | roll, name, dept, cgpa, status | `roll,name,dept,cgpa,status` |
| **courses.txt** | code, title, credits, seats, prereq | `code,title,credits,seats,prereq` |
| **enrollments.txt** | roll, course_code, semester, status | `roll,course_code,semester,status` |
| **attendance_log.txt** | roll, course_code, date, status | `roll,course_code,date,status` |
| **fees.txt** | roll, semester, amount_due, amount_paid, due_date, paid_date | `roll,semester,amount_due,amount_paid,due_date,paid_date` |
| **grades.txt** | roll, course_code, quiz1, quiz2, quiz3, quiz4, quiz5, assignment, midterm, final, weighted_total, letter_grade | `roll,course_code,q1,q2,q3,q4,q5,asgn,mid,final,total,grade` |

---

## 🚀 Setup Instructions

### Prerequisites
- C++ Compiler (g++ recommended)
- Standard C++ Library
- 20+ MB of free disk space

### Installation Steps

1. **Clone or Download the Project**
   ```bash
   git clone [repository-url]
   cd "The Campus Analytics Engine"
   ```

2. **Compile the Project**
   ```bash
   g++ -o campus_engine main.cpp filehandler.cpp student_ops.cpp course_ops.cpp attendance.cpp grades.cpp fee_tracker.cpp reports.cpp
   ```

   Or compile with all files:
   ```bash
   g++ -std=c++11 -o campus_engine *.cpp
   ```

3. **Run the Application**
   ```bash
   ./campus_engine      # Linux/Mac
   campus_engine.exe    # Windows
   ```

4. **First Run**
   - The system will automatically create data files with sample data
   - You can start using all features immediately

### Quick Test
After running, you can:
1. Navigate to Student Management → View Active Students
2. Check Course Management → List Enrolled Students
3. Generate Reports → Merit List

---

## 📁 Data Files

### Sample Data Overview

All data files come pre-populated with realistic sample data:

- **15 Students** from CS, SE, and EE departments
- **15 Courses** with various credit hours and prerequisites
- **32 Enrollment Records** across different semesters
- **95 Attendance Records** spanning 10 days
- **30 Fee Records** for Fall and Spring semesters

### Creating Your Own Data
To use your own data:
1. Open any `.txt` file in a text editor
2. Follow the CSV format (comma-separated values)
3. Keep the header row intact
4. Save the file and restart the application

---

## 📂 Module Structure

### 1. File Handler Module
- **filehandler.h / filehandler.cpp**
- Handles all CSV file operations
- Custom character-by-character parsing
- Manual field handling with quote support

### 2. Student Operations Module
- **student_ops.h / student_ops.cpp**
- CRUD operations for students
- Selection sort implementation
- Roll number format: BSAI-YY-XXX

### 3. Course Operations Module
- **course_ops.h / course_ops.cpp**
- Enrollment with validation
- Prerequisite checking
- Credit load management

### 4. Attendance Module
- **attendance.h / attendance.cpp**
- Attendance marking with backup
- Percentage calculation
- Undo functionality

### 5. Grades Module
- **grades.h / grades.cpp**
- Manual best-three quiz selection
- Weighted total computation
- Attendance penalty enforcement

### 6. Fee Tracker Module
- **fee_tracker.h / fee_tracker.cpp**
- Date validation (DD-MM-YYYY)
- Late fine calculation (2% per week)
- Receipt generation

### 7. Reports Module
- **reports.h / reports.cpp**
- Comprehensive reporting
- Export functionality
- Department grouping

---

## 🎮 Sample Run

### Main Menu
```
=================================================
    STUDENT MANAGEMENT SYSTEM - MAIN MENU      
=================================================
1. Student Management
2. Course Management
3. Attendance Management
4. Grades Management
5. Fee Tracking
6. Reports
7. Exit
=================================================
Enter your choice: 1
```

### Adding a Student
```
--- ADD NEW STUDENT ---
Enter Roll Number (Format: BSAI-YY-XXX, e.g., BSAI-24-001): BSAI-24-015
Enter Student Name: Ahmed Khan
Enter Department: CS
Enter CGPA (0.0 - 4.0): 3.75
Student added successfully!
```

### Viewing Merit List
```
--- MERIT LIST ---
Rank    Roll Number   Name                           Department     CGPA
------------------------------------------------------------------------
1       BSAI-24-010   Nadia Khan                     CS             3.94
2       BSAI-24-006   Sana Tariq                     CS             3.89
3       BSAI-24-014   Sadia Khan                     EE             3.82
4       BSAI-24-002   Fatima Khan                    CS             3.78
...
Total Students: 15
```

### Generating Receipt
```
============================================================
                            FEE PAYMENT RECEIPT
============================================================
Student Name:       Ali Ahmed
Roll Number:        2024001
Semester:           Fall2024
Department:         CS
------------------------------------------------------------
Tuition Fee:                                   50000.00
Late Fine (2% per week):                        1000.00
Total Amount Due:                               51000.00
Amount Paid:                                    51000.00
------------------------------------------------------------
Outstanding Balance:                               0.00
Due Date:           15-01-2024
Payment Date:       10-01-2024
============================================================
              Thank you for your payment!
============================================================
```

### Department Summary
```
--- DEPARTMENT SUMMARY ---
Department           Students   Avg CGPA     Pass Rate
-----------------------------------------------------------------
CS                   8          3.56         100.00%
SE                   4          3.13         100.00%
EE                   3          3.52         100.00%
-----------------------------------------------------------------
```

---

## 🛠️ Technical Specifications

### Development Environment
- **Language:** C++ (Standard C++)
- **Compiler:** g++ 7.0+
- **Standard:** C++11/14

### Key Technical Decisions

#### Why Pure C++?
- Platform independence
- Performance optimization
- No external dependencies
- Complete control over memory management

#### Manual Algorithm Implementation
- **Selection Sort:** Student list sorting
- **Bubble Sort:** Fee defaulter sorting
- **Linear Search:** File row searching
- **Custom Parsing:** CSV file handling without regex

#### No STL Algorithms Used
- No `#include <algorithm>`
- No `sort()`, `find()`, `map`, `unordered_map`
- Manual implementation of all algorithms

---

## ⚠️ Error Handling

### Comprehensive Validation
- ✅ Input validation for all user entries
- ✅ File existence and readability checks
- ✅ Roll number format validation (BSAI-YY-XXX)
- ✅ CGPA range validation (0.0 - 4.0)
- ✅ Date format validation (DD-MM-YYYY)
- ✅ Duplicate record prevention
- ✅ Invalid menu choice handling

### Graceful Failure
- User-friendly error messages
- No unexpected crashes
- Proper memory management
- Backup and restore for attendance

---

## 🔮 Future Enhancements

- 🎨 Graphical User Interface (GUI)
- 🌐 Web-based dashboard
- 📊 Advanced analytics and data visualization
- 🤖 AI-powered student performance prediction
- 📱 Mobile application integration
- 🔔 Email and SMS notifications
- 📤 CSV/Excel export functionality
- 🔐 Multi-user authentication and roles

---

## 📝 License

This project is created for academic purposes as part of the Programming Fundamentals course at UMT Lahore.

---

## 👏 Acknowledgments

- **University of Management and Technology, Lahore**
- **BS Artificial Intelligence Program**
- **Programming Fundamentals Course Faculty**

---

## 📞 Contact

**Hamza Ali Bukhari**
- **Email:** f2025376441@umt.edu.pk
- **Program:** BS Artificial Intelligence
- **University:** UMT Lahore

---

### 🎓 *"Empowering Education Through Analytics"*

---

**© 2026 The Campus Analytics Engine - All Rights Reserved**
