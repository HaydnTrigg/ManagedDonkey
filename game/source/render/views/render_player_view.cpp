#include "render/views/render_player_view.hpp"

#include "effects/screen_shader.hpp"
#include "game/game_engine_display.hpp"
#include "interface/interface_constants.hpp"
#include "interface/overhead_map.hpp"
#include "interface/user_interface.hpp"
#include "main/main_screenshot.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_occlusion_queries.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "rasterizer/rasterizer_render_targets.hpp"
#include "rasterizer/rasterizer_stipple.hpp"
#include "rasterizer/rasterizer_synchronization.hpp"
#include "render/render.hpp"
#include "render/render_flags.hpp"
#include "render/render_lens_flares.hpp"
#include "render/render_transparents.hpp"
#include "render/render_tron_effect.hpp"
#include "render/screen_postprocess.hpp"
#include "render/views/render_view.hpp"
#include "render_methods/render_method_submit.hpp"

REFERENCE_DECLARE(0x019147B8, real, g_particle_hack_near_fade_scale);
REFERENCE_DECLARE(0x019147BC, real, render_debug_depth_render_scale_r);
REFERENCE_DECLARE(0x019147C0, real, render_debug_depth_render_scale_g);
REFERENCE_DECLARE(0x019147C4, real, render_debug_depth_render_scale_b);
REFERENCE_DECLARE(0x050FB3FC, long, render_debug_depth_render);
REFERENCE_DECLARE(0x01694EC8, c_screen_postprocess::s_settings const* const, c_screen_postprocess::x_settings);

HOOK_DECLARE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);
HOOK_DECLARE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);
HOOK_DECLARE_CLASS_MEMBER(0x00A39960, c_player_view, render_);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);
HOOK_DECLARE_CLASS(0x00A3A310, c_player_view, render_albedo_decals);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A6C0, c_player_view, render_lens_flares);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);
HOOK_DECLARE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B380, c_player_view, render_transparents);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);
HOOK_DECLARE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);
HOOK_DECLARE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests);

void c_player_view::animate_water()
{
	//INVOKE_CLASS_MEMBER(0x00A38030, c_player_view, animate_water);

	c_rasterizer_profile_scope _water_animate(_rasterizer_profile_element_distortions, L"water_animate");

	c_water_renderer::ripple_apply();
	c_water_renderer::ripple_slope();
}

void __thiscall c_player_view::render_distortions()
{
	//INVOKE_CLASS_MEMBER(0x00A38040, c_player_view, render_distortions);

	c_rasterizer_profile_scope _distortion_apply(_rasterizer_profile_element_distortions, L"distortion_apply");

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
	//INVOKE_CLASS_MEMBER(0x00A39860, c_player_view, queue_patchy_fog);

	if (game_is_splitscreen_deterministic())
		return;

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, queue_patchy_fog);
}

// #TODO: move this
struct s_surface_group_description
{
	dword_flags flags;
	IDirect3DTexture9* Texture;
	real DepthWidth;
	real __unknownC;
	dword DepthFormat;
	byte __unknown14[4];
	dword Srgb;
	dword MultiSample;
	real __unknown20;
	real __unknown24;
	dword __unknown28;
	dword __unknown2C;
	dword __unknown30;
	dword __unknown34;
	dword __unknown38;
	dword __unknown3C;
	dword Alias;
	dword __unknown44;
	dword index_unknown_of_size_54;
};

// #TODO: move this
struct c_render_surface_group
{
	s_surface_group_description description;
	long m_count_specializations;
	c_render_surface* m_default_surface;
};

// #TODO: move this
struct c_render_surfaces_interface
{
	static c_render_surface* get_render_surface_default(c_rasterizer::e_surface surface)
	{
		return m_render_surface_groups[surface].m_default_surface;
	}

	static c_render_surface_group(&m_render_surface_groups)[60];
};

// #TODO: move this
REFERENCE_DECLARE_ARRAY(0x0510D8D0, c_render_surface_group, c_render_surfaces_interface::m_render_surface_groups, 60);

// #TODO: move this
void __cdecl sub_14E56A0(long player_index, c_player_view* player_view)
{
	INVOKE(0x014E56A0, sub_14E56A0, player_index, player_view);
}

// #TODO: move this
void __cdecl vision_mode_render(long player_index, c_player_view const* player_view, real a3, real a4, real a5, long a6, long a7)
{
	INVOKE(0x014E3EE0, vision_mode_render, player_index, player_view, a3, a4, a5, a6, a7);
}

