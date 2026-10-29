#include "view_indistinguishability.hpp"

#include <iomanip>
#include <sstream>
#include <string>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

#include "view_status_bar.hpp"

using namespace ftxui;

ButtonOption createAnimatedButtonOption(const std::string& label, std::function<void()> on_click, int width = 22) {
    ButtonOption opt = ButtonOption::Animated(
        Color::RGB(35, 110, 85),
        Color::White,
        Color::RGB(55, 145, 110),
        Color::White
    );
    opt.label = label;
    opt.on_click = std::move(on_click);
    opt.transform = [width](const EntryState& state) {
        return hbox({
            filler(),
            text(state.label),
            filler(),
        }) | size(WIDTH, EQUAL, width) | border;
    };
    return opt;
}

Component createIndistinguishabilityView(AppContext& app_ctx) {
    auto* ctx = &app_ctx;
    auto* ind_ctx = &app_ctx.indistinguishability_ctx;

    // Inputs for m0 and m1
    InputOption input_m0_opt;
    input_m0_opt.multiline = false;
    Component input_m0 = Input(&ind_ctx->m0_input, "Enter message m0...", input_m0_opt)
        | size(WIDTH, EQUAL, 42)
        | size(HEIGHT, EQUAL, 1);

    InputOption input_m1_opt;
    input_m1_opt.multiline = false;
    Component input_m1 = Input(&ind_ctx->m1_input, "Enter message m1...", input_m1_opt)
        | size(WIDTH, EQUAL, 42)
        | size(HEIGHT, EQUAL, 1);

    // Button: Challenge Oracle
    auto challenge_btn_opt = createAnimatedButtonOption("Challenge Oracle", [ctx, ind_ctx] {
        ind_ctx->challenge_cipher.clear();
        ind_ctx->challenge_hex_blocks.clear();
        ind_ctx->scroll_offset = 0;

        try {
            if (ind_ctx->m0_input.empty() || ind_ctx->m1_input.empty()) {
                ind_ctx->message_info = "Error: Both messages m0 and m1 must be non-empty!";
                return;
            }

            auto cipher = ind_ctx->oracle.challenge(ind_ctx->m0_input, ind_ctx->m1_input);
            ind_ctx->challenge_cipher = cipher;

            constexpr int hex_width = 16;
            for (std::uint64_t block : cipher) {
                std::stringstream ss;
                ss << "0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(hex_width) << block;
                ind_ctx->challenge_hex_blocks.push_back(ss.str());
            }

            ind_ctx->last_guess_correct.reset();
            ind_ctx->last_actual_choice.reset();
            ind_ctx->last_user_guess.reset();
            ind_ctx->message_info = "Oracle tossed a coin and encrypted m_b! Can you tell which one?";
        } catch (const std::exception& e) {
            ind_ctx->message_info = std::string("Error: ") + e.what();
        }
    }, 20);
    Component challenge_btn = Button(challenge_btn_opt);

    // Button: Copy Oracle Key to TUI Playground
    auto copy_key_btn_opt = createAnimatedButtonOption("Use Key in Encrypt", [ctx, ind_ctx] {
        PublicKey pk = ind_ctx->oracle.getPublicKey();
        ctx->status_ctx.curr_public_key = pk;
        ctx->decrypt_ctx.decryptor.reset();
        ctx->encrypt_ctx.encryptor.emplace(RSAEncryptor(pk));
        ctx->status_ctx.status_content = "Oracle key copied! Switch to Dashboard to encrypt m0 and test.";
        ind_ctx->message_info = "Oracle key active in Playground! Check Encrypt view to test m0.";
    }, 22);
    Component copy_key_btn = Button(copy_key_btn_opt);

    // Button: Guess m0
    auto guess_m0_btn_opt = createAnimatedButtonOption("Guess m0 (b=0)", [ind_ctx] {
        if (!ind_ctx->oracle.hasActiveChallenge()) {
            ind_ctx->message_info = "No active challenge! Click 'Challenge Oracle' first.";
            return;
        }

        int actual = ind_ctx->oracle.getLastCoinToss();
        bool correct = ind_ctx->oracle.verifyGuess(0);

        ind_ctx->total_guesses++;
        if (correct) ind_ctx->correct_guesses++;

        ind_ctx->last_guess_correct = correct;
        ind_ctx->last_user_guess = 0;
        ind_ctx->last_actual_choice = actual;

        if (correct) {
            ind_ctx->message_info = "CORRECT! The Oracle encrypted m0.";
        } else {
            ind_ctx->message_info = "WRONG! The Oracle encrypted m1.";
        }
    }, 18);
    Component guess_m0_btn = Button(guess_m0_btn_opt);

    // Button: Guess m1
    auto guess_m1_btn_opt = createAnimatedButtonOption("Guess m1 (b=1)", [ind_ctx] {
        if (!ind_ctx->oracle.hasActiveChallenge()) {
            ind_ctx->message_info = "No active challenge! Click 'Challenge Oracle' first.";
            return;
        }

        int actual = ind_ctx->oracle.getLastCoinToss();
        bool correct = ind_ctx->oracle.verifyGuess(1);

        ind_ctx->total_guesses++;
        if (correct) ind_ctx->correct_guesses++;

        ind_ctx->last_guess_correct = correct;
        ind_ctx->last_user_guess = 1;
        ind_ctx->last_actual_choice = actual;

        if (correct) {
            ind_ctx->message_info = "CORRECT! The Oracle encrypted m1.";
        } else {
            ind_ctx->message_info = "WRONG! The Oracle encrypted m0.";
        }
    }, 18);
    Component guess_m1_btn = Button(guess_m1_btn_opt);

    // Button: New Oracle Key
    auto new_key_btn_opt = createAnimatedButtonOption("New Oracle Key", [ctx, ind_ctx] {
        try {
            ind_ctx->oracle.reset(30);
            ind_ctx->challenge_cipher.clear();
            ind_ctx->challenge_hex_blocks.clear();
            ind_ctx->last_guess_correct.reset();
            ind_ctx->last_actual_choice.reset();
            ind_ctx->last_user_guess.reset();
            ind_ctx->message_info = "Generated new Oracle RSA key pair.";
        } catch (const std::exception& e) {
            ind_ctx->message_info = std::string("Error: ") + e.what();
        }
    }, 18);
    Component new_key_btn = Button(new_key_btn_opt);

    // Button: Reset Stats
    auto reset_stats_btn_opt = createAnimatedButtonOption("Reset Stats", [ind_ctx] {
        ind_ctx->total_guesses = 0;
        ind_ctx->correct_guesses = 0;
        ind_ctx->last_guess_correct.reset();
        ind_ctx->last_actual_choice.reset();
        ind_ctx->last_user_guess.reset();
        ind_ctx->message_info = "Statistics have been reset.";
    }, 18);
    Component reset_stats_btn = Button(reset_stats_btn_opt);

    Component status_bar = createStatusBarView(app_ctx);

    // Component tree
    Component left_column = Container::Vertical({
        input_m0,
        input_m1,
        Container::Horizontal({
            challenge_btn,
            copy_key_btn,
        }),
    });

    Component right_column = Container::Vertical({
        Container::Horizontal({
            guess_m0_btn,
            guess_m1_btn,
        }),
        Container::Horizontal({
            new_key_btn,
            reset_stats_btn,
        }),
    });

    Component controls = Container::Vertical({
        Container::Horizontal({
            left_column,
            right_column,
        }),
        status_bar,
    });

    return Renderer(controls, [=, ctx = &app_ctx] {
        auto* ictx = &ctx->indistinguishability_ctx;
        PublicKey oracle_pk = ictx->oracle.getPublicKey();

        // Cipher blocks rendering
        Elements cipher_elements;
        if (ictx->challenge_hex_blocks.empty()) {
            cipher_elements.push_back(text("No challenge yet") | dim | center);
        } else {
            for (const auto& hex_block : ictx->challenge_hex_blocks) {
                cipher_elements.push_back(text(hex_block) | color(Color::YellowLight));
            }
        }

        // Stats calculation
        int total = ictx->total_guesses;
        int correct = ictx->correct_guesses;
        double win_rate = total > 0 ? (100.0 * correct / total) : 0.0;

        std::stringstream win_rate_ss;
        win_rate_ss << std::fixed << std::setprecision(1) << win_rate << " %";

        // Last guess status element
        Element last_result_elem;
        if (!ictx->last_guess_correct.has_value()) {
            if (ictx->oracle.hasActiveChallenge()) {
                last_result_elem = text(">> Challenge active! Make your guess.") | bold | color(Color::Cyan);
            } else {
                last_result_elem = text("Awaiting challenge...") | dim;
            }
        } else if (*ictx->last_guess_correct) {
            last_result_elem = text("Success: Winner! Correct guess.") | bold | color(Color::GreenLight);
        } else {
            last_result_elem = text("Failed: Wrong guess.") | bold | color(Color::RedLight);
        }

        // Oracle panel
        Element oracle_panel = window(
            text("1. Oracle (Challenger)"),
            vbox({
                text("IND-CPA Experiment:"),
                text("Submit two plaintexts of your choice. The Oracle picks b in {0,1}"),
                text("and returns the ciphertext c = Enc(pk, m_b)."),
                separator(),
                hbox({text("m0: ") | bold, input_m0->Render()}),
                hbox({text("m1: ") | bold, input_m1->Render()}),
                hbox({
                    challenge_btn->Render(),
                    copy_key_btn->Render(),
                }),
                separator(),
                hbox({
                    text("Oracle PK: ") | bold,
                    text("e=" + std::to_string(oracle_pk.e) + ", n=" + std::to_string(oracle_pk.n))
                        | color(Color::YellowLight),
                }),
                separator(),
                text("Challenge Ciphertext (c):") | bold,
                vbox(std::move(cipher_elements)) | frame | size(HEIGHT, LESS_THAN, 5) | border,
            })
        ) | flex;

        // Attacker / Guesser panel
        Element attacker_panel = window(
            text("2. Attacker (Distinguisher)"),
            vbox({
                text("Deterministic RSA Vulnerability:"),
                text("Because textbook RSA has NO randomness (padding), you can win 100%!"),
                text("Copy the key, encrypt m0 yourself on Dashboard, and compare c!"),
                separator(),
                last_result_elem,
                text(ictx->message_info.value_or("")) | color(Color::White),
                separator(),
                text("Submit Your Guess:") | bold,
                hbox({
                    guess_m0_btn->Render(),
                    guess_m1_btn->Render(),
                }),
                separator(),
                text("Attacker Statistics:") | bold,
                hbox({text("Total Guesses:   "), text(std::to_string(total)) | bold}),
                hbox({text("Correct Guesses: "), text(std::to_string(correct)) | bold | color(Color::Green)}),
                hbox({text("Win Rate:        "), text(win_rate_ss.str()) | bold | color(win_rate > 50.0 ? Color::GreenLight : Color::White)}),
                hbox({text("Random Chance:   "), text("50.0 %") | dim}),
                separator(),
                hbox({
                    new_key_btn->Render(),
                    reset_stats_btn->Render(),
                }),
            })
        ) | flex;

        return vbox({
            hbox({
                oracle_panel,
                attacker_panel,
            }) | flex,
            status_bar->Render(),
        });
    });
}
