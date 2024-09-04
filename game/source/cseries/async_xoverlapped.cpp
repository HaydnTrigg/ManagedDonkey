#include "cseries/async_xoverlapped.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/online/online_error.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "text/draw_string.hpp"
#include "xbox/xbox.hpp"

#include <windows.h>

//REFERENCE_DECLARE(0x0240AED0, s_overlapped_globals, g_overlapped_globals);

HOOK_DECLARE(0x005A8D50, find_task_slot);
HOOK_DECLARE(0x005A8DD0, overlapped_dispose);
HOOK_DECLARE(0x005A8DE0, overlapped_initialize);
HOOK_DECLARE(0x005A8E20, overlapped_render);
HOOK_DECLARE(0x005A8E30, overlapped_task_block_until_finished);
HOOK_DECLARE(0x005A8ED0, overlapped_task_is_running);
//HOOK_DECLARE(0x005A8F40, overlapped_task_start_internal);
HOOK_DECLARE(0x005A8F50, overlapped_task_terminate);
HOOK_DECLARE(0x005A8FA0, overlapped_task_toggle_debug_rendering);
HOOK_DECLARE(0x005A8FB0, overlapped_task_wait_for_all_tasks_to_finish);
HOOK_DECLARE(0x005A9050, overlapped_update);
HOOK_DECLARE(0x005A91E0, task_block_until_finished);
HOOK_DECLARE(0x005A9230, task_is_complete);
HOOK_DECLARE(0x005A9280, task_now_finished);

