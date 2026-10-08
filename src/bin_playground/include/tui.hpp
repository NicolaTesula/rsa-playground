#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "app_context.hpp"

enum class ScreenState {
    MainMenu,
    GeneratePrimes,
    PrimalityTest,
    Encrypt,
    Decrypt
};

class TUI {
private:
    AppContext m_app_context;
    ScreenState m_current_screen;

public:
    TUI();
    void run();
    ScreenState getScreenState() const;
    void setScreenState(const ScreenState& screen_state);
};