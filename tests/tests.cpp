#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Book.h"
#include "User.h"
#include "Library.h"
#include <ctime>
#include <thread>
#include <stdexcept>
#include <sstream>
#include <iostream>

// Тесты для класса Book
TEST_CASE("Book constructor and getters", "[Book]") {
    Book book("Title", "Author", "123");
    
    REQUIRE(book.GetTitle() == "Title");
    REQUIRE(book.GetAuthor() == "Author");
    REQUIRE(book.GetIsbn() == "123");
    REQUIRE(book.IsAvailable() == true);
}

TEST_CASE("Book SetAvailable", "[Book]") {
    Book book("Title", "Author", "123");
    
    book.SetAvailable(false);
    REQUIRE(book.IsAvailable() == false);
    
    book.SetAvailable(true);
    REQUIRE(book.IsAvailable() == true);
}

TEST_CASE("Book Display", "[Book]") {
    Book book("Test Book", "Test Author", "TEST123");
    
    // Перенаправляем вывод для проверки
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    book.Display();
    
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    
    REQUIRE(output.find("Test Book") != std::string::npos);
    REQUIRE(output.find("Test Author") != std::string::npos);
    REQUIRE(output.find("TEST123") != std::string::npos);
}

// Тесты для класса User и его наследников
TEST_CASE("User constructor and getters", "[User]") {
    Student student("John", "001", "john@example.com");
    
    REQUIRE(student.GetName() == "John");
    REQUIRE(student.GetUserId() == "001");
    REQUIRE(student.GetEmail() == "john@example.com");
    REQUIRE(student.GetBorrowedBooks().empty());
}

TEST_CASE("Student limits", "[User]") {
    Student student("John", "001", "john@example.com");
    
    REQUIRE(student.GetMaxBooks() == 3);
    REQUIRE(student.GetBorrowDays() == 14);
    REQUIRE(student.GetType() == UserType::STUDENT);
    REQUIRE(student.CanBorrow() == true);
}

TEST_CASE("Faculty limits", "[User]") {
    Faculty faculty("Dr. Smith", "002", "smith@example.com");
    
    REQUIRE(faculty.GetMaxBooks() == 10);
    REQUIRE(faculty.GetBorrowDays() == 30);
    REQUIRE(faculty.GetType() == UserType::FACULTY);
    REQUIRE(faculty.CanBorrow() == true);
}

TEST_CASE("Guest limits", "[User]") {
    Guest guest("Visitor", "003", "visitor@example.com");
    
    REQUIRE(guest.GetMaxBooks() == 1);
    REQUIRE(guest.GetBorrowDays() == 7);
    REQUIRE(guest.GetType() == UserType::GUEST);
    REQUIRE(guest.CanBorrow() == true);
}

TEST_CASE("Borrow and return books", "[User]") {
    Student student("John", "001", "john@example.com");
    
    // Берем первую книгу
    student.BorrowBook("BOOK1");
    REQUIRE(student.GetBorrowedBooks().size() == 1);
    REQUIRE(student.CanBorrow() == true);
    
    // Берем вторую книгу
    student.BorrowBook("BOOK2");
    REQUIRE(student.GetBorrowedBooks().size() == 2);
    REQUIRE(student.CanBorrow() == true);
    
    // Берем третью книгу (достигаем лимита)
    student.BorrowBook("BOOK3");
    REQUIRE(student.GetBorrowedBooks().size() == 3);
    REQUIRE(student.CanBorrow() == false);
    
    // Возвращаем одну книгу
    student.ReturnBook("BOOK2");
    REQUIRE(student.GetBorrowedBooks().size() == 2);
    REQUIRE(student.CanBorrow() == true);
    
    // Пытаемся вернуть несуществующую книгу
    REQUIRE_THROWS_AS(student.ReturnBook("NONEXISTENT"), std::runtime_error);
}

// Тесты для класса Library
class LibraryFixture {
protected:
    Library library;
    
