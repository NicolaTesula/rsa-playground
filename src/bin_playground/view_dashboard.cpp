#include "view_dashboard.hpp"
#include "view_prime_generator.hpp"
#include "view_primality_check.hpp"
#include "view_encrypt.hpp"
#include "view_decrypt.hpp"
#include "view_status_bar.hpp"

using namespace ftxui;

Component createDashboardView(AppContext& app_ctx) {
    Component gen_primes = createPrimeGenView(app_ctx);
    Component check_primality = createPrimalityCheckView(app_ctx);
    Component encrypt = createEncryptView(app_ctx);
    Component decrypt = createDecryptView(app_ctx);
    Component status_bar = createStatusBarView(app_ctx);

    Component root = Container::Vertical({
        Container::Horizontal({gen_primes, check_primality}),
        Container::Horizontal({encrypt, decrypt})
    });

    return Renderer(root, [=] {
        return vbox({
            text("RSA PLAYGROUND") | bold | center | color(Color::Cyan),
            separator(),
            gridbox({
                {
                    filler(),
                    window(text("Generate Primes"), gen_primes->Render()),
                    window(text("Primality Test"), check_primality->Render()),
                    filler()
                },
                {
                    filler(),
                    window(text("Encrypt"), encrypt->Render()),
                    window(text("Decrypt"), decrypt->Render()),
                    filler()
                },
            }) | flex, // gridbox
            status_bar->Render()
        });
    });
}