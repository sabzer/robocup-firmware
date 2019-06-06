#include "mtrain.hpp"

#include "MicroPackets.hpp"

#include "modules/GenericModule.hpp"
#include "modules/BatteryModule.hpp"
#include "modules/FPGAModule.hpp"
#include "modules/IMUModule.hpp"
#include "modules/KickerModule.hpp"
#include "modules/LEDModule.hpp"
#include "modules/MotionControlModule.hpp"
#include "modules/RadioModule.hpp"
#include "modules/RotaryDialModule.hpp"

#define SUPER_LOOP_FREQ 1
#define SUPER_LOOP_PERIOD (1000 / SUPER_LOOP_FREQ)

// Max number of super loop cycles a proc can miss if it
// needs to run
#define MAX_MISS_CNT 5


struct MODULE_META_DATA {
    // Time in ms of last module execution
    uint32_t lastRunTime;

    // Time in ms of next module execution
    uint32_t nextRunTime;

    // Time in ms between module executions
    const uint32_t modulePeriod;

    // Estimate in ms of module runtime 
    const uint8_t moduleRunTime;

    GenericModule* module;

    MODULE_META_DATA(uint32_t lastRunTime,
                     uint32_t modulePeriod,
                     uint32_t moduleRunTime,
                     GenericModule* module)
        : lastRunTime(lastRunTime),
          nextRunTime(lastRunTime + modulePeriod),
          modulePeriod(modulePeriod),
          moduleRunTime(moduleRunTime),
          module(module) {}
};


MotionCommand motionCommand;
MotorCommand motorCommand;
MotorFeedback motorFeedback;
IMUData imuData;
BatteryVoltage batteryVoltage;
FPGAStatus fpgaStatus;
RadioError radioError;
RobotID robotID;
KickerCommand kickerCommand;
KickerInfo kickerInfo;


int main() {
    std::vector<MODULE_META_DATA> moduleList;

    // Init led first to show startup progress with LED's
    LEDModule led(&batteryVoltage,
                  &fpgaStatus,
                  &radioError);

    FPGAModule fpga(&motorCommand,
                    &fpgaStatus,
                    &motorFeedback);

    led.fpgaInitialized();

    RadioModule radio(&batteryVoltage,
                      &fpgaStatus,
                      &kickerInfo,
                      &robotID,
                      &kickerCommand,
                      &motionCommand,
                      &radioError);

    led.radioInitialized();

    KickerModule kicker(&kickerCommand,
                        &kickerInfo);

    led.kickerInitialized();

    BatteryModule battery(&batteryVoltage);
    RotaryDialModule dial(&robotID);
    MotionControlModule motion(&batteryVoltage,
                               &imuData,
                               &motionCommand,
                               &motorFeedback,
                               &motorCommand);
    IMUModule imu(&imuData);

    led.fullyInitialized();
    

    uint32_t curTime = HAL_GetTick();
    moduleList.emplace_back(curTime, MotionControlModule::period, MotionControlModule::runtime, &motion);
    moduleList.emplace_back(curTime, IMUModule::period,           IMUModule::runtime,           &imu);
    moduleList.emplace_back(curTime, FPGAModule::period,          FPGAModule::runtime,          &fpga);
    moduleList.emplace_back(curTime, RadioModule::period,         RadioModule::runtime,         &radio);
    moduleList.emplace_back(curTime, KickerModule::period,        KickerModule::runtime,        &kicker);
    moduleList.emplace_back(curTime, BatteryModule::period,       BatteryModule::runtime,       &battery);
    moduleList.emplace_back(curTime, RotaryDialModule::period,    RotaryDialModule::runtime,    &dial);
    moduleList.emplace_back(curTime, LEDModule::period,           LEDModule::runtime,           &led);

    while (true) {
        uint32_t loopStartTime = HAL_GetTick();
        uint32_t loopEndTime = HAL_GetTick() + SUPER_LOOP_PERIOD;

        for (unsigned int i = 0; i < moduleList.size(); i++) {
            MODULE_META_DATA& module = moduleList.at(i);
            uint32_t currentTime = HAL_GetTick();

            // Check if we should run the module
            //      It's time to run it and
            //      we have enough time in the super loop
            //
            // Subtraction allows for rollover compensation
            // then convertion to signed allows simple comparison
            if ((int32_t)(currentTime - module.nextRunTime) >= 0 &&
                (int32_t)(loopEndTime - currentTime) >= module.moduleRunTime) {
                module.lastRunTime = currentTime;
                module.nextRunTime = currentTime + module.modulePeriod;
                // todo add delta to nextruntime to take into account runs
                // at end of loop

                module.module->entry();
            }

            // Check if we missed a module X times in a row
            if ((int32_t)(currentTime - module.nextRunTime) > MAX_MISS_CNT*module.moduleRunTime) {
                // missed
                led.missedModuleRun();
            }
        }

        uint32_t elapsed = HAL_GetTick() - loopStartTime;
        if (elapsed < SUPER_LOOP_PERIOD) {
            HAL_Delay(SUPER_LOOP_PERIOD - elapsed);
        } else {
            // danger
            led.missedSuperLoop();
        }
        
    }
}