#include "indistinguishability_game.hpp"

#include <stdexcept>

Oracle::Oracle() : Oracle(30) {}

Oracle::Oracle(std::uint8_t bit_width)
    : m_decryptor(bit_width),
      m_encryptor(m_decryptor.get_public_key()),
      m_coinToss(-1),
      m_has_active_challenge(false) {
    std::random_device rd;
    this->gen.seed(rd());
    this->dist = std::uniform_int_distribution<>(0, 1);
}

void Oracle::reset(std::uint8_t bit_width) {
    m_decryptor = RSADecryptor(bit_width);
    m_encryptor = RSAEncryptor(m_decryptor.get_public_key());
    m_coinToss = -1;
    m_has_active_challenge = false;
}

std::vector<std::uint64_t> Oracle::challenge(const std::string& m_0, const std::string& m_1) {
    if (m_0.empty() || m_1.empty()) {
        throw std::invalid_argument("Both challenge messages must be non-empty!");
    }

    m_coinToss = dist(gen);
    m_has_active_challenge = true;

    if (m_coinToss == 0) {
        return this->m_encryptor.encrypt_msg(m_0);
    } else {
        return this->m_encryptor.encrypt_msg(m_1);
    }
}

PublicKey Oracle::getPublicKey() const {
    return this->m_encryptor.get_public_key();
}

bool Oracle::verifyGuess(int guess) {
    if (!m_has_active_challenge) {
        throw std::logic_error("No active challenge to guess!");
    }
    this->m_has_active_challenge = false;
    return (guess == m_coinToss);
}

bool Oracle::hasActiveChallenge() const {
    return m_has_active_challenge;
}

int Oracle::getLastCoinToss() const {
    return m_coinToss;
}