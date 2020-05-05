#ifndef TERRAIN_TEXTURE_ATLAS_H
#define TERRAIN_TEXTURE_ATLAS_H

#include "map/terrain_types.h"

class TerrainTextureAtlas
{
public:
	static float* get_coordinates(terrain_type type);

private:
	static float calculate_offset(terrain_type type);
};

#endif /* TERRAIN_TEXTURE_ATLAS_H */
