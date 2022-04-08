//
// Created by Mihai145 on 3/21/2022.
//

#ifndef TEMA_LAB_2_STATE_H
#define TEMA_LAB_2_STATE_H

#include <string>
#include <vector>
#include <unordered_set>

class State {
private:
    std::string name;

public:
    State();

    explicit State(std::string name);

    State(const State &other);

    State &operator=(State other);

    /* Hash-ul unui State */
    struct Hash {
        size_t operator()(const State &state) const;
    };

    /* Returneaza toate starile distincte din subset */
    std::vector<State> get_states_from_subset() const;

    /* Returneaza concatenarea starilor primite intr-o singura stare-multime */
    static State make_state_from_subset(const std::vector<State> &states);

    /* Returneaza concatenarea starilor primite intr-o singura stare-multime */
    static State make_state_from_subset(const std::unordered_set<State, State::Hash> &states);

    /* Wrapper pentru o stare intr-o multime: q => {q,} */
    static State make_state_from_subset(const State &state);

    friend std::istream &operator>>(std::istream &is, State &state);

    friend std::ostream &operator<<(std::ostream &os, const State &state);

    bool operator==(const State &other) const;

    bool operator<(const State &other) const;
};

/* Pentru a putea face hash-uri (State, char) */
struct pair_State_char_hash {
    size_t operator()(const std::pair<State, char> &pair) const;
};

#endif //TEMA_LAB_2_STATE_H
