#include <iostream>
#include <algorithm>
#include <cstring>
#include "Repo.h"
#include "Validator.h"
#include "Observer.h"

class Service : public Observable {
private:
    Repo& repo;
    Validator& val;
public:
    Service(Repo& repo, Validator& val) : repo{repo}, val{val} {};
    Service(const Service& ot) = delete;
    vector<Task>& get_all() {
        return repo.get_all();
    }
    vector<Task> sortare()
    {
        vector<Task> rez = repo.get_all();
        sort(rez.begin(), rez.end(), [](Task t1, Task t2) {
            return strcmp(t1.get_descriere().c_str(), t2.get_descriere().c_str()) < 0;
        });
        return rez;
    }

    void adaugare(int id, string desri, vector<string> prog, string stare)
    {
        val.validare(id, desri, prog, stare);
        Task t{id, desri, prog, stare};
        repo.store(t);
        notify();
    }

    vector<Task> filrare(string prog)
    {
        vector<Task> tasksWithProgrammer;
        for (const auto& task : repo.get_all())
        {
            const auto& programmers = task.get_programatori();
            if (find(programmers.begin(), programmers.end(), prog) != programmers.end())
            {
                tasksWithProgrammer.push_back(task);
            }
        }
        return tasksWithProgrammer;
    }

    void modifica(int id, string stare)
    {
        for(auto& task : repo.get_all())
        {
            if(task.get_id() == id)
            {
                repo.modify(task, stare);
                notify();
                return;
            }
        }
    }

};


