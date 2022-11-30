#include "input/input.hpp"

#include "cseries/cseries.hpp"
#include "input/input_abstraction.hpp"
#include "shell/shell.hpp"

#include <assert.h>

bool __cdecl input_peek_key(key_stroke* key, e_input_type input_type)
{
	return INVOKE(0x005118C0, input_peek_key, key, input_type);
}

bool __cdecl input_has_gamepad(short gamepad_index)
{
	assert(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	return INVOKE(0x00511A40, input_has_gamepad, gamepad_index);
}

unsigned char __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511B60, input_key_frames_down, key_code, input_type);
}

unsigned short __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type)
{
	return INVOKE(0x00511CE0, input_key_msec_down, key_code, input_type);
}

unsigned char __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511DF0, input_mouse_frames_down, mouse_button, input_type);
}

unsigned short __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type)
{
	return INVOKE(0x00511E30, input_mouse_msec_down, mouse_button, input_type);
}