    LibraryFixture() {
        library.AddBook("C++ Primer", "Stanley Lippman", "9780321714114");
        library.AddBook("Effective C++", "Scott Meyers", "9780321334879");
        library.AddBook("Clean Code", "Robert Martin", "9780132350884");
        
        library.RegisterUser("John Student", "S001", "john@edu.com", UserType::STUDENT);
        library.RegisterUser("Dr. Smith", "F001", "smith@uni.edu", UserType::FACULTY);
        library.RegisterUser("Visitor", "G001", "visitor@mail.com", UserType::GUEST);
    }
};

TEST_CASE_METHOD(LibraryFixture, "Add book", "[Library]") {
    REQUIRE_NOTHROW(library.AddBook("New Book", "New Author", "NEW123"));
    
    // Попытка добавить книгу с пустыми полями
    REQUIRE_THROWS_AS(library.AddBook("", "Author", "ISBN123"), std::invalid_argument);
    REQUIRE_THROWS_AS(library.AddBook("Title", "", "ISBN123"), std::invalid_argument);
    REQUIRE_THROWS_AS(library.AddBook("Title", "Author", ""), std::invalid_argument);
    
    // Попытка добавить книгу с существующим ISBN
    REQUIRE_THROWS_AS(library.AddBook("Different Title", "Different Author", "9780321714114"), std::runtime_error);
}

TEST_CASE_METHOD(LibraryFixture, "Remove book", "[Library]") {
    // Успешное удаление книги
    REQUIRE(library.RemoveBook("9780321714114") == true);
    
    // Попытка удалить несуществующую книгу
    REQUIRE(library.RemoveBook("NONEXISTENT") == false);
    
    // Попытка удалить взятую книгу
    library.BorrowBook("S001", "9780132350884");
    REQUIRE_THROWS_AS(library.RemoveBook("9780132350884"), std::runtime_error);
}

TEST_CASE_METHOD(LibraryFixture, "Find book", "[Library]") {
    // Поиск существующей книги
    Book* book = library.FindBook("9780321714114");
    REQUIRE(book != nullptr);
    REQUIRE(book->GetTitle() == "C++ Primer");
    
    // Поиск несуществующей книги
    REQUIRE(library.FindBook("NONEXISTENT") == nullptr);
    
    // Попытка поиска с пустым ISBN
    REQUIRE_THROWS_AS(library.FindBook(""), std::invalid_argument);
}

TEST_CASE_METHOD(LibraryFixture, "Find books by title", "[Library]") {
    // Поиск по заголовку и автору
    std::vector<Book*> results = library.FindBooksByTitle("C++", "Lippman");
    REQUIRE(results.size() == 1);
    REQUIRE(results[0]->GetIsbn() == "9780321714114");
    
    // Поиск по частичному заголовку
    results = library.FindBooksByTitle("Effective", "Meyers");
    REQUIRE(results.size() == 1);
    REQUIRE(results[0]->GetIsbn() == "9780321334879");
    
    // Поиск несуществующей книги
    results = library.FindBooksByTitle("Nonexistent", "Author");
    REQUIRE(results.empty());
    
    // Попытка поиска с пустыми параметрами
    REQUIRE_THROWS_AS(library.FindBooksByTitle("", "Author"), std::invalid_argument);
    REQUIRE_THROWS_AS(library.FindBooksByTitle("Title", ""), std::invalid_argument);
}

TEST_CASE_METHOD(LibraryFixture, "Register user", "[Library]") {
    // Успешная регистрация пользователя
    REQUIRE_NOTHROW(library.RegisterUser("New Student", "S002", "new@edu.com", UserType::STUDENT));
    
    // Попытка зарегистрировать пользователя с пустыми полями
    REQUIRE_THROWS_AS(library.RegisterUser("", "ID", "email@test.com", UserType::STUDENT), std::invalid_argument);
    REQUIRE_THROWS_AS(library.RegisterUser("Name", "", "email@test.com", UserType::STUDENT), std::invalid_argument);
    REQUIRE_THROWS_AS(library.RegisterUser("Name", "ID", "", UserType::STUDENT), std::invalid_argument);
    
    // Попытка зарегистрировать пользователя с существующим ID
    REQUIRE_THROWS_AS(library.RegisterUser("Different Name", "S001", "different@test.com", UserType::FACULTY), std::runtime_error);
}

