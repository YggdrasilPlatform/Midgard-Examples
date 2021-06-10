#include <yggdrasil.h>

#define RGBA8(r, g, b) (u32)(((0xFF) << 24) | ((b) << 16) | ((g) << 8) | (r))

extern "C" u16 calculateFractalPoint(double cx, double cy, double juliaX, double juliaY);

extern "C" void mandelbrot_main() {

	bsp::Display::init();
	bsp::Display::turnOn();
	bsp::Display::clear(0);

	constexpr bsp::drv::Palette FractalPalette = {
			RGBA8(0, 0, 0), RGBA8(25, 7, 26), RGBA8(9, 1, 47), RGBA8(4, 4, 73),
			RGBA8(0, 7, 100), RGBA8(12, 44, 138), RGBA8(24, 82, 177), RGBA8(25, 128, 209),
			RGBA8(134, 181, 229), RGBA8(211, 236, 248), RGBA8(241, 233, 191), RGBA8(248, 201, 95),
			RGBA8(255, 170, 0), RGBA8(204, 128, 0), RGBA8(153, 87, 0), RGBA8(106, 52, 3)
	};
	bsp::Display::setPalette(FractalPalette);


	u8 *Framebuffer = static_cast<u8*>(bsp::Display::getFramebufferAddress());
	auto Width = bsp::Display::getWidth();
	auto Height = bsp::Display::getHeight();

	bool juliaMode = false;
	bool juliaPosMode = false;

	double delta = 2.0F/Width;
	double cx = -1;
	double cy = -1;
	double juliaX = 0;
	double juliaY = 0;

	bsp::ygg::prph::Joystick::setDeadzone(20);

	bool lastButtonLeft = false;
	bool lastButtonRight = false;

	while (true) {
		auto [posLeft, magLeft, buttonLeft] = bsp::ygg::prph::Joystick::getLeft();
		auto [posRight, magRight, buttonRight] = bsp::ygg::prph::Joystick::getRight();

		auto [lx, ly] = posLeft;
		auto [rx, ry] = posRight;

		if (buttonLeft && !lastButtonLeft)
			juliaMode = !juliaMode;

		if (buttonRight && !lastButtonRight && juliaMode)
			juliaPosMode = !juliaPosMode;

		bsp::LDA = juliaMode;
		bsp::LDB = juliaPosMode;

		delta *= (1 + ry * 0.001F);

		if (juliaPosMode) {
			juliaX += ly * delta;
			juliaY += lx * delta;
		} else {
			cx += ly * delta;
			cy += lx * delta;
		}

		if (bsp::ButtonA) {
			juliaMode = false;
			juliaPosMode = false;

			delta = 2.0F/Width;
			cx = -1;
			cy = -1;
			juliaX = 0;
			juliaY = 0;
		}


		for (uint16_t x = 0; x < Width; x += 2) {
			for (uint16_t y = 0; y < Height; y += 2) {
				double calcX = cx + x * delta;
				double calcY = cy + y * delta;

				if (!juliaMode)						/* Calculation for mandelbrotset*/
					Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -calcY, calcX) % 16;
				else								/* Calculation for juliaset*/
					Framebuffer[x + y * Width] = calculateFractalPoint(-calcY, calcX, -juliaY, juliaX) % 16;
			}
		}

		lastButtonLeft = buttonLeft;
		lastButtonRight = buttonRight;
	}
}
