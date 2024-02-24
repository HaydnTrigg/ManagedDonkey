#pragma once

#include "game/players.hpp"
#include "interface/user_interface_messages.hpp"

struct c_load_boot_betrayer_screen_message :
	public c_load_screen_message
{
public:
	c_load_boot_betrayer_screen_message(e_controller_index controller, e_window_index window, long layered_position, s_player_identifier const* current_player_id, s_player_identifier const* target_player_id);
	virtual ~c_load_boot_betrayer_screen_message();

protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
};
static_assert(sizeof(c_load_boot_betrayer_screen_message) == sizeof(c_load_screen_message) + 0x10);

