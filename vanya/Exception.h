#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception
{
    int code;
    char message[80];
public:
    Exception();
    ~Exception();
    Exception(int errorCode, const char* errorMessage);
    void show() const;

};

#endif
