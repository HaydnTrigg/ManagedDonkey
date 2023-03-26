#include "game/game_engine_juggernaut.hpp"

#include "memory/byte_swapping.hpp"


void c_game_engine_juggernaut_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_score_to_win_round);
	bswap_word_inplace(m_score_unknown);

	ASSERT(array_is_zeroed(m_pad));

	m_juggernaut_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