TEST_CASE_METHOD(LibraryFixture, "Find user", "[Library]") {
    // Поиск существующего пользователя
    User* user = library.FindUser("S001");
    REQUIRE(user != nullptr);
    REQUIRE(user->GetName() == "John Student");
    
    // Поиск несуществующего пользователя
    REQUIRE(library.FindUser("NONEXISTENT") == nullptr);
}

TEST_CASE_METHOD(LibraryFixture, "Borrow book", "[Library]") {
    // Успешное взятие книги
    REQUIRE(library.BorrowBook("S001", "9780321714114") == true);
    
    // Попытка взять уже взятую книгу
    REQUIRE(library.BorrowBook("F001", "9780321714114") == false);
    
    // Попытка взять книгу несуществующим пользователем
    REQUIRE(library.BorrowBook("NONEXISTENT", "9780321714114") == false);
    
    // Попытка взять несуществующую книгу
    REQUIRE(library.BorrowBook("S001", "NONEXISTENT") == false);
    
    // Попытка взять книгу с пустыми параметрами
    REQUIRE_THROWS_AS(library.BorrowBook("", "9780321714114"), std::invalid_argument);
    REQUIRE_THROWS_AS(library.BorrowBook("S001", ""), std::invalid_argument);
    
    // Попытка взять книгу при достижении лимита
    // Гость берет одну книгу (достигает лимита)
    REQUIRE(library.BorrowBook("G001", "9780321334879") == true);
    
    // Гость пытается взять вторую книгу
    REQUIRE(library.BorrowBook("G001", "9780132350884") == false);
}

TEST_CASE_METHOD(LibraryFixture, "Return book", "[Library]") {
    // Сначала берем книгу
    REQUIRE(library.BorrowBook("S001", "9780321714114") == true);
    
    // Успешный возврат книги
    REQUIRE(library.ReturnBook("S001", "9780321714114") == true);
    
    // Попытка вернуть уже возвращенную книгу
    REQUIRE(library.ReturnBook("S001", "9780321714114") == false);
    
    // Попытка вернуть несуществующую книгу
    REQUIRE(library.ReturnBook("S001", "NONEXISTENT") == false);
    
    // Попытка вернуть книгу несуществующим пользователем
    REQUIRE(library.ReturnBook("NONEXISTENT", "9780321714114") == false);
    
    // Попытка вернуть книгу с пустыми параметрами
    REQUIRE_THROWS_AS(library.ReturnBook("", "9780321714114"), std::invalid_argument);
    REQUIRE_THROWS_AS(library.ReturnBook("S001", ""), std::invalid_argument);
    
    // Попытка вернуть книгу, которая не была взята
    REQUIRE(library.ReturnBook("F001", "9780321334879") == false);
}

TEST_CASE_METHOD(LibraryFixture, "Overdue books", "[Library]") {
    // Берем книгу
    REQUIRE(library.BorrowBook("G001", "9780321334879") == true);
    
    // Ждем, пока книга станет просроченной (для гостя 7 дней = 21 секунда)
    std::this_thread::sleep_for(std::chrono::seconds(22));
    
    // Проверяем, что книга просрочена
    std::vector<BorrowingRecord> overdue = library.GetOverdueBooks();
    REQUIRE(overdue.size() == 1);
    REQUIRE(overdue[0].userId_ == "G001");
    REQUIRE(overdue[0].isbn_ == "9780321334879");
    
    // Возвращаем книгу
    REQUIRE(library.ReturnBook("G001", "9780321334879") == true);
    
    // Проверяем, что после возврата книга больше не считается просроченной
    overdue = library.GetOverdueBooks();
    REQUIRE(overdue.empty());
}

