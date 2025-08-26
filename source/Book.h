#pragma once

#include <string>
#include <vector>
#include <stdexcept>

class Book{
private:
    std::string title_;
    std::string author_;
    std::string isbn_;
    bool available_;

public:
    Book(const std::string& title, const std::string& author, const std::string& isbn);

    std::string GetTitle() const;
    std::string GetAuthor() const;
    std::string GetIsbn() const;
    bool IsAvailable() const;

    void SetAvailable(bool status);

    void Display() const;
};