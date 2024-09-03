#include "scenario/scenario_trigger_volumes.hpp"

//.text:005FA190 ; public: c_trigger_volume_query::c_trigger_volume_query(scenario_trigger_volume const*)
//.text:005FA1D0 ; public: real c_trigger_volume_query::distance_to_face(long, real_point3d const*) const
//.text:005FA210 ; public: real c_trigger_volume_query::distance_to_face_local(long, real_point3d const*) const
//.text:005FA380 ; public: void c_trigger_volume_query::get_local_bounds(real_rectangle3d*) const
//.text:005FA450 ; 
//.text:005FA4E0 ; bool __cdecl scenario_trigger_volume_get_matrix(long, real_matrix4x3*)
//.text:005FA500 ; long __cdecl scenario_trigger_volume_return_objects(long)
//.text:005FA520 ; long __cdecl scenario_trigger_volume_return_objects_by_type(long, long)
//.text:005FA680 ; bool __cdecl scenario_trigger_volume_test_object(long, long)
//.text:005FA6E0 ; bool __cdecl scenario_trigger_volume_test_players(long)
//.text:005FA780 ; bool __cdecl scenario_trigger_volume_test_players_all(long)
//.text:005FA820 ; bool __cdecl scenario_trigger_volume_test_point(long, real_point3d const*)
//.text:005FAB10 ; public: real_point3d* c_trigger_volume_query::transform_to_trigger_space(real_point3d const*, real_point3d*) const
//.text:005FAB30 ; public: vector3d* c_trigger_volume_query::transform_to_trigger_space(vector3d const*, vector3d*) const

bool __cdecl trigger_volume_build_faces(scenario_trigger_volume const* volume, real_point3d(&faces)[k_faces_per_cube_count][4])
{
	//return INVOKE(0x005FAB50, trigger_volume_build_faces, volume, faces);

	real_matrix4x3 matrix{};
	if (trigger_volume_get_matrix(volume, &matrix))
	{
		real_rectangle3d rectangle{};
		rectangle.x.lower = 0.0f;
		rectangle.x.upper = volume->extents.x;
		rectangle.y.lower = 0.0f;
		rectangle.y.upper = volume->extents.y;
		rectangle.z.lower = 0.0f;
		rectangle.z.upper = volume->extents.z;

		rectangle3d_build_faces(&rectangle, k_faces_per_cube_count, faces);
		for (long i = 0; i < k_faces_per_cube_count; ++i)
			matrix4x3_transform_points(&matrix, 4, faces[i], faces[i]);

		return true;
	}

	return false;
}

//.text:005FABF0 ; long __cdecl trigger_volume_face_index_to_axis(long)
//.text:005FAC00 ; e_trigger_volume_side __cdecl trigger_volume_face_index_to_side(long)

bool __cdecl trigger_volume_get_center(scenario_trigger_volume const* volume, union real_point3d* center)
{
	return INVOKE(0x005FAC20, trigger_volume_get_center, volume, center);
}

//.text:005FACB0 ; real __cdecl trigger_volume_get_height(scenario_trigger_volume const*)
//.text:005FADE0 ; void __cdecl trigger_volume_get_position(scenario_trigger_volume const*, real_point3d*)

bool __cdecl trigger_volume_get_matrix(scenario_trigger_volume const* volume, real_matrix4x3* matrix)
{
	return INVOKE(0x005FACC0, trigger_volume_get_matrix, volume, matrix);
}

real __cdecl trigger_volume_get_radius(scenario_trigger_volume const* volume)
{
	return INVOKE(0x005FAF10, trigger_volume_get_radius, volume);
}

//.text:005FAF20 ; s_sector_point_template<real_point3d>* __cdecl trigger_volume_get_sector_point(scenario_trigger_volume const*, long)
//.text:005FAF40 ; real __cdecl trigger_volume_get_sink(scenario_trigger_volume const*)
//.text:005FAFE0 ; s_trigger_volume_triangle const* __cdecl trigger_volume_get_triangle(scenario_trigger_volume const*, long)
//.text:005FB000 ; void __cdecl trigger_volume_set_height(scenario_trigger_volume*, real)
//.text:005FB020 ; void __cdecl trigger_volume_set_position(scenario_trigger_volume*, real_point3d const*)
//.text:005FB0C0 ; void __cdecl trigger_volume_set_sink(scenario_trigger_volume*, real)

