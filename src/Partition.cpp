//
// Created by Mihai145 on 3/21/2022.
//

#include <iostream>
#include <cstdlib>

#include "Partition.h"

Partition::Partition(std::vector<std::unordered_set<State, State::Hash>> init_sets) {
    size = max_set_index = (int) init_sets.size();
    for (int i = 1; i <= max_set_index; ++i) {
        sets[i] = init_sets[i - 1];
    }
}

bool Partition::empty() const {
    return (size == 0);
}

std::unordered_set<State, State::Hash> Partition::get_set() {
    std::pair<int, std::unordered_set<State, State::Hash>> p = *sets.begin();
    size--;
    sets.erase(p.first);
    return p.second;
}

std::unordered_set<State, State::Hash> Partition::get_set(int index) const {
    return sets.at(index);
}

int Partition::get_index(const std::unordered_set<State, State::Hash> &A) const {
    int index_A = -1;
    for (std::pair<int, std::unordered_set<State, State::Hash>> p: sets) {
        if (p.second == A) {
            index_A = p.first;
            break;
        }
    }

    return index_A;
}

bool Partition::contains(const std::unordered_set<State, State::Hash> &A) const {
    if (get_index(A) != -1) {
        return true;
    }

    return false;
}

std::vector<int> Partition::get_intersecting_sets(const std::unordered_set<State, State::Hash> &X) const {
    std::vector<int> intersecting_sets;

    for (std::pair<int, std::unordered_set<State, State::Hash>> p: sets) {
        if (!intersect_sets(p.second, X).empty() && !difference_sets(p.second, X).empty()) {
            intersecting_sets.push_back(p.first);
        }
    }

    return intersecting_sets;
}

void Partition::insert(const std::unordered_set<State, State::Hash> &A) {
    sets[++max_set_index] = A;
    size++;
}

void Partition::remove(const std::unordered_set<State, State::Hash> &A) {
    int index_A = get_index(A);

    if (index_A == -1) {
        std::cerr << "Operatie de stergere din paritie invalida!\n";
        exit(0);
    }

    sets.erase(index_A);
    size--;
}

void
Partition::replace(const std::unordered_set<State, State::Hash> &A, const std::unordered_set<State, State::Hash> &B,
                   const std::unordered_set<State, State::Hash> &C) {
    remove(A);
    insert(B);
    insert(C);
}


std::unordered_set<State, State::Hash> Partition::intersect_sets(const std::unordered_set<State, State::Hash> &A,
                                                                 const std::unordered_set<State, State::Hash> &B) {
    std::unordered_set<State, State::Hash> intersection;
    for (const State &state: A) {
        if (B.find(state) != B.end()) {
            intersection.insert(state);
        }
    }
    return intersection;
}

std::unordered_set<State, State::Hash>
Partition::difference_sets(const std::unordered_set<State, State::Hash> &A,
                           const std::unordered_set<State, State::Hash> &B) {
    std::unordered_set<State, State::Hash> difference;
    for (const State &state: A) {
        if (B.find(state) == B.end()) {
            difference.insert(state);
        }
    }
    return difference;
}