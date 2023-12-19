#pragma once
#include <exception>

class NonFatalException : public virtual std::exception {
    
    const char* code12() const noexcept override
    {
        return "Integer overflow";
    }
    const char* code13() const noexcept override
    {
        return "Integer underflow";
    }
    const char* code14() const noexcept override
    {
        return "Invalid color";
    }
 
//public:
//    explicit Exception(const char* message)
//        : msg_(message) {}
//
//    explicit Exception(const std::string& message)
//        : msg_(message) {}
//
//    virtual ~Exception() noexcept {}
//
//
//    virtual const char* what() const noexcept {
//       return msg_.c_str();
//    }
//
//private:
//    int exceptionCode;
} NonFatalException;

class FatalException : public virtual std::exception {
 
public:
    explicit Exception(const char* message)
        : msg_(message) {}

    explicit Exception(const std::string& message)
        : msg_(message) {}

    virtual ~Exception() noexcept {}


    virtual const char* what() const noexcept {
       return msg_.c_str();
    }
    
private:
    int exceptionCode;
};


