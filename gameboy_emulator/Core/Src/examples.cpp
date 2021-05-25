#include <stdio.h>

#if 0
#define BOARD MIDGARD
#define YGGDRASIL_PERIPHERAL_DEFS
#include <yggdrasil.h>

#include <array>
#include <cstdio>

extern "C" void example() {

	using namespace bsp;
	using namespace bsp::ygg::prph;

	ColorSensor::init();
	HumiditySensor::init();
	PressureSensor::init();
	SixAxisSensor::init();
	Display::init();
	Encoder.enable();

	std::string buffer(0xFF, 0x00);

	while (true) {
		Display::clear(15);

		Display::drawString(5, 10, "Hardware test", 20, Font24);

		// Map User buttons to user LEDs
		{
			bool btnA = ButtonA;
			bool btnB = ButtonB;
			bool btnC = ButtonC;
			bool btnD = ButtonD;

			LedBlue 	= btnA;
			LedRed 		= btnB;
			LedYellow 	= btnC;
			LedGreen 	= btnD;

			snprintf(buffer.data(), buffer.size(), "Buttons: %d%d%d%d", btnA, btnB, btnC, btnD);
			Display::drawString(10, 50 + 17 * 0, buffer.c_str(), 255, Font16);
		}


		// Print Potentiometer value and output the same voltage on the DAC A pin
		{
			float value = ADCPotentiometer;

			DACA = value;

			snprintf(buffer.data(), buffer.size(), "Potentiometer: %f", value);
			Display::drawString(10, 50 + 17 * 1, buffer.c_str(), 255, Font16);
		}

		// Drive a dedicated pin with up to 2A
		{
			DriverD = true;
		}

		// Read encoder value
		{
			auto direction = Encoder.getDirection();
			auto count = Encoder.getCount();

			snprintf(buffer.data(), buffer.size(), "Encoder: %d %d", enumValue(direction), count);
			Display::drawString(10, 50 + 17 * 2, buffer.c_str(), 255, Font16);
		}

		// Read random data and calculate crc32 hash from it
		{
			// Read 128 bytes of random data
			auto randomData = Random::get<std::array<u8, 128>>();

			// Calculate CRC32 with CRC-32 (ethernet) settings
			auto crc32 = Hash::getCRC32(randomData);

			snprintf(buffer.data(), buffer.size(), "Entropy: %02X %02X %02X %02X %02X ...", randomData[0], randomData[1], randomData[2], randomData[3], randomData[4]);
			Display::drawString(10, 50 + 17 * 3, buffer.c_str(), 255, Font16);

			snprintf(buffer.data(), buffer.size(), "CRC32 of Random data: %08lX", crc32);
			Display::drawString(10, 50 + 17 * 4, buffer.c_str(), 255, Font16);
		}

		// Get current color sensor value
		{
			auto color = ColorSensor::getColor8();
			snprintf(buffer.data(), buffer.size(), "R: %02X, G: %02X, B: %02X, A: %02X", color.r, color.g, color.b, color.a);
			Display::drawString(10, 50 + 17 * 5, buffer.c_str(), 255, Font16);
		}

		// Get current Humidity and Temperature
		{
			auto [humidity, temperature] = HumiditySensor::getSensorData();
			snprintf(buffer.data(), buffer.size(), "%f%% @ %f C", humidity, temperature);
			Display::drawString(10, 50 + 17 * 6, buffer.c_str(), 255, Font16);
		}

		// Get current atmospheric pressure
		{
			auto pressure = PressureSensor::getPressure();
			snprintf(buffer.data(), buffer.size(), "%fhPa", pressure);
			Display::drawString(10, 50 + 17 * 7, buffer.c_str(), 255, Font16);
		}

		// Get joystick position
		{
			auto [lx, ly] = Joystick::getLeft().pos;
			auto [rx, ry] = Joystick::getRight().pos;

			snprintf(buffer.data(), buffer.size(), "Joystick: L [%d, %d]  R [%d, %d]", lx, ly, rx, ry);
			Display::drawString(10, 50 + 17 * 8, buffer.c_str(), 255, Font16);
		}

		// Print the current time
		{
			// Get time
			time_t time = RealTimeClock::getTime();


			struct tm * timeinfo;
			timeinfo = gmtime(&time);
			strftime(buffer.data(), buffer.size(), "%b %d %H:%M:%S", timeinfo);
			Display::drawString(10, 50 + 17 * 9, buffer.c_str(), 255, Font16);
		}

		// Get current orientation of Yggdrasil
		{
			auto [roll, pitch] = SixAxisSensor::getBoardOrientation();

			snprintf(buffer.data(), buffer.size(), "Roll: %f, Pitch: %f", roll, pitch);
			Display::drawString(10, 50 + 17 * 10, buffer.c_str(), 255, Font16);
		}

		core::delay(1000);
	}

}
#endif
