#pragma once

#include "app_context.hpp"

class TUI {
private:
    AppContext m_app_context;

public:
    TUI();
    void run();
};