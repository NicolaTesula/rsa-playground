#include <vector>
#include <string>
#include <print>
#include <iostream>

#include "tui.hpp"
#include "prime_generator.hpp"


TUI::TUI() : 
m_is_running(true),
m_select_options({"Generate Primes", "Encrypt Message"})
{}

void TUI::run() {
    std::print("Please select an option\n");

    int counter = 1;
    for (std::string& option : m_select_options) {
        std::print("{}  {}\n", counter++, option);
    }

    int user_selection;
    std::cin >> user_selection;
    
    switch (user_selection) {
        case 1: {
            std::print("Please input the number of bits your primes should have (max 31):\n");
            int num_bits;
            std::cin >> num_bits;
            std::print("User requested two primes with {} bits. Coming right up!\n", num_bits);
            std::uint64_t p, q = 0;
            std::tie(p, q) = generate_primes(num_bits);
            std::print("Your primes are {} and {}. Their product is: {}\n", p, q, p*q);
        }
    }
}

bool TUI::is_running() const {
    return m_is_running;
}