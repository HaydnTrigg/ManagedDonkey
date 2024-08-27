#include "scenario/scenario_pvs.hpp"

void __cdecl scenario_zone_set_pvs_get_row(long scenario_index, s_scenario_pvs_row* pvs_row, long zone_set_index, s_cluster_reference cluster_reference, bool a5)
{
	INVOKE(0x005F1050, scenario_zone_set_pvs_get_row, scenario_index, pvs_row, zone_set_index, cluster_reference, a5);
}

bool __cdecl scenario_zone_set_pvs_row_test(long scenario_index, s_scenario_pvs_row const* pvs_row, s_cluster_reference cluster_reference)
{
	INVOKE(0x005F1240, scenario_zone_set_pvs_row_test, scenario_index, pvs_row, cluster_reference);
}

void __cdecl scenario_zone_set_pvs_write_row(s_game_cluster_bit_vectors* v0, s_scenario_pvs_row const* v1)
{
	INVOKE(0x005F1280, scenario_zone_set_pvs_write_row, v0, v1);
}
