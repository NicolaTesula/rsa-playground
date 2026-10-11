#include "view_prime_generator.hpp"
#include "prime_generator.hpp"

using namespace ftxui;

Component createPrimeGenView(AppContext& app_ctx) {
    int& bit_width = app_ctx.primes_ctx.bit_width;

    // Slider
    Component bit_width_slider = Slider("Bit width ", &bit_width, 8, 31, 1)
        | size(WIDTH, GREATER_THAN, 48);

    // Button
    ButtonOption btn_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85), // background unfocused
        Color::White, // text unfocused
        Color::RGB(55, 145, 110), // background focused
        Color::White // text focused
    );
    btn_options.label = "Generate";
    btn_options.on_click = [ctx = &app_ctx] {
        try {
            const auto width = static_cast<std::uint8_t>(ctx->primes_ctx.bit_width);
            auto [p, q] = generate_primes(width);
            while (p == q) {
                std::tie(p, q) = generate_primes(width);
            }

            RSADecryptor decryptor(p, q);
            const PublicKey public_key = decryptor.get_public_key();

            // Update the context
            ctx->decrypt_ctx.decryptor.emplace(std::move(decryptor));
            ctx->status_ctx.curr_public_key = public_key;
            ctx->encrypt_ctx.encryptor.reset();
            ctx->encrypt_ctx.encrypted.clear();
            ctx->encrypt_ctx.encrypted_hex_blocks.clear();
            ctx->encrypt_ctx.encrypted_scroll_offset = 0;
            ctx->primes_ctx.p = p;
            ctx->primes_ctx.q = q;
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
        })  | size(WIDTH, EQUAL, 20) 
            | size(HEIGHT, LESS_THAN, 4)
            | border;

        return button;
    };
    Component generate_btn = Button(btn_options);

    // Container
    Component controls = Container::Vertical({
        bit_width_slider,
        generate_btn
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        Element bit_width_text = text(std::to_string(ctx->primes_ctx.bit_width))
            | size(WIDTH, GREATER_THAN, 2);

        Element prime_p = text(std::to_string(ctx->primes_ctx.p))
            | align_right
            | size(WIDTH, EQUAL, 10)
            | border;
    
        Element prime_q = text(std::to_string(ctx->primes_ctx.q))
            | align_right
            | size(WIDTH, EQUAL, 10)
            | border;

        return vbox({
            hbox({
                bit_width_slider->Render(),
                filler(),
                bit_width_text
            }),
            hbox({
                vbox({
                    filler(),
                    generate_btn->Render()
                }),
                filler(),
                hbox({
                    prime_p,
                    prime_q
                }),
            }) | flex
        });
    });
}
