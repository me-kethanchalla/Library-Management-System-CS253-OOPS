# Library Management System

A C++-based library management system that uses object-oriented programming principles to manage books and users (Students, Faculty, and Librarians). All data (books, users, global IDs) is saved and loaded from files to ensure persistence across sessions.

## Overview

- **Manage Books:** Add, update, remove, borrow, and return books.
- **Manage Users:** Role-specific features for Students, Faculty, and Librarians.
- **Track Activity:** View borrowing history and fines.
- **Data Persistence:** Auto-save and load data so that your work is never lost.

## Running the Program

To run the program seamlessly:
- Ensure you have an optimal C++ compiler set up in your Visual Studio Code environment.
- You can also run the code on online compilers such as [OnlineGDB](https://www.onlinegdb.com) or [Programmiz](https://www.programiz.com).
- Include all necessary libraries as indicated in the code.
- For any issues during execution, feel free to contact:  
  **IITK Email:** kethanc23@iitk.ac.in

## User Guidelines

### Logging In & Pre-loaded Sample Credentials

- **Student:**  
  - Usernames: `student1` to `student5`  
  - Password: `pass`

- **Faculty:**  
  - Usernames: `faculty1` to `faculty3`  
  - Password: `pass`

- **Librarian:**  
  - Username: `librarian`  
  - Password: `pass`

*Note: These accounts are auto-generated if no user data exists. New accounts can be created via the system menus.*

- **Tracking Book IDs:**  
  - Book IDs start from **10001** and auto-increment when new books are added.  
  - Always note the book ID displayed when a book is added. Use it when borrowing, returning, or updating a book.

- **Tracking User IDs:**  
  - User IDs begin at **20001** and auto-increment with each new user (Student, Faculty, or Librarian).  
  - Do not alter these IDs manually; they are used as unique identifiers in the system.  
  - When performing operations such as viewing borrowing history or removing a user, refer to the correct user ID.
---

## OOP Design & Implementation

1. **Classes & Inheritance:**  
   - **Main Classes:** Book, User, Account, Library.  
   - **Derived Classes:** Student, Faculty, Librarian (inherited from User).

2. **Encapsulation:**  
   - Use private attributes with public getters and setters for controlled access.

3. **Polymorphism:**  
   - Override common functions like `borrow_book()` and `return_book()` in derived classes for role-specific behavior.

4. **File I/O:**  
   - Data is serialized/deserialized to/from:  
     `books.txt`, `students.txt`, `faculty.txt`, `librarians.txt`, and `global_ids.txt`.

## Files Included

- **Source Code:**  
  - `lms.cpp` – Main source file.
- **Data Files:**  
  - `books.txt` – Stores book details.
  - `students.txt` – Stores student details.
  - `faculty.txt` – Stores faculty details.
  - `librarians.txt` – Stores librarian details.
  - `global_ids.txt` – Stores global auto-increment IDs for books and users.




### Navigating Menus

- **Students/Faculty:**  
  Options include borrowing/returning books, viewing history, checking fines, and paying fines.
  
- **Librarians:**  
  Options include adding, updating, or removing books and adding or removing user accounts.

*Numeric inputs are validated; if an invalid value is entered, you’ll be prompted until a valid number is provided.*

### Auto-Saving & Data Persistence

- All modifications are auto-saved when you exit the program using the menu option.
- Ensure all data files are in the same directory as your executable.
