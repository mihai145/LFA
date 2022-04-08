//
// Created by Mihai145 on 3/21/2022.
//

#include "Base_Automaton.h"

#include <queue>

void Base_Automaton::set_alphabet(const std::unordered_set<char> &alp) {
    alphabet = alp;
}

void Base_Automaton::add_state(const State &state) {
    states.insert(state);
}

void Base_Automaton::set_initial_state(const State &init_state) {
    if (states.find(init_state) == states.end()) {
        std::cerr << "Starea initiala nu se afla in multimea starilor automatului!\n";
        exit(0);
    }

    initial_state = init_state;
}

void Base_Automaton::add_final_state(const State &final_state) {
    if (states.find(final_state) == states.end()) {
        std::cerr << "Starea finala " << final_state << " nu se afla in multimea starilor automatului!\n";
        exit(0);
    }

    final_states.insert(final_state);
}

std::unordered_set<State, State::Hash> Base_Automaton::get_states() {
    return states;
}

std::unordered_set<State, State::Hash> Base_Automaton::get_reachable_states() const {
    /* BFS pentru a determina starile reachable */

    std::queue<State> queue;
    std::unordered_set<State, State::Hash> visited;

    queue.push(initial_state);
    visited.insert(initial_state);

    while (!queue.empty()) {
        State current = queue.front();
        queue.pop();

        for (const char &c: alphabet) {
            if (next_states.find(std::make_pair(current, c)) == next_states.end()) {
                continue;
            }

            for (const State &next_state: next_states.at(std::make_pair(current, c))) {
                if (visited.find(next_state) == visited.end()) {
                    queue.push(next_state);
                    visited.insert(next_state);
                }
            }
        }
    }

    return visited;
};

bool Base_Automaton::accepts_word(const std::string &word) const {
    std::unordered_set<State, State::Hash> current_step_states;
    current_step_states.insert(initial_state);

    for (char c: word) {
        std::unordered_set<State, State::Hash> next_step_states;

        for (const State &state: current_step_states) {
            if (next_states.find(std::make_pair(state, c)) == next_states.end()) {
                continue;
            }

            for (const State &next_state: next_states.at(std::make_pair(state, c))) {
                next_step_states.insert(next_state);
            }
        }

        /* Automatul aborteaza */
        if (next_step_states.empty()) {
            return false;
        }

        std::swap(current_step_states, next_step_states);
    }

    if (std::any_of(current_step_states.begin(), current_step_states.end(), [this](const State &state) {
        return (final_states.find(state) != final_states.end());
    })) {
        return true;
    }

    return false;
}

void Base_Automaton::complete() {
    State sink_state("sink");
    this->add_state(sink_state);

    for (const State &state: this->states) {
        for (const char &c: this->alphabet) {
            if (next_states.find(std::make_pair(state, c)) == next_states.end()) {
                this->add_edge(state, c, sink_state);
            }
        }
    }
}

std::istream &operator>>(std::istream &is, Base_Automaton &automaton) {
    /* Citim alfabetul automatului */
    int nr_letters;
    is >> nr_letters;
    for (int i = 0; i < nr_letters; ++i) {
        char c;
        is >> c;
        automaton.alphabet.insert(c);
    }

    /* Citim starile automatului */
    int nr_states;
    is >> nr_states;

    State state;
    for (int i = 0; i < nr_states; ++i) {
        is >> state;
        automaton.add_state(state);
    }

    /* Citim tranzitiile automatului */
    int nr_edges;
    is >> nr_edges;

    State start, end;
    char c;
    for (int i = 0; i < nr_edges; ++i) {
        is >> start >> c >> end;
        automaton.add_edge(start, c, end);
    }

    /* Starea initiala */
    State init_state;
    is >> init_state;
    automaton.set_initial_state(init_state);

    /* Starile finale */
    int nr_final_states;
    is >> nr_final_states;

    State final_state;
    for (int i = 0; i < nr_final_states; ++i) {
        is >> final_state;
        automaton.add_final_state(final_state);
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, Base_Automaton &automaton) {
    /* Afisam alfabetul automatului */
    os << "Alfabetul automatului are " << automaton.alphabet.size() << " litere: ";
    for (const char &c: automaton.alphabet) {
        os << c << ' ';
    }
    os << '\n';

    /* Afisam starile automatului */
    os << "Automatul are " << automaton.states.size() << " stari: ";
    for (const State &state: automaton.states) {
        os << state << ' ';
    }
    os << '\n';

    /* Afisam muchiile automatului */
    for (auto &out: automaton.next_states) {
        for (const State &state: out.second) {
            /* Formatare: (starea curenta, caracter) -> starea urmatoare */
            os << '(' << out.first.first << ", " << out.first.second << ") -> " << state << '\n';
        }
    }

    /* Afisam starea initiala a automatului */
    os << "Starea initiala: " << automaton.initial_state << '\n';

    /* Afisam starile finale ale automatului */
    os << "Automatul are " << automaton.final_states.size() << " stari finale: ";
    for (const State &final_state: automaton.final_states) {
        os << final_state << ' ';
    }

    return os;
}