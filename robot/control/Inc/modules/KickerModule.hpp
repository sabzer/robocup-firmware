#pragma once

#include "GenericModule.hpp"
#include "MicroPackets.hpp" 
#include "drivers/KickerBoard.hpp"

class KickerModule : public GenericModule {
public:
    // How many times per second this module should run
    static constexpr float freq = 100.0f; // Hz
    static constexpr uint32_t period = static_cast<uint32_t>(1000 / freq);

    // How long a single call to this module takes
    static constexpr uint32_t runtime = 1; // ms

    KickerModule(KickerCommand *const kickerCommand,
                 KickerInfo *const kickerInfo);

    virtual void entry(void);

private:
    KickerCommand *const kickerCommand;
    KickerInfo *const kickerInfo;
    
    // Time of last command to kick
    // Stops double triggers on the same packet
    uint32_t prevKickTime;
    KickerBoard kicker;
};