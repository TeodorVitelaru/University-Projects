#pragma once
#include "Task.h"

class Repo {
private:
    string filename;
    vector<Task> tasks;
public:
    Repo(const string& filename) : filename{filename} {
        load_from_file();
    }
    Repo(const Repo& ot) = delete;
    void load_from_file();
    void write_to_file();
    void store(const Task& task);
    bool exists(int id);
    void modify(Task& task, string stare);
    vector<Task>& get_all()
    {
        return this->tasks;
    }

};



