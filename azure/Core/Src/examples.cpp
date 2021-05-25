#include <stdio.h>

#include <yggdrasil.h>

#include <array>
#include <cstdio>


using namespace bsp;
using namespace bsp::ygg::prph;


enum class Result:u8{
	Failed,
	Passed
};

std::array stringResult = {"Failed", "Passed"};

std::array testOrder = {  "Display",		// 0
						  "BTN / LED A",	// 1
						  "BTN / LED B",	// 2
						  "BTN / LED C",	// 3
						  "BTN / LED D",	// 4
						  "Encoder CW",		// 5
						  "Encoder CCW",	// 6
						  "Encoder BTN",	// 7
						  "Poti CW",		// 8
						  "Poti CCW",		// 9
						  "Joystick L Y+",	// 10
						  "Joystick L Y-",	// 11
						  "Joystick L X+",	// 12
						  "Joystick L X-",	// 13
						  "Joystick L BTN",	// 14
						  "Joystick R Y+",	// 15
						  "Joystick R Y-",	// 16
						  "Joystick R X+",	// 17
						  "Joystick R X-",	// 18
						  "Joystick R BTN",	// 19
						  "7-Segment",		// 20
						  "Color sensor",	// 21
						  "Humidity", 		// 22
						  "Pressure sensor",// 23
						  "Pressure", 		// 24
						  "6-Axis sensor",	// 25
						  "Acceleration",	// 26
						  "RTC Time",		// 27
	};

std::array<Result, 35> testResult;

static void helpScreen() {
	Display::clear(0);
	Display::drawString(5, 10, "Hardware test", 20, Font24);
	Display::drawString(5, 50 + 21 * 0, "The buttons will be used for:", drv::Color::White, Font20);
	Display::drawString(5, 50 + 21 * 1, "Button A: Confirm", drv::Color::White, Font20);
	Display::drawString(5, 50 + 21 * 2, "Button B: Skip", drv::Color::White, Font20);

	Display::drawString(5, 50 + 21 * 4, "Place board on flat surface", drv::Color::Red, Font20);

	Display::drawString(5, 50 + 21 * 6, "Press confirm to start", drv::Color::White, Font20);
	while (!ButtonA);
}

static bool check(u8 testNumber, bool condition) {
	if (ButtonB) {
		testResult[testNumber] = Result::Failed;
		while (!ButtonB);
		core::delay(250);
		return false;
	}
	if (condition) {
		testResult[testNumber] = Result::Passed;
		return false;
	}
	return true;
}

static void autoCheck(u8 testNumber, bool condition) {
	if (condition) {
		testResult[testNumber] = Result::Passed;
	}
	else {
		testResult[testNumber] = Result::Failed;
	}

}