void __thiscall c_player_view::renderA_()
{
	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	s_screen_effect_settings screen_effect_settings{};
	s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
	screen_effect_sample(
		&rasterizer_camera->position,
		&rasterizer_camera->forward,
		&screen_effect_settings,
		&screen_effect_shader_sample_result,
		m_output_user_index);
	
	c_rasterizer::set_current_splitscreen_res(m_splitscreen_res);
	
	if (!game_engine_suppress_render_scene(m_output_user_index))
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_game_states);

		render_method_clear_externs();

		s_cluster_reference cluster_reference{};
		c_world_view::get_starting_cluster(&cluster_reference);

		c_atmosphere_fog_interface::compute_cluster_weights(cluster_reference, &rasterizer_camera->position);

		{
			real horizontal_field_of_view = observer_get_camera(m_output_user_index)->horizontal_field_of_view;
			if (horizontal_field_of_view < k_real_epsilon)
				horizontal_field_of_view = k_real_epsilon;
			g_particle_hack_near_fade_scale = 1.0f / horizontal_field_of_view;
		}

		c_player_view::submit_attachments();

		bool water_updated = c_water_renderer::update_water_part_list();
		if (water_updated && render_water_interaction_enabled && c_water_renderer::is_active_ripple_exist())
		{
			c_player_view::animate_water();
		}

		{
			c_rasterizer_profile_scope _chud_turbulence(_rasterizer_profile_element_interface_hud, L"chud turbulence");
			chud_draw_turbulence(m_output_user_index);
		}

		c_player_view::setup_camera_fx_parameters(screen_effect_settings.__unknown0);
		c_player_view::setup_cinematic_clip_planes();
		m_lights_view.clear_simple_light_draw_list(m_output_user_index);
		m_lights_view.build_simple_light_draw_list(m_player_index);

		{
			c_atmosphere_fog_interface::sub_A397D0();

			s_weighted_atmosphere_parameters weighted_atmosphere_parameters{};
			c_atmosphere_fog_interface::populate_atmosphere_parameters(cluster_reference, &weighted_atmosphere_parameters);

			c_atmosphere_fog_interface::set_default_atmosphere_constants(&weighted_atmosphere_parameters);
			c_atmosphere_fog_interface::restore_atmosphere_constants();
		}

		rasterizer_stipple_initialize();
		c_decal_system::submit_all();

		bool rendering_albedo = c_player_view::render_albedo();

		if (screenshot_allow_postprocess())
			c_screen_postprocess::sub_A62710(
				&m_rasterizer_projection,
				&m_rasterizer_camera,
				&__matrix634,
				&m_projection_matrix,
				c_rasterizer::_surface_color_half_fp16_0,
				c_rasterizer::_surface_depth_fp32,
				c_rasterizer::_surface_color_half_fp16_1);

		if (rendering_albedo)
		{
			c_rasterizer::set_using_albedo_sampler(true);
			c_player_view::submit_occlusion_tests(false, true);

			if (__unknown26B4)
			{
				c_cpu_gpu_synchronizer::wait_for_gpu_to_catch_up();
				__unknown26B4 = false;
			}

			if (render_debug_toggle_default_static_lighting)
				c_player_view::render_static_lighting();

			if (render_debug_toggle_default_dynamic_lighting)
			{
				c_player_view::render_lightmap_shadows();

				c_rasterizer::setup_targets_static_lighting(
					__unknown29C,
					__unknown2A0,
					true,
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					false);

				IDirect3DSurface9* accum_LDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_LDR)->m_d3d_surface;
				IDirect3DSurface9* accum_HDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_HDR)->m_d3d_surface;
				IDirect3DSurface9* depth_stencil_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_depth_stencil)->m_d3d_surface;

				m_lights_view.render(
					m_output_user_index,
					m_player_index,
					accum_LDR_surface,
					accum_HDR_surface,
					depth_stencil_surface);
			}

			if (render_debug_toggle_default_sfx)
			{
				c_rasterizer::setup_targets_static_lighting(
					__unknown29C,
					__unknown2A0,
					c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					true);

				if (!render_debug_depth_render)
					c_player_view::render_first_person(false);

				if (render_debug_toggle_default_sfx)
				{
					if (c_rasterizer::get_is_tiling_enabled())
						c_tron_effect::resolve_and_process_z_camera(m_player_index, &rasterizer_camera->window_pixel_bounds, false);
					
					c_screen_postprocess::render_ssao(
						&m_rasterizer_projection,
						&m_rasterizer_camera,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_accum_HDR,
						c_rasterizer::_surface_color_half_fp16_0);

					if (screenshot_allow_postprocess())
						c_screen_postprocess::sub_A61CD0(
							&m_rasterizer_projection,
							&m_rasterizer_camera,
							c_rasterizer::_surface_accum_LDR,
							c_rasterizer::_surface_accum_HDR,
							c_rasterizer::_surface_normal,
							c_rasterizer::_surface_depth_fp32,
							c_rasterizer::_surface_gbuf,
							c_rasterizer::_surface_post_LDR,
							c_rasterizer::_surface_post_HDR,
							c_rasterizer::_surface_depth_half_fp32,
							c_rasterizer::_surface_color_half_fp16_0,
							c_rasterizer::_surface_color_half_fp16_1,
							c_rasterizer::_surface_normal_half,
							c_rasterizer::_surface_post_half_LDR);

					c_rasterizer::setup_targets_static_lighting(
						__unknown29C,
						__unknown2A0,
						c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);

					if (water_updated || render_underwater_fog_enabled)
						c_player_view::render_water();

					if (g_rasterizer_game_states->patchy_fog)
						c_player_view::queue_patchy_fog();

					c_player_view::render_transparents();

					c_player_view::distortion_generate();
					bool distortion_history = c_render_globals::get_distortion_visible();
					if (distortion_history || water_updated || render_underwater_fog_enabled)
						distortion_history = true;
					c_render_globals::set_distortion_history(distortion_history);

					{
						c_rasterizer_profile_scope _game_engine(_rasterizer_profile_element_game_engine, L"game_engine");
						game_engine_render(m_output_user_index);
					}

					c_player_view::render_distortions();

					if (!render_debug_depth_render)
					{
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), true);
						c_player_view::render_first_person(true);
					}

					{
						c_rasterizer_profile_scope _setup_targets_static_lighting_alpha_blend(_rasterizer_profile_element_total, L"setup_targets_static_lighting_alpha_blend");
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), false);
					}

					{
						c_rasterizer_profile_scope _render_lens_flares(_rasterizer_profile_element_total, L"render_lens_flares");
						c_player_view::render_lens_flares();
					}

					{
						c_rasterizer_profile_scope _submit_occlusion_tests(_rasterizer_profile_element_total, L"submit_occlusion_tests");
						c_player_view::submit_occlusion_tests(true, false);
					}

					{
						c_rasterizer_profile_scope _chud_draw_screen_LDR(_rasterizer_profile_element_total, L"chud_draw_screen_LDR");

						if (sub_A89440(m_output_user_index))
						{
							c_rasterizer::setup_targets_static_lighting_alpha_blend(false, true);
							c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f);
							c_rasterizer::set_render_target(1, c_rasterizer::_surface_none, 0xFFFFFFFF);
							sub_A88FE0(m_output_user_index);
						}
					}

					{
						c_rasterizer_profile_scope _CCCCCCCCCCCCCCCCCCC(_rasterizer_profile_element_total, L"CCCCCCCCCCCCCCCCCCC");
						render_setup_window(&m_rasterizer_camera, &m_rasterizer_projection);
						c_rasterizer::setup_targets_static_lighting(
							__unknown29C,
							__unknown2A0,
							true,
							c_camera_fx_values::g_HDR_target_stops,
							false,
							false,
							false);
						c_player_view::render_weather_occlusion();
					}
				}
			}

			if (screenshot_allow_postprocess())
			{
				c_rasterizer_profile_scope _postprocess_copy(_rasterizer_profile_element_total, L"postprocess copy");
				c_screen_postprocess::copy(
					c_rasterizer_globals::_explicit_shader_copy_surface,
					c_rasterizer::_surface_accum_LDR,
					c_rasterizer::_surface_albedo,
					c_rasterizer::_sampler_filter_mode_unknown1,
					c_rasterizer::_sampler_address_mode_unknown1,
					1.0f,
					1.0f,
					1.0f,
					1.0f,
					NULL);
			}
		}
	}
}


