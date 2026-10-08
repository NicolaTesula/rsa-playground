#include <functional>
#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

#include "views/view_generate_primes.hpp"
#include "app_context.hpp"

using namespace ftxui;

Component createPrimeGenView(AppContext& context, std::function<void()> on_back) {
    // Local state
    static int bit_width = 31;
    static std::string status_msg = "Satus"; // TODO

    // Create the bit width slider
    SliderOption<int> opts;
    opts.color_inactive = Color::GrayDark;
    opts.color_active = Color::BlueLight;
    opts.min = 10;
    opts.max = 31;
    opts.increment = 1;
    opts.value = &bit_width;
    Component slider_bits = Slider(opts);

    // Create the buttons
    Component btn_generate = Button("Generate", [&] { /* TODO */ })
        | color(Color::White)
        | bgcolor(Color::RGB(35, 110, 85));
    Component btn_back = Button("Back", on_back)
        | color(Color::White)
        | bgcolor(Color::RGB(55, 65, 85));
    
    // Bundle the slider and buttons such that keyboard events reach all entries
    Component container = Container::Vertical({
        slider_bits,
        btn_generate,
        btn_back
    });

    // Renderer for the visual layout of this view
    return Renderer(container, [=, &context] {
        // Show details when hovering over the components
        if (slider_bits->Focused()) {
            context.detailsContent = "Sets the width of both generated primes p and q.\nThe max value is limited to 31 bits right now, the module can thus be maximum 62 bits wide.";
        } else if (btn_generate->Focused()) {
            context.detailsContent = "Draws random numbers from a uniform distribution in the given range and uses the Miller-Rabin-Test to determine, whether they are composite or likely prime (by default with >[TODO]\% probability).";
        } else if (btn_back->Focused()) {
            context.detailsContent = "Return to the main menu.";
        } else {
            context.detailsContent = "Hover over an element to get more info.";
        }

        return vbox({
            text("Prime Number Generator") | bold | center | color(Color::Pink1),
            separator(),
            hbox({
                text("Bit width: "),
                slider_bits->Render(),
                text(std::to_string(bit_width) + " bits")
            }),
            separator(),
            btn_generate->Render(),
            separator(),
            btn_back->Render()
        });
    });
}