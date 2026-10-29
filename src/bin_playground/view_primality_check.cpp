#include <charconv>
#include <cstdint>
#include <system_error>

#include "view_primality_check.hpp"
#include "miller_rabin_test.hpp"

using namespace ftxui;

Component createPrimalityCheckView(AppContext& app_ctx) {
    // Input field
    InputOption input_options;
    input_options.multiline = false;
    Component input_field = Input(
        &app_ctx.primality_ctx.user_input,
        "Enter a number to check",
        input_options
    ) 
        | size(WIDTH, EQUAL, 51)
        | size(HEIGHT, EQUAL, 1);

    // Button
    ButtonOption button_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    button_options.label = "Check";
    button_options.on_click = [ctx = &app_ctx] {
        ctx->primality_ctx.test_result.reset();
        try {
            const std::string& input = ctx->primality_ctx.user_input;
            std::uint32_t number = 0;
            const auto [end, error] = std::from_chars(
                input.data(),
                input.data() + input.size(),
                number
            );
            if (input.empty() || error != std::errc{} || end != input.data() + input.size()) {
                throw std::invalid_argument("Enter a valid non-negative 32-bit integer.");
            }

            ctx->primality_ctx.num_to_check = number;
            ctx->primality_ctx.test_result = miller_rabin_test(number);
            ctx->status_ctx.status_content.reset();
        } catch (const std::exception& error) {
            ctx->status_ctx.status_content = error.what();
        }
    };
    button_options.transform = [] (const EntryState& state) {
        Element button = hbox({
            filler(),
            text(state.label),
            filler(),
        }) | size(WIDTH, EQUAL, 20) | border;

        return button;
    };
    Component check_btn = Button(button_options);

    // Container
    Component controls = Container::Vertical({
        input_field,
        check_btn
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        std::string test_result_txt = "";
        if (ctx->primality_ctx.test_result.has_value()) {
            test_result_txt =
                *ctx->primality_ctx.test_result == MillerRabinResult::IsComposite
                    ? std::to_string(ctx->primality_ctx.num_to_check) + " is composite"
                    : std::to_string(ctx->primality_ctx.num_to_check) + " is probably prime";
        }

        return vbox({
            input_field->Render(),
            hbox({
                vbox({
                    filler(),
                    check_btn->Render(),
                }),
                window(
                    text("Result"),
                    text(test_result_txt)
                ) | flex
            }) | flex
        });
    });
}