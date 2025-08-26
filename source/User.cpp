#include "User.h"

User::User(const std::string& name, const std::string& userId, const std::string& email) 
        : name_(name), userId_(userId), email_(email){}

//getters
std::string User::GetName() const {
    return name_;
}
 
std::string User::GetUserId() const {
    return userId_;
}
 
std::string User::GetEmail() const {
    return email_;
}
 
std::vector<std::string> User::GetBorrowedBooks() const{
    return borrowedBooks_;
}

bool User::CanBorrow() const {
    return borrowedBooks_.size() < static_cast<size_t>(GetMaxBooks());
}

void User::BorrowBook(const std::string& isbn){
    borrowedBooks_.push_back(isbn);
}

void User::ReturnBook(const std::string& isbn){
    auto cur_book = std::find(borrowedBooks_.begin(),borrowedBooks_.end(), isbn);
    if (cur_book == borrowedBooks_.end()) {
        throw std::runtime_error("User did not borrow this book");
    }
    
    borrowedBooks_.erase(cur_book);
}



Student::Student(const std::string& name, const std::string& userId, const std::string& email)
    : User(name, userId, email) {}

int Student::GetMaxBooks() const { return 3; }
int Student::GetBorrowDays() const { return 14; }
UserType Student::GetType() const { return UserType::STUDENT; }


Faculty::Faculty(const std::string& name, const std::string& userId, const std::string& email)
    : User(name, userId, email) {}

int Faculty::GetMaxBooks() const { return 10; }
int Faculty::GetBorrowDays() const { return 30; }    
UserType Faculty::GetType() const { return UserType::FACULTY; }


Guest::Guest(const std::string& name, const std::string& userId, const std::string& email)
    : User(name, userId, email) {}

int Guest::GetMaxBooks() const { return 1; }
int Guest::GetBorrowDays() const { return 7; }
UserType Guest::GetType() const { return UserType::GUEST; }
