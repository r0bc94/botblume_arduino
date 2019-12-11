#include "water_level_measure.h"

static __UINT8_TYPE__ readWaterLevel(__UINT8_TYPE__ analogPin);
static __UINT8_TYPE__ getAggregatedValue(__UINT8_TYPE__);

struct Message *getMeasurement(__UINT8_TYPE__ analogPin) {
    __UINT8_TYPE__ rawMeasurement = readWaterLevel(analogPin);
    __UINT8_TYPE__ aggregatedValue = getAggregatedValue(rawMeasurement);

    struct Message *result = (Message*) malloc(sizeof(Message));
    result->originalValue = rawMeasurement;
    result->percentage = aggregatedValue;

    return result;
}

__UINT8_TYPE__ readWaterLevel(__UINT8_TYPE__ analogPing) {
    __UINT8_TYPE__ analogValue = analogRead(analogPing);
    return analogValue; 
}

__UINT8_TYPE__ getAggregatedValue(__UINT8_TYPE__ rawValue) {
    if (rawValue <= CLAMP_LEFT_VALUE) {
        return CLAMP_LEFT_VALUE;
    } else if (rawValue >= CLAMP_RIGHT_VALUE) {
        return 100
        ;
    } else {
        //todo: The denominator is actually dependend of the LEFT_CLAMP value.
        return (__INT8_TYPE__) (1 / 4) * rawValue;
    }
}