void __thiscall c_player_view::renderB_()
{
	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	s_screen_effect_settings screen_effect_settings{};
	s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
	screen_effect_sample(
		&rasterizer_camera->position,
		&rasterizer_camera->forward,
		&screen_effect_settings,
		&screen_effect_shader_sample_result,
		m_output_user_index);

	bool rendering_albedo = c_player_view::render_albedo();

	if (rendering_albedo)
	{
		if (screenshot_allow_postprocess())
		{
			c_rasterizer_profile_scope _postprocess(_rasterizer_profile_element_total, L"postprocess");

			c_screen_postprocess::postprocess_player_view(
				m_player_window->camera_fx_values,
				&m_rasterizer_projection,
				&m_rasterizer_camera,
				screen_effect_settings,
				m_splitscreen_res,
				&m_observer_depth_of_field,
				{});

			if (render_debug_toggle_default_lightmaps_texaccum == 3)
			{
				c_screen_postprocess::setup_rasterizer_for_postprocess(false);
				c_screen_postprocess::copy(
					c_rasterizer_globals::_explicit_shader_copy_surface,
					c_rasterizer::_surface_accum_LDR,
					c_rasterizer::_surface_disable,
					c_rasterizer::_sampler_filter_mode_unknown1,
					c_rasterizer::_sampler_address_mode_unknown1,
					1.0f,
					1.0f,
					1.0f,
					1.0f,
					NULL);
			}
		}
	}
}

