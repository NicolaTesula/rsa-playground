#include <print>

#include "tui.hpp"

int main() {
    TUI tui;

    while (tui.is_running()) {
        tui.run();
    }
}