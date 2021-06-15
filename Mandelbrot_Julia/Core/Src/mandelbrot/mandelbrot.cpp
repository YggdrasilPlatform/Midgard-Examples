#include <yggdrasil.h>

#define RGBA8(r, g, b) (u32)(((0xFF) << 24) | ((b) << 16) | ((g) << 8) | (r))

extern "C" u16 calculateFractalPoint(double cx, double cy, double juliaX, double juliaY);

struct PaletteInformation {
	const bsp::drv::Palette * palette;
	size_t size;
};

extern "C" void mandelbrot_main() {

	bsp::Display::init();
	bsp::Display::turnOn();
	bsp::Display::clear(0);

	constexpr bsp::drv::Palette FractalPalette1 = {
			RGBA8(0, 0, 0), RGBA8(25, 7, 26), RGBA8(9, 1, 47), RGBA8(4, 4, 73),
			RGBA8(0, 7, 100), RGBA8(12, 44, 138), RGBA8(24, 82, 177), RGBA8(25, 128, 209),
			RGBA8(134, 181, 229), RGBA8(211, 236, 248), RGBA8(241, 233, 191), RGBA8(248, 201, 95),
			RGBA8(255, 170, 0), RGBA8(204, 128, 0), RGBA8(153, 87, 0), RGBA8(106, 52, 3)
	};


	auto FractalPalette2 = [] {
		bsp::drv::Palette result;
		u8 r = 255, g = 0, b = 0;
		result[0] = RGBA8(0, 0, 0);

		for(u8 i = 1; i <= 60; i++) {
			if (i <= 10) {
				r = 255;
				g += 25;
				b = 0;
			}
			else if ((i > 10) && (i <= 20)) {
				r -= 25;
				g = 255;
				b = 0;
			}
			else if ((i > 20) && (i <= 30)) {
				r = 0;
				g = 255;
				b += 25;
			}
			else if ((i > 30) && (i <= 40)) {
				r = 0;
				g -= 25;
				b = 255;
			}
			else if ((i > 40) && (i <= 50)) {
				r += 25;
				g = 0;
				b = 255;
			}
			else if ((i > 50) && (i <= 60)) {
				r = 255;
				g = 0;
				b -= 25;
			}

			result[i] = RGBA8(r, g, b);
		}
		result[61] = RGBA8(75, 75, 75);
		result[62] = RGBA8(127, 127, 127);
		result[63] = RGBA8(255, 255, 255);

		return result;
	}();

	std::array palettes = {PaletteInformation{&FractalPalette1, 16}, PaletteInformation{&FractalPalette2, 64}};
	u8 selectedPalette = 0;

	bsp::Display::setPalette(FractalPalette1);

	u8 *Framebuffer = static_cast<u8*>(bsp::Display::getFramebufferAddress());
	auto Width = bsp::Display::getWidth();
	auto Height = bsp::Display::getHeight();

	// Mode variables
	bool highQualityMode = false;
	bool orientationMode = false;
	bool juliaMode = false;
	bool juliaPosMode = false;

	double delta = 2.0F/Width;
	double cx = -1;
	double cy = -1;
	double juliaX = 0;
	double juliaY = 0;

	bsp::ygg::prph::Joystick::setDeadzone(20);

	bsp::ygg::prph::SixAxisSensor::init();

	bsp::Encoder.init();
	bsp::Encoder.enable();
	bsp::Encoder.setCount(10000);	// Preset the encoder count to avoid overflows
	int encCount = 0;				// Encoder value since the last rendering

	bool lastButtonLeft = false;
	bool lastButtonRight = false;

	bsp::LDA = false;
	bsp::LDB = false;

	std::string passedTime;

	while (true) {
		auto [posLeft, magLeft, buttonLeft] = bsp::ygg::prph::Joystick::getLeft();
		auto [posRight, magRight, buttonRight] = bsp::ygg::prph::Joystick::getRight();

		// Read the board orientation
		auto orientation = bsp::ygg::prph::SixAxisSensor::getBoardOrientation();


		auto [lx, ly] = posLeft;
		auto [rx, ry] = posRight;

		if (buttonLeft && !lastButtonLeft)
			juliaMode = !juliaMode;

		if (buttonRight && !lastButtonRight && juliaMode)
			juliaPosMode = !juliaPosMode;

		bsp::LDC = juliaMode;
		bsp::LDD = juliaPosMode;


		encCount = bsp::Encoder.getCount();				// Read the encoder
		bsp::Encoder.setCount(10000);					// Reset the encoder to avoid overflows
		delta *= (1 + (encCount - 10000) * 0.005F);		// Calculate delta from the difference


		if (bsp::EncoderButton) {
			juliaMode = false;
			juliaPosMode = false;

			delta = 2.0F/Width;
			cx = -1;
			cy = -1;
			juliaX = 0;
			juliaY = 0;
		}

		if (bsp::ButtonA) {
			highQualityMode = !highQualityMode;				// Toggle display mode
			bsp::LDA = highQualityMode;						// Set the indicator led
			if (!highQualityMode) bsp::Display::clear(0);	// Clear the screen when goign to low quality, otherwise the unused pixel wont be black
			while(bsp::ButtonA);							// Wait until the button is released
		}


		if (bsp::ButtonB) {
			orientationMode = !orientationMode;				// Toggle orientation mode
			bsp::LDB = orientationMode;						// Set the indicator led
			while(bsp::ButtonB);							// Wait until the button is released
		}

		if (bsp::ButtonC) {
			selectedPalette++;
			selectedPalette %= palettes.size();
			bsp::Display::setPalette(*palettes[selectedPalette].palette);
			while(bsp::ButtonC);
		}

		if (orientationMode) {
			cx += orientation.roll * delta * 2;				// Move fractal according to the board orientation
			cy += -orientation.pitch * delta * 2;
		}
		else {
			if (juliaPosMode) {
				juliaX += ly * delta;
				juliaY += lx * delta;
			} else {
				cx += ly * delta;
				cy += lx * delta;
			}
		}

		if (highQualityMode) {
			for (uint16_t x = 0; x < Width; x ++) {			// Draw the fractal in high quality
				for (uint16_t y = 0; y < Height; y ++) {
					double calcX = cx + x * delta;
					double calcY = cy + y * delta;
					if (!juliaMode)						/* Calculation for mandelbrotset*/
						Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -calcY, calcX) % palettes[selectedPalette].size;
					else								/* Calculation for juliaset*/
						Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -juliaY, juliaX) % palettes[selectedPalette].size;
				}
			}
		}
		else {
			for (uint16_t x = 0; x < Width; x += 2) {		// Draw the fractal in low quality using only 50% of all pixels
				for (uint16_t y = 0; y < Height; y += 2) {
					double calcX = cx + x * delta;
					double calcY = cy + y * delta;
					if (!juliaMode)						/* Calculation for mandelbrotset*/
						Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -calcY, calcX) % palettes[selectedPalette].size;
					else								/* Calculation for juliaset*/
						Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -juliaY, juliaX) % palettes[selectedPalette].size;
				}
			}
		}

		lastButtonLeft = buttonLeft;
		lastButtonRight = buttonRight;
	}
}