void __thiscall c_player_view::renderC_()
{
	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	s_screen_effect_settings screen_effect_settings{};
	s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
	screen_effect_sample(
		&rasterizer_camera->position,
		&rasterizer_camera->forward,
		&screen_effect_settings,
		&screen_effect_shader_sample_result,
		m_output_user_index);

	if (screenshot_allow_ui_render())
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);

		c_rasterizer_profile_scope _interface_and_hud(_rasterizer_profile_element_interface_hud, L"interface_and_hud");

		c_rasterizer::restore_last_viewport();
		c_rasterizer::restore_last_scissor_rect();
		c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f);

		{
			c_rasterizer_profile_scope _vision_mode(_rasterizer_profile_element_total, L"vision_mode");
		
			if (screen_effect_settings.__unknown38 > 0.0f)
			{
				vision_mode_render(
					m_player_index,
					this,
					screen_effect_settings.__unknown38,
					g_main_render_timing_data->game_seconds_elapsed,
					screen_effect_settings.__unknown3C,
					0,
					0);
			}
			else
			{
				sub_14E56A0(
					m_player_index,
					this);
			}
		}

		render_screen_shaders(&screen_effect_shader_sample_result, 1, c_rasterizer::sub_A48770(), c_rasterizer::_surface_none, NULL);

		{
			c_rasterizer_profile_scope _chud_draw_screen(_rasterizer_profile_element_total, L"chud_draw_screen");

			// added by us
			c_rasterizer::set_viewport(rasterizer_camera->window_pixel_bounds, 0.0f, 1.0f);
			c_rasterizer::set_scissor_rect(&rasterizer_camera->window_pixel_bounds);

			chud_draw_screen(m_output_user_index);

			// added by us
			c_rasterizer::restore_last_viewport();
			c_rasterizer::restore_last_scissor_rect();
		}
		{
			c_rasterizer_profile_scope _fade_to_black(_rasterizer_profile_element_total, L"fade_to_black");
			game_engine_render_fade_to_black(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _user_interface_render(_rasterizer_profile_element_total, L"user_interface_render");

			short_rectangle2d display_pixel_bounds{};
			interface_get_current_window_settings(NULL, NULL, &display_pixel_bounds, NULL);
			user_interface_render(
				m_controller_index,
				m_output_user_index,
				m_player_index,
				&display_pixel_bounds,
				c_rasterizer::sub_A48770(),
				false);
		}
		{
			c_rasterizer_profile_scope _watermarks(_rasterizer_profile_element_total, L"watermarks");
			game_engine_render_watermarks();
		}
		{
			c_rasterizer_profile_scope _chud_draw_screen_saved_film(_rasterizer_profile_element_total, L"chud_draw_screen_saved_film");
			chud_draw_screen_saved_film(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _player_training(_rasterizer_profile_element_total, L"player_training");
			player_training_render_screen(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _debug_render(_rasterizer_profile_element_total, L"debug_render");
		}
		{
			c_rasterizer_profile_scope _overhead_map(_rasterizer_profile_element_total, L"overhead_map");
			overhead_map_render();
		}
		{
			c_rasterizer_profile_scope _player_effect_render(_rasterizer_profile_element_total, L"player_effect_render");
			player_effect_render(m_output_user_index);
		}
	}
}

void __thiscall c_player_view::renderD_()
{
	game_engine_render_debug(m_output_user_index);

	if (!sub_610260())
		render_debug_window_render(m_output_user_index);

	c_rasterizer::end();
}

void __thiscall c_player_view::render_()
{
	//HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_);

	render_camera* rasterizer_camera = get_rasterizer_camera_modifiable();

	s_screen_effect_settings screen_effect_settings{};
	s_screen_effect_shader_sample_result screen_effect_shader_sample_result{};
	screen_effect_sample(
		&rasterizer_camera->position,
		&rasterizer_camera->forward,
		&screen_effect_settings,
		&screen_effect_shader_sample_result,
		m_output_user_index);

	c_rasterizer::set_current_splitscreen_res(m_splitscreen_res);

	if (!game_engine_suppress_render_scene(m_output_user_index))
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_game_states);

		render_method_clear_externs();

		s_cluster_reference cluster_reference{};
		c_world_view::get_starting_cluster(&cluster_reference);

		c_atmosphere_fog_interface::compute_cluster_weights(cluster_reference, &rasterizer_camera->position);

		{
			real horizontal_field_of_view = observer_get_camera(m_output_user_index)->horizontal_field_of_view;
			if (horizontal_field_of_view < k_real_epsilon)
				horizontal_field_of_view = k_real_epsilon;
			g_particle_hack_near_fade_scale = 1.0f / horizontal_field_of_view;
		}

		c_player_view::submit_attachments();

		bool water_updated = c_water_renderer::update_water_part_list();
		if (water_updated && render_water_interaction_enabled && c_water_renderer::is_active_ripple_exist())
		{
			c_player_view::animate_water();
		}

		{
			c_rasterizer_profile_scope _chud_turbulence(_rasterizer_profile_element_interface_hud, L"chud turbulence");
			chud_draw_turbulence(m_output_user_index);
		}

		c_player_view::setup_camera_fx_parameters(screen_effect_settings.__unknown0);
		c_player_view::setup_cinematic_clip_planes();
		m_lights_view.clear_simple_light_draw_list(m_output_user_index);
		m_lights_view.build_simple_light_draw_list(m_player_index);

		{
			c_atmosphere_fog_interface::sub_A397D0();

			s_weighted_atmosphere_parameters weighted_atmosphere_parameters{};
			c_atmosphere_fog_interface::populate_atmosphere_parameters(cluster_reference, &weighted_atmosphere_parameters);

			c_atmosphere_fog_interface::set_default_atmosphere_constants(&weighted_atmosphere_parameters);
			c_atmosphere_fog_interface::restore_atmosphere_constants();
		}

		rasterizer_stipple_initialize();
		c_decal_system::submit_all();

		bool rendering_albedo = c_player_view::render_albedo();

		if (screenshot_allow_postprocess())
			c_screen_postprocess::sub_A62710(
				&m_rasterizer_projection,
				&m_rasterizer_camera,
				&__matrix634,
				&m_projection_matrix,
				c_rasterizer::_surface_color_half_fp16_0,
				c_rasterizer::_surface_depth_fp32,
				c_rasterizer::_surface_color_half_fp16_1);

		if (rendering_albedo)
		{
			c_rasterizer::set_using_albedo_sampler(true);
			c_player_view::submit_occlusion_tests(false, true);

			if (__unknown26B4)
			{
				c_cpu_gpu_synchronizer::wait_for_gpu_to_catch_up();
				__unknown26B4 = false;
			}

			if (render_debug_toggle_default_static_lighting)
				c_player_view::render_static_lighting();

			if (render_debug_toggle_default_dynamic_lighting)
			{
				c_player_view::render_lightmap_shadows();

				c_rasterizer::setup_targets_static_lighting(
					__unknown29C,
					__unknown2A0,
					true,
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					false);

				IDirect3DSurface9* accum_LDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_LDR)->m_d3d_surface;
				IDirect3DSurface9* accum_HDR_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_accum_HDR)->m_d3d_surface;
				IDirect3DSurface9* depth_stencil_surface = c_render_surfaces_interface::get_render_surface_default(c_rasterizer::_surface_depth_stencil)->m_d3d_surface;

				m_lights_view.render(
					m_output_user_index,
					m_player_index,
					accum_LDR_surface,
					accum_HDR_surface,
					depth_stencil_surface);
			}

			if (render_debug_toggle_default_sfx)
			{
				c_rasterizer::setup_targets_static_lighting(
					__unknown29C,
					__unknown2A0,
					c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
					c_camera_fx_values::g_HDR_target_stops,
					false,
					false,
					true);

				if (!render_debug_depth_render)
					c_player_view::render_first_person(false);

				if (render_debug_toggle_default_sfx)
				{
					if (c_rasterizer::get_is_tiling_enabled())
						c_tron_effect::resolve_and_process_z_camera(m_player_index, &rasterizer_camera->window_pixel_bounds, false);

					c_screen_postprocess::render_ssao(
						&m_rasterizer_projection,
						&m_rasterizer_camera,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_accum_HDR,
						c_rasterizer::_surface_color_half_fp16_0);

					if (screenshot_allow_postprocess())
						c_screen_postprocess::sub_A61CD0(
							&m_rasterizer_projection,
							&m_rasterizer_camera,
							c_rasterizer::_surface_accum_LDR,
							c_rasterizer::_surface_accum_HDR,
							c_rasterizer::_surface_normal,
							c_rasterizer::_surface_depth_fp32,
							c_rasterizer::_surface_gbuf,
							c_rasterizer::_surface_post_LDR,
							c_rasterizer::_surface_post_HDR,
							c_rasterizer::_surface_depth_half_fp32,
							c_rasterizer::_surface_color_half_fp16_0,
							c_rasterizer::_surface_color_half_fp16_1,
							c_rasterizer::_surface_normal_half,
							c_rasterizer::_surface_post_half_LDR);

					c_rasterizer::setup_targets_static_lighting(
						__unknown29C,
						__unknown2A0,
						c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(),
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);

					if (water_updated || render_underwater_fog_enabled)
						c_player_view::render_water();

					if (g_rasterizer_game_states->patchy_fog)
						c_player_view::queue_patchy_fog();

					c_player_view::render_transparents();

					c_player_view::distortion_generate();
					bool distortion_history = c_render_globals::get_distortion_visible();
					if (distortion_history || water_updated || render_underwater_fog_enabled)
						distortion_history = true;
					c_render_globals::set_distortion_history(distortion_history);

					{
						c_rasterizer_profile_scope _game_engine(_rasterizer_profile_element_game_engine, L"game_engine");
						game_engine_render(m_output_user_index);
					}

					c_player_view::render_distortions();

					if (!render_debug_depth_render)
					{
						c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), true);
						c_player_view::render_first_person(true);
					}

					c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), false);

					c_player_view::render_lens_flares();
					c_player_view::submit_occlusion_tests(true, false);

					{
						c_rasterizer_profile_scope _chud_draw_screen_LDR(_rasterizer_profile_element_total, L"chud_draw_screen_LDR");

						if (sub_A89440(m_output_user_index))
						{
							c_rasterizer::setup_targets_static_lighting_alpha_blend(false, true);
							c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f);
							c_rasterizer::set_render_target(1, c_rasterizer::_surface_none, 0xFFFFFFFF);
							sub_A88FE0(m_output_user_index);
						}
					}

					render_setup_window(&m_rasterizer_camera, &m_rasterizer_projection);
					c_rasterizer::setup_targets_static_lighting(
						__unknown29C,
						__unknown2A0,
						true,
						c_camera_fx_values::g_HDR_target_stops,
						false,
						false,
						false);
					c_player_view::render_weather_occlusion();
				}
			}

			if (screenshot_allow_postprocess())
			{
				c_screen_postprocess::postprocess_player_view(
					m_player_window->camera_fx_values,
					&m_rasterizer_projection,
					&m_rasterizer_camera,
					screen_effect_settings,
					m_splitscreen_res,
					&m_observer_depth_of_field,
					m_output_user_index);

				if (render_debug_toggle_default_lightmaps_texaccum == 3)
				{
					c_screen_postprocess::setup_rasterizer_for_postprocess(false);
					c_screen_postprocess::copy(
						c_rasterizer_globals::_explicit_shader_copy_surface,
						c_rasterizer::_surface_accum_LDR,
						c_rasterizer::_surface_disable,
						c_rasterizer::_sampler_filter_mode_unknown1,
						c_rasterizer::_sampler_address_mode_unknown1,
						1.0f,
						1.0f,
						1.0f,
						1.0f,
						NULL);
				}
			}
		}
	}

	if (screenshot_allow_ui_render())
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);

		c_rasterizer_profile_scope _interface_and_hud(_rasterizer_profile_element_interface_hud, L"interface_and_hud");

		c_rasterizer::restore_last_viewport();
		c_rasterizer::restore_last_scissor_rect();
		c_rasterizer::setup_render_target_globals_with_exposure(1.0f, 1.0f, 1.0f);

		{
			c_rasterizer_profile_scope _vision_mode(_rasterizer_profile_element_total, L"vision_mode");

			if (screen_effect_settings.__unknown38 <= 0.0f)
			{
				sub_14E56A0(
					m_player_index,
					this);
			}
			else
			{
				vision_mode_render(
					m_player_index,
					this,
					screen_effect_settings.__unknown38,
					g_main_render_timing_data->game_seconds_elapsed,
					screen_effect_settings.__unknown3C,
					0,
					0);
			}
		}

		render_screen_shaders(&screen_effect_shader_sample_result, 1, c_rasterizer::sub_A48770(), c_rasterizer::_surface_none, NULL);

		{
			c_rasterizer_profile_scope _chud_draw_screen(_rasterizer_profile_element_total, L"chud_draw_screen");

			// added by us
			c_rasterizer::set_viewport(rasterizer_camera->window_pixel_bounds, 0.0f, 1.0f);
			c_rasterizer::set_scissor_rect(&rasterizer_camera->window_pixel_bounds);

			chud_draw_screen(m_output_user_index);

			// added by us
			c_rasterizer::restore_last_viewport();
			c_rasterizer::restore_last_scissor_rect();
		}
		{
			c_rasterizer_profile_scope _fade_to_black(_rasterizer_profile_element_total, L"fade_to_black");
			game_engine_render_fade_to_black(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _user_interface_render(_rasterizer_profile_element_total, L"user_interface_render");

			short_rectangle2d display_pixel_bounds{};
			interface_get_current_window_settings(NULL, NULL, &display_pixel_bounds, NULL);
			user_interface_render(
				m_controller_index,
				m_output_user_index,
				m_player_index,
				&display_pixel_bounds,
				c_rasterizer::sub_A48770(),
				false);
		}
		{
			c_rasterizer_profile_scope _watermarks(_rasterizer_profile_element_total, L"watermarks");
			game_engine_render_watermarks();
		}
		{
			c_rasterizer_profile_scope _chud_draw_screen_saved_film(_rasterizer_profile_element_total, L"chud_draw_screen_saved_film");
			chud_draw_screen_saved_film(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _player_training(_rasterizer_profile_element_total, L"player_training");
			player_training_render_screen(m_output_user_index);
		}
		{
			c_rasterizer_profile_scope _debug_render(_rasterizer_profile_element_total, L"debug_render");
		}
		{
			c_rasterizer_profile_scope _overhead_map(_rasterizer_profile_element_total, L"overhead_map");
			overhead_map_render();
		}
		{
			c_rasterizer_profile_scope _player_effect_render(_rasterizer_profile_element_total, L"player_effect_render");
			player_effect_render(m_output_user_index);
		}
	}

	game_engine_render_debug(m_output_user_index);

	if (!sub_610260())
		render_debug_window_render(m_output_user_index);

	c_rasterizer::end();
}

