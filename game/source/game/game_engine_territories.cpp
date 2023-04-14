#include "game/game_engine_territories.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F80, c_game_engine_territories_variant*, territories_variant);

void c_game_engine_territories_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_word_inplace(m_variant_flags);
	bswap_word_inplace(m_respawn_on_capture);
	bswap_word_inplace(m_capture_time);
	bswap_word_inplace(m_sudden_death_time);
	m_defender_traits.byteswap();
	m_attacker_traits.byteswap();
}

void c_territories_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

