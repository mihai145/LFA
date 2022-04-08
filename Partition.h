//
// Created by Mihai145 on 3/21/2022.
//

#ifndef TEMA_LAB_2_PARTITION_H
#define TEMA_LAB_2_PARTITION_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "State.h"

class Partition {
private:
    std::unordered_map<int, std::unordered_set<State, State::Hash>> sets;
    int size, max_set_index;

public:
    Partition() = delete;

    explicit Partition(std::vector<std::unordered_set<State, State::Hash>> init_sets);

    bool empty() const;

    /* Returneaza o multime din partitie (o elimina) */
    std::unordered_set<State, State::Hash> get_set();

    /* Returneaza multimea cu un anumit index din partitie (nu o elimina) */
    std::unordered_set<State, State::Hash> get_set(int index) const;

    /* Returneaza indexul unei multimi din partitie (sau -1 daca nu exista) */
    int get_index(const std::unordered_set<State, State::Hash> &A) const;

    /* Returneaza T/F daca partitia contine nu anumit set */
    bool contains(const std::unordered_set<State, State::Hash> &A) const;

    /* Returneaza indicii seturilor care se intersecteaza cu X */
    std::vector<int> get_intersecting_sets(const std::unordered_set<State, State::Hash> &X) const;

    /* Insereaza setul A in partitie */
    void insert(const std::unordered_set<State, State::Hash> &A);

    /* Sterge setul A din partitie */
    void remove(const std::unordered_set<State, State::Hash> &A);

    /* Sterge setul A, insereaza seturile B si C */
    void replace(const std::unordered_set<State, State::Hash> &A, const std::unordered_set<State, State::Hash> &B,
                 const std::unordered_set<State, State::Hash> &C);

    /* Intersectia a doua multimi */
    static std::unordered_set<State, State::Hash>
    intersect_sets(const std::unordered_set<State, State::Hash> &A, const std::unordered_set<State, State::Hash> &B);

    /* Diferenta a doua multimi */
    static std::unordered_set<State, State::Hash>
    difference_sets(const std::unordered_set<State, State::Hash> &A, const std::unordered_set<State, State::Hash> &B);
};


#endif //TEMA_LAB_2_PARTITION_H
