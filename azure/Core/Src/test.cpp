#include <yggdrasil.h>

extern "C" void test() {

	bool result = bsp::ygg::prph::PressureSensor::init();
	printf("Success: %d\n", result);


	while (true) {
		auto pressure = bsp::ygg::prph::PressureSensor::getPressure();

		printf("%f\n", pressure);

		//auto time = bsp::ygg::prph::RealTimeClock::getTime();

		//struct tm * timeinfo;
		//timeinfo = gmtime(&time);
		//printf("%s\n", asctime(timeinfo));

		bsp::core::delay(100);
	}

}
