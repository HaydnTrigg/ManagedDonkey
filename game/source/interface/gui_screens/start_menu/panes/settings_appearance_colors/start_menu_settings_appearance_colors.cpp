#include "interface/gui_screens/start_menu/panes/settings_appearance_colors/start_menu_settings_appearance_colors.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

//HOOK_DECLARE_CLASS_MEMBER(0x00AF9A30, c_start_menu_settings_appearance_colors, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AF9C20, c_start_menu_settings_appearance_colors, initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00AF9F70, c_start_menu_settings_appearance_colors, set_color_values_from_profile);

bool __cdecl parse_xml_armor1(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9D90, parse_xml_armor1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor1));
}

bool __cdecl parse_xml_armor2(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9DB0, parse_xml_armor2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor2));
}

bool __cdecl parse_xml_armor3(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_armor3));

	long color_armor3 = string_id_retrieve("color_armor3");
	if (color_armor3 == _string_id_invalid)
		return false;

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, color_armor3);
}

bool __cdecl parse_xml_color(c_gui_screen_widget* screen_widget, wchar_t* buffer, long buffer_length, long name)
{
	return INVOKE(0x00AF9DD0, parse_xml_color, screen_widget, buffer, buffer_length, name);
}

bool __cdecl parse_xml_emblem1(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9EE0, parse_xml_emblem1, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem1));
}

bool __cdecl parse_xml_emblem2(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9F00, parse_xml_emblem2, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem2));
}

bool __cdecl parse_xml_emblem3(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	//return INVOKE(0x00AF9F20, parse_xml_emblem3, this_ptr, buffer, buffer_length);

	return parse_xml_color((c_gui_screen_widget*)this_ptr, buffer, buffer_length, STRING_ID(gui, color_emblem3));
}

//bool __thiscall c_start_menu_settings_appearance_colors::handle_controller_input_message_(c_controller_input_message const* input_message)
//{
//	//return DECLFUNC(0x00AF9A30, bool, __thiscall, c_start_menu_settings_appearance_colors*, c_controller_input_message const*)(this, input_message);
//
//	bool result = false;
//
//	e_event_type event_type = input_message->get_event_type();
//	e_controller_component component = input_message->get_component();
//
//	c_gui_list_widget* focused_parent_list_widget = NULL;
//	if (c_gui_widget* focused_widget = get_focused_widget())
//		focused_parent_list_widget = focused_widget->get_parent_list();
//
//	c_gui_list_widget* group_selector_child_list = get_child_list_widget(STRING_ID(gui, group_selector));
//	if (focused_parent_list_widget && group_selector_child_list)
//	{
//		if (focused_parent_list_widget == group_selector_child_list)
//		{
//			if ((event_type == _event_type_controller_component && component == _controller_component_button_a) || event_type == _event_type_horizontal_navigation4)
//			{
//				c_gui_list_item_widget* parent_list_item = NULL;
//				if (c_gui_widget* focused_widget = get_focused_widget())
//					parent_list_item = focused_widget->get_parent_list_item();
//
//				long parent_list_item_name = parent_list_item->m_name.get_value();
//				if (c_gui_list_widget* child_list = get_child_list_widget(parent_list_item_name))
//				{
//					long focused_item_index = child_list->get_focused_item_index();
//					for (c_gui_list_item_widget* child_list_item = (c_gui_list_item_widget*)child_list->get_first_child_widget_by_type(_gui_widget_type_list_item);
//						child_list_item;
//						child_list_item = get_next_list_item_widget(true))
//					{
//						if (focused_item_index == child_list_item->get_list_item_index())
//						{
//							transfer_focus(child_list_item);
//							result = true;
//						}
//					}
//				}
//
//			}
//			else if (event_type == _event_type_controller_component && component == _controller_component_button_b)
//			{
//				close_current_subpane();
//				return true;
//			}
//		}
//		else if (event_type == _event_type_controller_component && (component == _controller_component_button_a || component == _controller_component_button_b))
//		{
//			long parent_list_name = focused_parent_list_widget->m_name.get_value();
//			for (c_gui_list_item_widget* child_list_item = (c_gui_list_item_widget*)group_selector_child_list->get_first_child_widget_by_type(_gui_widget_type_list_item);
//				child_list_item;
//				child_list_item = get_next_list_item_widget(true))
//			{
//				if (child_list_item->m_name.get_value() == parent_list_name)
//				{
//					group_selector_child_list->set_focused_item_index(child_list_item->get_list_item_index(), false);
//					transfer_focus(child_list_item);
//					return true;
//				}
//			}
//
//			if (component != _controller_component_button_a)
//			{
//				set_color_values_from_profile();
//			}
//			else
//			{
//				e_controller_index controller_index = get_any_responding_controller();
//				c_player_profile_interface* player_profile = controller_get(controller_index)->get_player_profile_interface();
//				long focused_item_index = focused_parent_list_widget->get_focused_item_index();
//
//				s_emblem_info emblem_info = player_profile->m_emblem_info;
//				switch (parent_list_name)
//				{
//				case STRING_ID(gui, color_armor1):
//					player_profile->set_primary_change_color(focused_item_index, true);
//					return true;
//				case STRING_ID(gui, color_armor2):
//					player_profile->set_secondary_change_color(focused_item_index, true);
//					return true;
//				//case STRING_ID(gui, color_armor3):
//				//	//player_profile->set_tertiary_change_color(focused_item_index, true);
//				//	return true;
//				case STRING_ID(gui, color_emblem1):
//					//emblem_info.emblem_primary_color = focused_item_index;
//					//player_profile->set_emblem_info(&emblem_info, true);
//					emblem_info.emblem_primary_color = focused_item_index;
//					player_profile->m_emblem_info = emblem_info;
//					return true;
//				case STRING_ID(gui, color_emblem2):
//					//emblem_info.emblem_secondary_color = focused_item_index;
//					//player_profile->set_emblem_info(&emblem_info, true);
//					emblem_info.emblem_secondary_color = focused_item_index;
//					player_profile->m_emblem_info = emblem_info;
//					return true;
//				case STRING_ID(gui, color_emblem3):
//					//emblem_info.emblem_background_color = focused_item_index;
//					//player_profile->set_emblem_info(&emblem_info, true);
//					emblem_info.emblem_background_color = focused_item_index;
//					player_profile->m_emblem_info = emblem_info;
//					return true;
//				default:
//				{
//					if (parent_list_name == string_id_retrieve("color_armor3"))
//					{
//						//player_profile->set_tertiary_change_color(focused_item_index, true);
//						return true;
//					}
//
//					return true;
//				}
//				break;
//				}
//
//				return true;
//			}
//		}
//	}
//
//	if (event_type == _event_type_controller_component)
//	{
//		e_controller_index controller_index = input_message->get_controller();
//		switch (component)
//		{
//		case _controller_component_button_x:
//			//player_emblem_toggle_alternate_emblem_foreground(controller_index);
//			return true;
//		case _controller_component_button_left_stick:
//			//player_emblem_toggle_flip_emblem_foreground(controller_index);
//			return true;
//		case _controller_component_button_right_stick:
//			//player_emblem_toggle_flip_emblem_background(controller_index);
//			return true;
//		}
//	}
//
//	if (!result)
//	{
//		//result = c_start_menu_pane_screen_widget::handle_controller_input_message(input_message);
//		result = DECLFUNC(0x00B1F620, bool, __thiscall, c_start_menu_pane_screen_widget*, c_controller_input_message const*)(this, input_message);
//	}
//
//	return result;
//}

