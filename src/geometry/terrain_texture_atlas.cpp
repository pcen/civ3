#include "terrain_texture_atlas.h"

#define TEX_COORD_NUM 12

const float TEX_COORD_BASE[TEX_COORD_NUM] = {
	0.5f,  1.0f,
	1.0f,  0.75f,
	1.0f,  0.25f,
	0.5f,  0.0f,
	0.0f,  0.25f,
	0.0f,  0.75f,
};

/* Relative size of each texture (length) in atlas
 * TODO: read from generated atlas config file
 */
const float relative_pct = 0.5f;

/*   sum of % of previous textures in atlas
 * + base % * relative % of single texture in atlas
 */
float* TerrainTextureAtlas::get_coordinates(terrain_type type)
{
	float* coordinates = new float[TEX_COORD_NUM];
	float previous = calculate_offset(type);
	for (int i = 0; i < TEX_COORD_NUM; i += 2) {
		coordinates[i] = previous + TEX_COORD_BASE[i] * relative_pct;
		coordinates[i + 1] = TEX_COORD_BASE[i + 1];
	}
	return coordinates;
}

/* TODO: calculate offsets from atlas config file
 */
float TerrainTextureAtlas::calculate_offset(terrain_type type)
{
	switch (type) {
	case terrain_type::OCEAN:
		return 0.0f;
	case terrain_type::GRASS:
		return 0.5f;
	default:
		return 0.0f;
	}
}
