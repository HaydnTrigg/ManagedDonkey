#include "main/main_render.hpp"

#include "bink/bink_playback.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries_events.hpp"
#include "cutscene/cinematics.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "interface/c_controller.hpp"
#include "interface/interface_constants.hpp"
#include "interface/terminal.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "main/main_time.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "profiler/profiler.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_globals.hpp"
#include "render/render.hpp"
#include "render/views/render_view.hpp"
#include "simulation/simulation.hpp"
#include "text/draw_string.hpp"

//HOOK_DECLARE(0x006042C0, main_render);
HOOK_DECLARE(0x00604860, main_render_pregame);
//HOOK_DECLARE(0x00604D70, main_render_view); // paired with `main_render_view_inline_hook`

bool debug_render_horizontal_splitscreen = false;
bool debug_force_all_player_views_to_default_player = false;

c_player_render_camera_iterator::c_player_render_camera_iterator() :
	m_window_count(),
	m_window_arrangement(),
	m_next(0),
	m_output_user_index(k_output_user_none),
	m_current_observer_result(nullptr)
{
	if (cinematic_in_progress()/* || player_is_reading_terminal()*/)
	{
		m_window_count = 1;
	}
	else
	{
		long output_user_active_count = player_mapping_output_user_active_count();

		long player_count = 1;
		if (output_user_active_count > 1)
			player_count = output_user_active_count;

		long window_count = 1;
		if (player_count > MAXIMUM_PLAYER_WINDOWS)
			window_count = MAXIMUM_PLAYER_WINDOWS;
		m_window_count = window_count;
	}

	ASSERT(m_window_count <= MAXIMUM_PLAYER_WINDOWS);

	if (m_window_count <= 1)
	{
		m_window_arrangement = 0;
	}
	else
	{
		bool is_widescreen = rasterizer_get_is_widescreen();
		if (debug_render_horizontal_splitscreen)
			is_widescreen = true;

		if (is_widescreen)
			m_window_arrangement = 2;
		else
			m_window_arrangement = 1;
	}
}

bool c_player_render_camera_iterator::next()
{
	bool result = m_next < m_window_count;
	if (result)
	{
		m_next++;

		if (debug_force_all_player_views_to_default_player)
		{
			m_output_user_index = player_mapping_first_active_output_user();
		}
		else
		{
			do
			{
				m_output_user_index = next_output_user(m_output_user_index);
			} while (m_output_user_index != k_output_user_none && !player_mapping_output_user_is_active(m_output_user_index));

			if (m_output_user_index >= k_number_of_output_users)
			{
				m_output_user_index = k_output_user_none;
				result = false;
			}
		}

		if (m_output_user_index != k_output_user_none)
		{
			m_current_observer_result = observer_get_camera(m_output_user_index);
			ASSERT(m_current_observer_result != NULL);
		}
	}

	return result;
}

long c_player_render_camera_iterator::get_window_count() const
{
	return m_window_count;
}

long c_player_render_camera_iterator::get_window_arrangement() const
{
	return m_window_arrangement;
}

e_output_user_index c_player_render_camera_iterator::get_output_user_index() const
{
	return m_output_user_index;
}

s_observer_result const* c_player_render_camera_iterator::get_observer_result() const
{
	return m_current_observer_result;
}

bool __cdecl sub_42E5D0()
{
	return INVOKE(0x0042E5D0, sub_42E5D0);
}

