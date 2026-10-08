#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "tui.hpp"
#include "views/view_main_menu.hpp"
#include "views/view_generate_primes.hpp"

// TODO
TUI::TUI() : m_current_screen(ScreenState::MainMenu), m_app_context{} {}

void TUI::run() {
    using namespace ftxui;

    // Used to decide which tab to draw
    int active_tab = 0;

    // Get the current view for the left panel
    Component menu_view = createMainMenuView(
        m_app_context,
        [&] (int selected) { active_tab = selected + 1; }
    );
    Component prime_gen_view = createPrimeGenView(m_app_context, [&] { active_tab = 0; });

    // Container::Tab schaltet bei Änderung von active_tab automatisch live um:
    Component screen_tabs = Container::Tab({
        menu_view,
        prime_gen_view
    }, &active_tab);

    // Connect the view with the layout
    auto main_ui = Renderer(screen_tabs, [&] {
        Element title = hbox({
            text("Welcome to the RSA Playground!")
                | bold 
                | border 
                | hcenter 
                | flex 
                | color(Color::Cyan)
        });
    
        Element view_panel = window(text("Actions"), screen_tabs->Render())
            | xflex;
    
        Element details_panel = window(text("Details"), paragraph(m_app_context.detailsContent))
            | size(WIDTH, EQUAL, 42)
            | size(HEIGHT, EQUAL, 15);
    
        return vbox({
            title,
            hbox({
                view_panel,
                details_panel
            })
        });
    });
    
    // Start the main update loop
    auto screen = App::Fullscreen(); // App::Fullscreen() to use entire terminal
    screen.Loop(main_ui);
}

ScreenState TUI::getScreenState() const {
    return m_current_screen;
}

void TUI::setScreenState(const ScreenState& screen_state) {
    m_current_screen = screen_state;
}