//
// Created by Mihai145 on 3/21/2022.
//

#include "State.h"

State::State() = default;

State::State(std::string name) : name{std::move(name)} {}

State::State(const State &other) {
    name = other.name;
}

State &State::operator=(State other) {
    std::swap(name, other.name);
    return *this;
}

std::vector<State> State::get_states_from_subset() const {
    std::vector<State> states;
    std::string current_state_name;
    for (int i = 1; i < this->name.size(); i++) {
        if (std::isalnum(this->name[i])) {
            current_state_name += this->name[i];
        } else {
            if (!current_state_name.empty()) {
                states.emplace_back(State(current_state_name));
                current_state_name.clear();
            }
        }
    }
    return states;
}

State State::make_state_from_subset(const std::vector<State> &states) {
    std::string current_state_name = "{";
    for (const State &state: states) {
        current_state_name += state.name;
        current_state_name += ',';
    }
    current_state_name += '}';
    return State(current_state_name);
}

State State::make_state_from_subset(const std::unordered_set<State, State::Hash> &states) {
    std::string current_state_name = "{";
    for (const State &state: states) {
        current_state_name += state.name;
        current_state_name += ',';
    }
    current_state_name += '}';
    return State(current_state_name);
}

State State::make_state_from_subset(const State &state) {
    std::string current_state_name = "{";
    current_state_name += state.name;
    current_state_name += ",}";
    return State(current_state_name);
}

std::istream &operator>>(std::istream &is, State &state) {
    is >> state.name;
    return is;
}

std::ostream &operator<<(std::ostream &os, const State &state) {
    os << state.name;
    return os;
}

bool State::operator==(const State &other) const {
    if (name == other.name) { return true; }
    return false;
}

bool State::operator<(const State &other) const {
    return name < other.name;
}

size_t State::Hash::operator()(const State &state) const {
    return std::hash<std::string>()(state.name);
}

size_t pair_State_char_hash::operator()(const std::pair<State, char> &pair) const {
    return State::Hash()(pair.first) ^ std::hash<char>()(pair.second);
}