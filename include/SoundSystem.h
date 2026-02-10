#pragma once
#include <SFML/Audio.hpp>
#include <list>
#include <stdexcept>
#include <unordered_map>

class SoundSystem {
       public:
        ///
        ///
        ///
        static void PlaySound(const std::string& sourcePath) {
                sf::SoundBuffer& buffer = GetBuffer(sourcePath);

                m_ActiveSounds.emplace_back(buffer);
                m_ActiveSounds.back().play();
        }

        static void Update() {
                // Remove the sound if it has finished playing.
                m_ActiveSounds.remove_if(
                    [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Status::Stopped; });
        }

       private:
        SoundSystem() = default;
        ~SoundSystem() = default;

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
        static inline std::list<sf::Sound> m_ActiveSounds;
};
