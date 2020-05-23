#include <stdlib.h>
#include "color_map.h"

static unsigned int		color_lerp(unsigned int start, unsigned int end,
	float pc)
{
	float				r;
	float				g;
	float				b;

	if (pc <= 0.0f)
		return (start);
	else if (pc >= 1.0f)
		return (end);
	r = (float)(start & 0xff0000) * (1.0f - pc) + (float)(end & 0xff0000) * pc;
	g = (float)(start & 0x00ff00) * (1.0f - pc) + (float)(end & 0x00ff00) * pc;
	b = (float)(start & 0x0000ff) * (1.0f - pc) + (float)(end & 0x0000ff) * pc;
	return ((((unsigned int)r) & 0xff0000) |
		(((unsigned int)g) & 0x00ff00) |
		(((unsigned int)b) & 0x0000ff));
}

unsigned int		*create_color_map(
	unsigned int start_color, unsigned int end_color, unsigned int max_color,
	unsigned int size)
{
	unsigned int		*map;
	unsigned int		i;
	const float			size_f = (float)size;

	map = malloc(sizeof(unsigned int) * (size + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (i < size)
	{
		map[i] = color_lerp(start_color, end_color, (1.0f - (float)i / size_f));
		i++;
	}
	map[size] = max_color;
	return (map);
}