bool __thiscall c_player_view::render_albedo()
{
	//return INVOKE_CLASS_MEMBER(0x00A3A0E0, c_player_view, render_albedo);

	c_rasterizer_profile_scope _albedo(_rasterizer_profile_element_texaccum, L"albedo");

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_player_view, render_albedo);
	return result;
}

void __cdecl c_player_view::render_albedo_decals(bool a1, bool a2)
{
	//INVOKE(0x00A3A310, c_player_view::render_albedo_decals, a1, a2);

	c_rasterizer_profile_scope _decorators(_rasterizer_profile_element_total, L"decorators");

	HOOK_INVOKE_CLASS(, c_player_view, render_albedo_decals, decltype(&c_player_view::render_albedo_decals), a1, a2);
}

//.text:00A3A3C0 ; 

void __thiscall c_player_view::render_effects(e_effect_pass pass)
{
	//INVOKE_CLASS_MEMBER(0x00A3A3F0, c_player_view, render_effects, pass);

	c_rasterizer_profile_scope _render_effects(_rasterizer_profile_element_effects, L"render_effects");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_effects, pass);
}

void __thiscall c_player_view::render_first_person(bool a1)
{
	//INVOKE_CLASS_MEMBER(0x00A3A420, c_player_view, render_first_person);

	c_rasterizer_profile_scope _first_person(_rasterizer_profile_element_total, L"first_person");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person, a1);
}

