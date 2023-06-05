#include "hf2p/hf2p.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "cseries/cseries_windows.hpp"
#include "fmod/fmod.hpp"
#include "game/game_engine_util.hpp"
#include "game/multiplayer_definitions.hpp"
#include "game/players.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "units/units.hpp"

#include <stdlib.h>

REFERENCE_DECLARE(0x018B59D4, bool, g_hf2p_first_run);
REFERENCE_DECLARE(0x04FE67A0, dword, mainmenu_spartan_unit_index);
REFERENCE_DECLARE(0x04FE67A4, dword, mainmenu_elite_unit_index);
REFERENCE_DECLARE(0x052697B1, bool, g_hf2p_use_keyboard_hints);

HOOK_DECLARE(0x006006F0, hf2p_game_initialize);
HOOK_DECLARE(0x00600790, hf2p_game_dispose);
HOOK_DECLARE(0x00600850, hf2p_game_update);

void __cdecl game_statistics_reset()
{
	INVOKE(0x00853FC0, game_statistics_reset);
}

void* hp2p_ui_proxy = reinterpret_cast<void*>(0x0244ED28);

void __cdecl hf2p_initialize()
{
	//INVOKE(0x00600630, hf2p_initialize);

	//game_statistics_reset();
	//hp2p_ui_proxy = nullptr;
	//
	//// crashes up the call stack
	//// #TODO: investigate
	//fmod_initialize();
}

void __cdecl hf2p_game_initialize()
{
	//HOOK_INVOKE(, hf2p_game_initialize);

	// Press <E> to pick up
	g_hf2p_use_keyboard_hints = true;
}

void __cdecl hf2p_scenario_tags_load_finished()
{
	//INVOKE(0x00600750, hf2p_scenario_tags_load_finished);

	g_hf2p_first_run = true;
	if (!g_hf2p_first_run)
	{
		hf2p_initialize();
		g_hf2p_first_run = true;
	}
}

void __cdecl hf2p_scenario_load()
{
	INVOKE(0x00600770, hf2p_scenario_load);
}

void __cdecl hf2p_game_dispose()
{
	//HOOK_INVOKE(, hf2p_game_dispose);
	//
	//fmod_terminate();
}

dword& mainmenu_unit_index = mainmenu_spartan_unit_index;

void __cdecl hf2p_game_update()
{
	// this function runs in `main_loop_body_main_part`

	// update `mainmenu_unit_index`
	DECLFUNC(0x007B7940, void, __cdecl)();

	if (mainmenu_unit_index != 0xFFFFFFFF)
	{
		{
			// only allow one instance of this
			s_s3d_player_armor_configuration_loadout& loadout = get_armor_loadout();

			if (!loadout.armor_is_set)
			{
				for (long color_index = 0; color_index < k_color_type_count; color_index++)
					loadout.colors[color_index].value = ~((system_milliseconds() * rand()) % 0xFFFFFF);

				//for (long armor_index = 0; armor_index < _armor_type_pelvis + 1; armor_index++)
				//	loadout.armors[armor_index] = (system_milliseconds() * rand()) % 70;

				loadout.armor_is_set = true;
			}
			DECLFUNC(0x005A4430, void, __cdecl, s_s3d_player_armor_configuration_loadout*, dword)(&loadout, mainmenu_unit_index);

			for (long color_index = 0; color_index < k_color_type_count; color_index++)
			{
				rgb_color& color = loadout.colors[color_index];
				real_rgb_color real_color;
				pixel32_to_real_rgb_color(color, &real_color);
				object_set_base_change_color_by_index(mainmenu_unit_index, color_index, &real_color);
			}
			DECLFUNC(0x005A2FA0, void, __cdecl, dword)(mainmenu_unit_index);
		}

		long primary_weapon_index = game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(/* random */ short(0xFFFD), _weapon_set_primary);
		game_engine_add_starting_weapon_to_player(mainmenu_unit_index, primary_weapon_index, 1);
	}

	printf("");
}

s_s3d_player_armor_configuration_loadout& get_armor_loadout(bool update_mainmenu_unit)
{
	static s_s3d_player_armor_configuration_loadout loadout{};

	if (update_mainmenu_unit)
		loadout.armor_is_set = false;

	return loadout;
}

c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& get_armor_regions(e_player_model_choice player_model_choice)
{
	static c_static_array<c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>, k_player_model_choice_count> armor_regions;

	if (g_cache_file_globals.tags_loaded)
	{
		s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();

		for (s_multiplayer_customized_model_character& customized_spartan_character : universal_data->customized_spartan_characters)
		{
			char const* armor_region = customized_spartan_character.armor_region.get_string();

			bool ignore_requirements = false;
			c_static_array<c_static_string<64>, 100>* armor_type = nullptr;
			if (csstricmp(armor_region, "helmet") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_helmet];
			}
			else if (csstricmp(armor_region, "chest") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_chest];
			}
			else if (csstricmp(armor_region, "shoulders") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_shoulders];
			}
			else if (csstricmp(armor_region, "arms") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_arms];
			}
			else if (csstricmp(armor_region, "legs") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_legs];
			}
			else if (csstricmp(armor_region, "acc") == 0)
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_acc];
			}
			else if (csstricmp(armor_region, "pelvis") == 0)
			{
				ignore_requirements = true;
				armor_type = &armor_regions[_player_model_choice_spartan][_armor_type_pelvis];
			}

			for (long i = 0; i < customized_spartan_character.customized_areas.count(); i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_spartan_character.customized_areas[i];

				// you can only see the arms in first person
				if (ignore_requirements || customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}

		for (s_multiplayer_customized_model_character& customized_elite_character : universal_data->customized_elite_characters)
		{
			char const* armor_region = customized_elite_character.armor_region.get_string();

			c_static_array<c_static_string<64>, 100>* armor_type = nullptr;
			if (csstricmp(armor_region, "helmet") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_helmet];
			}
			else if (csstricmp(armor_region, "chest") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_chest];
			}
			else if (csstricmp(armor_region, "shoulders") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_shoulders];
			}
			else if (csstricmp(armor_region, "arms") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_arms];
			}
			else if (csstricmp(armor_region, "legs") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_legs];
			}
			else if (csstricmp(armor_region, "acc") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_acc];
			}
			else if (csstricmp(armor_region, "pelvis") == 0)
			{
				armor_type = &armor_regions[_player_model_choice_elite][_armor_type_pelvis];
			}
			else if (csstricmp(armor_region, "upper_body") == 0) // there's an upper body armor type??
			{
				continue;
			}
			else
			{
				continue;
			}

			for (long i = 0; i < customized_elite_character.customized_areas.count(); i++)
			{
				s_multiplayer_customized_model_selection& customized_area = customized_elite_character.customized_areas[i];

				// you can only see the arms in first person
				if (customized_area.third_person_armor_object.index != NONE /*&& customized_area.first_person_armor_object.index != NONE*/)
					(*armor_type)[i] = customized_area.selection_name.get_string();
			}
		}
	}

	return armor_regions[player_model_choice];
}

