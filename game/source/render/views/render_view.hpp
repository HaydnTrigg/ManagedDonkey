#pragma once

#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "render/depth_of_field.hpp"
#include "render/render_cameras.hpp"
#include "render/render_game_state.hpp"
#include "render/render_patchy_fog.hpp"

enum e_output_user_index;
struct s_observer_result;

// 0165DB98
struct c_view
{
protected:

	// HACK: so we don't have to manually construct the class
	struct
	{
		void(__thiscall* render)(c_view*);
		long(__thiscall* render_setup)(c_view*);
		long(__thiscall* compute_visibility)(c_view*);
		long(__thiscall* render_submit_visibility)(c_view*);
	}* __vftable;

public:
	c_view() :
		__vftable(reinterpret_cast<decltype(__vftable)>(0x0165DB98)),
		m_rasterizer_camera(),
		m_rasterizer_projection(),
		m_render_camera(),
		m_render_projection()
	{
	}

	void render() { __vftable->render(this); }
	long render_setup() { return __vftable->render_setup(this); }
	long compute_visibility() { return __vftable->compute_visibility(this); }
	long render_submit_visibility() { return __vftable->render_submit_visibility(this); }

	static void __cdecl abort_current_view_stack();
	static void __cdecl begin(c_view* view);
	static void __cdecl end();
	static long __cdecl get_current_stack_level();
	static c_view* __cdecl top();

	render_camera const* get_render_camera() const;
	render_camera* get_render_camera_modifiable();

	render_camera const* get_rasterizer_camera() const;
	render_camera* get_rasterizer_camera_modifiable();

	render_projection const* get_rasterizer_projection() const;
	render_projection* get_rasterizer_projection_modifiable();

	render_projection const* get_render_projection() const;
	render_projection* get_render_projection_modifiable();

protected:
	static long& g_view_stack_top;
	static c_view*(&g_view_stack)[4];

	render_camera m_rasterizer_camera;
	render_projection m_rasterizer_projection;
	render_camera m_render_camera;
	render_projection m_render_projection;
};
static_assert(sizeof(c_view) == 0x294);

// 0165DBAC
struct c_ui_view :
	public c_view
{
public:
	c_ui_view() :
		c_view(),
		__unknown298(),
		m_render_target()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBAC);
	}

	void setup_camera(s_observer_result const* result, c_rasterizer::e_surface surface);

	// if true bypass if statement in `c_gui_widget::render`
	bool __unknown298;

	c_rasterizer::e_surface m_render_target;
};
static_assert(sizeof(c_ui_view) == sizeof(c_view) + 0x8);

// 0165DBC0
struct c_fullscreen_view :
	public c_view
{
public:
	c_fullscreen_view() :
		c_view()
	{
		__vftable = reinterpret_cast<decltype(__vftable)>(0x0165DBC0);
	}

	void setup_camera(s_observer_result const* result);
	void render_blank_frame(real_rgb_color const* color);
};
static_assert(sizeof(c_fullscreen_view) == sizeof(c_view));

// 01692BDC
struct c_world_view :
	public c_view
{
public:
	void get_starting_cluster(s_cluster_reference* starting_cluster);

//protected:
	s_location m_location;
	byte __data286[0x2];
};
static_assert(sizeof(c_world_view) == sizeof(c_view) + 0x4);

__interface IDirect3DSurface9;

// 0165E0C4
struct c_lights_view :
	public c_world_view
{
public:
	void submit_simple_light_draw_list_to_shader() const;
	void build_simple_light_draw_list(long a1);
	void clear_simple_light_draw_list(long a1);
	void render(e_output_user_index output_user_index, long player_index, IDirect3DSurface9* a3, IDirect3DSurface9* a4, IDirect3DSurface9* a5);

	static long& g_gel_bitmap_index;
	static real& g_render_light_intensity;
	static dword& g_debug_clip_planes;

protected:
	byte __data298[0xD54];
	vector4d m_light_draw_list[40];
	real m_light_intensity_scale;
};
static_assert(sizeof(c_lights_view) == sizeof(c_world_view) + 0xFD8);

// 0165E0D8
struct c_lightmap_shadows_view :
	public c_world_view
{
protected:
	byte __data298[0x40];
};
static_assert(sizeof(c_lightmap_shadows_view) == sizeof(c_world_view) + 0x40);

// 0165E0EC
struct c_occlusion_view :
	public c_lightmap_shadows_view
{
};
static_assert(sizeof(c_occlusion_view) == sizeof(c_lightmap_shadows_view));

// 0165E108
struct c_reflection_view :
	public c_view
{
};
static_assert(sizeof(c_reflection_view) == sizeof(c_view));