HOOK_DECLARE_CALL(0x0074CA3C, overlapped_task_start_internal); // c_content_enumeration_overlapped_task::success
HOOK_DECLARE_CALL(0x00A82786, overlapped_task_start_internal); // user_interface_session_string_verify_update
//HOOK_DECLARE_CALL(0x00AA4306, overlapped_task_start_internal); // c_player_profile_interface::update_service_tag
//HOOK_DECLARE_CALL(0x00AD3699, overlapped_task_start_internal); // c_player_marketplace::show_purchase_ui
//HOOK_DECLARE_CALL(0x00AD36F6, overlapped_task_start_internal); // c_player_marketplace::start_asset_consume
//HOOK_DECLARE_CALL(0x00AD3810, overlapped_task_start_internal); // c_player_marketplace::start_assets_enumerate
HOOK_DECLARE_CALL(0x00AE668B, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AE6868, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AEA33B, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00AF0D67, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable50
HOOK_DECLARE_CALL(0x00AF0FBE, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable49
HOOK_DECLARE_CALL(0x00AF26D6, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_item_selected::__vftable50
HOOK_DECLARE_CALL(0x00AF5088, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF52D2, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF8495, overlapped_task_start_internal); // c_start_menu_settings_appearance_emblem::handle_list_item_chosen
HOOK_DECLARE_CALL(0x00B0373C, overlapped_task_start_internal); // c_gui_screen_pregame_selection::begin_selected_item_text_edit
HOOK_DECLARE_CALL(0x00B0B8D6, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0BAA0, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0DA48, overlapped_task_start_internal); // c_gui_screen_game_options::update_save_as_new_operation

// #TODO: add me to `s_overlapped_globals`
c_static_array<XOVERLAPPED, 64> task_slot_overlapped;

struct s_task_slot
{
	// added back
	XOVERLAPPED overlapped;

	c_overlapped_task* task;
	dword calling_result;
	bool terminated;

	// pad?
	byte __data9[0x3];
};
//static_assert(sizeof(s_task_slot) == 0xC);
static_assert(sizeof(s_task_slot) == 0x28);

struct s_overlapped_globals
{
	// added back
	bool toggle_debug_rendering;

	c_static_array<s_task_slot, 64> task_slots;

	bool error_injection_enabled;
	c_static_string<64> error_injection_context;

	bool paused;
	c_static_string<64> pause_context;

	long description_count;
	c_static_array<c_static_string<64>, 64> descriptions;
};
//static_assert(sizeof(s_overlapped_globals) == 0x30C);
static_assert(sizeof(s_overlapped_globals) == 0x1A8C);

static s_overlapped_globals g_overlapped_globals{};

c_async_xoverlapped_scope_lock::c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_enter(_critical_section_xoverlapped_memory);
}

c_async_xoverlapped_scope_lock::~c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_leave(_critical_section_xoverlapped_memory);
}

bool c_overlapped_task::busy() const
{
	//return DECLFUNC(0x00476C30, bool, __thiscall, c_overlapped_task const*)(this);

	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}

c_overlapped_task* c_overlapped_task::constructor(char const* file, long line)
{
	//DECLFUNC(0x005A8C80, c_overlapped_task*, __thiscall, c_overlapped_task const*, char const*, long)(this, file, line);

	REFERENCE_DECLARE(this, dword, vftable);
	vftable = 0x0165B220;

	m_task_flags.clear();
	set_file(file);
	set_line(line);

	set_task_state_internal(_overlapped_task_state_none);

	return this;
}

//.text:005A8CB0 ; public: virtual __cdecl c_overlapped_task::~c_overlapped_task()
//.text:005A8CE0 ; public: virtual void* __cdecl c_overlapped_task::`vector deleting destructor'(unsigned int)
//.text:005A8D30 ; public: virtual void __cdecl c_overlapped_task::complete()
//.text:005A8D40 ; public: virtual void __cdecl c_overlapped_task::failure(dword calling_result, dword overlapped_error, dword overlapped_extended_error)

s_task_slot* __cdecl find_task_slot(c_overlapped_task const* task)
{
	//return INVOKE(0x005A8D50, find_task_slot, task);

	ASSERT(task);

	s_task_slot* task_slot = NULL;
	for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
	{
		if (g_overlapped_globals.task_slots[task_slot_index].task == task)
			task_slot = &g_overlapped_globals.task_slots[task_slot_index];
	}

	return task_slot;
}

//.text:005A8DA0 ; public: virtual bool __cdecl c_overlapped_task::is_result_successful(dword calling_result, dword overlapped_error, dword overlapped_extended_error)

void __cdecl overlapped_dispose()
{
	//INVOKE(0x005A8DD0, overlapped_dispose);

	overlapped_task_wait_for_all_tasks_to_finish();
	c_virtual_keyboard_task::dispose_instance();
	overlapped_memory_dispose();
}

void __cdecl overlapped_initialize()
{
	//INVOKE(0x005A8DE0, overlapped_initialize);

	g_overlapped_globals.toggle_debug_rendering = true;
	csmemset(&g_overlapped_globals, 0, sizeof(g_overlapped_globals));
	overlapped_memory_initialize();
	c_virtual_keyboard_task::get_instance(__FILE__, __LINE__, k_any_controller, NULL, NULL, NULL, 0, 0, true);
	g_overlapped_globals.error_injection_enabled = false;
	g_overlapped_globals.paused = 0;
	g_overlapped_globals.description_count = 0;
}

void __cdecl overlapped_render()
{
	//INVOKE(0x005A8E20, overlapped_render);

	if (g_overlapped_globals.toggle_debug_rendering)
	{
		c_simple_font_draw_string draw_string;

		short_rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);

		draw_string.set_bounds(&bounds);
		draw_string.set_color(global_real_argb_green);

		draw_string.draw(NULL, "---------- xoverlapped tasks ----------\r\n");

		for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
		{
			char const* status = NULL;
			char string[256]{};

			{
				c_async_xoverlapped_scope_lock scope_lock;

				s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

				if (task_slot->task)
				{
					switch (task_slot->task->get_task_state())
					{
					case _overlapped_task_state_none:
						status = "none";
						break;
					case _overlapped_task_state_starting:
						status = "starting";
						break;
					case _overlapped_task_state_pending:
						status = "pending";
						break;
					case _overlapped_task_state_completing:
						status = "completing";
						break;
					case _overlapped_task_state_succeeded:
						status = "success";
						break;
					case _overlapped_task_state_failure:
						status = "failure";
						break;
					default:
						status = "<unknown>";
						break;
					}

					csnzprintf(string, sizeof(string), "#%ld task info: context= '%s', status= '%s', file= '%s', line= '%ld'\r\n",
						task_slot_index++,
						task_slot->task->get_context_string(),
						status,
						task_slot->task->m_file,
						task_slot->task->m_line);
				}
			}

			if (string[0])
				draw_string.draw_more(NULL, string);
		}

		draw_string.draw_more(NULL, "---------------------------------------");
	}
}

