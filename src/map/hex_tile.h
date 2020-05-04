#ifndef HEX_TILE_H
#define HEX_TILE_H

#include "defs/defs.h"

#include "terrain_types.h"

class HexMap;

class HexTile
{
public:
	HexTile(HexMap* parent, int x, int y, terrain_type type, bool is_valid = true);

	void set_axial_coordinates(void);

	terrain_type type;

	bool valid;
	int x, y; // offset coordinates
	int q, r, s; // axial (cube) coordinates

	std::string str(void);
	std::vector<HexTile*> get_neighbors(void);
	void set_world_coords(float wx, float wy);

private:
	HexMap* map;
	float world_x, world_y;
};

#endif /* HEX_TILE_H */
