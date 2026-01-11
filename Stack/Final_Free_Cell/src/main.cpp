#include "raylib.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

// ========== Stack Implementation ==========
template <class T>
class Stack {
public:
    int size, top;
    T *ptr;

    Stack(int a = 100) {
        ptr = new T[a];
        top = -1;
        size = a;
    }

    Stack(const Stack& other) {
        size = other.size;
        top = other.top;
        ptr = new T[size];
        for (int i = 0; i <= top; i++) {
            ptr[i] = other.ptr[i];
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] ptr;
            size = other.size;
            top = other.top;
            ptr = new T[size];
            for (int i = 0; i <= top; i++) {
                ptr[i] = other.ptr[i];
            }
        }
        return *this;
    }
