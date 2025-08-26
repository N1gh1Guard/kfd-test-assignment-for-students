#include "LibraryUI.h"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <algorithm>

using namespace std;

void LibraryUI::Run() {
    while (true) {
        ShowMenu();
        int choice = GetIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                HandleBookManagement();
                break;
            case 2:
                HandleUserManagement();
                break;
            case 3:
                HandleBorrowingManagement();
                break;
            case 0:
                cout << "Goodbye!\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void LibraryUI::ShowMenu() {
    cout << "\n=== Library Management System ===\n";
    cout << "1. Book Management\n";
    cout << "2. User Management\n";
    cout << "3. Borrowing Management\n";
    cout << "0. Exit\n";
}

int LibraryUI::GetIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string LibraryUI::GetStringInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void LibraryUI::HandleBookManagement() {
    while (true) {
        cout << "\n=== Book Management ===\n";
        cout << "1. Add Book\n";
        cout << "2. Remove Book\n";
        cout << "3. Search Books\n";
        cout << "4. View All Books\n";
        cout << "0. Back to Main Menu\n";
        
        int choice = GetIntInput("Enter your choice: ");
        
        try {
            switch (choice) {
                case 1: {
                    string title = GetStringInput("Enter book title: ");
                    string author = GetStringInput("Enter book author: ");
                    string isbn = GetStringInput("Enter book ISBN: ");
                    
                    library_.AddBook(title, author, isbn);
                    cout << "Book added successfully!\n";
                    break;
                }
                case 2: {
                    string isbn = GetStringInput("Enter ISBN of book to remove: ");
                    if (library_.RemoveBook(isbn)) {
                        cout << "Book removed successfully!\n";
                    } else {
                        cout << "Book not found or could not be removed.\n";
                    }
                    break;
                }
                case 3: {
                    string title = GetStringInput("Enter search title (leave empty for any): ");
                    string author = GetStringInput("Enter search author (leave empty for any): ");
                    vector<Book*> results = library_.FindBooksByTitle(title, author);
                    if (results.empty()) {
                        cout << "No books found matching your query.\n";
                    } else {
                        cout << "Search results:\n";
                        for (const auto& book : results) {
                            book->Display();  // Исправить на стрелочку
                        }
                    }
                    break;
                }
                case 4:{
                    vector<Book*> allBooks = library_.GetAllBooks();
                    if (allBooks.empty()) {
                        cout << "No books in the library.\n";
                    } else {
                        cout << "\n=== All Books ===\n";
                        for (const auto& book : allBooks) {
                            book->Display();
                        }
                        cout << "Total books: " << allBooks.size() << "\n";
                    }
                    break;
                }
                case 0: {
                    return;
                }
                default:
                    cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void LibraryUI::HandleUserManagement() {
    while (true) {
        cout << "\n=== User Management ===\n";
        cout << "1. Register User\n";
        cout << "2. Find User\n";
        cout << "3. View All Users\n";
        cout << "0. Back to Main Menu\n";
        
        int choice = GetIntInput("Enter your choice: ");
        
        try {
            switch (choice) {
                case 1: {
                    string name = GetStringInput("Enter user name: ");
                    string userId = GetStringInput("Enter user ID: ");
                    string email = GetStringInput("Enter user email: ");
                    
                    cout << "Select user type:\n";
                    cout << "1. Student\n";
                    cout << "2. Faculty\n";
                    cout << "3. Guest\n";
                    int typeChoice = GetIntInput("Enter choice: ");
                    
                    UserType type;
                    switch (typeChoice) {
                        case 1: type = UserType::STUDENT; break;
                        case 2: type = UserType::FACULTY; break;
                        case 3: type = UserType::GUEST; break;
                        default: 
                            cout << "Invalid choice, defaulting to Guest.\n";
                            type = UserType::GUEST;
                    }
                    
                    library_.RegisterUser(name, userId, email, type);
                    cout << "User registered successfully!\n";
                    break;
                }
                case 2: {
                    string userId = GetStringInput("Enter user ID to find: ");
                    User* user = library_.FindUser(userId);
                    if (user) {
                        cout << "User found:\n";
                        cout << "Name: " << user->GetName() << "\n";
                        cout << "ID: " << user->GetUserId() << "\n";
                        cout << "Email: " << user->GetEmail() << "\n";
                        cout << "Type: ";
                        switch (user->GetType()) {
                            case UserType::STUDENT: cout << "Student\n"; break;
                            case UserType::FACULTY: cout << "Faculty\n"; break;
                            case UserType::GUEST: cout << "Guest\n"; break;
                        }
                        cout << "Borrowed books: " << user->GetBorrowedBooks().size() << "\n";
                    } else {
                        cout << "User not found.\n";
                    }
                    break;
                }
                case 3:{
                    vector<User*> allUsers = library_.GetAllUsers();
                    if (allUsers.empty()) {
                        cout << "No users registered.\n";
                    } else {
                        cout << "\n=== All Users ===\n";
                        for (const auto& user : allUsers) {
                            cout << "Name: " << user->GetName() << "\n";
                            cout << "ID: " << user->GetUserId() << "\n";
                            cout << "Email: " << user->GetEmail() << "\n";
                            cout << "Type: ";
                            switch (user->GetType()) {
                                case UserType::STUDENT: cout << "Student\n"; break;
                                case UserType::FACULTY: cout << "Faculty\n"; break;
                                case UserType::GUEST: cout << "Guest\n"; break;
                            }
                            cout << "Borrowed books: " << user->GetBorrowedBooks().size() << "/" << user->GetMaxBooks() << "\n";
                            cout << "----------------------------\n";
                        }
                        cout << "Total users: " << allUsers.size() << "\n";
                    }
                    break;
                }
                case 0: {
                    return;
                }
                default:
                    cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void LibraryUI::HandleBorrowingManagement() {
    while (true) {
        cout << "\n=== Borrowing Management ===\n";
        cout << "1. Borrow Book\n";
        cout << "2. Return Book\n";
        cout << "3. View Overdue Books\n";
        cout << "4. View All Borrow History\n";
        cout << "5. View Current Borrow History\n";
        cout << "0. Back to Main Menu\n";
        
        int choice = GetIntInput("Enter your choice: ");
        
        try {
            switch (choice) {
                case 1: {
                    string userId = GetStringInput("Enter user ID: ");
                    string isbn = GetStringInput("Enter book ISBN: ");
                    
                    if (library_.BorrowBook(userId, isbn)) {
                        cout << "Book borrowed successfully!\n";
                    } else {
                        cout << "Failed to borrow book. It might be unavailable or user has reached limit.\n";
                    }
                    break;
                }
                case 2: {
                    string userId = GetStringInput("Enter user ID: ");
                    string isbn = GetStringInput("Enter book ISBN: ");
                    
                    if (library_.ReturnBook(userId, isbn)) {
                        cout << "Book returned successfully!\n";
                    } else {
                        cout << "Failed to return book. It might not be borrowed by this user.\n";
                    }
                    break;
                }
                case 3: {
                    vector<BorrowingRecord> overdue = library_.GetOverdueBooks();
                    if (overdue.empty()) {
                        cout << "No overdue books.\n";
                    } else {
                        cout << "Overdue books:\n";
                        for (const auto& record : overdue) {
                            cout << "User ID: " << record.userId_ 
                                 << ", ISBN: " << record.isbn_
                                 << ", Due Date: " << ctime(&record.dueDate_);
                        }
                    }
                    break;
                }
                case 4:{
                    vector<BorrowingRecord> history = library_.GetBorrowHistory();
                    
                    cout << "\n=== Borrow History ===\n";
                    
                    if (history.empty()) {
                        cout << "No borrow history.\n";
                    } else {
                        cout << "All borrow records:\n";
                        for (const auto& record : history) {
                            cout << "User ID: " << record.userId_ 
                                << ", ISBN: " << record.isbn_
                                << ", Borrow Date: " << ctime(&record.borrowDate_)
                                << ", Due Date: " << ctime(&record.dueDate_)
                                << ", Returned: " << (record.returned_ ? "Yes" : "No") << "\n";
                        }
                    }
                    break;
                }
                case 5:{
                    vector<BorrowingRecord> current = library_.GetCurrentBorrowedBooks();
                        
                    cout << "\n=== Currently Borrowed Books ===\n";
                    if (current.empty()) {
                        cout << "No currently borrowed books.\n";
                    } else {
                        for (const auto& record : current) {
                            cout << "User ID: " << record.userId_ 
                                << ", ISBN: " << record.isbn_
                                << ", Due Date: " << ctime(&record.dueDate_) << "\n";
                        }
                        cout << "Total currently borrowed: " << current.size() << "\n";
                    }
                    break;
                }
                case 0: {
                    return;
                }
                default:
                    cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}