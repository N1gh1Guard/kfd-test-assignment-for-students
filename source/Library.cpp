#include "Library.h"
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <memory>

BorrowingRecord::BorrowingRecord(const std::string& userId, const std::string& isbn, 
                               std::time_t borrowDate, std::time_t dueDate)
    : userId_(userId), isbn_(isbn), borrowDate_(borrowDate), dueDate_(dueDate), returned_(false) {}

// Book management
void Library::AddBook(const std::string& title, const std::string& author, const std::string& isbn) {
    if(title.empty() || author.empty() || isbn.empty()) {
        throw std::invalid_argument("No title, author or ISBN was provided");
    }
    
    if(books_.find(isbn) != books_.end()) {
        throw std::runtime_error("Book with ISBN " + isbn + " already exists");
    }

    books_.emplace(isbn, std::make_unique<Book>(title, author, isbn));
}

bool Library::RemoveBook(const std::string& isbn) {
    if (isbn.empty()) {
        throw std::invalid_argument("No ISBN was provided");
    }

    auto it = books_.find(isbn);
    if (it == books_.end()) {
        return false;
    }

    if (!it->second->IsAvailable()) {
        throw std::runtime_error("Book with ISBN " + isbn + " is currently borrowed");
    }

    books_.erase(it);
    return true;
}

Book* Library::FindBook(const std::string& isbn) const {
    if (isbn.empty()) {
        throw std::invalid_argument("No ISBN was provided");
    }

    auto it = books_.find(isbn);
    if (it != books_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Book*> Library::FindBooksByTitle(const std::string& title, const std::string& author) const {
    std::vector<Book*> results;
    
    if(title.empty() || author.empty()){
        throw std::invalid_argument("No title or author was provided");
    }

    for (const auto& pair : books_) {
        Book* book = pair.second.get();
        
        if (book->GetTitle().find(title) != std::string::npos && book->GetAuthor().find(author) != std::string::npos) {
            results.push_back(book);
        }
    }
    
    return results;
}

std::vector<Book*> Library::GetAllBooks() const {
    std::vector<Book*> result;
    for (const auto& pair : books_) {
        result.push_back(pair.second.get());
    }
    return result;
}

// User management
std::unique_ptr<User> Library::CreateUserByType(const std::string& name, const std::string& userId, const std::string& email, UserType type) {
    if(name.empty() || userId.empty() || email.empty()) {
        throw std::invalid_argument("No name, userID or email was provided");
    }
    
    switch(type) {
        case UserType::STUDENT:
            return std::make_unique<Student>(name, userId, email);
        case UserType::FACULTY:
            return std::make_unique<Faculty>(name, userId, email);
        case UserType::GUEST:
            return std::make_unique<Guest>(name, userId, email);
        default:
            throw std::invalid_argument("Invalid user type");
    }
}

void Library::RegisterUser(const std::string& name, const std::string& userId, 
                         const std::string& email, UserType type) {
    if(name.empty() || userId.empty() || email.empty()) {
        throw std::invalid_argument("No name, userID or email was provided");
    }
    
    if (users_.find(userId) != users_.end()) {
        throw std::runtime_error("User with ID " + userId + " already exists");
    }

    auto user = CreateUserByType(name, userId, email, type);
    users_[userId] = std::move(user);
}

User* Library::FindUser(const std::string& userId) const {
    auto it = users_.find(userId);
    if (it != users_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<User*> Library::GetAllUsers() const {
    std::vector<User*> result;
    for (const auto& pair : users_) {
        result.push_back(pair.second.get());
    }
    return result;
}

// Borrow management
std::time_t calculateDueDate(int borrowDays) {
    std::time_t now = std::time(nullptr);
    return now + (borrowDays * 3); // Для удобства тестирования сделал внутренний день равный 3 секундам
}

bool Library::BorrowBook(const std::string& userId, const std::string& isbn) {
    if(userId.empty() || isbn.empty()) {
        throw std::invalid_argument("No userID or ISBN was provided");
    }

    User* user = FindUser(userId);
    if(!user) {
        return false;
    }
    
    Book* book = FindBook(isbn);
    if(!book) {
        return false;
    }
    
    if(!book->IsAvailable()) {
        return false;
    }
    
    if(!user->CanBorrow()) {
        return false;
    }

    std::time_t dueDate = calculateDueDate(user->GetBorrowDays());
    borrowHistory_.emplace_back(userId, isbn, std::time(nullptr), dueDate);
    
    book->SetAvailable(false);
    user->BorrowBook(isbn);
    
    return true;
}

bool Library::ReturnBook(const std::string& userId, const std::string& isbn) {
    if(userId.empty() || isbn.empty()) {
        throw std::invalid_argument("No userID or ISBN was provided");
    }

    User* user = FindUser(userId);
    if(!user) {
        return false;
    }
    
    Book* book = FindBook(isbn);
    if(!book){
        return false;
    }

    if(book->IsAvailable()) {
        return false;
    }

    bool recordFound = false;
    for(auto& record : borrowHistory_) {
        if (record.userId_ == userId && record.isbn_ == isbn && !record.returned_) {
            record.returned_ = true;
            recordFound = true;
            break;
        }
    }

    if(!recordFound) {
        return false;
    }

    book->SetAvailable(true);
    user->ReturnBook(isbn);
    
    return true;
}

std::vector<BorrowingRecord> Library::GetOverdueBooks() const {
    std::vector<BorrowingRecord> overdue;
    std::time_t now = std::time(nullptr);

    for(const auto& record : borrowHistory_) {
        if (!record.returned_ && record.dueDate_ < now) {
            overdue.push_back(record);
        }
    }

    return overdue;
}

std::vector<BorrowingRecord> Library::GetBorrowHistory() const {
    return borrowHistory_;
}

std::vector<BorrowingRecord> Library::GetCurrentBorrowedBooks() const {
    std::vector<BorrowingRecord> result;
    for (const auto& record : borrowHistory_) {
        if (!record.returned_) {
            result.push_back(record);
        }
    }
    return result;
}