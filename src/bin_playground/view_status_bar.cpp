#include "view_status_bar.hpp"

using namespace ftxui;

Component createStatusBarView(AppContext &app_ctx) {
    // Create PUB: [n: , e: ] | PRIV: [d: ] || _____________ status bar
    return Renderer([&app_ctx] {
        std::string keys_txt;
        if (app_ctx.status_ctx.curr_public_key.has_value()) {
            const auto& key = *app_ctx.status_ctx.curr_public_key;
            keys_txt = "PUB:  [n: " + std::to_string(key.n)
                + ", e: " + std::to_string(key.e) + "]";
        } else {
            keys_txt = "PUB:   [not generated]";
        }
        if (app_ctx.decrypt_ctx.decryptor.has_value()) {
            keys_txt += "\nPRIV: [d: "
                + std::to_string(app_ctx.decrypt_ctx.decryptor->get_private_key())
                + "]";
        } else {
            keys_txt += "\nPRIV: [not generated]";
        }

        Element keys_box = hbox({
            text(keys_txt)
        });

        return window(
            text("Status"),
            hbox({
                text(keys_txt),
                filler() | size(WIDTH, EQUAL, 15),
                separator(),
                text(app_ctx.status_ctx.status_content.value_or(""))
                    | size(WIDTH, GREATER_THAN, 100)
            })
        );
    });
}