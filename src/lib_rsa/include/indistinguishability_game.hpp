#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include "public_key.hpp"
#include "rsa_decryptor.hpp"
#include "rsa_encryptor.hpp"

class Oracle {
private:
    std::mt19937 gen;                      // RNG
    std::uniform_int_distribution<> dist;  // Distribution to draw from
    RSADecryptor m_decryptor;              // Generates the public key
    RSAEncryptor m_encryptor;              // Handles the encryption of received messages
    int m_coinToss;                        // The number in {0,1} that determined the message to be encrypted
    bool m_has_active_challenge;           // Is a challenge already happening

public:
    Oracle();
    explicit Oracle(std::uint8_t bit_width);

    // Resets the key pair and challenge state
    void reset(std::uint8_t bit_width = 30);

    // To be called by the challenger. Requests the oracle to encrypt one of both messages
    std::vector<std::uint64_t> challenge(const std::string& m_0, const std::string& m_1);

    // Getter for the public key
    PublicKey getPublicKey() const;

    // Takes in the attacker's guess and checks if it is correct
    bool verifyGuess(int guess);

    // Checks if a challenge is waiting for a guess
    bool hasActiveChallenge() const;

    // The choice (0 or 1) of the last challenge
    int getLastCoinToss() const;
};
