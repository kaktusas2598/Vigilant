#include "SoundManager.hpp"
#include "ErrorHandler.hpp"

namespace Vigilant {

    SoundManager* SoundManager::s_pInstance = 0;
    
    SoundManager::SoundManager() {
        // (int frequency, Uint16 format, int channels, int chunksize)
        Mix_OpenAudio(22050, AUDIO_S16, 2, 4096);
    }

    SoundManager::~SoundManager() {
        Mix_CloseAudio();
    }

    bool SoundManager::load(std::string fileName, std::string id, SoundType type) {
        if (type == SOUND_MUSIC) {
            Mix_Music* music = Mix_LoadMUS(fileName.c_str());
            if (music == 0) {
                exitWithError("Could not load music file");
                return false;
            }
            musicMap[id] = music;
            return true;
        } else if (type == SOUND_SFX) {
            Mix_Chunk* fx = Mix_LoadWAV(fileName.c_str());
            if (fx == 0) {
                exitWithError("Could not load wav file");
                return false;
            }
            sFXMap[id] = fx;
            return true;
        }
        return false;
    }

    void SoundManager::playSound(std::string id, int loop) {
        // -1 means any channel
        Mix_PlayChannel(-1, sFXMap[id], loop);
    }

    void SoundManager::playMusic(std::string id, int loop) {
        Mix_PlayMusic(musicMap[id], loop);
    }
}
