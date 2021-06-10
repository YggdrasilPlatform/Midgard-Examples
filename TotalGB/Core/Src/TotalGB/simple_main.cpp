#include <yggdrasil.h>
#include <stdio.h>
#include <stdint.h>
#include <array>

#include "mario.h"

extern "C"{

#include "core/gb.h"

#define MY_UNUSED(a) (void)(a)
// same as above, just more clear that it *is* meant
// to be used, just not al implemented yet...
#define STUB(a) MY_UNUSED(a)

using namespace bsp;
using namespace bsp::ygg::prph;

enum { 
	SCREEN_WIDTH = 480,
	SCREEN_HEIGHT = 800,
	SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT,
	SCALE = 2,

	SCREEN_XOFFSET = 0,//(SCREEN_WIDTH - (160 * SCALE)) / 2,
	SCREEN_YOFFSET = 0//(SCREEN_HEIGHT - (144 * SCALE)) / 2,
};


static struct GB_Core gameboy = {0};

// [CORE CALLBACKS] (these are optional)
static void core_on_error(struct GB_Core* gb,
    void* user, struct GB_ErrorData* e
) {
    MY_UNUSED(gb); MY_UNUSED(user);

    switch (e->type) {
        case GB_ERROR_TYPE_UNKNOWN_INSTRUCTION:
            if (e->data.unk_instruction.cb_prefix) {
                printf("[ERROR] UNK Opcode 0xCB%02X\n", e->data.unk_instruction.opcode);
            }
            else {
                printf("[ERROR] UNK Opcode 0x%02X\n", e->data.unk_instruction.opcode);
            }
            break;

        case GB_ERROR_TYPE_INFO:
            printf("[INFO] %s\n", e->data.info.message);
            break;

        case GB_ERROR_TYPE_WARN:
            printf("[WARN] %s\n", e->data.warn.message);
            break;

        case GB_ERROR_TYPE_ERROR:
            printf("[ERROR] %s\n", e->data.error.message);
            break;

        case GB_ERROR_TYPE_UNK:
            printf("[ERROR] Unknown gb error...\n");
            break;
    }
}

// TODO: why am i not passing in a struct from my gb core cb???
// need to change this later on. the user shouldn't have to access
// fields in the struct!!!
static void core_on_vsync(struct GB_Core* gb,
    void* user
) {
    MY_UNUSED(user);

    uint8_t* fb = (uint8_t*)0x20020000;

    for (size_t y = 0; y < 144; ++y) {
        for (size_t x = 0; x < 160; ++x) {
            fb[(x * 3 + 0 + (800 - 160*3)/2) * 480 + y * 3 + 0 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 0 + (800 - 160*3)/2) * 480 + y * 3 + 1 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 0 + (800 - 160*3)/2) * 480 + y * 3 + 2 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 1 + (800 - 160*3)/2) * 480 + y * 3 + 0 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 1 + (800 - 160*3)/2) * 480 + y * 3 + 1 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 1 + (800 - 160*3)/2) * 480 + y * 3 + 2 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 2 + (800 - 160*3)/2) * 480 + y * 3 + 0 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 2 + (800 - 160*3)/2) * 480 + y * 3 + 1 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
            fb[(x * 3 + 2 + (800 - 160*3)/2) * 480 + y * 3 + 2 + (480 - 144*3)/2] = gb->ppu.pixles[y][159 - x];
        }
    }
}

static void poll_input() {
	const auto [lx, ly] = Joystick::getLeft().pos;

	GB_set_buttons(&gameboy, GB_BUTTON_A, ButtonA);
	GB_set_buttons(&gameboy, GB_BUTTON_B, ButtonB);
	GB_set_buttons(&gameboy, GB_BUTTON_SELECT, ButtonC);
	GB_set_buttons(&gameboy, GB_BUTTON_START, ButtonD);
	GB_set_buttons(&gameboy, GB_BUTTON_LEFT, lx < -30);
	GB_set_buttons(&gameboy, GB_BUTTON_RIGHT, lx > +30);
	GB_set_buttons(&gameboy, GB_BUTTON_UP, ly > +30);
	GB_set_buttons(&gameboy, GB_BUTTON_DOWN, ly < -30);
}

int simple_main() {
	bsp::Display::init();
	bsp::Display::turnOn();

	std::array<uint32_t, 0x100> palette_array = { 0xFFF7E7C6, 0xFFD68E49, 0xFFA63725, 0xFF331E50, 0xFF000000 };

	bsp::Display::Impl::setPalette(palette_array);
	bsp::Display::clear(0);

	GB_init(&gameboy);

	// tell core to use time() to update the RTC rather than clock cycles!
	GB_set_rtc_update_config(
        &gameboy, GB_RTC_UPDATE_CONFIG_USE_LOCAL_TIME
    );

    // setup the callbacks we are interested in!
    GB_set_vblank_callback(&gameboy, core_on_vsync, NULL);
    GB_set_error_callback(&gameboy, core_on_error, NULL);

    // todo: load rom here!!!!!!!
    /*
    NOTE: from gb.h
    
    // pass the fully loaded rom data.
	// this memory is NOT owned.
	// freeing the memory should still be handled by the caller!
	bool GB_loadrom(struct GB_Core* gb, const uint8_t* data, size_t size);
	*/

    GB_loadrom(&gameboy, mario_rom_data, sizeof(mario_rom_data));

	// this is big brain stuff (not really)
	// so gb has 4 shades of bad green, and because the device
	// needs to have the fb set to indexes into a platte, well
	// then we just use indexes as colours.
	// this means you can still get fancy SGB and GBC supported GB
	// colours games to work nice by customising the palette.
    struct GB_PaletteEntry palette_entry = {0};
    for (int i = 0; i < 4; ++i) palette_entry.BG[i] = i;
    for (int i = 0; i < 4; ++i) palette_entry.OBJ0[i] = i;
    for (int i = 0; i < 4; ++i) palette_entry.OBJ1[i] = i;

    GB_set_palette_from_palette(&gameboy, &palette_entry);

	for (;;) {
		poll_input();
		GB_run_frame(&gameboy); // no vsync, just go as fast (or slow) as possible
		// todo: render frame?
	}

	return 0;
}

} // extern "C"
