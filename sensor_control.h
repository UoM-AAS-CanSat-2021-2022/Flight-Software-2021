#include <basic_mPU6050.h>

//#include <BMP388_DEV.h>
// BMP388_DEV bmp388;

// variables
// basic_mPU6050<> mpu;

// classes
struct sensor_class {
	// BMP388 data
	float temp;
	float pressure;
	float altitude;

	// MPU data
	//vector acc;
	//vector gyro;
	//vector field;

	/*GPS
	float latitude;
	float longitude;
	int n_satellites;
	*/

	// others
	float voltage = 0;
	void setup() {
		// BMP388 setup
		/*
		bmp388.begin();
		bmp388.set_timestandby(TIME_STANDBY_1280MS);
		bmp388.start_normalconversion();


		*/

		// MPU setup
		// mpu.setup();
		// mpu.set_bias();
	}

	void read() {
		if (config.SIMULATION_MODE == SimState::SIMULATION_ACTIVATE) {
			altitude = config.SIMULATION_VALUE;
		}

		// BMP388
		/*
		bmp388.get_measurements(temp, pressure, altitude);
		*/

		// MPU
		/*
		println(mpu.temp());
		temp = mpu.temp();
		acc.update(mpu.ax(), mpu.ay(), mpu.az());
		gyro.update(mpu.gx(), mpu.gy(), mpu.gz());
		*/

		// voltage sensor
		voltage = (analog_read(A0) * 5) / 1024;
	}
};