void __cdecl overlapped_task_block_until_finished(c_overlapped_task const* task)
{
	//INVOKE(0x005A8E30, overlapped_task_block_until_finished, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
		task_block_until_finished(task_slot);
}

bool __cdecl overlapped_task_is_running(c_overlapped_task const* task)
{
	//return INVOKE(0x005A8ED0, overlapped_task_is_running, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
		return task_slot->task->get_task_state() == _overlapped_task_state_pending || task_slot->task->get_task_state() == _overlapped_task_state_completing;

	return false;
}

bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, char const* file, long line)
{
	//return INVOKE(0x005A8F40, overlapped_task_start_internal, task, file, line);

	ASSERT(task);

	bool result = false;
	task->set_file(file);
	task->set_line(line);

	c_async_xoverlapped_scope_lock scope_lock;

	s_task_slot* first_free_task_slot = NULL;
	for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
	{
		if (!g_overlapped_globals.task_slots[task_slot_index].task)
		{
			first_free_task_slot = &g_overlapped_globals.task_slots[task_slot_index];
			break;
		}
	}

	if (first_free_task_slot)
	{
		first_free_task_slot->task = task;
		first_free_task_slot->task->set_task_state_internal(_overlapped_task_state_starting);

		bool context_matches_description = false;
		if (strlen(task->get_context_string()) < 64)
		{
			for (long description_index = 0; description_index < g_overlapped_globals.description_count; description_index++)
			{
				if (g_overlapped_globals.descriptions[description_index].is_equal(task->get_context_string()))
				{
					context_matches_description = true;
					break;
				}
			}
		
			if (!context_matches_description)
			{
				if (g_overlapped_globals.description_count < g_overlapped_globals.descriptions.get_count())
				{
					g_overlapped_globals.descriptions[g_overlapped_globals.description_count++].set(task->get_context_string());
				}
				else
				{
					generate_event(_event_level_warning, "xoverlapped: task context string '%s' not added to error-code-injection table, maximum tasks already being tracked!",
						task->get_context_string());
				}
			}
		}
		else
		{
			generate_event(_event_level_warning, "xoverlapped: task context string '%s' is too long to fit into error-code-injection table (maximum %ld characters)",
				task->get_context_string(),
				64);
		}

		result = true;
	}
	else
	{
		generate_event(_event_level_status, "xoverlapped: overlapped_task_start() failed: maximum tasks already active!");
	}

	return result;
}

void __cdecl overlapped_task_terminate(c_overlapped_task* task)
{
	//INVOKE(0x005A8F50, overlapped_task_terminate, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
		task_slot->terminated = true;
}

void __cdecl overlapped_task_toggle_debug_rendering(bool toggle_debug_rendering)
{
	//INVOKE(0x005A8FA0, overlapped_task_toggle_debug_rendering, toggle_debug_rendering);

	g_overlapped_globals.toggle_debug_rendering = toggle_debug_rendering;
}

void __cdecl overlapped_task_wait_for_all_tasks_to_finish()
{
	//INVOKE(0x005A8FB0, overlapped_task_wait_for_all_tasks_to_finish);

	for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
	{
		c_async_xoverlapped_scope_lock scope_lock;
		if (g_overlapped_globals.task_slots[task_slot_index].task)
		{
			if (g_overlapped_globals.task_slots[task_slot_index].task->get_task_state() == _overlapped_task_state_pending)
				task_block_until_finished(&g_overlapped_globals.task_slots[task_slot_index]);
		}
	}
}

