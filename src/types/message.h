#ifndef message_h
#define message_h
/**
 * Represents the mqtt message that contains the current water level.
 */
struct Message {
    __UINT8_TYPE__ percentage; /* Calculated perceptual water level*/
    __UINT8_TYPE__ originalValue; /* Original value read from the adc*/ 
};
#endif