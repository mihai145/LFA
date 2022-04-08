#include <fstream>
#include <string>

#include "Automaton.h"

int main() {
    std::ifstream cin("input.txt");
    std::ofstream cout("output.txt");

    NFA nfa;
    cin >> nfa;
    DFA nfa_to_dfa = nfa.convert_to_dfa();

    cout << nfa << "\n\n" << nfa_to_dfa << "\n\n";

    for (const std::string &s: {"a", "aba", "abaaabb", "bb", "bba", "aab"}) {
        cout << "Automatul NFA " << ((!nfa.accepts_word(s)) ? "nu " : "") << "accepta cuvantul " << s << '\n';
        cout << "Automatul DFA echivalent " << ((!nfa_to_dfa.accepts_word(s)) ? "nu " : "") << "accepta cuvantul " << s
             << '\n';
        cout << "==========\n";
    }

    cout << "\n\n\n";

    DFA dfa;
    cin >> dfa;
    DFA minimized = dfa.minimize();

    cout << dfa << "\n\n\n" << minimized << "\n\n\n";

    for (const std::string &s: {"010", "0", "1", "001110010", "0110", "100"}) {
        cout << "Automatul DFA " << ((!dfa.accepts_word(s)) ? "nu " : "") << "accepta cuvantul " << s << '\n';
        cout << "Automatul DFA minimizat " << ((!minimized.accepts_word(s)) ? "nu " : "") << "accepta cuvantul " << s
             << '\n';
        cout << "==========\n";
    }

    return 0;
}
