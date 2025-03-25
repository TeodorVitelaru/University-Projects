

#include "Repo.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Errors.h"


void Repo::load_from_file()
{
    ifstream f(this->filename);
    if (!f.is_open())
        throw Exception("Fisierul nu exista!");
    while (!f.eof())
    {
        int id;
        string descriere;
        vector<string> programatori;
        string stare;
        string line;
        f >> id;
        f >> descriere;
        f >> stare;
        getline(f, line); // read the rest of the line (the programatori
        if (!getline(f, line)) {
            break;
        }

        // Use a stringstream to split the line into individual programmers
        stringstream ss(line);
        string programmer;
        while (ss >> programmer) {
            programatori.push_back(programmer);
        }
        Task t{id, descriere, programatori, stare};
        this->tasks.push_back(t);
    }
}

void Repo::write_to_file() {
    ofstream f(this->filename);
    if (!f.is_open())
        throw Exception("Fisierul nu exista!");
    for (const auto& t : this->tasks) {
        f << t.get_id() << endl << t.get_descriere() << endl << t.get_stare() << endl;
        for (const auto& p : t.get_programatori()) {
            f << p << " ";
        }
        f << endl;
    }
}

void Repo::store(const Task& task)
{
    if(exists(task.get_id()))
        throw Exception("Task-ul exista deja!");
    this->tasks.push_back(task);
    write_to_file();
}

bool Repo::exists(int id) {
    for (const auto& t : this->tasks) {
        if (t.get_id() == id)
            return true;
    }
    return false;
}

void Repo::modify(Task &task, string stare) {
    task.set_stare(stare);
    write_to_file();
}