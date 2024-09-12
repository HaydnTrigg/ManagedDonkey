#include "interface/user_interface_main_menu_music.hpp"

c_user_interface_main_menu_music::c_user_interface_main_menu_music() :
	__unknown0(0),
	m_state((e_music_state)0),
	m_game_shell_music_state(0),
	__unknownC(0),
	m_looping_sound_index(NONE),
	__unknown14(0),
	__unknown18(0)
{
	DECLFUNC(0x00AD54F0, c_user_interface_main_menu_music*, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::change_state(c_user_interface_main_menu_music::e_music_state)
{
	DECLFUNC(0x00AD5520, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::update_state()
{
	DECLFUNC(0x00AD5530, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

real c_user_interface_main_menu_music::fade_out_progress()
{
	return (real)DECLFUNC(0x00AD55E0, double, __thiscall, c_user_interface_main_menu_music*)(this);
}

//.text:00AD5690 ; 

long c_user_interface_main_menu_music::get_music_index()
{
	return DECLFUNC(0x00AD56C0, long, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::handle_music_for_loading_screen()
{
	DECLFUNC(0x00AD56E0, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

bool c_user_interface_main_menu_music::music_done_fading_out()
{
	return DECLFUNC(0x00AD5790, bool, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::render()
{
	DECLFUNC(0x00AD57E0, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::start()
{
	DECLFUNC(0x00AD5820, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::stop()
{
	DECLFUNC(0x00AD5850, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::update()
{
	DECLFUNC(0x00AD5880, void, __thiscall, c_user_interface_main_menu_music*)(this);
}

void c_user_interface_main_menu_music::update_game_shell_music_state()
{
	DECLFUNC(0x00AD58F0, void, __thiscall, c_user_interface_main_menu_music*)(this);
}