void __cdecl overlapped_update()
{
	//INVOKE(0x005A9050, overlapped_update);

	for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
	{
		{
			c_async_xoverlapped_scope_lock scope_lock;
			s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

			if (task_slot->task && task_slot->task->get_task_state() == _overlapped_task_state_pending)
			{
				dword return_result = ERROR_SUCCESS;
				dword calling_result = ERROR_SUCCESS;
				dword overlapped_error = ERROR_SUCCESS;
				dword overlapped_extended_error = ERROR_SUCCESS;
				if (task_is_complete(task_slot, &return_result, &calling_result, &overlapped_error, &overlapped_extended_error))
					task_now_finished(task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
				else
					task_slot->task->update(return_result);
			}
		}

		{
			c_async_xoverlapped_scope_lock scope_lock;
			s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

			if (task_slot->task && task_slot->task->get_task_state() == _overlapped_task_state_starting)
			{
				long pending_task_count = 0;
			
				for (s_task_slot& pending_task_slot : g_overlapped_globals.task_slots)
				{
					if (pending_task_slot.task && pending_task_slot.task->get_task_state() == _overlapped_task_state_pending)
						pending_task_count++;
				}
			
				if (pending_task_count < 3)
				{
					task_slot->calling_result = task_slot->task->start(&task_slot->overlapped);
					task_slot->task->set_task_state_internal(_overlapped_task_state_pending);
					task_slot->terminated = false;
				}
			}
		}
	}
}

//.text:005A9130 ; public: virtual void __cdecl c_overlapped_task::reset()

e_overlapped_task_state c_overlapped_task::get_task_state() const
{
	return m_task_state;
}

void c_overlapped_task::set_task_state_internal(e_overlapped_task_state task_state)
{
	//DECLFUNC(0x005A91B0, void, __thiscall, c_overlapped_task const*, e_overlapped_task_state)(this, task_state);

	m_task_state = task_state;
}

//.text:005A91D0 ; public: virtual void __cdecl c_overlapped_task::success(dword return_result)

void __cdecl task_block_until_finished(s_task_slot* task_slot)
{
	//INVOKE(0x005A91E0, task_block_until_finished, task_slot);

	dword return_result = ERROR_SUCCESS;
	dword calling_result = ERROR_SUCCESS;
	dword overlapped_error = ERROR_SUCCESS;
	dword overlapped_extended_error = ERROR_SUCCESS;

	while (task_slot->task && !task_is_complete(task_slot, &return_result, &calling_result, &overlapped_error, &overlapped_extended_error))
	{
		overlapped_update();
		switch_to_thread();
	}

	if (task_slot->task)
		task_now_finished(task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
}

bool __cdecl task_is_complete(s_task_slot* task_slot, dword* return_result, dword* calling_result, dword* overlapped_error, dword* overlapped_extended_error)
{
	//return INVOKE(0x005A9230, task_is_complete, task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);

	ASSERT(task_slot != NULL);
	ASSERT(task_slot->task != NULL);
	ASSERT(return_result != NULL);
	ASSERT(calling_result != NULL);
	ASSERT(overlapped_error != NULL);
	ASSERT(overlapped_extended_error != NULL);

	*return_result = ERROR_SUCCESS;
	*calling_result = task_slot->calling_result;
	*overlapped_error = ERROR_SUCCESS;
	*overlapped_extended_error = ERROR_SUCCESS;

	bool result = true;
	if (task_slot->task->get_task_state() == _overlapped_task_state_starting)
	{
		result = false;
	}
	else if (task_slot->calling_result == ERROR_IO_PENDING)
	{
		result = false;
	
		if (task_slot->overlapped.InternalLow != ERROR_IO_PENDING)
		{
			result = true;
			*overlapped_error = XGetOverlappedResult(&task_slot->overlapped, return_result, FALSE);
			if (*overlapped_error)
			{
				if (*overlapped_error != ERROR_IO_INCOMPLETE && *overlapped_error != ERROR_IO_PENDING)
					*overlapped_extended_error = XGetOverlappedExtendedError(&task_slot->overlapped);
			}
		}
	}
	
	if (g_overlapped_globals.paused)
	{
		if (g_overlapped_globals.pause_context.is_equal(task_slot->task->get_context_string()))
			result = false;
	}
	
	return result;
}

void __cdecl task_now_finished(s_task_slot* task_slot, dword return_result, dword calling_result, dword overlapped_error, dword overlapped_extended_error)
{
	//INVOKE(0x005A9280, task_now_finished, task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
	
	ASSERT(task_slot->task != NULL);
	ASSERT(task_slot->task->get_task_state() == _overlapped_task_state_starting || task_slot->task->get_task_state() == _overlapped_task_state_pending);

	bool succeeded = false;
	if (task_slot->task->get_task_state() == _overlapped_task_state_starting)
	{
		succeeded = false;
	}
	else
	{
		succeeded = !task_slot->terminated && task_slot->task->is_result_successful(calling_result, overlapped_error, overlapped_extended_error);

		if (g_overlapped_globals.error_injection_enabled)
		{
			if (g_overlapped_globals.error_injection_context.is_equal(task_slot->task->get_context_string()))
			{
				generate_event(_event_level_warning, "xoverlapped: injecting error for task %s",
					g_overlapped_globals.error_injection_context.get_string());
		
				succeeded = false;
			}
		}
	}

	task_slot->task->set_task_state_internal(_overlapped_task_state_completing);

	e_overlapped_task_state task_state = _overlapped_task_state_none;
	if (succeeded)
	{
		task_state = _overlapped_task_state_succeeded;
		task_slot->task->success(return_result);

		generate_event(_event_level_status, "xoverlapped: task '%s' succeeded. return result= %s",
			task_slot->task->get_context_string(),
			online_error_get_string(return_result).get_string());
	}
	else
	{
		task_state = _overlapped_task_state_failure;

		if (task_slot->terminated)
			overlapped_error = ERROR_CANCELLED;

		task_slot->task->failure(calling_result, overlapped_error, overlapped_extended_error);
		if (task_slot->terminated ||
			overlapped_error == ERROR_CANCELLED ||
			overlapped_error == ERROR_FUNCTION_FAILED && overlapped_extended_error == ERROR_CANCELLED)
		{
			generate_event(_event_level_message, "xoverlapped: task '%s' was cancelled.",
				task_slot->task->get_context_string());
		}
		else
		{
			generate_event(_event_level_warning, "xoverlapped: task '%s' failed. calling result= %s, error= %s, extended error= %s, file= '%s', line= '%ld'",
				task_slot->task->get_context_string(),
				online_error_get_string(calling_result).get_string(),
				online_error_get_string(overlapped_error).get_string(),
				online_error_get_string(overlapped_extended_error).get_string(),
				task_slot->task->m_file,
				task_slot->task->m_line);
		}
	}

	task_slot->task->complete();

	if (task_slot->task->task_was_recycled_during_completion())
	{
		task_slot->task->task_recycled_during_completion(false);
	}
	else if (task_slot->task->busy())
	{
		task_slot->task->set_task_state_internal(task_state);
	}

	csmemset(task_slot, 0, sizeof(s_task_slot));
}

void c_overlapped_task::task_recycled_during_completion(bool recycled_during_completion)
{
	//return DECLFUNC(0x005A9360, void, __thiscall, c_overlapped_task*, bool)(this, recycled_during_completion);

	m_task_flags.set(_overlapped_task_task_recycled_during_completion_bit, recycled_during_completion);
}

bool c_overlapped_task::task_was_recycled_during_completion() const
{
	//return DECLFUNC(0x005A9380, bool, __thiscall, c_overlapped_task const*)(this);

	return m_task_flags.test(_overlapped_task_task_recycled_during_completion_bit);
}

