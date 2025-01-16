#pragma once

#pragma once
#include <string>

using namespace std;

class Exception {
    string msg;
public:
    Exception(string m) : msg{ m } {
    };

    string get_msg() {
        return msg;
    }
};