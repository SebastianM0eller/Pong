#pragma once
#include <SFML/Audio.hpp>
#include <stdexcept>
#include <unordered_map>

class GameSound : public sf::Sound {
       public:
        using sf::Sound::Sound;  // For the constructor.
        sf::Clock lifeTime;      // To track the lifeTime.
};

class SoundSystem {
       public:
        ///
        /// Clear the cached sounds and buffers.
        /// Allocate sounds to avoid stuttering.
        /// Throws a runtime_error if the internal path is invalid.
        ///
        static void Init() {
                m_ActiveSounds.clear();
                m_Buffers.clear();

                m_ActiveSounds.reserve(16);

                sf::SoundBuffer& buffer = GetBuffer("assets/sounds/sqr_beep.wav");

                for (int i = 0; i < 16; i++) {
                        m_ActiveSounds.emplace_back(buffer);
                }
        }
        ///
        /// Plays the sound from the specified sourcePath.
        /// If it fails to load the buffer from the specified sourcePath
        /// it throws a runtime_error.
        ///
        static void PlaySound(const std::string& sourcePath) {
                sf::SoundBuffer& buffer = GetBuffer(sourcePath);

                for (auto& s : m_ActiveSounds) {
                        if (s.getStatus() != sf::Sound::Status::Playing &&
                            s.lifeTime.getElapsedTime().asSeconds() > 0.5f) {
                                s.setBuffer(buffer);
                                s.play();
                                return;
                        }
                }

                m_ActiveSounds.emplace_back(buffer);
                m_ActiveSounds.back().play();
        }

       private:
        SoundSystem() = default;
        ~SoundSystem() = default;

        ///
        /// Retrieves a buffer based on the specified sourcePath.
        /// If it fails to generate the buffer, it throws a runtime_error.
        ///
        static sf::SoundBuffer& GetBuffer(const std::string& sourcePath) {
                auto it = m_Buffers.find(sourcePath);
                if (it != m_Buffers.end()) return it->second;

                sf::SoundBuffer& newBuffer = m_Buffers[sourcePath];

                if (!newBuffer.loadFromFile(sourcePath)) {
                        throw std::runtime_error("Failed to load SoundBuffer from file: " + sourcePath);
                }
                return newBuffer;
        }

        static inline std::unordered_map<std::string, sf::SoundBuffer> m_Buffers;
        static inline std::vector<GameSound> m_ActiveSounds;
};