void __cdecl main_render()
{
	INVOKE(0x006042C0, main_render);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	//REFERENCE_DECLARE(0x02446778, long, dword_2446778);
	//
	//PROFILER(main_render)
	//{
	//	bool should_draw = !sub_42E5D0() || !debug_no_drawing;
	//
	//	{
	//		c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
	//		rasterizer_lag_timing_mark_render_start();
	//		main_render_process_messages();
	//
	//		if (dword_2446778 > 50)
	//		{
	//			should_draw = false;
	//			dword_2446778 = 0;
	//		}
	//
	//		if (rasterizer_lag_timing_get_gamestate_delay() > 10)
	//			dword_2446778++;
	//		else
	//			dword_2446778 = 0;
	//
	//		if (should_draw)
	//		{
	//			bool render_game = true;
	//			bool render_sapien = false;
	//
	//			if (c_rasterizer::begin_frame())
	//			{
	//				PROFILER(render)
	//				{
	//					PROFILER(general)
	//					{
	//						if (game_in_progress() && game_get_active_structure_bsp_mask())
	//						{
	//							if (simulation_starting_up())
	//								render_game = false;
	//						}
	//						else
	//						{
	//							render_game = false;
	//						}
	//
	//						if (render_game)
	//						{
	//							render_sapien = game_in_editor();
	//							render_game = !render_sapien;
	//						}
	//
	//						main_render_frame_begin();
	//
	//						if (render_game)
	//						{
	//							main_render_game();
	//						}
	//						else if (render_sapien)
	//						{
	//							main_render_sapien();
	//						}
	//						else
	//						{
	//							main_render_pregame(_main_pregame_frame_normal, NULL);
	//						}
	//
	//						should_draw = !texture_cache_is_blocking() && !geometry_cache_is_blocking();
	//					}
	//				}
	//			}
	//			else
	//			{
	//				should_draw = false;
	//			}
	//
	//			c_render_globals::increment_frame_index();
	//		}
	//
	//		//if (should_draw)
	//		//{
	//		//	if (__int64 blocking_cycles = g_main_render_block_watch.stop())
	//		//	{
	//		//		real blocking_milliseconds = c_stop_watch::cycles_to_seconds(blocking_cycles);
	//		//		status_printf("blocking time: %.2f ms", blocking_milliseconds * 1000.0f);
	//		//	}
	//		//}
	//
	//		rasterizer_lag_timing_mark_render_end();
	//	}
	//
	//	if (should_draw)
	//	{
	//		__int64 target_display_vblank_index = main_time_get_target_display_vblank_index();
	//		restricted_region_mirror_locked_for_current_thread(k_game_state_shared_region);
	//		main_time_throttle(target_display_vblank_index);
	//
	//		// done in `sub_604A20`
	//		c_rasterizer::end_frame();
	//		g_main_render_timing_data->reset();
	//	}
	//}
}

void __cdecl main_render_assert_no_pending_messages()
{
	INVOKE(0x00604420, main_render_assert_no_pending_messages);

	//effects_assert_no_pending_messages();
}

void __cdecl main_render_frame_begin()
{
	INVOKE(0x00604430, main_render_frame_begin);
}

void __cdecl main_render_game()
{
	INVOKE(0x00604440, main_render_game);
}

bool g_show_watermark = true;

//void __cdecl game_engine_render_window_watermarks(e_output_user_index user_index)
void __cdecl game_engine_render_window_watermarks(long user_index)
{
}

void __cdecl game_engine_render_frame_watermarks_for_controller(e_controller_index controller_index)
{
	if (g_show_watermark)
	{
		// #TODO: implement this
	}
}

void __cdecl game_engine_render_frame_watermarks(bool pregame)
{
	if (g_show_watermark && (pregame || simulation_starting_up()) && !bink_playback_active())
	{
		c_static_wchar_string<1024> status;
		c_font_cache_mt_safe font_cache;
		c_rasterizer_draw_string draw_string;

		wchar_t const* spinner_states[] = { L"/", L"-", L"\\" };
		long spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);
		status.print(L"Establishing connection... %s|n(please do not turn off your Xbox 360)|n|n%S",
			spinner_states[spinner_state_index],
			simulation_get_starting_up_description());

		draw_string.set_font(6);
		draw_string.set_justification(2);
		draw_string.set_color(global_real_argb_white);

		short_rectangle2d display_title_safe_pixel_bounds{};
		interface_get_current_display_or_window_settings(NULL, NULL, NULL, &display_title_safe_pixel_bounds);
		real_rectangle2d bounds{};
		set_real_rectangle2d(&bounds,
			display_title_safe_pixel_bounds.x0,
			display_title_safe_pixel_bounds.x1,
			// this calculation seems to be correct, could possibly go a little lower
			((display_title_safe_pixel_bounds.y1 - (4.0f * draw_string.get_line_height())) / 2),
			display_title_safe_pixel_bounds.y1
		);
		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, status.get_string());
	}

	if (pregame || !game_in_progress())
		game_engine_render_window_watermarks(NONE);
	
	//game_engine_render_frame_watermarks_for_controller(controller_get_first_non_guest_signed_in_controller());
	game_engine_render_frame_watermarks_for_controller(static_cast<e_controller_index>(DECLFUNC(0x00A94980, short, __cdecl)()));
}

