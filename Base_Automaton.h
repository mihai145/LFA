//
// Created by Mihai145 on 3/21/2022.
//

#ifndef TEMA_LAB_2_BASE_AUTOMATON_H
#define TEMA_LAB_2_BASE_AUTOMATON_H

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include <unordered_set>
#include <unordered_map>

#include "State.h"

class Base_Automaton {
protected:
    std::unordered_set<char> alphabet;
    std::unordered_set<State, State::Hash> states, final_states;
    std::unordered_map<std::pair<State, char>, std::unordered_set<State, State::Hash>, pair_State_char_hash> next_states;
    State initial_state;

public:
    /* Setam alfabetul automatului */
    void set_alphabet(const std::unordered_set<char> &alp);

    /* Adaugam o stare in automat */
    void add_state(const State &state);

    /* Setam starea finala a automatului - aceasta trebuie sa fie una dintre starile automatului */
    void set_initial_state(const State &init_state);

    /* Adaugam o stare finala automatului - aceasta trebuie sa fie una dintre starile automatului */
    void add_final_state(const State &final_state);

    /* Difera intre automatele DFA si NFA */
    virtual void add_edge(const State &, const char &, const State &) = 0;

    /* Getter pentru starile automatului */
    std::unordered_set<State, State::Hash> get_states();

    /* Getter pentru starile reachable ale automatului */
    std::unordered_set<State, State::Hash> get_reachable_states() const;

    /* Verifica daca un cuvant este acceptat de automat */
    bool accepts_word(const std::string &word) const;

    /* Completeaza un automat (sink-state) */
    void complete();

    friend std::istream &operator>>(std::istream &is, Base_Automaton &automaton);

    friend std::ostream &operator<<(std::ostream &os, Base_Automaton &automaton);
};

#endif //TEMA_LAB_2_BASE_AUTOMATON_H
