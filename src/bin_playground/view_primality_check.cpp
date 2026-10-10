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
    button_options.on_click = [&] {
        try {
            app_ctx.primality_ctx.num_to_check = std::stoi(app_ctx.primality_ctx.user_input);
            app_ctx.primality_ctx.test_result = miller_rabin_test(app_ctx.primality_ctx.num_to_check);
        } catch (const std::exception& error) {
            app_ctx.status_ctx.status_content = error.what();
        }
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
            test_result_txt = (app_ctx.primality_ctx.test_result == MillerRabinResult::IsComposite) ? "Is composite" : "Is probably prime";
        }

        return vbox({
            input_field->Render(),
            hbox({
                check_btn->Render() | border | flex,
                window(
                    text("Result"),
                    text(test_result_txt)
                ) | flex
            }) | flex
        });
    });
}