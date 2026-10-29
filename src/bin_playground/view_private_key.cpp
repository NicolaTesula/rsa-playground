#include <charconv>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <system_error>

#include "extended_euclidean_algorithm.hpp"
#include "view_private_key.hpp"

using namespace ftxui;

Component createPrivateKeyView(AppContext& app_ctx) {
    auto* ctx = &app_ctx;

    InputOption input_options;
    input_options.multiline = false;
    Component exponent_input = Input(
        &ctx->private_key_ctx.public_exponent_input,
        "e (blank = current public key)",
        input_options
    )
        | size(WIDTH, EQUAL, 42)
        | size(HEIGHT, EQUAL, 1);

    ButtonOption button_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    button_options.label = "Extract d";
    button_options.on_click = [ctx] {
        ctx->private_key_ctx.private_key.reset();
        try {
            if (!ctx->factorize_ctx.factors.has_value()) {
                throw std::invalid_argument(
                    "Factorize p and q before extracting the private key."
                );
            }

            const auto [p, q] = *ctx->factorize_ctx.factors;
            if (p < 2 || q < 2) {
                throw std::invalid_argument("The factors p and q must be greater than 1.");
            }

            std::uint64_t e = 0;
            const std::string& input = ctx->private_key_ctx.public_exponent_input;
            if (input.empty()) {
                if (!ctx->status_ctx.curr_public_key.has_value()) {
                    throw std::invalid_argument(
                        "Enter a public exponent e or generate a public key."
                    );
                }
                e = ctx->status_ctx.curr_public_key->e;
            } else {
                const auto [end, error] = std::from_chars(
                    input.data(),
                    input.data() + input.size(),
                    e
                );
                if (error != std::errc{} || end != input.data() + input.size() || e == 0) {
                    throw std::invalid_argument(
                        "Enter a valid positive 64-bit public exponent."
                    );
                }
            }

            const std::uint64_t p_minus_one = p - 1;
            const std::uint64_t q_minus_one = q - 1;
            if (q_minus_one != 0
                && p_minus_one > std::numeric_limits<std::uint64_t>::max() / q_minus_one) {
                throw std::overflow_error("phi(n) exceeds the 64-bit integer range.");
            }
            const std::uint64_t phi = p_minus_one * q_minus_one;
            const auto max_signed =
                static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max());
            if (e > max_signed || phi > max_signed) {
                throw std::overflow_error(
                    "The extended Euclidean algorithm supports values up to INT64_MAX."
                );
            }

            const EuclideanResult result = extended_euclidean_algorithm(e, phi);
            if (result.gcd != 1) {
                throw std::invalid_argument(
                    "The public exponent e is not coprime to phi(n)."
                );
            }

            const auto signed_phi = static_cast<std::int64_t>(phi);
            std::int64_t d = result.u % signed_phi;
            if (d < 0) {
                d += signed_phi;
            }
            ctx->private_key_ctx.private_key = static_cast<std::uint64_t>(d);
            ctx->status_ctx.status_content.reset();
        } catch (const std::exception& error) {
            ctx->status_ctx.status_content = error.what();
        }
    };
    button_options.transform = [](const EntryState& state) {
        return hbox({
            filler(),
            text(state.label),
            filler(),
        }) | size(WIDTH, EQUAL, 20) | border;
    };
    Component extract_button = Button(button_options);

    Component controls = Container::Vertical({
        exponent_input,
        extract_button
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        const std::string private_key = ctx->private_key_ctx.private_key.has_value()
            ? std::to_string(*ctx->private_key_ctx.private_key)
            : "-";

        return vbox({
            text("Public exponent: "),
            exponent_input->Render(),
            filler(),
            hbox({
                extract_button->Render(),
                filler(),
                window(text("Private key d"), text(private_key) | align_right)
                    | size(WIDTH, EQUAL, 22)
            })
        });
    });
}
