#include "interface/debug_menu/debug_menu_item_numbered.hpp"

#include "cseries/cseries.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "text/draw_string.hpp"

c_debug_menu_item_numbered::~c_debug_menu_item_numbered()
{
}

void c_debug_menu_item_numbered::render(c_font_cache_base* font_cache, int16_point2d const& point)
{
	render_number(font_cache, point);

	int16_point2d next_point{};
	set_point2d(&next_point, point.x + get_indent(), point.y);
	c_debug_menu_item::render(font_cache, next_point);
}

c_debug_menu_item_numbered::c_debug_menu_item_numbered(c_debug_menu* menu, const char* name, struct c_debug_menu* child) :
	c_debug_menu_item(menu, name, child, true)
{
}

void c_debug_menu_item_numbered::render_number(c_font_cache_base* font_cache, int16_point2d const& point)
{
	char string[64]{};

	short index = get_index();
	bool is_number = index < 10;
	bool is_letter = index >= 10 && index < 36;

	c_rasterizer_draw_string draw_string{};

	short_rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	ASSERT(!(is_number && is_letter));

	if (is_number)
	{
		csnzprintf(string, sizeof(string), "   %s%d", index < 9 ? " " : "", get_index() + 1);
	}
	else if (is_letter)
	{
		csnzprintf(string, sizeof(string), "(%c)%d", index + 87, get_index() + 1);
	}
	else
	{
		csnzprintf(string, sizeof(string), "   %d", index + 1);
	}

	set_rectangle2d(&bounds, point.x - 3, point.y, bounds.x1, bounds.y1);

	real_argb_color const* color = global_real_argb_black;
	if (get_active())
		color = get_enabled_color();

	draw_string.set_color(color);
	draw_string.set_bounds(&bounds);
	draw_string.draw(font_cache, string);
}

short c_debug_menu_item_numbered::get_indent()
{
	return 45;
}

void c_debug_menu_item_type::render(c_font_cache_base* font_cache, int16_point2d const& point)
{
}

void c_debug_menu_item_type::to_string(char* buffer, long buffer_size)
{
}

void c_debug_menu_item_type::render_value(c_font_cache_base* font_cache, int16_point2d const& point)
{
}

c_debug_menu_item_type::c_debug_menu_item_type(c_debug_menu* menu, const char* name, bool readonly) :
	c_debug_menu_item_numbered(menu, name, NULL),
	m_readonly(readonly)
{
}

bool c_debug_menu_item_type::get_readonly()
{
	return m_readonly;
}

template<typename t_type>
inline c_debug_menu_value_hs_global_external<t_type>::c_debug_menu_value_hs_global_external(char const* hs_global_name)
{
	ASSERT(hs_global_name);

	m_hs_global_external_index = NONE;
	for (short hs_global_external_index = 0; hs_global_external_index < k_console_global_count && m_hs_global_external_index == NONE; hs_global_external_index++)
	{
		if (csstricmp(hs_global_name, k_console_globals[hs_global_external_index]->name))
			continue;

		if (!k_console_globals[hs_global_external_index]->pointer)
			continue;

		e_hs_type type = k_console_globals[hs_global_external_index]->type;
		if (IN_RANGE_INCLUSIVE(type, _hs_type_boolean, _hs_type_long_integer))
			m_hs_global_external_index = hs_global_external_index;
	}
}

template<typename t_type>
inline t_type c_debug_menu_value_hs_global_external<t_type>::get()
{
	byte bytes[sizeof(t_type)];
	csmemset(bytes, 0, sizeof(t_type));

	if (m_hs_global_external_index != NONE)
	{
		ASSERT(k_console_globals[m_hs_global_external_index]->pointer != NULL);

		*reinterpret_cast<t_type*>(bytes) = *static_cast<t_type*>(k_console_globals[m_hs_global_external_index]->pointer);
	}

	return *reinterpret_cast<t_type*>(bytes);
}
