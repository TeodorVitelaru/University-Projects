#include <vector>
#include <string>
#pragma once
using namespace std;

class Task {
private:
    int id;
    string descriere;
    vector<string> programatori;
    string stare;
    public:
    Task(int id, string descriere, vector<string> programatori, string stare) : id{id}, descriere{descriere}, programatori{programatori}, stare{stare} {};
    Task(const Task& ot) = default;
    int get_id() const {
        return id;
    }
    string get_descriere() const {
        return descriere;
    }
    vector<string> get_programatori() const {
        return programatori;
    }
    string get_stare() const {
        return stare;
    }
    void set_stare(string stare) {
        this->stare = stare;
    }
    void set_programatori(vector<string> programatori) {
        this->programatori = programatori;
    }
    void set_id(int id) {
        this->id = id;
    }
    void set_descriere(string descriere) {
        this->descriere = descriere;
    }

    int nr_prog() const
    {
        return programatori.size();
    }



};



