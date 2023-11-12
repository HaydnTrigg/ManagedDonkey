#pragma once

enum e_render_method_extern
{
	_render_method_extern_none = 0,
	_render_method_extern_texture_global_target_texaccum,
	_render_method_extern_texture_global_target_normal,
	_render_method_extern_texture_global_target_z,
	_render_method_extern_texture_global_target_shadow_buffer1,
	_render_method_extern_texture_global_target_shadow_buffer2,
	_render_method_extern_texture_global_target_shadow_buffer3,
	_render_method_extern_texture_global_target_shadow_buffer4,
	_render_method_extern_texture_global_target_texture_camera,
	_render_method_extern_texture_global_target_reflection,
	_render_method_extern_texture_global_target_refraction,
	_render_method_extern_texture_lightprobe_texture,
	_render_method_extern_texture_dominant_light_intensity_map,
	_render_method_extern_texture_unused1,
	_render_method_extern_texture_unused2,
	_render_method_extern_object_change_color_primary,
	_render_method_extern_object_change_color_secondary,
	_render_method_extern_object_change_color_tertiary,
	_render_method_extern_object_change_color_quaternary,
	_render_method_extern_object_change_color_quinary,
	_render_method_extern_object_change_color_primary_anim,
	_render_method_extern_object_change_color_secondary_anim,
	_render_method_extern_texture_dynamic_environment_map_0,
	_render_method_extern_texture_dynamic_environment_map_1,
	_render_method_extern_texture_cook_torrance_cc0236,
	_render_method_extern_texture_cook_torrance_dd0236,
	_render_method_extern_texture_cook_torrance_c78d78,
	_render_method_extern_light_dir_0,
	_render_method_extern_light_color_0,
	_render_method_extern_light_dir_1,
	_render_method_extern_light_color_1,
	_render_method_extern_light_dir_2,
	_render_method_extern_light_color_2,
	_render_method_extern_light_dir_3,
	_render_method_extern_light_color_3,
	_render_method_extern_texture_unused_3,
	_render_method_extern_texture_unused_4,
	_render_method_extern_texture_unused_5,
	_render_method_extern_texture_dynamic_light_gel_0,
	_render_method_extern_flat_envmap_matrix_x,
	_render_method_extern_flat_envmap_matrix_y,
	_render_method_extern_flat_envmap_matrix_z,
	_render_method_extern_debug_tint,
	_render_method_extern_screen_constants,
	_render_method_extern_active_camo_distortion_texture,
	_render_method_extern_scene_ldr_texture,
	_render_method_extern_scene_hdr_texture,
	_render_method_extern_water_memory_export_address,
	_render_method_extern_tree_animation_timer,
	_render_method_extern_emblem_player_shoulder_texture,
	_render_method_extern_emblem_clan_chest_texture,

	k_render_method_extern_count
};

extern char const* k_render_method_extern_names[k_render_method_extern_count];

extern void __cdecl render_method_submit_invalidate_cache();

