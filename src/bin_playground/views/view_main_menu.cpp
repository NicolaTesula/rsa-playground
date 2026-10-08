#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "app_context.hpp"

using namespace ftxui;

Component createMainMenuView(AppContext& app_context, std::function<void(int)> on_select) {
    static std::vector<std::string> entries = {
        "Generate a pair of primes",
        "Check a number for primality",
        "Encrypt a message",
        "Decrypt a message",
        "Play indistinguishability game",
        "Factorize a prime product"
    };
    static int selected = 0;

    // Configure options for the menu
    MenuOption option;
    option.on_enter = [&, on_select] {
        if (on_select) {
            on_select(selected);
        }
    };
    // TODO: Mouse click should also fire the on_enter event...
    // option.entries_option.on_click = [&, on_select] {
    //     if (on_select) on_select(selected);
    // };
    
    // Create and return the menu component
    return Menu(&entries, &selected, option);
}