void __thiscall c_player_view::render_first_person_albedo()
{
	//INVOKE_CLASS_MEMBER(0x00A3A5F0, c_player_view, render_first_person_albedo);

	c_rasterizer_profile_scope _first_person_albedo(_rasterizer_profile_element_total, L"first_person_albedo");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_first_person_albedo);
}

//.text:00A3A6B0 ; 

void __thiscall c_player_view::render_lens_flares()
{
	//INVOKE_CLASS_MEMBER(0x00A3A6C0, c_player_view::render_lens_flares);

	c_rasterizer::set_using_albedo_sampler(false);
	rasterizer_occlusions_retrieve(m_player_index);

	{
		c_rasterizer_profile_scope _lens_flares(_rasterizer_profile_element_transparents, L"lens flares");
		lens_flares_render(m_output_user_index);
	}

	c_rasterizer::set_using_albedo_sampler(c_rasterizer::surface_valid(c_rasterizer::_surface_albedo));
}

void __thiscall c_player_view::render_lightmap_shadows()
{
	//INVOKE_CLASS_MEMBER(0x00A3A700, c_player_view, render_lightmap_shadows);

	c_rasterizer_profile_scope _lightmap_shadows(_rasterizer_profile_element_total, L"lightmap_shadows");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_lightmap_shadows);
}

