//
// Created by Mihai145 on 3/21/2022.
//

#include "Automaton.h"
#include "Partition.h"

#include <queue>


/* Metode DFA */

void DFA::add_edge(const State &start, const char &c, const State &end) {
    if (alphabet.find(c) == alphabet.end()) {
        std::cerr << "Tranzitie invalida, litera " << c << " nu este in alfabetul automatului!\n";
        exit(0);
    }

    if (!next_states[std::make_pair(start, c)].empty()) {
        std::cerr << "Un DFA are o singura tranzitie din fiecare pereche (stare, caracter)!\n";
        exit(0);
    }

    next_states[std::make_pair(start, c)].insert(end);
}

DFA DFA::minimize() const {
    DFA copy = *this;
    copy.complete();

    DFA dfa;
    dfa.set_alphabet(copy.alphabet);

    std::unordered_set<State, State::Hash> reachable_states = copy.get_reachable_states();

    std::unordered_set<State, State::Hash> reachable_non_final_states = Partition::difference_sets(reachable_states,
                                                                                                   copy.final_states);
    std::unordered_set<State, State::Hash> reachable_final_states = Partition::intersect_sets(reachable_states,
                                                                                              copy.final_states);

    Partition P = Partition(
            std::vector<std::unordered_set<State, State::Hash>>{reachable_non_final_states, reachable_final_states}),
            W = Partition(
            std::vector<std::unordered_set<State, State::Hash>>{reachable_non_final_states, reachable_final_states});

    while (!W.empty()) {
        std::unordered_set<State, State::Hash> A = W.get_set();

        for (const char &c: this->alphabet) {
            /* X = multimea starilor care prin c ajung intr-o stare din A */
            std::unordered_set<State, State::Hash> X;
            for (const State &state: reachable_states) {
                if (next_states.find(std::make_pair(state, c)) == next_states.end()) { continue; }

                const State next_state = *next_states.at(std::make_pair(state, c)).begin();
                if (A.find(next_state) != A.end()) {
                    X.insert(state);
                }
            }

            std::vector<int> intersecting_sets_indices = P.get_intersecting_sets(X);

            for (int intersecting_set_index: intersecting_sets_indices) {
                std::unordered_set<State, State::Hash> intersecting_set = P.get_set(intersecting_set_index);
                std::unordered_set<State, State::Hash> intersection = Partition::intersect_sets(intersecting_set, X),
                    difference = Partition::difference_sets(intersecting_set, X);

                P.replace(intersecting_set, intersection, difference);

                if (W.contains(intersecting_set)) {
                    W.replace(intersecting_set, intersection, difference);
                } else {
                    if (intersection.size() > difference.size()) {
                        W.insert(difference);
                    } else {
                        W.insert(intersection);
                    }
                }
            }
        }
    }

    std::unordered_map<State, State, State::Hash> corespondent;
    while (!P.empty()) {
        std::unordered_set<State, State::Hash> set = P.get_set();
        const State equivalent_states = State::make_state_from_subset(set);

        for (const State &state: set) {
            corespondent[state] = equivalent_states;
        }

        dfa.add_state(equivalent_states);

        if (set.find(this->initial_state) != set.end()) {
            dfa.set_initial_state(equivalent_states);
        }

        if (this->final_states.find(*set.begin()) != this->final_states.end()) {
            dfa.add_final_state(equivalent_states);
        }
    }

    for (const State &state: dfa.states) {
        const State repr = state.get_states_from_subset().back();
        for (const char &c: dfa.alphabet) {
            dfa.add_edge(state, c, corespondent[*this->next_states.at(std::make_pair(repr, c)).begin()]);
        }
    }

    return dfa;
}


/* Metode NFA */

void NFA::add_edge(const State &start, const char &c, const State &end) {
    if (alphabet.find(c) == alphabet.end()) {
        std::cerr << "Tranzitie invalida, litera " << c << " nu este in alfabetul automatului!\n";
        exit(0);
    }

    next_states[std::make_pair(start, c)].insert(end);
}

//pentru testare: https://www.javatpoint.com/automata-conversion-from-nfa-to-dfa
DFA NFA::convert_to_dfa() const {
    DFA dfa;
    dfa.set_alphabet(this->alphabet);

    /* Subset construction */
    std::queue<State> to_process;
    std::unordered_set<State, State::Hash> in_queue;
    State initial_state_subset_wrapper = State::make_state_from_subset(this->initial_state);
    to_process.push(initial_state_subset_wrapper);
    in_queue.insert(initial_state_subset_wrapper);

    while (!to_process.empty()) {
        State current_subset = to_process.front();
        to_process.pop();

        std::vector<State> subset = current_subset.get_states_from_subset();

        for (const char &letter: this->alphabet) {
            std::unordered_set<State, State::Hash> next_subset;

            for (const State &state: subset) {
                if (this->next_states.find(std::make_pair(state, letter)) == this->next_states.end()) {
                    continue;
                }

                for (const State &next_state: this->next_states.at(std::make_pair(state, letter))) {
                    next_subset.insert(next_state);
                }
            }

            if (next_subset.empty()) { continue; }

            /* Trebuie ca subseturile care contin aceleasi stari sa fie echivalente, de aceea le vom ordona lexicografic */
            std::vector<State> ordered_next_subset;
            for (const State &state: next_subset) {
                ordered_next_subset.push_back(state);
            }
            std::sort(ordered_next_subset.begin(), ordered_next_subset.end());

            State next_state = State::make_state_from_subset(ordered_next_subset);
            if (in_queue.find(next_state) == in_queue.end()) {
                to_process.push(next_state);
                in_queue.insert(next_state);
            }

            dfa.add_state(current_subset);
            dfa.add_state(next_state);
            dfa.add_edge(current_subset, letter, next_state);
        }
    }

    /* Setam starea initiala a DFA-ului rezultat */
    State dfa_initial_state = this->initial_state;
    dfa.set_initial_state(State::make_state_from_subset(dfa_initial_state));

    /* Setam starile finale ale DFA-ului rezultat */
    std::unordered_set<State, State::Hash> dfa_states = dfa.get_states();
    for (const State &subset: dfa_states) {
        std::vector<State> subset_states = subset.get_states_from_subset();

        if (std::any_of(subset_states.begin(), subset_states.end(), [this](const State &state) {
            return (this->final_states.find(state) != this->final_states.end());
        })) {
            dfa.add_final_state(subset);
        }
    }

    return dfa;
}