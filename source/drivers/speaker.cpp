#include "drivers/speaker.hpp"
#include <rtos/ThisThread.h>

// Include the generated song data
// Copy the arrays from SongConversion/song_data.c here
const uint8_t song_pwm1[50000] = { /* paste song_pwm1 data */ };
const uint8_t song_pwm2[50000] = { /* paste song_pwm2 data */ };

// TODO: Add your code here
namespace drivers
{
   /**
    * @brief Class constructor speaker
    *
    */
    CSpeaker::CSpeaker(PinName f_in1, PinName f_in2)
    : m_in1(f_in1),
      m_in2(f_in2),
      m_index(0),
      m_playing(false),
      m_volume(0.1f)  // Start with low volume (10%)
    {
        /* constructor behaviour */
        // Set PWM period for 8kHz audio
        m_in1.period(1.0f / 8000.0f);
        // Start the playback thread
        m_thread.start(callback(this, &CSpeaker::playLoop));
    }

    /** @brief  CSpeaker class destructor
     */
    CSpeaker::~CSpeaker()
    {
        m_thread.terminate();
    };

    void CSpeaker::playSong()
    {
        m_playing = true;
        m_index = 0;
    }

    void CSpeaker::stopSong()
    {
        m_playing = false;
    }

    void CSpeaker::setVolume(float volume)
    {
        if (volume < 0.0f) volume = 0.0f;
        if (volume > 1.0f) volume = 1.0f;
        m_volume = volume;
    }

    void CSpeaker::playLoop()
    {
        while(true) {
            if(m_playing) {
                float duty1 = (song_pwm1[m_index] / 255.0f) * m_volume;
                float duty2 = (song_pwm2[m_index] / 255.0f) * m_volume;
                m_in1.write(duty1);
                m_in2.write(duty2);
                m_index = (m_index + 1) % 50000;  // assuming 50000 samples
            }
            ThisThread::sleep_for(125us);
        }
    }

}; // namespace drivers