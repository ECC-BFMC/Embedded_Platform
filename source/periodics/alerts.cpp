#include "periodics/alerts.hpp"

#define NOTE_B0  31
#define  NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define  NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define  NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define  NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define  NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define  NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define  NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3  185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define  NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4  277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define  NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4  415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define  NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5  622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define  NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5  932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define  NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define  NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define  NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define  NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructoralerts
    *
    */
    CAlerts::CAlerts(
        std::chrono::milliseconds f_period
    )
    : utils::CTask(f_period)
    , m_isActive(false)
    , m_period((uint16_t)(f_period.count()))
    , buzzer(PC_7)
    , current_step(0)
    , step_counter(0)
    , tone_active(false)
    , warning_active(false)
    , alert_id(0)
    {
        /* constructor behaviour */
    }

    /** @brief  CAlerts class destructor
     */
    CAlerts::~CAlerts()
    {
    };

    void CAlerts::serialCallbackAlertsCommand(char const * a, char * b) {
        uint8_t l_isActivate=0;
        uint8_t l_res = sscanf(a,"%hhu",&l_isActivate);

        if(l_res==1){

            m_isActive=(l_isActivate>=1);
            sprintf(b,"%hhu", l_isActivate);

            alert_id = l_isActivate;

            if(0 == l_isActivate){
                buzzer.period_us(2);
                buzzer.pulsewidth_us(1);

                setNewPeriod(5000);
                m_period = 5000;

            } else if(1<= l_isActivate && 4 >= l_isActivate){
                current_step = 0;
                step_counter = 0;
                tone_active = false;
                warning_active = false;
                setNewPeriod(10);
                m_period = 10;
                buzzer.pulsewidth_us(0);
            }
            
        }else{
            sprintf(b,"syntax error");
        }
    }

    void CAlerts::_run()
    {
        if(!m_isActive) return;

        if(1 == alert_id){
            switch (current_step) {
                case 0:
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_B4);  // Setăm frecvența PWM pentru B4 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B4)/2);  // 50% duty cycle
                        tone_active = true;
                        step_counter = 0;  // Resetăm contorul
                    } else if (step_counter >= 10) {  // 100 ms pentru tonul B4 si dupa..
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        step_counter = 0;
                        tone_active = false;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 1:
                    if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms si dupa..
                        buzzer.period_us(1000000 / NOTE_B5);  // Setăm frecvența PWM pentru B5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 2:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul B5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_FS5);  // Setăm frecvența PWM pentru FS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_FS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 3:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul FS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_DS5);  // Setăm frecvența PWM pentru DS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_DS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 4:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_FS5);  // Setăm frecvența PWM pentru FS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_FS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 5:
                    if (tone_active && step_counter >= 12) {  // 120 ms pentru tonul FS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 20) {  // Pauza de 200 ms
                        buzzer.period_us(1000000 / NOTE_DS5);  // Setăm frecvența PWM pentru DS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_DS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 6:
                    if (tone_active && step_counter >= 15) {  // 150 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 25) {  // Pauza de 250 ms
                        buzzer.period_us(1000000 / NOTE_C5);  // Setăm frecvența PWM pentru C5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_C5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 7:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul C5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_C6);  // Setăm frecvența PWM pentru C6 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_C6)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 8:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul C6
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_G5);  // Setăm frecvența PWM pentru G5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_G5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 9:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul G5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_E5);  // Setăm frecvența PWM pentru E5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_E5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 10:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul E5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_C6);  // Setăm frecvența PWM pentru C6 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_C6)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 11:
                    if (tone_active && step_counter >= 12) {  // 120 ms pentru tonul C6
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 20) {  // Pauza de 200 ms
                        buzzer.period_us(1000000 / NOTE_G5);  // Setăm frecvența PWM pentru G5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_G5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 12:
                    if (tone_active && step_counter >= 15) {  // 150 ms pentru tonul G5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 25) {  // Pauza de 250 ms
                        buzzer.period_us(1000000 / NOTE_B4);  // Setăm frecvența PWM pentru B4 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B4)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 13:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul B4
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_B5);  // Setăm frecvența PWM pentru B5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 14:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul B5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_FS5);  // Setăm frecvența PWM pentru FS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_FS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 15:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul FS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_DS5);  // Setăm frecvența PWM pentru DS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_DS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 16:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_B5);  // Setăm frecvența PWM pentru B5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 17:
                    if (tone_active && step_counter >= 12) {  // 120 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 20) {  // Pauza de 200 ms
                        buzzer.period_us(1000000 / NOTE_FS5);  // Setăm frecvența PWM pentru FS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_FS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 18:
                    if (tone_active && step_counter >= 15) {  // 150 ms pentru tonul FS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 25) {  // Pauza de 250 ms
                        buzzer.period_us(1000000 / NOTE_DS5);  // Setăm frecvența PWM pentru DS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_DS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 19:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_DS5);  // Setăm frecvența PWM pentru DS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_DS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 20:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul DS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_E5);  // Setăm frecvența PWM pentru E5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_E5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 21:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul E5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_F5);  // Setăm frecvența PWM pentru F5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_F5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 22:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul F5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_F5);  // Setăm frecvența PWM pentru F5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_F5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 23:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul F5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_FS5);  // Setăm frecvența PWM pentru FS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_FS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 24:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul FS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_G5);  // Setăm frecvența PWM pentru G5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_G5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 25:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul G5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_G5);  // Setăm frecvența PWM pentru G5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_G5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 26:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul G5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_GS5);  // Setăm frecvența PWM pentru GS5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_GS5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 27:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul GS5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_A5);  // Setăm frecvența PWM pentru A5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_A5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step++;
                    } else {
                        step_counter++;
                    }
                    break;

                case 28:
                    if (tone_active && step_counter >= 10) {  // 100 ms pentru tonul A5
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;
                    } else if (!tone_active && step_counter >= 12) {  // Pauza de 120 ms
                        buzzer.period_us(1000000 / NOTE_B5);  // Setăm frecvența PWM pentru B5 în µs
                        buzzer.pulsewidth_us((1000000 / NOTE_B5)/2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                        current_step = 0;  // Repornește secvența
                    } else {
                        step_counter++;
                    }
                    break;

                default:
                    current_step = 0;  // Repornește secvența de la început
                    step_counter = 0;
                    tone_active = false;
                    buzzer.pulsewidth_us(0);  // Ne asigurăm că buzzerul este oprit
                    break;
            }
        } else if(2 == alert_id){

            switch (current_step) {
                case 0:  // Prima notă: C4 (Do) - 262 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_C4);  // Frecvența pentru C4 (262 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_C4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 1:  // A doua notă: D4 (Re) - 294 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_D4);  // Frecvența pentru D4 (294 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_D4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 2:  // A treia notă: E4 (Mi) - 330 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_E4);  // Frecvența pentru E4 (330 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_E4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 3:  // A patra notă: G4 (Sol) - 392 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_G4);  // Frecvența pentru G4 (392 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_G4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, încheiem secvența
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        current_step++;  // Trecem la finalul secvenței
                    } else {
                        step_counter++;
                    }
                    break;

                case 4:  // Secvența de startup a fost completată
                    buzzer.period_us(2);
                    buzzer.pulsewidth_us(1);  // Asigurăm că tonul este oprit
                    
                    alert_id = 0;
                    setNewPeriod(5000);
                    m_period = 5000;
                    break;

                default:
                    current_step = 0;  // Resetăm secvența
                    tone_active = false;
                    step_counter = 0;
                    buzzer.pulsewidth_us(0);  // Oprim tonul
                    break;
            }
        } else if(3 == alert_id){

            switch (current_step) {
                case 0:  // Prima notă: E4
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_E4);  // Frecvența pentru C4 (262 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_E4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 50ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 1:  // A doua notă: D4 (Re) - 294 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_D4);  // Frecvența pentru D4 (294 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_D4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 2:  // A treia notă: C4
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_C4);  // Frecvența pentru E4 (330 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_C4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 3:  // A patra notă: B3
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_B3);  // Frecvența pentru G4 (392 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_B3) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, încheiem secvența
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        current_step++;  // Trecem la finalul secvenței
                    } else {
                        step_counter++;
                    }
                    break;

                case 4:  // A cincea notă: A3
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_A3);  // Frecvența pentru G4 (392 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_A3) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, încheiem secvența
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        current_step++;  // Trecem la finalul secvenței
                    } else {
                        step_counter++;
                    }
                    break;

                case 5:  // Ultima notă: G3
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_G3);  // Frecvența pentru G4 (392 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_G3) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, încheiem secvența
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        current_step++;  // Trecem la finalul secvenței
                    } else {
                        step_counter++;
                    }
                    break;

                case 6:  // Secvența de startup a fost completată
                    buzzer.period_us(2);
                    buzzer.pulsewidth_us(1);  // Asigurăm că tonul este oprit
                    
                    alert_id = 0;
                    setNewPeriod(5000);
                    m_period = 5000;
                    break;

                default:
                    current_step = 0;  // Resetăm secvența
                    tone_active = false;
                    step_counter = 0;
                    buzzer.pulsewidth_us(0);  // Oprim tonul
                    break;
            }
        } else if(4 == alert_id){

            switch (current_step) {
                case 0:  // Prima notă: E4
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_G4);  // Frecvența pentru C4 (262 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_G4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 50ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 1:  // A doua notă: D4 (Re) - 294 Hz
                    if (!tone_active) {
                        buzzer.period_us(1000000 / NOTE_B4);  // Frecvența pentru D4 (294 Hz)
                        buzzer.pulsewidth_us((1000000 / NOTE_B4) / 2);  // Activăm tonul
                        tone_active = true;
                        step_counter = 0;
                    } else if (step_counter >= 10) {  // După 100ms, trecem la următoarea notă
                        buzzer.pulsewidth_us(0);  // Oprim tonul
                        tone_active = false;
                        step_counter = 0;
                        current_step++;  // Trecem la următoarea notă
                    } else {
                        step_counter++;
                    }
                    break;

                case 2:  // A treia notă: C4
                    buzzer.period_us(2);
                    buzzer.pulsewidth_us(1);  // Asigurăm că tonul este oprit
                    
                    alert_id = 0;
                    setNewPeriod(5000);
                    m_period = 5000;
                    break;
            }
        }
    }

}; // namespace periodics