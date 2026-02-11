#pragma once
#include <SFML/Audio.hpp>
#include <list>
#include <stdexcept>
#include <unordered_map>

class GameSound : public sf::Sound {
       public:
        using sf::Sound::Sound;  // For the constructor
        sf::Clock lifeTime;
};

class SoundSystem {
       public:
        ///
        /// Plays the sound from the specified sourcePath.
        /// If it fails to load the buffer from the specified sourcePath
        /// it throws a runtime_error.
        ///
        static void PlaySound(const std::string& sourcePath) {
                sf::SoundBuffer& buffer = GetBuffer(sourcePath);

                m_ActiveSounds.emplace_back(buffer);
                m_ActiveSounds.back().play();
        }

        ///
        /// Deletes sounds that have finished playing.
        /// The lifeTime is checked to avoid instant deletes
        /// resulting in the sound not being played
        ///
        static void Update() {
                m_ActiveSounds.remove_if([](const GameSound& s) {
                        return (s.getStatus() == sf::Sound::Status::Stopped &&
                                s.lifeTime.getElapsedTime().asSeconds() > 0.1f);
                });
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
        static inline std::list<GameSound> m_ActiveSounds;
};