TEST_CASE_METHOD(LibraryFixture, "User type specific behavior", "[Library]") {
    // Проверяем разные лимиты для разных типов пользователей
    
    // Студент может взять 3 книги
    for (int i = 0; i < 3; i++) {
        std::string isbn = "TEST" + std::to_string(i);
        library.AddBook("Test Book " + std::to_string(i), "Test Author", isbn);
        REQUIRE(library.BorrowBook("S001", isbn) == true);
    }
    
    // Студент не может взять четвертую книгу
    library.AddBook("Extra Book", "Extra Author", "EXTRA");
    REQUIRE(library.BorrowBook("S001", "EXTRA") == false);
    
    // Преподаватель может взять больше книг
    for (int i = 0; i < 5; i++) {
        std::string isbn = "FACULTY" + std::to_string(i);
        library.AddBook("Faculty Book " + std::to_string(i), "Faculty Author", isbn);
        REQUIRE(library.BorrowBook("F001", isbn) == true);
    }
    
    // Гость может взять только одну книгу
    REQUIRE(library.BorrowBook("G001", "9780321714114") == true);
    REQUIRE(library.BorrowBook("G001", "9780132350884") == false);
}

TEST_CASE_METHOD(LibraryFixture, "Get all books", "[Library]") {
    auto books = library.GetAllBooks();
    REQUIRE(books.size() == 3);
    
    // Проверяем, что все книги имеют правильные данные
    bool foundPrimer = false, foundEffective = false, foundClean = false;
    for (const auto& book : books) {
        if (book->GetTitle() == "C++ Primer") foundPrimer = true;
        if (book->GetTitle() == "Effective C++") foundEffective = true;
        if (book->GetTitle() == "Clean Code") foundClean = true;
    }
    
    REQUIRE(foundPrimer);
    REQUIRE(foundEffective);
    REQUIRE(foundClean);
}

TEST_CASE_METHOD(LibraryFixture, "Get all users", "[Library]") {
    auto users = library.GetAllUsers();
    REQUIRE(users.size() == 3);
    
    // Проверяем, что все пользователи имеют правильные данные
    bool foundStudent = false, foundFaculty = false, foundGuest = false;
    for (const auto& user : users) {
        if (user->GetName() == "John Student") foundStudent = true;
        if (user->GetName() == "Dr. Smith") foundFaculty = true;
        if (user->GetName() == "Visitor") foundGuest = true;
    }
    
    REQUIRE(foundStudent);
    REQUIRE(foundFaculty);
    REQUIRE(foundGuest);
}

TEST_CASE_METHOD(LibraryFixture, "Get borrow history", "[Library]") {
    // Изначально история пуста
    auto history = library.GetBorrowHistory();
    REQUIRE(history.empty());
    
    // Берем книгу
    REQUIRE(library.BorrowBook("S001", "9780321714114"));
    
    // Теперь в истории должна быть одна запись
    history = library.GetBorrowHistory();
    REQUIRE(history.size() == 1);
    REQUIRE(history[0].userId_ == "S001");
    REQUIRE(history[0].isbn_ == "9780321714114");
    REQUIRE_FALSE(history[0].returned_);
    
    // Возвращаем книгу
    REQUIRE(library.ReturnBook("S001", "9780321714114"));
    
    // История все еще должна содержать запись, но с флагом returned
    history = library.GetBorrowHistory();
    REQUIRE(history.size() == 1);
    REQUIRE(history[0].returned_);
}

TEST_CASE_METHOD(LibraryFixture, "Get current borrowed books", "[Library]") {
    // Изначально нет текущих займов
    auto current = library.GetCurrentBorrowedBooks();
    REQUIRE(current.empty());
    
    // Берем книгу
    REQUIRE(library.BorrowBook("S001", "9780321714114"));
    
    // Теперь должна быть одна текущая книга
    current = library.GetCurrentBorrowedBooks();
    REQUIRE(current.size() == 1);
    REQUIRE(current[0].userId_ == "S001");
    REQUIRE(current[0].isbn_ == "9780321714114");
    REQUIRE_FALSE(current[0].returned_);
    
    // Возвращаем книгу
    REQUIRE(library.ReturnBook("S001", "9780321714114"));
    
    // Теперь снова нет текущих займов
    current = library.GetCurrentBorrowedBooks();
    REQUIRE(current.empty());
}

