#include "interface/gui_screens/game_browser/gui_game_browser.hpp"

#include "interface/user_interface_memory.hpp"
#include "tag_files/string_ids.hpp"

void load_game_browser(e_controller_index controller, long search_flags, e_browser_type type)
{
	if (c_load_game_browser_screen_message* message = new c_load_game_browser_screen_message(STRING_ID(gui, game_browser), controller, _window_index4, STRING_ID(gui, top_most), search_flags, type))
		user_interface_messaging_post(message);
}

