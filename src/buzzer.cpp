#include "buzzer.h"
#include "usart.h"

#define BUZZER_PIN 0 // CHANGE THIS

// This function should initialize the buzzer.
// This should mostly involve initializing the pins and other register values.
void setup_buzzer(void) {

}

// This function should set the volume of the buzzer.
void set_volume_buzzer(volume_t volume) {
    switch (volume) {
        case VOLUME_LOW:
            break;
        case VOLUME_MEDIUM:
            break;
        case VOLUME_HIGH:
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid volume level", (double) volume);
            break;
    }
}

// This function should set the frequency of the buzzer.
void set_frequency_buzzer(float frequency) {
    
}
