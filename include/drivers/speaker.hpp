#ifndef SPEAKER_HPP
#define SPEAKER_HPP

// TODO: Add your code here

#include <mbed.h>
#include <rtos/Thread.h>

namespace drivers
{
   /**
    * @brief Class speaker
    *
    */
    class CSpeaker
    {
        public:
            /* Constructor */
            CSpeaker(
                PinName f_in1,
                PinName f_in2
            );
            /* Destructor */
            ~CSpeaker();

            /* Play the song */
            void playSong();
            /* Stop playing */
            void stopSong();
            /* Set volume (0.0 to 1.0) */
            void setVolume(float volume);

        private:
            /* private variables & method member */
            PwmOut m_in1;
            PwmOut m_in2;
            Thread m_thread;
            size_t m_index;
            bool m_playing;
            float m_volume;

            /* Playback loop */
            void playLoop();
    }; // class CSpeaker
}; // namespace drivers

#endif // SPEAKER_HPP