void __cdecl c_player_view::render_misc_transparents()
{
	c_rasterizer_profile_scope _render_misc_transparents(_rasterizer_profile_element_widgets, L"render_misc_transparents");

	INVOKE(0x00A3A790, c_player_view::render_misc_transparents);
}

//.text:00A3A7A0 ; public: void __cdecl c_player_view::render_patchy_fog()
//.text:00A3A7F0 ; render_patchy_fog_callback
//.text:00A3A850 ; public: virtual void __cdecl c_player_view::render_setup()

void __thiscall c_player_view::render_static_lighting()
{
	//INVOKE_CLASS_MEMBER(0x00A3A8C0, c_player_view, render_static_lighting);

	c_rasterizer_profile_scope _static_lighting(_rasterizer_profile_element_static_lighting, L"static_lighting");

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

void __thiscall c_player_view::render_transparents()
{
	//INVOKE_CLASS_MEMBER(0x00A3B380, c_player_view, render_transparents);

	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_z, 0);
	render_method_submit_extern_texture_static(_render_method_extern_scene_ldr_texture, 0);
	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_texaccum, 1);
	render_method_submit_extern_texture_static(_render_method_extern_texture_global_target_normal, 1);
	m_lights_view.submit_simple_light_draw_list_to_shader();

	{
		c_rasterizer_profile_scope _transparents(_rasterizer_profile_element_transparents, L"transparents");

		c_rasterizer::setup_targets_static_lighting_alpha_blend(c_screen_postprocess::x_settings->__unknown0 || screenshot_in_progress(), true);
		c_transparency_renderer::set_active_camo_bounds(&m_rasterizer_camera.window_pixel_bounds, &m_rasterizer_camera.render_pixel_bounds);
		c_rasterizer::set_using_albedo_sampler(false);

		{
			c_rasterizer_profile_scope _transparents_sky(_rasterizer_profile_element_transparents, L"transparents: sky");
			c_object_renderer::submit_and_render_sky(2, m_player_index);
		}

		{
			c_rasterizer_profile_scope _transparents_misc(_rasterizer_profile_element_transparents, L"transparents: misc");
			render_misc_transparents();
		}

		c_player_view::render_effects(_effect_pass_transparents);

		{
			c_rasterizer_profile_scope _transparents_other(_rasterizer_profile_element_transparents, L"transparents: other");
			c_transparency_renderer::sort();
			c_transparency_renderer::render(true);
			c_rasterizer::set_using_albedo_sampler(c_rasterizer::surface_valid(c_rasterizer::_surface_albedo));
		}
	}

	render_method_clear_extern(_render_method_extern_texture_global_target_texaccum);
	render_method_clear_extern(_render_method_extern_texture_global_target_normal);
}

