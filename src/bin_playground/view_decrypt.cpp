#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

#include <ftxui/component/event.hpp>
#include <ftxui/screen/string.hpp>

#include "view_decrypt.hpp"
#include "rsa_decryptor.hpp"

using namespace ftxui;

namespace {

std::vector<std::string> wrapText(const std::string& content, const int width) {
    std::vector<std::string> lines;
    std::vector<std::string> current_glyphs;
    int current_width = 0;

    const auto append_glyphs = [](const std::vector<std::string>& glyphs,
                                    const std::size_t begin,
                                    const std::size_t end) {
        std::string line;
        for (std::size_t i = begin; i < end; ++i) {
            line += glyphs[i];
        }
        return line;
    };

    for (const auto& glyph : Utf8ToGlyphs(content)) {
        if (glyph == "\n") {
            lines.push_back(append_glyphs(current_glyphs, 0, current_glyphs.size()));
            current_glyphs.clear();
            current_width = 0;
            continue;
        }

        const int glyph_width = string_width(glyph);
        if (current_width + glyph_width > width && !current_glyphs.empty()) {
            const auto break_at = std::find_if(
                current_glyphs.rbegin(),
                current_glyphs.rend(),
                [](const std::string& current_glyph) {
                    return current_glyph == " " || current_glyph == "\t";
                }
            );

            if (break_at == current_glyphs.rend()) {
                lines.push_back(append_glyphs(current_glyphs, 0, current_glyphs.size()));
                current_glyphs.clear();
                current_width = 0;
            } else {
                const std::size_t break_index = static_cast<std::size_t>(
                    std::distance(break_at, current_glyphs.rend()) - 1
                );
                lines.push_back(append_glyphs(current_glyphs, 0, break_index));

                std::vector<std::string> remainder(
                    current_glyphs.begin() + static_cast<std::ptrdiff_t>(break_index + 1),
                    current_glyphs.end()
                );
                while (!remainder.empty()
                        && (remainder.front() == " " || remainder.front() == "\t")) {
                    remainder.erase(remainder.begin());
                }
                current_glyphs = std::move(remainder);
                current_width = string_width(
                    append_glyphs(current_glyphs, 0, current_glyphs.size())
                );
            }
        }

        current_glyphs.push_back(glyph);
        current_width += glyph_width;
    }

    if (!current_glyphs.empty() || lines.empty() || content.back() == '\n') {
        lines.push_back(append_glyphs(current_glyphs, 0, current_glyphs.size()));
    }
    return lines;
}

std::vector<std::uint64_t> inputStringToCipherVec(std::string& input, const char delim = ' ') {
    std::vector<std::uint64_t> result;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delim)) {
        token.erase(
            std::remove_if(token.begin(), token.end(), [](unsigned char ch) {
                return std::isspace(ch);
            }),
            token.end()
        );

        if (token.empty()) {
            continue;
        }

        char* end = nullptr;
        const unsigned long long value = std::strtoull(token.c_str(), &end, 16);
        if (end == token.c_str()) {
            throw std::invalid_argument("Invalid cipher block: " + token);
        }

        result.push_back(static_cast<std::uint64_t>(value));
    }

    return result;
}
    
}  // namespace


Component createDecryptView(AppContext& app_ctx) {
    // Input field
    InputOption input_options;
    input_options.multiline = false;
    Component input_field = Input(
        &app_ctx.decrypt_ctx.user_input,
        "Enter cipher to decrypt, e.g. \"0x19ACE 0xB5CCA\"",
        input_options
    ) 
        | size(WIDTH, EQUAL, 51)
        | size(HEIGHT, EQUAL, 1);

    // Button
    Component decrypt_btn = createDecryptButton(app_ctx);

    // Checkbox
    CheckboxOption use_encrypted_options;
    use_encrypted_options.label = "Use Encrypted";
    use_encrypted_options.checked = &app_ctx.decrypt_ctx.use_encrypted;
    use_encrypted_options.on_change = [&app_ctx] {
        if (!app_ctx.decrypt_ctx.use_encrypted) {
            return;
        }

        const auto& encrypted_blocks = app_ctx.encrypt_ctx.encrypted_hex_blocks;
        if (encrypted_blocks.empty()) {
            app_ctx.decrypt_ctx.use_encrypted = false;
            app_ctx.status_ctx.status_content = "No encrypted output available to use.";
            return;
        }

        std::string ciphertext;
        for (const auto& block : encrypted_blocks) {
            if (!ciphertext.empty()) {
                ciphertext += ' ';
            }
            ciphertext += block;
        }

        app_ctx.decrypt_ctx.user_input = std::move(ciphertext);
        app_ctx.status_ctx.status_content.reset();
    };
    Component use_encrypted = Checkbox(use_encrypted_options);

    // Output Field
    constexpr int viewport_height = 4;
    Component decrypted_output = createDecryptedField(app_ctx, viewport_height);

    // Container
    Component controls = Container::Vertical({
        input_field,
        use_encrypted,
        Container::Horizontal({
            decrypt_btn,
            decrypted_output
        })
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        return vbox({
            input_field->Render(),
            hbox({
                vbox({
                    filler(),
                    use_encrypted->Render(),
                    decrypt_btn->Render(),
                    filler()
                }),
                window(
                    text("Decrypted"),
                    decrypted_output->Render()
                        | size(WIDTH, EQUAL, 30)
                        | size(HEIGHT, EQUAL, viewport_height)
                )
            })
        });
    });
}

