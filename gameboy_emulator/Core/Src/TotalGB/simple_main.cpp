#define BOARD MIDGARD
#define YGGDRASIL_PERIPHERAL_DEFS
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
	// colours might be inverted, idk if its dark -> light or reversed.
	std::array<uint32_t, 0x100> palette_array = { 0xFFF7E7C6, 0xFFD68E49, 0xFFA63725, 0xFF331E50, 0xFF000000 };
	//std::array<uint32_t, 0x100> palette_array = { 0xFF000000, 0xFF00004E, 0xFF00009C, 0xFF0000EA, 0xFF002400, 0xFF00244E, 0xFF00249C, 0xFF0024EA, 0xFF004800, 0xFF00484E, 0xFF00489C, 0xFF0048EA, 0xFF006C00, 0xFF006C4E, 0xFF006C9C, 0xFF006CEA, 0xFF009000, 0xFF00904E, 0xFF00909C, 0xFF0090EA, 0xFF00B400, 0xFF00B44E, 0xFF00B49C, 0xFF00B4EA, 0xFF00D800, 0xFF00D84E, 0xFF00D89C, 0xFF00D8EA, 0xFF00FC00, 0xFF00FC4E, 0xFF00FC9C, 0xFF00FCEA, 0xFF240000, 0xFF24004E, 0xFF24009C, 0xFF2400EA, 0xFF242400, 0xFF24244E, 0xFF24249C, 0xFF2424EA, 0xFF244800, 0xFF24484E, 0xFF24489C, 0xFF2448EA, 0xFF246C00, 0xFF246C4E, 0xFF246C9C, 0xFF246CEA, 0xFF249000, 0xFF24904E, 0xFF24909C, 0xFF2490EA, 0xFF24B400, 0xFF24B44E, 0xFF24B49C, 0xFF24B4EA, 0xFF24D800, 0xFF24D84E, 0xFF24D89C, 0xFF24D8EA, 0xFF24FC00, 0xFF24FC4E, 0xFF24FC9C, 0xFF24FCEA, 0xFF480000, 0xFF48004E, 0xFF48009C, 0xFF4800EA, 0xFF482400, 0xFF48244E, 0xFF48249C, 0xFF4824EA, 0xFF484800, 0xFF48484E, 0xFF48489C, 0xFF4848EA, 0xFF486C00, 0xFF486C4E, 0xFF486C9C, 0xFF486CEA, 0xFF489000, 0xFF48904E, 0xFF48909C, 0xFF4890EA, 0xFF48B400, 0xFF48B44E, 0xFF48B49C, 0xFF48B4EA, 0xFF48D800, 0xFF48D84E, 0xFF48D89C, 0xFF48D8EA, 0xFF48FC00, 0xFF48FC4E, 0xFF48FC9C, 0xFF48FCEA, 0xFF6C0000, 0xFF6C004E, 0xFF6C009C, 0xFF6C00EA, 0xFF6C2400, 0xFF6C244E, 0xFF6C249C, 0xFF6C24EA, 0xFF6C4800, 0xFF6C484E, 0xFF6C489C, 0xFF6C48EA, 0xFF6C6C00, 0xFF6C6C4E, 0xFF6C6C9C, 0xFF6C6CEA, 0xFF6C9000, 0xFF6C904E, 0xFF6C909C, 0xFF6C90EA, 0xFF6CB400, 0xFF6CB44E, 0xFF6CB49C, 0xFF6CB4EA, 0xFF6CD800, 0xFF6CD84E, 0xFF6CD89C, 0xFF6CD8EA, 0xFF6CFC00, 0xFF6CFC4E, 0xFF6CFC9C, 0xFF6CFCEA, 0xFF900000, 0xFF90004E, 0xFF90009C, 0xFF9000EA, 0xFF902400, 0xFF90244E, 0xFF90249C, 0xFF9024EA, 0xFF904800, 0xFF90484E, 0xFF90489C, 0xFF9048EA, 0xFF906C00, 0xFF906C4E, 0xFF906C9C, 0xFF906CEA, 0xFF909000, 0xFF90904E, 0xFF90909C, 0xFF9090EA, 0xFF90B400, 0xFF90B44E, 0xFF90B49C, 0xFF90B4EA, 0xFF90D800, 0xFF90D84E, 0xFF90D89C, 0xFF90D8EA, 0xFF90FC00, 0xFF90FC4E, 0xFF90FC9C, 0xFF90FCEA, 0xFFB40000, 0xFFB4004E, 0xFFB4009C, 0xFFB400EA, 0xFFB42400, 0xFFB4244E, 0xFFB4249C, 0xFFB424EA, 0xFFB44800, 0xFFB4484E, 0xFFB4489C, 0xFFB448EA, 0xFFB46C00, 0xFFB46C4E, 0xFFB46C9C, 0xFFB46CEA, 0xFFB49000, 0xFFB4904E, 0xFFB4909C, 0xFFB490EA, 0xFFB4B400, 0xFFB4B44E, 0xFFB4B49C, 0xFFB4B4EA, 0xFFB4D800, 0xFFB4D84E, 0xFFB4D89C, 0xFFB4D8EA, 0xFFB4FC00, 0xFFB4FC4E, 0xFFB4FC9C, 0xFFB4FCEA, 0xFFD80000, 0xFFD8004E, 0xFFD8009C, 0xFFD800EA, 0xFFD82400, 0xFFD8244E, 0xFFD8249C, 0xFFD824EA, 0xFFD84800, 0xFFD8484E, 0xFFD8489C, 0xFFD848EA, 0xFFD86C00, 0xFFD86C4E, 0xFFD86C9C, 0xFFD86CEA, 0xFFD89000, 0xFFD8904E, 0xFFD8909C, 0xFFD890EA, 0xFFD8B400, 0xFFD8B44E, 0xFFD8B49C, 0xFFD8B4EA, 0xFFD8D800, 0xFFD8D84E, 0xFFD8D89C, 0xFFD8D8EA, 0xFFD8FC00, 0xFFD8FC4E, 0xFFD8FC9C, 0xFFD8FCEA, 0xFFFC0000, 0xFFFC004E, 0xFFFC009C, 0xFFFC00EA, 0xFFFC2400, 0xFFFC244E, 0xFFFC249C, 0xFFFC24EA, 0xFFFC4800, 0xFFFC484E, 0xFFFC489C, 0xFFFC48EA, 0xFFFC6C00, 0xFFFC6C4E, 0xFFFC6C9C, 0xFFFC6CEA, 0xFFFC9000, 0xFFFC904E, 0xFFFC909C, 0xFFFC90EA, 0xFFFCB400, 0xFFFCB44E, 0xFFFCB49C, 0xFFFCB4EA, 0xFFFCD800, 0xFFFCD84E, 0xFFFCD89C, 0xFFFCD8EA, 0xFFFCFC00, 0xFFFCFC4E, 0xFFFCFC9C, 0xFFFCFCEA, };
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

    GB_loadrom(&gameboy, data, sizeof(data));

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