extern "C" void example() {

	std::string buffer(0xFF, 0x00);

	while (true) {
		Display::init();
		testResult[0] = Result::Passed;

		Display::clear(0);
		Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
		Display::drawString(5, 50 + 21 * 0, "Please follow the instructions", drv::Color::White, Font20);

		// Test Button A to D and LED A to D
		{
			Display::drawString(5, 50 + 21 * 1, "[01] Press Button A ", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 2, "     when LED A is on", drv::Color::White, Font20);

			LDA = true;
			while(!ButtonA);
			testResult[1] = Result::Passed;
			LDA = false;

			Display::drawString(5, 50 + 21 * 3, "[02] Press Button B ", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 4, "     when LED B is on", drv::Color::White, Font20);

			LDB = true;
			while(!ButtonB);
			testResult[2] = Result::Passed;
			LDB = false;

			Display::drawString(5, 50 + 21 * 5, "[03] Press Button C ", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 6, "     when LED C is on", drv::Color::White, Font20);

			LDC = true;
			while(!ButtonC);
			testResult[3] = Result::Passed;
			LDC = false;

			Display::drawString(5, 50 + 21 * 7, "[04] Press Button D ", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 8, "     when LED D is on", drv::Color::White, Font20);

			LDD = true;
			while(!ButtonD);
			testResult[4] = Result::Passed;
			LDD = false;
		}

		helpScreen();


		{
			Encoder.enable();
			auto direction = Encoder.getDirection();
			auto count = Encoder.getCount();
			do{
				direction = Encoder.getDirection();
				count = Encoder.getCount();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[05] Turn Encoder clockwise", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Encoder: %d %d", enumValue(direction), count);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			} while(check(5, (count < 500) && (count > 96) && (enumValue(direction) == 0)));
			Encoder.setCount(0);

			do{
				direction = Encoder.getDirection();
				count = Encoder.getCount();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[06] Turn Encoder counterclockwise", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Encoder: %d %d", enumValue(direction), count);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			} while(check(6, (count < 65439) && (count > 65000) && (enumValue(direction) == 1)));
			Encoder.setCount(0);

			Display::clear(0);
			Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
			Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 1, "[07] Press Encoder button", drv::Color::White, Font20);
			do{
				core::delay(100);
			} while(check(7, EncoderButton));
		}


		{
			float value = ADCPotentiometer;

			do{
				value = ADCPotentiometer;

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[08] Turn Poti clockwise", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Potentiometer: %f", value);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			} while(check(8, value < 0.01));

			do{
				value = ADCPotentiometer;

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[09] Turn Poti counterclockwise", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Potentiometer: %f", value);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(9, value > 0.99));
		}

		{
			auto data = Joystick::getLeft();

			do{
				data = Joystick::getLeft();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[10] Move left joystick up", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos y: %d / 80", data.pos.y);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(10, data.pos.y >= 80));

			do{
				data = Joystick::getLeft();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[11] Move left joystick down", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos y: %d / -80", data.pos.y);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(11, data.pos.y <= -80));

			do{
				data = Joystick::getLeft();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[12] Move left joystick right", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos x: %d / 80", data.pos.x);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(12, data.pos.x >= 80));

			do{
				data = Joystick::getLeft();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[13] Move left joystick left", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos x: %d / -80", data.pos.x);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(13, data.pos.x <= -80));

			Display::clear(0);
			Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
			Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 1, "[14] Press left joystick", drv::Color::White, Font20);
			do{
				core::delay(100);
			} while(check(14, LeftJoyStickButton));

		}

		{
			auto data = Joystick::getRight();

			do{
				data = Joystick::getRight();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[15] Move right joystick up", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos y: %d / 80", data.pos.y);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(15, data.pos.y >= 80));

			do{
				data = Joystick::getRight();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[16} Move right joystick down", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos y: %d / -80", data.pos.y);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(16, data.pos.y <= -80));

			do{
				data = Joystick::getRight();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[17] Move right joystick right", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos x: %d / 80", data.pos.x);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(17, data.pos.x >= 80));

			do{
				data = Joystick::getRight();

				Display::clear(0);
				Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
				Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
				Display::drawString(5, 50 + 21 * 1, "[18] Move right joystick left", drv::Color::White, Font20);

				snprintf(buffer.data(), buffer.size(), "Joystick pos x: %d / -80", data.pos.x);
				Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);
				core::delay(100);
			}  while(check(18, data.pos.x <= -80));

			Display::clear(0);
			Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
			Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 1, "[19] Press right joystick", drv::Color::White, Font20);
			do{
				core::delay(100);
			} while(check(19, RightJoyStickButton));

		}

		core::delay(1000);

		{
			auto randomData = Random::get<u16>();
			Display::clear(0);
			Display::drawString(5, 10, "Hardware test", drv::Color::Lime, Font24);
			Display::drawString(5, 50 + 21 * 0, "User IO Test", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 1, "[20] 7 Segment", drv::Color::White, Font20);

			snprintf(buffer.data(), buffer.size(), "Value: %04X", randomData);
			Display::drawString(5, 50 + 21 * 2, buffer.c_str(), drv::Color::White, Font20);

			do{
				SevenSegment::setHexadecimal(randomData);
				core::delay(1);
			}  while(check(20, ButtonA));
			SevenSegment::disable();
		}

		{
			Display::clear(0);
			Display::drawString(5, 10, "Hardware test", 20, Font24);
			Display::drawString(5, 50 + 21 * 0, "Sensor Test", drv::Color::White, Font20);
			Display::drawString(5, 50 + 21 * 1, "[21] Color sensor ID", drv::Color::White, Font20);
			autoCheck(21, ColorSensor::init());
			core::delay(250);

			Display::drawString(5, 50 + 21 * 2, "[22] Humidity sensor", drv::Color::White, Font20);
			HumiditySensor::init();
			auto humidity = HumiditySensor::getHumidity();
			snprintf(buffer.data(), buffer.size(), "Humidity: %0.1f%%RH", humidity);
			Display::drawString(5, 50 + 21 * 3, buffer.c_str(), drv::Color::White, Font20);
			autoCheck(22, (humidity > 0) && (humidity <= 100));
			core::delay(250);

			Display::drawString(5, 50 + 21 * 4, "[23] Pressure sensor ID", drv::Color::White, Font20);
			autoCheck(23, PressureSensor::init());
			core::delay(250);

			auto pdata = PressureSensor::getPressure();
			Display::drawString(5, 50 + 21 * 5, "[24] Pressure sensor value", drv::Color::White, Font20);
			snprintf(buffer.data(), buffer.size(), "Pressure: %0.1fhPa", pdata);
			Display::drawString(5, 50 + 21 * 6, buffer.c_str(), drv::Color::White, Font20);
			autoCheck(24, (pdata > 850) && (pdata < 1100));
			core::delay(250);

			Display::drawString(5, 50 + 21 * 7, "[25] 6-Axis sensor ID", drv::Color::White, Font20);
			autoCheck(25, SixAxisSensor::init());
			core::delay(250);

			Display::drawString(5, 50 + 21 * 8, "[26] 6-Axis sensor value", drv::Color::White, Font20);
			auto [x, y, z] = SixAxisSensor::getAcceleration();
			snprintf(buffer.data(), buffer.size(), "Accel: [%0.2f, %0.2f, %0.2f]", x, y, z);
			Display::drawString(5, 50 + 21 * 9, buffer.c_str(), drv::Color::White, Font20);
			autoCheck(26, (std::abs(x) < 0.02) && (std::abs(y) < 0.02) && (std::abs(z) > 0.9));
			core::delay(250);

			tm setTime = {0};
			setTime.tm_year = 2021;
			setTime.tm_hour = 10;
			setTime.tm_min = 30;
			RealTimeClock::setTime(mktime(&setTime));
			core::delay(1000);

			time_t time = RealTimeClock::getTime();
			Display::drawString(5, 50 + 21 * 10, "[27] RTC get time", drv::Color::White, Font20);
			tm * timeInfo;
			timeInfo = gmtime(&time);
			strftime(buffer.data(), buffer.size(), "%H:%M:%S", timeInfo);
			Display::drawString(10, 50 + 21 * 11, buffer.c_str(), drv::Color::White, Font20);
			autoCheck(27, (timeInfo->tm_hour == 10) && (timeInfo->tm_min == 30));
			core::delay(250);
		}

		Display::clear(0);
		Display::drawString(5, 10, "Test report", 20, Font24);

		for(u8 i = 0; i < testOrder.size(); i++){
			snprintf(buffer.data(), buffer.size(), "[%02d] %s", i, stringResult[enumValue(testResult[i])]);
			auto color = drv::Color::Green;
			if(testResult[i] == Result::Failed) color = drv::Color::Red;

			Display::drawString(5, 50 + 21 * i, buffer.c_str(), color, Font20);
			Display::drawString(250, 50 + 21 * i, testOrder[i], drv::Color::White, Font20);


		}

		Display::drawString(250, 50 + 21 * 34, "Press Button A to see the fonts", drv::Color::White, Font20);
		while(!ButtonA);


		Display::clear(0);

		while (true){
			Display::drawString(5, 10, "Black", drv::Color::Black, Font24);
			Display::drawString(5, 10 + 25 * 1, "Navy", drv::Color::Navy, Font24);
			Display::drawString(5, 10 + 25 * 2, "Blue", drv::Color::Blue, Font24);
			Display::drawString(5, 10 + 25 * 3, "Green", drv::Color::Green, Font24);
			Display::drawString(5, 10 + 25 * 4, "Teal", drv::Color::Teal, Font24);
			Display::drawString(5, 10 + 25 * 5, "Lime", drv::Color::Lime, Font24);
			Display::drawString(5, 10 + 25 * 6, "Aqua", drv::Color::Aqua, Font24);
			Display::drawString(5, 10 + 25 * 7, "Maroon", drv::Color::Maroon, Font24);
			Display::drawString(5, 10 + 25 * 8, "Purple", drv::Color::Purple, Font24);
			Display::drawString(5, 10 + 25 * 9, "Olive", drv::Color::Olive, Font24);
			Display::drawString(5, 10 + 25 * 10, "Gray", drv::Color::Gray, Font24);
			Display::drawString(5, 10 + 25 * 11, "Red", drv::Color::Red, Font24);
			Display::drawString(5, 10 + 25 * 12, "Fuchsia", drv::Color::Fuchsia, Font24);
			Display::drawString(5, 10 + 25 * 13, "Yellow", drv::Color::Yellow, Font24);
			Display::drawString(5, 10 + 25 * 14, "Orange", drv::Color::Orange, Font24);
			Display::drawString(5, 10 + 25 * 15, "White", drv::Color::White, Font24);

			Display::drawString(5, 10 + 25 * 17, "Fonts:", drv::Color::White, Font24);
			Display::drawString(5, 10 + 25 * 18, "Font 8", drv::Color::White, Font8);
			Display::drawString(5, 10 + 25 * 19, "Font 12", drv::Color::White, Font12);
			Display::drawString(5, 10 + 25 * 20, "Font 16", drv::Color::White, Font16);
			Display::drawString(5, 10 + 25 * 21, "Font 20", drv::Color::White, Font20);
			Display::drawString(5, 10 + 25 * 22, "Font 24", drv::Color::White, Font24);

		};
	}

}
