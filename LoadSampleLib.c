// On unix, make sure to compile using -ldl and -pthread flags.
// See https://github.com/dotnet/samples/blob/main/core/nativeaot/NativeLibrary

// Set this value accordingly to your workspace settings.
#if defined(_WIN32)
#define PathToLibrary "bin\\Debug\\net7.0\\win-x64\\native\\libsample.dll"
#elif defined(__APPLE__)
#define PathToLibrary "./bin/Debug/net7.0/osx-x64/native/libsample.dylib"
#else
#define PathToLibrary "./bin/release/net7.0/linux-x64/native/libsample.so"
#endif

#ifdef _WIN32
#include "windows.h"
#define symLoad GetProcAddress
#else
#include "dlfcn.h"
#include <unistd.h>
#define symLoad dlsym
#endif

#include <stdlib.h>
#include <stdio.h>

#ifndef F_OK
#define F_OK 0
#endif

int callAdd(int a, int b);
int callSubtract(int a, int b);
int callMultiply(int a, int b);
char *callGreet(char *name);

typedef int (*arithmeticFunction)(int, int);

int main()
{
    // Check if the library file exists
    if (access(PathToLibrary, F_OK) == -1)
    {
        puts("Couldn't find library at the specified path");
        return 0;
    }

    int a = 8.0, b = 2.0;

    // Sum
    int sum = callAdd(a, b);
    printf("The sum is %d.\n", sum);

    // Subtract
    int difference = callSubtract(a, b);
    printf("The difference is %d.\n", difference);

    // Multiply
    int product = callMultiply(a, b);
    printf("The product is %d.\n", product);

    // Greet
    char *result = callGreet("General Kenobi");
    printf("%s.\n", result);

    // Greet
    char *nullResult = callGreet(NULL);
    printf("%s.\n", nullResult);
}

// Common method to load arithmetic functions since they have similar signatures.
arithmeticFunction loadFunction(char *functionName)
{
#ifdef _WIN32
    HINSTANCE handle = LoadLibraryA(path);
#else
    void *handle = dlopen(PathToLibrary, RTLD_LAZY);
#endif

    // CoreRT/NativeAOT libraries do not support unloading
    // See https://github.com/dotnet/corert/issues/7887
    return (arithmeticFunction)symLoad(handle, functionName);
}

// Call the add function defined in the C# shared library
int callAdd(int a, int b)
{
    arithmeticFunction add = loadFunction("add");
    return add(a, b);
}

// Calls the subtract functon defined in the C# shared library
int callSubtract(int a, int b)
{
    arithmeticFunction subtract = loadFunction("subtract");
    return subtract(a, b);
}

// Calls the multiply functon defined in the C# shared library
int callMultiply(int a, int b)
{
    arithmeticFunction multiply = loadFunction("multiply");
    return multiply(a, b);
}

// Call the greet function defined in the C# shared library
char *callGreet(char *name)
{
// Library loading
#ifdef _WIN32
    HINSTANCE handle = LoadLibraryA(PathToLibrary);
#else
    void *handle = dlopen(PathToLibrary, RTLD_LAZY);
#endif

    // Declare a typedef
    typedef char *(*greetFunction)(char *);

    // Import Symbol named funcName
    greetFunction greet = (greetFunction)symLoad(handle, "greet");

    // The C# function will return a pointer
    return greet(name);
}