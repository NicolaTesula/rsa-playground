#include <charconv>
#include <cstdint>
#include <system_error>

#include "fermat_factorization.hpp"
#include "pollards_algorithm.hpp"
#include "view_factorize.hpp"

using namespace ftxui;

Component createFactorizeView(AppContext& app_ctx) {
    auto* ctx = &app_ctx;

    // Input field
    InputOption input_options;
    input_options.multiline = false;
    Component input_field = Input(
        &ctx->factorize_ctx.user_input,
        "Enter number to factorize",
        input_options
    ) 
        | size(WIDTH, EQUAL, 30)
        | size(HEIGHT, EQUAL, 1);

    CheckboxOption source_options;
    source_options.label = "Use current public key modulus";
    source_options.checked = &ctx->factorize_ctx.use_public_key_modulus;
    source_options.on_change = [ctx] {
        if (ctx->factorize_ctx.use_public_key_modulus
            && !ctx->status_ctx.curr_public_key.has_value()) {
            ctx->factorize_ctx.use_public_key_modulus = false;
            ctx->status_ctx.status_content =
                "Generate a public key before selecting its modulus.";
            return;
        }
        ctx->status_ctx.status_content.reset();
    };
    Component use_public_key_modulus = Checkbox(source_options);

    static const std::vector<std::string> methods{"Fermat", "Pollard"};
    Component method_selector = Toggle(
        methods,
        &ctx->factorize_ctx.selected_method
    );

    // Button
    ButtonOption btn_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    btn_options.label = "Factorize";
    btn_options.on_click = [ctx] {
        ctx->factorize_ctx.factors.reset();
        try {
            std::uint64_t number = 0;
            if (ctx->factorize_ctx.use_public_key_modulus) {
                if (!ctx->status_ctx.curr_public_key.has_value()) {
                    throw std::invalid_argument(
                        "Generate a public key before factorizing its modulus."
                    );
                }
                number = ctx->status_ctx.curr_public_key->n;
            } else {
                const std::string& input = ctx->factorize_ctx.user_input;
                const auto [end, error] = std::from_chars(
                    input.data(),
                    input.data() + input.size(),
                    number
                );
                if (input.empty() || error != std::errc{}
                    || end != input.data() + input.size()) {
                    throw std::invalid_argument(
                        "Enter a valid non-negative 64-bit integer."
                    );
                }
            }

            switch (ctx->factorize_ctx.selected_method) {
                case 0:
                    ctx->factorize_ctx.factors = fermat_factorization(number);
                    break;
                case 1:
                    ctx->factorize_ctx.factors = pollards_algorithm(number);
                    break;
                default:
                    throw std::invalid_argument("Select a factorization method.");
            }
            ctx->status_ctx.status_content.reset();
        } catch (const std::exception& error) {
            ctx->status_ctx.status_content = error.what();
        }
    };
    btn_options.transform = [] (const EntryState& state) {
        Element button = hbox({
            filler(),
            text(state.label),
            filler(),
        }) | size(WIDTH, EQUAL, 20) | border;

        return button;
    };
    Component factorize_btn = Button(btn_options);

    // Container
    Component controls = Container::Vertical({
        input_field,
        use_public_key_modulus,
        method_selector,
        factorize_btn
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        const auto factors = ctx->factorize_ctx.factors;
        const std::string p = factors.has_value()
            ? std::to_string(factors->p)
            : "-";
        const std::string q = factors.has_value()
            ? std::to_string(factors->q)
            : "-";

        return vbox({
            hbox({
                text("Number: "),
                input_field->Render()
            }),
            use_public_key_modulus->Render(),
            hbox({
                text("Method: "),
                method_selector->Render()
            }),
            hbox({
                factorize_btn->Render(),
                filler(),
                window(text("p"), text(p) | align_right)
                    | size(WIDTH, EQUAL, 12),
                window(text("q"), text(q) | align_right)
                    | size(WIDTH, EQUAL, 12)
            })
        });
    });
}