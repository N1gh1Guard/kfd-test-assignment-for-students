#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <ctime>
#include <stdexcept>

#include "User.h"
#include "Book.h"

class User;
class Book;

struct BorrowingRecord {
    std::string userId_;
    std::string isbn_;
    std::time_t borrowDate_;
    std::time_t dueDate_;
    bool returned_;

    BorrowingRecord(const std::string& userId, const std::string& isbn, 
                   std::time_t borrowDate, std::time_t dueDate);
};

class Library {
private:
    std::unordered_map<std::string, std::unique_ptr<User>> users_;
    std::unordered_map<std::string, std::unique_ptr<Book>> books_;
    std::vector<BorrowingRecord> borrowHistory_;

public:
    Library() = default;
    ~Library() = default;
    
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;
    
    Library(Library&&) = default;
    Library& operator=(Library&&) = default;

    //Book management
    void AddBook(const std::string& title, const std::string& author, const std::string& isbn);
    bool RemoveBook(const std::string& isbn);
    Book* FindBook(const std::string& isbn) const;
    std::vector<Book*> FindBooksByTitle(const std::string& title, const std::string& author) const;
    std::vector<Book*> GetAllBooks() const;
    //User management
    std::unique_ptr<User> CreateUserByType(const std::string& name, const std::string& userId,const std::string& email, UserType type);
    void RegisterUser(const std::string& name, const std::string& userId, const std::string& email, UserType type);
    User* FindUser(const std::string& userId) const;
    std::vector<User*> GetAllUsers() const;
    
    //Borrow management
    bool BorrowBook(const std::string& userId, const std::string& isbn);
    bool ReturnBook(const std::string& userId, const std::string& isbn);
    
    std::vector<BorrowingRecord> GetOverdueBooks() const;
    std::vector<BorrowingRecord> GetBorrowHistory() const;
    std::vector<BorrowingRecord> GetCurrentBorrowedBooks() const;
};