void __thiscall c_player_view::render_water()
{
	//INVOKE_CLASS_MEMBER(0x00A3B470, c_player_view, render_water);

	c_rasterizer_profile_scope _water_render(_rasterizer_profile_element_water, L"water_render");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_water);
}

void __thiscall c_player_view::render_weather_occlusion()
{
	//INVOKE_CLASS_MEMBER(0x00A3B500, c_player_view, render_weather_occlusion);

	c_rasterizer_profile_scope _weather_occlusion(_rasterizer_profile_element_total, L"weather_occlusion");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, render_weather_occlusion);
}

void c_player_view::setup_camera(long player_index, long window_count, long window_arrangement, e_output_user_index output_user_index, s_observer_result const* result, bool render_freeze)
{
	INVOKE_CLASS_MEMBER(0x00A3B7F0, c_player_view, setup_camera, player_index, window_count, window_arrangement, output_user_index, result, render_freeze);
}

void __thiscall c_player_view::setup_camera_fx_parameters(real a1)
{
	INVOKE_CLASS_MEMBER(0x00A3B990, c_player_view, setup_camera_fx_parameters, a1);
}

void __thiscall c_player_view::setup_cinematic_clip_planes()
{
	INVOKE_CLASS_MEMBER(0x00A3BBA0, c_player_view, setup_cinematic_clip_planes);
}

//.text:00A3BD10 ; protected: void __cdecl c_player_view::static_lighting_setup_internals()

void __thiscall c_player_view::submit_attachments()
{
	INVOKE_CLASS_MEMBER(0x00A3BDB0, c_player_view, submit_attachments);
}

void __thiscall c_player_view::distortion_generate()
{
	//INVOKE_CLASS_MEMBER(0x00A3BDF0, c_player_view, distortion_generate);

	c_rasterizer_profile_scope _distortion_generate(_rasterizer_profile_element_distortions, L"distortion_generate");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, distortion_generate);
}

void __thiscall c_player_view::submit_occlusion_tests(bool a1, bool a2)
{
	//INVOKE_CLASS_MEMBER(0x00A3BF20, c_player_view, submit_occlusion_tests, a1, a2);

	c_rasterizer_profile_scope _occlusion_tests(_rasterizer_profile_element_occlusions, L"occlusion_tests");

	HOOK_INVOKE_CLASS_MEMBER(, c_player_view, submit_occlusion_tests, a1, a2);
}

void c_player_view::frame_advance()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_main_render_timing_data);
	effects_frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	effects_frame_advance_gpu(g_main_render_timing_data->game_seconds_elapsed);
	c_water_renderer::frame_advance(g_main_render_timing_data->game_seconds_elapsed);
	c_patchy_fog::frame_advance_all(g_main_render_timing_data->game_seconds_elapsed);
}

