#include "render/views/render_player_view.hpp"

#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/render_flags.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"

REFERENCE_DECLARE(0x019147BC, real, render_debug_depth_render_scale_r);
REFERENCE_DECLARE(0x019147C0, real, render_debug_depth_render_scale_g);
REFERENCE_DECLARE(0x019147C4, real, render_debug_depth_render_scale_b);
REFERENCE_DECLARE(0x050FB3FC, long, render_debug_depth_render);

HOOK_DECLARE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);
HOOK_DECLARE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);
HOOK_DECLARE_CLASS_MEMBER(0x00A39960, c_player_view, render_);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);
HOOK_DECLARE_CLASS(0x00A3A310, c_player_view, render_albedo_decals);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

void __thiscall c_player_view::render_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);

	c_d3d_pix_event _distortion_apply(g_rasterizer_profile_pix_colors[1], L"distortion_apply");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_distortions);
}

void c_player_view::create_frame_textures(long player_index)
{
	INVOKE_CLASS_MEMBER(0x00A38D70, c_player_view, create_frame_textures, player_index);
}

void __cdecl c_player_view::get_player_render_camera_orientation(real_matrix4x3* camera)
{
	//INVOKE(0x00A39640, get_player_render_camera_orientation, camera);

	ASSERT(camera);
	ASSERT(c_player_view::x_current_player_view, "cannot get the render camera when not in the player view render loop");

	render_camera const* rasterizer_camera = c_player_view::x_current_player_view->get_rasterizer_camera();

	camera->position = rasterizer_camera->position;
	camera->forward = rasterizer_camera->forward;
	camera->up = rasterizer_camera->up;
	cross_product3d(&camera->up, &camera->forward, &camera->left);
	camera->scale = 1.0f;
}

void __thiscall c_player_view::queue_patchy_fog()
{
	if (players_get_active_and_in_game_count(true) > 1)
		return;

	INVOKE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);
}

void __thiscall c_player_view::render_()
{
	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_);
}

bool __thiscall c_player_view::render_albedo()
{
	//return INVOKE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);

	c_d3d_pix_event _albedo(g_rasterizer_profile_pix_colors[1], L"albedo");

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_player_view, render_albedo);
	return result;
}

void __cdecl c_player_view::render_albedo_decals(bool a1, bool a2)
{
	//INVOKE(0x00A3A310, c_player_view::render_albedo_decals, a1, a2);

	c_d3d_pix_event _decorators(g_rasterizer_profile_pix_colors[1], L"decorators");

	HOOK_INVOKE_CLASS(, c_player_view, render_albedo_decals, decltype(&c_player_view::render_albedo_decals), a1, a2);
}

//.text:00A3A3C0 ; 

void __thiscall c_player_view::render_effects(e_effect_pass pass)
{
	//INVOKE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects, pass);

	c_d3d_pix_event _render_effects(g_rasterizer_profile_pix_colors[1], L"render_effects");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_effects, pass);
}

void __thiscall c_player_view::render_first_person(bool a1)
{
	//INVOKE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);

	c_d3d_pix_event _first_person(g_rasterizer_profile_pix_colors[1], L"first_person");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person, a1);
}

void __thiscall c_player_view::render_first_person_albedo()
{
	//INVOKE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);

	c_d3d_pix_event _first_person_albedo(g_rasterizer_profile_pix_colors[1], L"first_person_albedo");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person_albedo);
}

//.text:00A3A6B0 ; 
//.text:00A3A6C0 ; protected: void __cdecl c_player_view::render_lens_flares()

void __thiscall c_player_view::render_lightmap_shadows()
{
	//INVOKE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);

	c_d3d_pix_event _lightmap_shadows(g_rasterizer_profile_pix_colors[1], L"lightmap_shadows");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_lightmap_shadows);
}

//.text:00A3A790 ; protected: void __cdecl c_player_view::render_misc_transparents()
//.text:00A3A7A0 ; public: void __cdecl c_player_view::render_patchy_fog()
//.text:00A3A7F0 ; render_patchy_fog_callback
//.text:00A3A850 ; public: virtual void __cdecl c_player_view::render_setup()

void __thiscall c_player_view::render_static_lighting()
{
	//INVOKE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);

	c_d3d_pix_event _static_lighting(g_rasterizer_profile_pix_colors[1], L"static_lighting");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_static_lighting);
}