// Тест для проверки создания пользователей разных типов
TEST_CASE("Create user by type", "[Library]") {
    Library lib;
    
    // Создание студента
    auto student = lib.CreateUserByType("Student", "S1", "student@test.com", UserType::STUDENT);
    REQUIRE(student->GetType() == UserType::STUDENT);
    REQUIRE(student->GetMaxBooks() == 3);
    
    // Создание преподавателя
    auto faculty = lib.CreateUserByType("Faculty", "F1", "faculty@test.com", UserType::FACULTY);
    REQUIRE(faculty->GetType() == UserType::FACULTY);
    REQUIRE(faculty->GetMaxBooks() == 10);
    
    // Создание гостя
    auto guest = lib.CreateUserByType("Guest", "G1", "guest@test.com", UserType::GUEST);
    REQUIRE(guest->GetType() == UserType::GUEST);
    REQUIRE(guest->GetMaxBooks() == 1);
    
    // Попытка создать пользователя с неверным типом
    REQUIRE_THROWS_AS(lib.CreateUserByType("Invalid", "I1", "invalid@test.com", static_cast<UserType>(999)), std::invalid_argument);
}

// Дополнительные тесты для проверки поиска книг с разными параметрами
TEST_CASE_METHOD(LibraryFixture, "Search books with empty parameters", "[Library]") {
    // Поиск с пустым заголовком, но указанным автором
    REQUIRE_THROWS_AS(library.FindBooksByTitle("", "Lippman"), std::invalid_argument);
    
    // Поиск с указанным заголовком, но пустым автором
    REQUIRE_THROWS_AS(library.FindBooksByTitle("C++", ""), std::invalid_argument);
}

// Тест для проверки отображения информации о книгах
TEST_CASE_METHOD(LibraryFixture, "Book display information", "[Library]") {
    Book* book = library.FindBook("9780321714114");
    REQUIRE(book != nullptr);
    
    // Перенаправляем вывод для проверки
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    book->Display();
    
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    
    REQUIRE(output.find("C++ Primer") != std::string::npos);
    REQUIRE(output.find("Stanley Lippman") != std::string::npos);
    REQUIRE(output.find("9780321714114") != std::string::npos);
}

// Тест для проверки пользовательских ограничений
TEST_CASE_METHOD(LibraryFixture, "User borrowing limits", "[Library]") {
    // Проверяем, что студент не может превысить лимит
    Student* student = dynamic_cast<Student*>(library.FindUser("S001"));
    REQUIRE(student != nullptr);
    
    // Добавляем дополнительные книги
    for (int i = 0; i < 3; i++) {
        std::string isbn = "STU_BOOK_" + std::to_string(i);
        library.AddBook("Student Book " + std::to_string(i), "Author", isbn);
    }
    
    // Студент берет книги до лимита
    for (int i = 0; i < 3; i++) {
        std::string isbn = "STU_BOOK_" + std::to_string(i);
        REQUIRE(library.BorrowBook("S001", isbn) == true);
    }
    
    // Студент не может взять четвертую книгу
    REQUIRE(library.BorrowBook("S001", "9780321714114") == false);
}

// Тест для проверки времени возврата книг
TEST_CASE_METHOD(LibraryFixture, "Book return timing", "[Library]") {
    // Берем книгу студентом
    REQUIRE(library.BorrowBook("S001", "9780321714114"));
    
    // Ждем немного
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Возвращаем книгу
    REQUIRE(library.ReturnBook("S001", "9780321714114"));
    
    // Проверяем, что книга снова доступна
    Book* book = library.FindBook("9780321714114");
    REQUIRE(book != nullptr);
    REQUIRE(book->IsAvailable() == true);
}