Component createDecryptedField(AppContext& app_ctx, const int viewport_height) {
    auto* ctx = &app_ctx;
    constexpr int viewport_width = 30;

    Component decrypted_output = Renderer([ctx, viewport_height](bool focused) {
        const auto lines = wrapText(
            ctx->decrypt_ctx.decrypted.value_or(""),
            viewport_width
        );
        const int end = std::min(
            ctx->decrypt_ctx.decrypted_scroll_offset + viewport_height,
            static_cast<int>(lines.size())
        );

        Elements rows;
        for (int i = ctx->decrypt_ctx.decrypted_scroll_offset; i < end; ++i) {
            rows.push_back(text(lines[i]) | size(WIDTH, EQUAL, viewport_width));
        }
        while (static_cast<int>(rows.size()) < viewport_height) {
            rows.push_back(text("") | size(WIDTH, EQUAL, viewport_width));
        }

        Element view = vbox(std::move(rows))
            | size(WIDTH, EQUAL, viewport_width)
            | size(HEIGHT, EQUAL, viewport_height);
        if (focused) {
            view |= color(Color::Cyan);
        }
        return view;
    });

    decrypted_output |= CatchEvent([ctx, viewport_height](Event event) {
        constexpr int viewport_width = 30;
        const auto lines = wrapText(
            ctx->decrypt_ctx.decrypted.value_or(""),
            viewport_width
        );
        const int last_offset = std::max(
            0,
            static_cast<int>(lines.size()) - viewport_height
        );

        if (event == Event::ArrowDown) {
            ctx->decrypt_ctx.decrypted_scroll_offset =
                std::min(ctx->decrypt_ctx.decrypted_scroll_offset + 1, last_offset);
            return true;
        }
        if (event == Event::ArrowUp) {
            ctx->decrypt_ctx.decrypted_scroll_offset =
                std::max(ctx->decrypt_ctx.decrypted_scroll_offset - 1, 0);
            return true;
        }
        if (event == Event::PageDown) {
            ctx->decrypt_ctx.decrypted_scroll_offset =
                std::min(ctx->decrypt_ctx.decrypted_scroll_offset + viewport_height, last_offset);
            return true;
        }
        if (event == Event::PageUp) {
            ctx->decrypt_ctx.decrypted_scroll_offset =
                std::max(ctx->decrypt_ctx.decrypted_scroll_offset - viewport_height, 0);
            return true;
        }
        return false;
    });

    return decrypted_output;
}

Component createDecryptButton(AppContext& app_ctx) {
    ButtonOption btn_options = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    btn_options.label = "Decrypt";
    btn_options.on_click = [ctx = &app_ctx] {
        ctx->decrypt_ctx.decrypted.reset();
        ctx->decrypt_ctx.decrypted_scroll_offset = 0;

        try {
            if (!ctx->status_ctx.curr_public_key.has_value()) {
                throw std::invalid_argument("No public key selected. Please generate one first!");
            }
            
            if (ctx->decrypt_ctx.user_input.empty()) {
                throw std::invalid_argument("Please enter a cipher to decrypt.");
            }
            if (!ctx->decrypt_ctx.decryptor.has_value()) {
                // This should never be entered!
                throw std::invalid_argument("No decryptor! Please generate a new key and encrypt again");
            }

            // First convert the user input into a vector of uint64s
            const std::vector<std::uint64_t> input_nums = inputStringToCipherVec(ctx->decrypt_ctx.user_input);

            // Check if the user inputted a block larger than the module
            bool any_num_too_large = std::any_of(input_nums.begin(), input_nums.end(), [&] (std::uint64_t num) {
                return num >= ctx->status_ctx.curr_public_key->n;
            });
            if (any_num_too_large) {
                throw std::invalid_argument("At least one of the numbers is larger than the module! Please check");
            }

            // Then encrypt the message, save the results in AppContext and reset the status content
            ctx->decrypt_ctx.decrypted = ctx->decrypt_ctx.decryptor->decrypt_msg(input_nums);
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
    Component decrypt_btn = Button(btn_options);

    return decrypt_btn;
}
