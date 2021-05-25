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

	bsp::Encoder.init();
	bsp::Encoder.enable();

	double delta = 2.0F/Width;
	double cx = -1;
	double cy = -1;
	double juliaX = 0;
	double juliaY = 0;

	bsp::ygg::prph::Joystick::setDeadzone(20);

	while (true) {
		auto [lx, ly] = bsp::ygg::prph::Joystick::getLeft().pos;
		auto [rx, ry] = bsp::ygg::prph::Joystick::getRight().pos;

		delta *= (1 + ry * 0.001F);

		cx += ly * delta;
		cy += lx * delta;

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
	}
}