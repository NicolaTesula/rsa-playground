#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "tui.hpp"
#include "view_dashboard.hpp"

using namespace ftxui;

// TODO
TUI::TUI() : m_app_context{} {}


void TUI::run() {
    // Create the layout
    Component ui = createDashboardView(m_app_context);
    
    // Start the main update loop
    App app = App::Fullscreen(); // App::Fullscreen() to use entire terminal
    app.Loop(ui);
}