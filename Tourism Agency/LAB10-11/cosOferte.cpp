#pragma once
#include "cosOferte.h"
#include <algorithm>
#include <random>
using namespace std;

void cosOferte::adauga_oferta_cos(const Oferta& of)
{
    this->cos.push_back(of);
    notify();
}

void cosOferte::adauga_random_cos(vector<Oferta> oferte, int numar)
{
    shuffle(oferte.begin(), oferte.end(), default_random_engine(std::random_device{}())); //amesteca vectorul oferte
    while (cos.size() < numar && !oferte.empty()) {
        cos.push_back(oferte.back());
        oferte.pop_back();
    }
    notify();
}

const vector<Oferta>& cosOferte::get_all_cos()
{
    return this->cos;
}

void cosOferte::sterge_cos() {
    this->cos.clear();
    notify();
}