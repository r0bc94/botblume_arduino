#ifndef water_level_measure_h
#define water_level_measure_h
/**
 * Offers a method to measure the current water level and redurns
 * the water level as a message, including the actual measurement and
 * the calculated percentage value.
 */
#include <Arduino.h>
#include "types/message.h"

#define CLAMP_LEFT_VALUE 12
#define CLAMP_RIGHT_VALUE 400

/**
 * Measures the current state of the water level and returns a
 * pointer to an initialized Message Struct, which contains the measurement
 * and the aggregated value.
 * 
 * @param analogPin Analog input, on which the water sensor is attached.
 * @returns A pointer to the message struct, containing the different measurement.
 */
struct Message *getMeasurement(__UINT8_TYPE__ analogPin);
#endif
