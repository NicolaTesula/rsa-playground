#include "view_status_bar.hpp"

using namespace ftxui;

Component createStatusBarView(AppContext &app_ctx) {
    // Create PUB: [n: , e: ] | PRIV: [d: ] || _____________ status bar
    return Renderer([&app_ctx] {
        std::string keys_txt;
        if (app_ctx.status_ctx.curr_public_key.has_value()) {
            const auto& key = *app_ctx.status_ctx.curr_public_key;
            keys_txt = "PUB: [n: " + std::to_string(key.n)
                + ", e: " + std::to_string(key.e) + "]";
        } else {
            keys_txt = "PUB: [not generated]";
        }
        keys_txt += "\nPRIV: [d: TODO]";

        Element keys_box = hbox({
            text(keys_txt)
        });

        return window(
            text("Status"),
            hbox({
                text(keys_txt),
                separator(),
                filler(),
                text(app_ctx.status_ctx.status_content.value_or(""))
                    | size(WIDTH, EQUAL, 42) 
                    | center
            })
        );
    });
}