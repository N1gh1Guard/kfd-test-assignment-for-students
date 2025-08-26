#include "Book.h"

#include <iostream>

Book::Book(const std::string& title, const std::string& author, const std::string& isbn)
    : title_(title), author_(author), isbn_(isbn), available_(true){}

std::string Book::GetTitle() const {
    return title_;
}
std::string Book::GetAuthor() const {
    return author_;
}
std::string Book::GetIsbn() const {
    return isbn_;
}
bool Book::IsAvailable() const {
    return available_;
}

void Book::SetAvailable(bool status){
    available_ = status;
}
    
void Book::Display() const {
    std::cout << "Title: " << title_ << "\nAuthor: " << author_ 
              << "\nISBN: " << isbn_ 
              << "\nStatus: " << (available_ ? "Available" : "Borrowed") << "\n\n";
}