void __cdecl main_render_pregame(e_main_pregame_frame pregame_frame_type, char const* text)
{
	//INVOKE(0x00604860, main_render_pregame, pregame_frame_type, text);

	if (!sub_42E5D0())
	{
		c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);

		c_view::abort_current_view_stack();

		c_fullscreen_view fullscreen_view;
		fullscreen_view.setup_camera(nullptr);
		fullscreen_view.begin(&fullscreen_view);
		fullscreen_view.render_blank_frame(&pregame_frame_colors[pregame_frame_type].blank_frame);

		s_render_fullscreen_text_context context;

		context.text = text;
		context.color = &pregame_frame_colors[pregame_frame_type].text_color;
		context.shadow_color = &pregame_frame_colors[pregame_frame_type].text_shadow_color;
		context.scale = pregame_frame_scales[pregame_frame_type];

		if (pregame_frame_type == _main_pregame_frame_normal && !main_game_change_in_progress() && main_halted_with_errors())
			context.text = events_get();

		bool simple_font = pregame_frame_type == _main_pregame_frame_minidump_upload_waiting || pregame_frame_type == _main_pregame_frame_minidump_upload_completed_successfully;
		render_fullscreen_text(&context, simple_font);
		overlapped_render();
		controllers_render();

		if (pregame_frame_type == _main_pregame_frame_normal)
		{
			fullscreen_view.render();
			terminal_draw(); // this belongs in `c_fullscreen_view::render`
		}

		if (pregame_frame_type == _main_pregame_frame_normal && bink_playback_active())
		{
			bink_playback_update();
			bink_playback_check_for_terminate_no_lock();
			bink_playback_render();
		}

		if (pregame_frame_type == _main_pregame_frame_normal ||
			pregame_frame_type == _main_pregame_frame_progress_report)
			game_engine_render_frame_watermarks(true);

		c_view::end();
	}
}

void __cdecl main_render_pregame_loading_screen()
{
	INVOKE(0x00604990, main_render_pregame_loading_screen);
}

void __cdecl sub_604A20()
{
	INVOKE(0x00604A20, sub_604A20);

	//TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	//c_rasterizer::end_frame();
	//g_main_render_timing_data->reset();
}

void __cdecl main_render_process_messages()
{
	INVOKE(0x00604A60, main_render_process_messages);

	//if (!thread_has_crashed(k_thread_render))
	//{
	//	objects_render_thread_update();
	//	effects_process_render_thread_messages();
	//	c_decal_system::process_render_thread_messages();
	//}
	//main_render_purge_pending_messages();
}

void __cdecl main_render_purge_pending_messages()
{
	INVOKE(0x00604AA0, main_render_purge_pending_messages);

	//reset_object_render_message_queue();
	//reset_effect_render_message_queue(0);
	//c_decal_system::reset_render_message_queue();
}

void __cdecl main_render_sapien()
{
	//INVOKE(0x00604AC0, main_render_sapien);

	main_render_game();
}

//.text:00604AD0 ; sets a bool related to D3D

void __cdecl main_render_start_blocking_frame()
{
	INVOKE(0x00604AE0, main_render_start_blocking_frame);
}

void __cdecl main_render_status_message(wchar_t const* loading_status)
{
	INVOKE(0x00604AF0, main_render_status_message, loading_status);
}

void __cdecl main_render_update_loading_screen()
{
	INVOKE(0x00604C70, main_render_update_loading_screen);
}

// I don't like this at all but for the moment
// I don't perticularly want to reimplement `main_render_game` as its quite large
__declspec(naked) void main_render_view_inline()
{
	__asm
	{
        // main_render_view(player_view, player_view->get_player_view_user_index())
        push dword ptr[esi + 0x26A4]
        push esi
        call main_render_view

        // jump out after the inlined `main_render_view`
		// we push the return address to the stack for the jump,
		// the stack will be cleaned up outside of this hook
		// this is **very** non-standard and bad, *Wait, That's Illegal*
        push 0x00604747
        jmp dword ptr[esp]
	}
}
//HOOK_DECLARE(0x006046EB, main_render_view_inline);

void __cdecl main_render_view(c_player_view* player_view, long player_index)
{
	//INVOKE(0x00604D70, main_render_view, player_view, player_index);

	c_player_view::set_global_player_view(player_view);
	c_view::begin(player_view);
	render_window_reset(player_view->get_player_view_user_index());
	player_view->create_frame_textures(player_index);

	// don't need this, because `bink_texture_view_update` is this and its bad
	//c_bink_texture_view::update(player_index);

	render_prepare_for_window(player_index, player_view->get_player_view_user_index());
	player_view->compute_visibility();
	player_view->render_submit_visibility();
	player_view->render();

	// all this logic just to call little ol' me
	render_debug_window_render(player_view->get_player_view_user_index());

	c_view::end();
	c_player_view::set_global_player_view(0);
}

