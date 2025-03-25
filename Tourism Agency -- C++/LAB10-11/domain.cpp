#include <iostream>
#include "domain.h"

string Oferta::get_denumire() const {
    return this->denumire;
}

string Oferta::get_destinatie() const {
    return this->destinatie;
}

string Oferta::get_tip() const {
    return this->tip;
}

int Oferta::get_pret() const {
    return this->pret;
}

void Oferta::set_denumire(string _denumire) {
    denumire = std::move(_denumire);
}

void Oferta::set_destinatie(string _destinatie) {
    destinatie = std::move(_destinatie);
}

void Oferta::set_tip(basic_string<char> _tip) {
    tip = std::move(_tip);
}

void Oferta::set_pret(int _pret) {
   pret = _pret;
}

ostream& operator<<(ostream& stream, const Oferta& of)
{
    stream << "Denumire:" << of.denumire <<"|| Destinatie:"<<of.destinatie << "|| Tip:"<<of.tip<<"|| Pret:"<<of.pret<<endl;
    return stream;
}