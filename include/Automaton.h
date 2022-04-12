//
// Created by Mihai145 on 3/21/2022.
//

#ifndef TEMA_LAB_2_AUTOMATON_H
#define TEMA_LAB_2_AUTOMATON_H

#include "Base_Automaton.h"

class DFA : public Base_Automaton {
public:
    void add_edge(const State &start, const char &c, const State &end) override;

    //pentru testare: https://en.wikipedia.org/wiki/DFA_minimization
    DFA minimize() const;
};

class NFA : public Base_Automaton {
public:
    void add_edge(const State &start, const char &c, const State &end) override;

    //pentru testare: https://www.javatpoint.com/automata-conversion-from-nfa-to-dfa
    DFA convert_to_dfa() const;
};


#endif //TEMA_LAB_2_AUTOMATON_H
