#include "game/game_options.hpp"

#include <string.h>
#include <assert.h>

c_game_variant::c_game_variant()
{
	m_game_engine_index = _game_engine_base_variant;
	memset(m_game_engine_variant, 0, sizeof(m_game_engine_variant));
}

e_game_engine_type c_game_variant::get_game_engine_index() const
{
	return m_game_engine_index;
}

void c_game_variant::set_game_engine_index(e_game_engine_type game_engine_index)
{
	m_game_engine_index = game_engine_index;
}

c_game_engine_base_variant const* c_game_variant::get_active_variant() const
{
	return reinterpret_cast<c_game_engine_base_variant const*>(m_game_engine_variant);
}

c_game_engine_base_variant* c_game_variant::get_active_variant_writeable()
{
	return reinterpret_cast<c_game_engine_base_variant*>(m_game_engine_variant);
}

c_game_engine_ctf_variant const* c_game_variant::get_ctf_variant() const
{
	assert(get_game_engine_index() == _game_engine_ctf_variant/* , "this variant is not ctf!" */);
	return static_cast<c_game_engine_ctf_variant const*>(get_active_variant());
}

c_game_engine_ctf_variant* c_game_variant::get_ctf_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_ctf_variant/* , "this variant is not ctf!" */);
	return static_cast<c_game_engine_ctf_variant*>(get_active_variant_writeable());
}

c_game_engine_slayer_variant const* c_game_variant::get_slayer_variant() const
{
	assert(get_game_engine_index() == _game_engine_slayer_variant/* , "this variant is not slayer!" */);
	return static_cast<c_game_engine_slayer_variant const*>(get_active_variant());
}

c_game_engine_slayer_variant* c_game_variant::get_slayer_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_slayer_variant/* , "this variant is not slayer!" */);
	return static_cast<c_game_engine_slayer_variant*>(get_active_variant_writeable());
}

c_game_engine_oddball_variant const* c_game_variant::get_oddball_variant() const
{
	assert(get_game_engine_index() == _game_engine_oddball_variant/* , "this variant is not oddball!" */);
	return static_cast<c_game_engine_oddball_variant const*>(get_active_variant());
}

c_game_engine_oddball_variant* c_game_variant::get_oddball_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_oddball_variant/* , "this variant is not oddball!" */);
	return static_cast<c_game_engine_oddball_variant*>(get_active_variant_writeable());
}

c_game_engine_king_variant const* c_game_variant::get_king_variant() const
{
	assert(get_game_engine_index() == _game_engine_king_variant/* , "this variant is not king of the hill!" */);
	return static_cast<c_game_engine_king_variant const*>(get_active_variant());
}

c_game_engine_king_variant* c_game_variant::get_king_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_king_variant/* , "this variant is not king of the hill!" */);
	return static_cast<c_game_engine_king_variant*>(get_active_variant_writeable());
}

c_game_engine_sandbox_variant const* c_game_variant::get_sandbox_variant() const
{
	assert(get_game_engine_index() == _game_engine_sandbox_variant/* , "this variant is not sandbox editor!" */);
	return static_cast<c_game_engine_sandbox_variant const*>(get_active_variant());
}

c_game_engine_sandbox_variant* c_game_variant::get_sandbox_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_sandbox_variant/* , "this variant is not sandbox editor!" */);
	return static_cast<c_game_engine_sandbox_variant*>(get_active_variant_writeable());
}

c_game_engine_vip_variant const* c_game_variant::get_vip_variant() const
{
	assert(get_game_engine_index() == _game_engine_vip_variant/* , "this variant is not vip!" */);
	return static_cast<c_game_engine_vip_variant const*>(get_active_variant());
}

c_game_engine_vip_variant* c_game_variant::get_vip_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_vip_variant/* , "this variant is not vip!" */);
	return static_cast<c_game_engine_vip_variant*>(get_active_variant_writeable());
}

c_game_engine_juggernaut_variant const* c_game_variant::get_juggernaut_variant() const
{
	assert(get_game_engine_index() == _game_engine_juggernaut_variant/* , "this variant is not juggernaut!" */);
	return static_cast<c_game_engine_juggernaut_variant const*>(get_active_variant());
}

c_game_engine_juggernaut_variant* c_game_variant::get_juggernaut_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_juggernaut_variant/* , "this variant is not juggernaut!" */);
	return static_cast<c_game_engine_juggernaut_variant*>(get_active_variant_writeable());
}

c_game_engine_territories_variant const* c_game_variant::get_territories_variant() const
{
	assert(get_game_engine_index() == _game_engine_territories_variant/* , "this variant is not territories!" */);
	return static_cast<c_game_engine_territories_variant const*>(get_active_variant());
}

c_game_engine_territories_variant* c_game_variant::get_territories_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_territories_variant/* , "this variant is not territories!" */);
	return static_cast<c_game_engine_territories_variant*>(get_active_variant_writeable());
}

c_game_engine_assault_variant const* c_game_variant::get_assault_variant() const
{
	assert(get_game_engine_index() == _game_engine_assault_variant/* , "this variant is not assault!" */);
	return static_cast<c_game_engine_assault_variant const*>(get_active_variant());
}

c_game_engine_assault_variant* c_game_variant::get_assault_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_assault_variant/* , "this variant is not assault!" */);
	return static_cast<c_game_engine_assault_variant*>(get_active_variant_writeable());
}

c_game_engine_infection_variant const* c_game_variant::get_infection_variant() const
{
	assert(get_game_engine_index() == _game_engine_infection_variant/* , "this variant is not infection!" */);
	return static_cast<c_game_engine_infection_variant const*>(get_active_variant());
}

c_game_engine_infection_variant* c_game_variant::get_infection_variant_writeable()
{
	assert(get_game_engine_index() == _game_engine_infection_variant/* , "this variant is not infection!" */);
	return static_cast<c_game_engine_infection_variant*>(get_active_variant_writeable());
}

bool c_game_variant::get_integer_game_engine_setting(e_game_variant_parameter parameter, long* out_value) const
{
	return get_game_engine_setting(parameter, _text_value_pair_parameter_type_integer, out_value);
}

bool c_game_variant::set_integer_game_engine_setting(e_game_variant_parameter parameter, long value)
{
	return set_game_engine_setting(parameter, _text_value_pair_parameter_type_integer, value);
}

bool c_game_variant::get_string_id_game_engine_setting(e_game_variant_parameter parameter, long* out_value) const
{
	return get_game_engine_setting(parameter, _text_value_pair_parameter_type_string_id, out_value);
}

bool c_game_variant::set_string_id_game_engine_setting(e_game_variant_parameter parameter, long value)
{
	return set_game_engine_setting(parameter, _text_value_pair_parameter_type_string_id, value);
}

bool c_game_variant::get_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, long* out_value) const
{
	return DECLFUNC(0x00574530, bool, __thiscall, c_game_variant const*, e_game_variant_parameter, e_text_value_pair_parameter_type, long*)(this, parameter, parameter_type, out_value);
}

bool c_game_variant::set_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, long value)
{
	return DECLFUNC(0x0057AAB0, bool, __thiscall, c_game_variant*, e_game_variant_parameter, e_text_value_pair_parameter_type, long)(this, parameter, parameter_type, value);
}

