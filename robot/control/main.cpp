#include "mtrain.hpp"
#include "iodefs.h"
#include "SPI.hpp"
#include <vector>
#include <optional>

#include "drivers/LSM9DS1.hpp"
#include "bsp.h"

#include "MicroPackets.hpp"

DebugInfo debugInfo;

int main() {
    std::shared_ptr<SPI> sharedSPI = std::make_shared<SPI>(DOT_STAR_SPI_BUS, std::nullopt, 100'000);
    LSM9DS1 lsm(sharedSPI, IMU_CS, IMU_CS2);
    lsm.initialize();

    while (true) {
        lsm.read_gyr();
        printf("Got value: %d\n", (int) lsm.gyro_z());
    }
}
