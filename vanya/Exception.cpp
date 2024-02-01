#include "Exception.h"
#include <iostream>
Exception::Exception() {}
Exception::~Exception() {}

Exception::Exception(int errorCode, const char *errorMessage)
{
    code = errorCode;
    std::strncpy(this->message, errorMessage, 80);

}
void Exception::show() const {
    std::cout << "\nError code: " << code << "\nError message: " << message << std::endl;
}
