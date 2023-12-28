#pragma once

#pragma pack(push, 1)
struct s_motd_data
{
	static long const k_chunk_type = 'motd';
	static long const k_version_major = 1;
	//static long const k_version_minor = 0;

	long message_length;
	char message[511];
	char __data[2721 - sizeof(message_length) - 511];
};
static_assert(sizeof(s_motd_data) == 2721);
#pragma pack(pop)