//.text:00A3AAA0 ; public: virtual void __cdecl c_player_view::render_submit_visibility()
//.text:00A3AB20 ; 

void __cdecl render_texture_camera_dispose()
{
	INVOKE(0x00A3AB90, render_texture_camera_dispose);
}

void __cdecl render_texture_camera_dispose_from_old_map()
{
	INVOKE(0x00A3ABA0, render_texture_camera_dispose_from_old_map);
}

//.text:00A3ABB0 ; void __cdecl render_texture_camera_enable_dynamic_lights(bool)
//.text:00A3ABD0 ; double __cdecl render_texture_camera_get_aspect_ratio()
//.text:00A3ABF0 ; 
//.text:00A3AC10 ; 
//.text:00A3AC30 ; 
//.text:00A3AC70 ; 
//.text:00A3ACB0 ; 

void __cdecl render_texture_camera_initialize()
{
	INVOKE(0x00A3ACD0, render_texture_camera_initialize);
}

void __cdecl render_texture_camera_initialize_for_new_map()
{
	INVOKE(0x00A3AD30, render_texture_camera_initialize_for_new_map);
}

//.text:00A3AE50 ; void __cdecl render_texture_camera_off()
//.text:00A3AE70 ; void __cdecl render_texture_camera_on()
//.text:00A3AE90 ; void __cdecl render_texture_camera_position_world_offset(real, real, real)
//.text:00A3AEE0 ; void __cdecl render_texture_camera_render_mode(long)
//.text:00A3AF20 ; void __cdecl render_texture_camera_set_aspect_ratio(real)
//.text:00A3AF50 ; void __cdecl render_texture_camera_set_fov(real)
//.text:00A3AFB0 ; void __cdecl render_texture_camera_set_fov_frame_target(real)
//.text:00A3AFF0 ; void __cdecl render_texture_camera_set_object_marker(long, long, real)
//.text:00A3B0F0 ; void __cdecl render_texture_camera_set_position(real, real, real)
//.text:00A3B150 ; void __cdecl render_texture_camera_set_position_and_direction(bool, real_point3d const*, vector3d const*, vector3d const*, real)
//.text:00A3B240 ; 
//.text:00A3B280 ; void __cdecl render_texture_camera_set_resolution(long, long)
//.text:00A3B2E0 ; void __cdecl render_texture_camera_set_target(real, real, real)
//.text:00A3B330 ; void __cdecl render_texture_camera_target_object(long, long)
//.text:00A3B380 ; protected: void __cdecl c_player_view::render_transparents()

void __thiscall c_player_view::render_water()
{
	//INVOKE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);

	c_d3d_pix_event _water_render(g_rasterizer_profile_pix_colors[1], L"water_render");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_water);
}

void __thiscall c_player_view::render_weather_occlusion()
{
	//INVOKE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);

	c_d3d_pix_event _weather_occlusion(g_rasterizer_profile_pix_colors[1], L"weather_occlusion");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_weather_occlusion);
}

void c_player_view::setup_camera(long player_index, long window_count, long window_arrangement, e_output_user_index output_user_index, s_observer_result const* result, bool render_freeze)
{
	INVOKE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera, player_index, window_count, window_arrangement, output_user_index, result, render_freeze);
}

//.text:00A3B990 ; public: void __cdecl c_player_view::setup_camera_fx_parameters()
//.text:00A3BBA0 ; protected: void __cdecl c_player_view::setup_cinematic_clip_planes()
//.text:00A3BD10 ; protected: void __cdecl c_player_view::static_lighting_setup_internals()
//.text:00A3BDB0 ; protected: void __cdecl c_player_view::submit_attachments()

void __thiscall c_player_view::distortion_generate()
{
	//INVOKE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

	c_d3d_pix_event _distortion_generate(g_rasterizer_profile_pix_colors[1], L"distortion_generate");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, distortion_generate);
}

void c_player_view::frame_advance()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	effects_frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	effects_frame_advance_gpu(g_main_render_timing_data->game_seconds_elapsed);
	c_water_renderer::frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	c_patchy_fog::frame_advance_all(g_main_render_timing_data->game_seconds_elapsed);
}

