#pragma once

#include "cstdint"

class DribblerController {
public:
    /**
     * @param dt Period of control loop in ms
     */
    DribblerController(uint32_t dt);

    /**
     * Updates the controller with the latest input and calculates
     * the correct motor command to reach this target
     * 
     * @param setpoint Target to reach (duty cycles)
     * @param command Next command to send to the motors (duty cycles)
     */
    void calculate(uint8_t setpoint, uint8_t& command);
private:
    // Current speed
    uint16_t pv; // lsb
    uint32_t dt; // ms

    static constexpr uint8_t MAX_SPEED = 128u; // lsb
    static constexpr uint8_t MIN_SPEED = 0u; // lsb

    // Time for dribbler to ramp up from min to max speed
    static constexpr float MAX_SPEED_RAMP_TIME = .5f; // s

    // Max change in command per second
    // Caution: Make sure any changes to this doesn't result
    // in a number less than one due to truncation
    static constexpr uint16_t MAX_DELTAV_PER_S =
        (MAX_SPEED - MIN_SPEED) * static_cast<uint8_t>(1 / MAX_SPEED_RAMP_TIME);
};