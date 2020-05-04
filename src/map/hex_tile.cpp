#include "hex_tile.h"
#include "hex_map.h"

HexTile::HexTile(HexMap* parent, int x, int y, terrain_type type, bool is_valid)
	: map{ parent }, x{ x }, y{ y }, q{ 0 }, r{ 0 }, s{ 0 },
	valid{ is_valid }, world_x{ 0.0f }, world_y{ 0.0f },
	type{ type } {}

std::string HexTile::str(void)
{
	std::stringstream ss;
	if (valid) ss << " (" << world_x << ", " << world_y << ")";
	else ss << "invalid";
	return ss.str();
}

void HexTile::set_axial_coordinates(void)
{
	q = x - (y - (y & 1)) / 2;
	r = y;
	s = -q - r;
}

std::vector<HexTile*> HexTile::get_neighbors(void)
{
	glm::ivec2 coords_to_check[6] = {
		{x - 1, y    }, {x + 1, y    },
		{x,     y + 1}, {x,     y - 1},
		{x - 1, y + 1}, {x + 1, y - 1} };

	std::vector<HexTile*> neighbors;

	for (auto& c : coords_to_check) {
		if (c.x >= 0 && c.y >= 0) {
			HexTile* neighbor = map->matrix_at(c.x, c.y);
			if (neighbor->valid)
				neighbors.push_back(neighbor);
		}
	}
	return neighbors;
}

void HexTile::set_world_coords(float wx, float wy)
{
	world_x = wx;
	world_y = wy;
}

unsigned int distance(HexTile* h1, HexTile* h2)
{
	if (!h1 || !h2) {
		std::cerr << "ERROR: nullptr hex in distance computation.\n";
		return 0;
	}
	return (std::abs(h1->q - h2->q) + std::abs(h1->s - h2->s) + std::abs(h1->r - h2->r)) / 2;
}
