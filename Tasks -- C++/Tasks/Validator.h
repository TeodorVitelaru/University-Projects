#include "Task.h"
#include "Errors.h"
#pragma once

class Validator
{
public:
    void validare(int id, string descriere, vector<string> programatori, string stare)
    {
        string err;
        if (id < 0)
            err.append("Id invalid!\n");
        if (descriere.size() == 0)
            err.append("Descriere invalida!\n");
        if (programatori.size() <1 || programatori.size() > 4)
            err.append("Programatori invalidi!\n");
        if (stare != "open" && stare != "inprogress" && stare != "closed")
            err.append("Stare invalida!\n");
        if (err.size() > 0)
            throw Exception(err);
    }

};
