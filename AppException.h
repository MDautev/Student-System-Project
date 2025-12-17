#ifndef APP_EXCEPTION_H
#define APP_EXCEPTION_H

#include <exception>
#include <string>

class AppException : public std::exception
{
protected:
    std::string message;

public:
    AppException(const std::string &msg = "Грешка в приложението") : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// Специализирани изключения
class InvalidEGNException : public AppException
{
public:
    InvalidEGNException() : AppException("Невалидно ЕГН!") {}
};

class StudentNotFoundException : public AppException
{
public:
    StudentNotFoundException() : AppException("Студентът не е намерен!") {}
};

class InvalidDateException : public AppException
{
public:
    InvalidDateException() : AppException("Невалидна дата!") {}
};

class InvalidGradeException : public AppException
{
public:
    InvalidGradeException() : AppException("Невалидна оценка!") {}
};

class InvalidGroupException : public AppException
{
public:
    InvalidGroupException() : AppException("Невалидна група!") {}
};

class DuplicateFacultyNumberException : public AppException
{
public:
    DuplicateFacultyNumberException() : AppException("Факултетният номер вече съществува!") {}
};

class InvalidGenderException : public AppException
{
public:
    InvalidGenderException() : AppException("Невалиден пол!") {}
};

class InvalidYearException : public AppException
{
public:
    InvalidYearException() : AppException("Невалидна година!") {}
};

class DuplicateEGNException : public AppException {
public:
    const char* what() const noexcept override {
        return "Вече съществува човек с това ЕГН!";
    }
};

#endif
