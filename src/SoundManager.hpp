#ifndef __SoundManager__
#define __SoundManager__

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

namespace Vigilant {

    enum SoundType {
        SOUND_MUSIC = 0,
        SOUND_SFX = 1
    };

    class SoundManager {
        public:
            static SoundManager* Instance() {
                if (s_pInstance == 0) {
                    s_pInstance = new SoundManager();
                    return s_pInstance;
                }
                return s_pInstance;
            }

            bool load(std::string fileName, std::string id, SoundType type);

            void playSound(std::string id, int loop);
            void playMusic(std::string id, int loop);
        private:
            static SoundManager* s_pInstance;

            std::map<std::string, Mix_Chunk*> sFXMap;
            std::map<std::string, Mix_Music*> musicMap;

            SoundManager();
            ~SoundManager();

            SoundManager(const SoundManager&);
            SoundManager &operator= (const SoundManager&);
    };

    typedef SoundManager TheSoundManager;
}

#endif // __SoundManager__