void __thiscall c_start_menu_settings_appearance_colors::initialize_()
{
	//DECLFUNC(0x00AF9C20, void, __thiscall, c_start_menu_settings_appearance_colors*)(this);

	DECLFUNC(0x00AB14D0, void, __thiscall, c_gui_screen_widget*)(this);
	//c_gui_screen_widget::initialize();

	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor1", this, parse_xml_armor1));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor2", this, parse_xml_armor2));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-armor3", this, parse_xml_armor3));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem1", this, parse_xml_emblem1));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem2", this, parse_xml_emblem2));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<color-emblem3", this, parse_xml_emblem3));
}

void c_start_menu_settings_appearance_colors::set_color_focused_list_item(long name, e_player_color_index player_color_index)
{
	DECLFUNC(0x00AF9F50, void, __thiscall, c_start_menu_settings_appearance_colors*, long, e_player_color_index)(this, name, player_color_index);

	//if (c_gui_list_widget* child_list_widget = get_child_list_widget(name))
	//	child_list_widget->set_focused_item_index(player_color_index, false);
}

void __thiscall c_start_menu_settings_appearance_colors::set_color_values_from_profile()
{
	//DECLFUNC(0x00AF9F70, void, __thiscall, c_start_menu_settings_appearance_colors*)(this);

	e_controller_index controller_index = get_any_responding_controller();
	if (controller_get(controller_index)->in_use())
	{
		c_player_profile_interface* player_profile = controller_get(controller_index)->get_player_profile_interface();
		set_color_focused_list_item(STRING_ID(gui, color_armor1), player_profile->get_primary_change_color());
		set_color_focused_list_item(STRING_ID(gui, color_armor2), player_profile->get_secondary_change_color());
		//set_color_focused_list_item(STRING_ID(gui, color_armor3), player_profile->get_tertiary_change_color());
		//set_color_focused_list_item(string_id_retrieve("color_armor3"), player_profile->get_tertiary_change_color());
		
		//s_emblem_info emblem_info = player_profile->get_emblem_info();
		s_emblem_info emblem_info = player_profile->m_emblem_info;
		set_color_focused_list_item(STRING_ID(gui, color_emblem1), emblem_info.emblem_primary_color);
		set_color_focused_list_item(STRING_ID(gui, color_emblem2), emblem_info.emblem_secondary_color);
		set_color_focused_list_item(STRING_ID(gui, color_emblem3), emblem_info.emblem_background_color);
	}
}
