#include "mtrain.hpp"
#include "SPI.hpp"
#include <memory>

#define RD (1 << 7)

int main() {
    std::shared_ptr<SPI> imuSPI = std::make_shared<SPI>(SpiBus::SpiBus2, std::nullopt, 100'000);
    DigitalOut nCs = p18;

    HAL_Delay(100);
    // Initialize
    nCs.write(0);
    imuSPI->transmit(0x10);
    imuSPI->transmit(0b110'11'0'00);
    nCs.write(1);
    HAL_Delay(100);

    while (true) {
        nCs.write(0);

        imuSPI->transmit(RD | 0x1C);
        uint8_t lo = imuSPI->transmitReceive(0);

        imuSPI->transmit(RD | 0x1D);
        uint8_t hi = imuSPI->transmitReceive(0);

        nCs.write(1);

        int result = (short) (lo << 8 | hi);
        // float result_dps = result / 32768.0 * 2000.0;
        printf("Got whoami: %x, %x\r\n", lo, hi); // result * 2000 / 32768);
        HAL_Delay(10);
    }
}