#include <cmath>

#include "constants.hpp"
#include "util/sout.hpp"
#include "sensor/bmp388.hpp"

Adafruit_BMP3XX bmp;

namespace bmp388 {
    void setup() {
        // Hardware I2C mode
        if (!bmp.begin_I2C()) {
            sout << "Could not find a valid BMP3 sensor, check wiring!" << std::endl;
            while (true);
        }

        // Set up oversampling and filter initialization
        bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    }

    // Adapted from readAltitude
    double calc_altitude(double pressure) {
        // Equation taken from BMP180 datasheet (page 16):
        //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

        // Note that using the equation from wikipedia can give bad results
        // at high altitude. See this thread for more information:
        //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

        float atmospheric = pressure / 100.0F;
        return 44330.0 * (1.0 - pow(atmospheric / SEALEVELPRESSURE_HPA, 0.1903));
    }

    // Read from the sensors and return all the results in a struct
    std::optional<AllReadings> read_all() {
        if (!bmp.performReading()) {
            return std::nullopt;
        }

        return {{
            bmp.temperature,
            bmp.pressure,
            calc_altitude(bmp.pressure),
        }};
    }
}
