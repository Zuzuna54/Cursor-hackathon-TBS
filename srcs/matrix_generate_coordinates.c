#include "morphosis.h"

static int						count_set_bits(int byte)
{
	int 						c;

	c = 0;
	while (byte)
	{
		byte &= (byte - 1);
		c++;
	}
	return c;
}

static float 					generate_number(uint *bytes)
{
	float 						res;
	int 						negative;
	float 						mantissa;

	res = 0;
	negative = 0;
	mantissa = 0;
	if (count_set_bits(bytes[0]) >= 4)
		negative = 1;
	if (WHOLE == 2)
	{
		if (count_set_bits(bytes[1]) >= 4)
			res = 1;
	}
	for (int i = 2; i < 8; i++)
	{
		mantissa += (float)count_set_bits(bytes[i]);
		mantissa *= 10;
	}
	mantissa /= 10000000;
	res += mantissa;

	return (negative ? res : res * -1);
}

void 					get_coords_from_hash(unsigned char *hash, t_mat_conv_data *data)
{
	uint 						coord_bytes[4][8];
	int							c;

	c = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
			coord_bytes[i][j] = (uint)hash[c++];
	}
	data->q.x = generate_number(coord_bytes[0]);
	data->q.y = generate_number(coord_bytes[1]);
	if (ZW == 1)
	{
		data->q.z = generate_number(coord_bytes[2]);
		data->q.w = generate_number(coord_bytes[3]);
	}
	else if (ZW == 2)
	{
		data->q.z = 0.0f;
		data->q.w = 0.0f;
	}
}
