#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

enum class UserType{
    STUDENT,
    FACULTY,
    GUEST
};

class User{
private:
    std::string name_;
    std::string userId_;
    std::string email_;
    std::vector<std::string> borrowedBooks_;

public:
    User(const std::string& name, const std::string& userId, const std::string& email);
    virtual ~User() = default;

    //getters
    std::string GetName() const;
    std::string GetUserId() const;
    std::string GetEmail() const;
    std::vector<std::string> GetBorrowedBooks() const;
    virtual UserType GetType() const = 0;

    //func
    virtual int GetMaxBooks() const = 0;
    virtual int GetBorrowDays() const = 0;
    bool CanBorrow() const;
    void BorrowBook(const std::string& isbn);
    void ReturnBook(const std::string& isbn);
};


class Student : public User{
public:
    Student(const std::string& name, const std::string& userId, const std::string& email);

    int GetMaxBooks() const override;
    int GetBorrowDays() const override;
    UserType GetType() const override;
};

class Faculty : public User{
public:
    Faculty(const std::string& name, const std::string& userId, const std::string& email);

    int GetMaxBooks() const override;
    int GetBorrowDays() const override;
    UserType GetType() const override;
};

class Guest : public User{
public:
    Guest(const std::string& name, const std::string& userId, const std::string& email);

    int GetMaxBooks() const override;
    int GetBorrowDays() const override;
    UserType GetType() const override;
};