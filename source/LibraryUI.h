#pragma once  // Убрать точку с запятой

#include "Library.h"

class LibraryUI {
private:
    Library library_;

    void ShowMenu();
    void HandleBookManagement();
    void HandleUserManagement();
    void HandleBorrowingManagement();
    int GetIntInput(const std::string& prompt);
    std::string GetStringInput(const std::string& prompt);

public:
    void Run();
};