#include "networking/logic/network_banhammer.hpp"

REFERENCE_DECLARE(0x0226DAD0, bool, g_network_banhammer_initialized);
REFERENCE_DECLARE(0x0226DAD8, s_network_banhammer_globals, g_network_banhammer_globals);

bool __cdecl network_banhammer_initialize()
{
	return INVOKE(0x004D7EC0, network_banhammer_initialize);
}

