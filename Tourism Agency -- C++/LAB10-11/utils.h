#pragma once
#include "domain.h"

#ifndef LAB6_UTILS_H
#define LAB6_UTILS_H

/*
 * functia compara pretul a doua oferte
 * of1: prima oferta
 * of2: a doua oferta
 * return: 1 daca p1>p2, 0 altfel
 */
int cmp_denumire(const Oferta& of1, const Oferta& of2);

/*
 * functia compara destinatia a doua oferte
 * of1: prima oferta
 * of2: a doua oferta
 * return: 1 daca d1>d2, 0 altfel
 */
int cmp_destinatie(const Oferta& of1, const Oferta& of2);

/*
 * functia compara tipul si pretul a doua oferte
 * of1: prima oferta
 * of2: a doua oferta
 * return: 1 daca t1>t2 si p1>p2, 0 altfel
 */
int cmp_tip_pret(const Oferta& of1, const Oferta& of2);

#endif //LAB6_UTILS_H
