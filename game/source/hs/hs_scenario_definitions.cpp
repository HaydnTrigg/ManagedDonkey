#include "hs/hs_scenario_definitions.hpp"

void hs_tag_reference::update_reference_names()
{
	UPDATE_REFERENCE_NAME(reference);
}

char const* const k_hs_type_names[k_hs_type_count]
{
	"unparsed",
	"special_form",
	"function_name",
	"passthrough",
	"void",
	"boolean",
	"real",
	"short",
	"long",
	"string",
	"script",
	"string_id",
	"unit_seat_mapping",
	"trigger_volume",
	"cutscene_flag",
	"cutscene_camera_point",
	"cutscene_title",
	"cutscene_recording",
	"device_group",
	"ai",
	"ai_command_list",
	"ai_command_script",
	"ai_behavior",
	"ai_orders",
	"ai_line",
	"starting_profile",
	"conversation",
	"zone_set",
	"designer_zone",
	"point_reference",
	"style",
	"object_list",
	"folder",
	"sound",
	"effect",
	"damage",
	"looping_sound",
	"animation_graph",
	"damage_effect",
	"object_definition",
	"bitmap",
	"shader",
	"render_model",
	"structure_definition",
	"lightmap_definition",
	"cinematic_definition",
	"cinematic_scene_definition",
	"bink_definition",
	"any_tag",
	"any_tag_not_resolving",
	"game_difficulty",
	"team",
	"mp_team",
	"controller",
	"button_preset",
	"joystick_preset",
	"player_character_type",
	"voice_output_setting",
	"voice_mask",
	"subtitle_setting",
	"actor_type",
	"model_state",
	"event",
	"character_physics",
	"primary_skull",
	"secondary_skull",
	"object",
	"unit",
	"vehicle",
	"weapon",
	"device",
	"scenery",
	"effect_scenery",
	"object_name",
	"unit_name",
	"vehicle_name",
	"weapon_name",
	"device_name",
	"scenery_name",
	"effect_scenery_name",
	"cinematic_lightprobe",
	"animation_budget_reference",
	"looping_sound_budget_reference",
	"sound_budget_reference"
};