// 0165E11C
struct c_first_person_view :
	public c_view
{
public:
	void __thiscall override_projection(bool first_person_squish);

//protected:

	// c_first_person_camera::update
	// c_first_person_view::override_projection
	// hs_external_globals[242]
	static real& m_fov_scale;

	// c_first_person_view::override_projection
	static real& m_z_far_scale; // from reach, missing `g_enable_first_person_squish && ` in if statement, always `1.0f`

	render_camera* m_camera;
};
static_assert(sizeof(c_first_person_view) == sizeof(c_view) + 0x4);

enum e_controller_index;
enum e_effect_pass;
enum e_splitscreen_res;

// 0165E130
struct c_player_view :
	public c_world_view
{
public:
	static c_player_view*& x_current_player_view;
	static c_player_view(&x_global_player_views)[4];

	static c_player_view* __cdecl get_current()
	{
		return x_current_player_view;
	}

	static c_player_view* __cdecl get_current(long view_index)
	{
		return &x_global_player_views[view_index];
	}

	static void __cdecl set_global_player_view(c_player_view* view)
	{
		// asserts

		x_current_player_view = view;
	}

	e_output_user_index get_player_view_output_user_index()
	{
		return m_output_user_index;
	}

protected:
	void animate_water();

public:
	void __thiscall render_distortions();
	void create_frame_textures(long player_index);
	static void __cdecl get_player_render_camera_orientation(real_matrix4x3* camera);
	void __thiscall queue_patchy_fog();
	void __thiscall render_();
	void __thiscall renderA_();
	void __thiscall renderB_();
	void __thiscall renderC_();
	void __thiscall renderD_();
	bool __thiscall render_albedo();
	static void __cdecl render_albedo_decals(bool a1, bool a2);
	void __thiscall render_effects(e_effect_pass pass);
	void __thiscall render_first_person(bool a1);
	void __thiscall render_first_person_albedo();
	void __thiscall render_lens_flares();
	void __thiscall render_lightmap_shadows();
	static void __cdecl render_misc_transparents();
	void __thiscall render_static_lighting();
	void __thiscall render_transparents();
	void __thiscall render_water();
	void __thiscall render_weather_occlusion();
	void setup_camera(long player_index, long window_count, long window_arrangement, e_output_user_index output_user_index, s_observer_result const* result, bool render_freeze);
	void __thiscall setup_camera_fx_parameters(real a1);
	void __thiscall setup_cinematic_clip_planes();
	void __thiscall submit_attachments();
	void __thiscall distortion_generate();
	void __thiscall submit_occlusion_tests(bool a1, bool a2);

	static void frame_advance();

//protected:
	// c_camera_fx_values?
	s_render_game_state::s_player_window* m_player_window;
	real __unknown29C;
	real __unknown2A0;

	s_observer_depth_of_field m_observer_depth_of_field;

	c_patchy_fog m_patchy_fog;

	// struct?
	real_point3d m_position;
	vector3d m_forward;
	vector3d m_up;
	long __unknown630;
	real_matrix4x3 __matrix634;
	s_oriented_bounding_box m_projection_matrix;

	c_first_person_view m_first_person_view;
	c_ui_view m_ui_view;
	c_lights_view m_lights_view;
	c_lightmap_shadows_view m_lightmap_shadows_view;
	c_reflection_view m_reflection_view;
	c_occlusion_view m_occlusion_view;

	short_rectangle2d __rectangle2690;

	long m_player_index;
	long m_player_view_count;
	long m_player_view_arrangement;
	e_output_user_index m_output_user_index;
	e_controller_index m_controller_index;
	e_splitscreen_res m_splitscreen_res;

	long __unknown26B0;

	// __unknown26B4 = window_index == iterator.get_window_count() - 1
	// is_last_window?
	bool __unknown26B4;
};
static_assert(sizeof(c_player_view) == sizeof(c_world_view) + 0x2420);

struct c_hud_camera_view :
	public c_player_view
{
public:
	c_hud_camera_view* constructor();

	void render(long player_index, c_player_view const* player_view, void* data);
};
static_assert(sizeof(c_hud_camera_view) == sizeof(c_player_view));

struct c_texture_camera_view :
	public c_player_view
{
protected:
	c_rasterizer_texture_ref m_accumulation_texture_ref;
	c_rasterizer_texture_ref m_albedo_texture_ref;
	c_rasterizer_texture_ref m_normal_texture_ref;
	c_rasterizer_texture_ref __unknown26C4_texture_ref;
	c_rasterizer_texture_ref __unknown26C8_texture_ref;
	long __unknown26CC;
	long m_depth_stencil_surface_index;
	dword_flags __unknown26D4_flags;
	long __unknown26D8;
	long __unknown26DC;
	long __unknown26E0;
	long __unknown26E4;
};
static_assert(sizeof(c_texture_camera_view) == sizeof(c_player_view) + 0x30);

extern bool render_debug_pix_events;

extern void __cdecl render_debug_frame_render();
extern void __cdecl render_debug_window_render(long user_index);

