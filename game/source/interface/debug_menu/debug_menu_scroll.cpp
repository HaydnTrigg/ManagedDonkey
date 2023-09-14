#include "interface/debug_menu/debug_menu_scroll.hpp"

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "interface/interface_constants.hpp"
#include "text/draw_string.hpp"

c_debug_menu_scroll::~c_debug_menu_scroll()
{
	c_debug_menu::~c_debug_menu();
}

void c_debug_menu_scroll::update()
{
	c_debug_menu::update();

	if (get_selection() + 1 >= get_first() + get_num_visible())
	{
		short v3 = get_num_items();
		if (v3 <= get_num_visible())
			v3 = get_num_visible();

		if (get_first() + 1 >= v3 - get_num_visible())
		{
			short v5 = get_num_items();
			if (v5 <= get_num_visible())
				v5 = get_num_visible();

			set_first(v5 - get_num_visible());
		}
		else
			set_first(get_first() + 1);
	}

	if (get_selection() - 1 < get_first())
	{
		if (get_first() - 1 <= 0)
			set_first(0);
		else
			set_first(get_first() - 1);
	}
}

void c_debug_menu_scroll::render(c_font_cache_base* font_cache, int16_point2d* point)
{
	//render_background(font_cache, point);
	//render_title(font_cache, point);
	//render_caption(font_cache, point);
	//render_global_caption(font_cache, point);
	//if (get_num_visible() > 0)
	//	render_items(font_cache, point, get_first() /* start_index */, get_first() + get_num_visible() - 1 /* end_index */);

	c_rasterizer_draw_string draw_string{};

	short_rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);

	draw_string.set_color(debug_real_argb_tv_magenta);
	if (get_first() > 0)
	{
		set_rectangle2d(&bounds, point->x, point->y + get_title_height(), bounds.x1, bounds.y1);

		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "^");

		bounds.y0 += get_item_height();
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "|");
	}

	if (get_num_items() - get_first() > get_num_visible())
	{
		set_rectangle2d(&bounds, point->x, (point->y + get_title_height()) + (get_num_visible() - 2) * get_item_height(), bounds.x1, bounds.y1);
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "|");

		bounds.y0 += get_item_height();
		draw_string.set_bounds(&bounds);
		draw_string.draw(font_cache, "v");
	}
}

void c_debug_menu_scroll::open()
{
	c_debug_menu::open();
	set_first(0);
}

long c_debug_menu_scroll::get_num_items_to_render()
{
	if (get_num_items() <= get_num_visible())
		return get_num_items();

	return get_num_visible();
}

c_debug_menu_scroll::c_debug_menu_scroll(c_debug_menu* parent, short num_visible, char const* name) :
	c_debug_menu(parent, name)
{
	set_num_visible(num_visible);
	set_first(0);
}

short c_debug_menu_scroll::get_num_visible()
{
	return m_num_visible;
}

short c_debug_menu_scroll::get_first()
{
	return m_first;
}

void c_debug_menu_scroll::set_num_visible(short num_visible)
{
	ASSERT(num_visible >= 0);

	m_num_visible = num_visible;
}

void c_debug_menu_scroll::set_first(short first)
{
	ASSERT(first >= 0 && first <= FLOOR(get_num_items(), get_num_visible()) - get_num_visible());

	m_first = first;
}

