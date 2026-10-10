#include <string>
#include <optional>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <bit>

#include <ftxui/component/event.hpp>

#include "view_encrypt.hpp"
#include "rsa_encryptor.hpp"

using namespace ftxui;

Component createEncryptView(AppContext& app_ctx) {
    // Input field
    InputOption input_options;
    input_options.multiline = false;
    Component input_field = Input(
        &app_ctx.encrypt_ctx.user_input,
        "Enter message to encrypt...",
        input_options
    ) 
        | size(WIDTH, EQUAL, 51)
        | size(HEIGHT, EQUAL, 1);

    // Button
    ButtonOption btn_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    btn_options.label = "Encrypt";
    btn_options.on_click = [ctx = &app_ctx] {
        ctx->encrypt_ctx.encrypted.clear();
        ctx->encrypt_ctx.encrypted_hex_blocks.clear();
        ctx->encrypt_ctx.encrypted_scroll_offset = 0;
        if (!ctx->status_ctx.curr_public_key.has_value()) {
            ctx->status_ctx.status_content = "No public key selected. Please generate one first!";
            return;
        }

        if (ctx->encrypt_ctx.user_input.empty()) {
            ctx->status_ctx.status_content = "Enter a message to encrypt.";
            return;
        }

        try {
            if (!ctx->encrypt_ctx.encryptor.has_value()) {
                ctx->encrypt_ctx.encryptor.emplace(*ctx->status_ctx.curr_public_key);
            }

            ctx->encrypt_ctx.encrypted =
                ctx->encrypt_ctx.encryptor->encrypt_msg(ctx->encrypt_ctx.user_input);
            const int hex_width =
                (std::bit_width(ctx->status_ctx.curr_public_key->n) + 3) / 4;
            for (const std::uint64_t block : ctx->encrypt_ctx.encrypted) {
                std::ostringstream output;
                output << "0x"
                       << std::uppercase << std::hex << std::setfill('0')
                       << std::setw(hex_width) << block;
                ctx->encrypt_ctx.encrypted_hex_blocks.push_back(output.str());
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
    Component encrypt_btn = Button(btn_options);

    // Output Field
    constexpr int viewport_height = 4;
    Component encrypted_output = createEncryptedField(app_ctx, viewport_height);

    // Container
    Component controls = Container::Vertical({
        input_field,
        Container::Horizontal({
            encrypt_btn,
            encrypted_output
        })
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        return vbox({
            input_field->Render(),
            hbox({
                vbox({
                    filler(),
                    encrypt_btn->Render(),
                    filler()
                }),
                window(
                    text("Encrypted"),
                    encrypted_output->Render()
                        | size(WIDTH, EQUAL, 30)
                        | size(HEIGHT, EQUAL, viewport_height)
                )
            })
        });
    });
}

Component createEncryptedField(AppContext& app_ctx, const int viewport_height) {
    auto* ctx = &app_ctx;
    
    Component encrypted_output = Renderer([ctx, viewport_height](bool focused) {
        Elements rows;
        const auto& blocks = ctx->encrypt_ctx.encrypted_hex_blocks;
        const auto end = std::min(
            ctx->encrypt_ctx.encrypted_scroll_offset + viewport_height,
            static_cast<int>(blocks.size())
        );
    
        for (int i = ctx->encrypt_ctx.encrypted_scroll_offset; i < end; ++i) {
            rows.push_back(text(blocks[i]) | align_right);
        }
    
        while (static_cast<int>(rows.size()) < viewport_height) {
            rows.push_back(text("") | align_right);
        }
    
        Element view = vbox(std::move(rows))
            | size(WIDTH, EQUAL, 30)
            | size(HEIGHT, EQUAL, viewport_height);
        if (focused) {
            view |= color(Color::Cyan);
        }
        return view;
    });
    
    encrypted_output |= CatchEvent([ctx, viewport_height](Event event) {
        const int last_offset = std::max(
            0,
            static_cast<int>(ctx->encrypt_ctx.encrypted_hex_blocks.size())
                - viewport_height
        );
    
        if (event == Event::ArrowDown || event == Event::PageDown) {
            ctx->encrypt_ctx.encrypted_scroll_offset =
                std::min(ctx->encrypt_ctx.encrypted_scroll_offset + 1, last_offset);
            return true;
        }
        if (event == Event::ArrowUp || event == Event::PageUp) {
            ctx->encrypt_ctx.encrypted_scroll_offset =
                std::max(ctx->encrypt_ctx.encrypted_scroll_offset - 1, 0);
            return true;
        }
        return false;
    });

    return encrypted_output;
}