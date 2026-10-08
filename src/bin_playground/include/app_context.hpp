#pragma once

#include <optional>

#include "rsa_decryptor.hpp"
#include "public_key.hpp"

struct AppContext {
    std::string detailsContent;
    std::optional<RSADecryptor> decryptor;
    std::optional<PublicKey> current_public_key;

    std::string getDetailsContent() { return detailsContent; }
};