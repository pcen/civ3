#include "hex_map.h"

/* HexMap implementation
 */
HexMap::HexMap(int width, int height)
	: m_hex_w{ width }, m_hex_h{ height }
{
	/* first 2 hex rows stack evenly before column drops off rectangular edge
	 */
	m_width_buffer = (int)std::ceil((float)(height - 2) / 2.0f);
	m_buffer_w = width + m_width_buffer;
	m_buffer_h = height;
	init_tiles();
	delineate_map();
}

HexMap::~HexMap() {}

HexTile* HexMap::hex_at(int x, int y)
{
	if (x < 0 || y < 0)
		std::cerr << "ERROR: invalid hex index: (" << x << ", " << y << ")\n";

	return &m_data.at(y * m_buffer_w + x + m_width_buffer);
}

HexTile* HexMap::matrix_at(int x, int y)
{
	if (x < 0)
		x += m_buffer_w;
	if (y < 0)
		y += m_buffer_h;

	if (x < 0 || y < 0)
		std::cerr << "ERROR: invalid matrix index: (" << x << ", " << y << ")\n";

	std::cerr << "accessing " << x << ", " << y << "\n";

	return &m_data.at(y * m_buffer_w + x);
}

void HexMap::print(void)
{
	std::cerr << "Hex Map:\n";
	int index = 0;
	for (auto& t : m_data) {
		index++;
		std::cerr << t.str();
		if (index % m_buffer_w == 0)
			std::cerr << "\n";
		else
			std::cerr << ", ";
	}
}

bool HexMap::init_tiles(void)
{
	m_data = std::vector<HexTile>();
	for (int y = 0; y < m_buffer_h; y++) {
		for (int x = 0; x < m_buffer_h; x++) {
			m_data.push_back(HexTile(this, (float)x, (float)y));
		}
	}
	return true;
}

bool HexMap::delineate_map(void)
{
	int i = m_width_buffer;
	bool even = false;

	for (int y = 0; y < m_buffer_h; y++) {
		for (int j = 0; j < i; j++)
			matrix_at(j, y)->invalidate();

		for (int k = 1; k < m_width_buffer - i + 1; k++)
			matrix_at(-k, y)->invalidate();

		if (even)
			i -= 1;

		even = !even;
	}

	return true;
}

bool HexMap::load_map_data(void)
{
	std::cerr << "ERROR: map data loading not implemented.\n";
	return false;
}

/* HexTile implementation
 */
HexTile::HexTile(HexMap* parent, float x, float y, bool is_valid)
	: map{ parent }, x{ x }, y{ y }, valid{ is_valid } {}

void HexTile::invalidate(void)
{
	valid = false;
}

std::string HexTile::str(void)
{
	std::stringstream ss;
	if (valid)
		ss << "(" << x << " - " << y << ")";
	else
		ss << "invalid";
	return ss.str();
}

int vec_min(glm::ivec2& v)
{
	return v.x < v.y ? v.x : v.y;
}

std::vector<HexTile*> HexTile::get_neighbors(void)
{
	glm::ivec2 coords_to_check[6] = {
		{ x - 1, y     }, { x + 1, y     },
		{ x,     y + 1 }, { x,     y - 1 },
		{ x - 1, y + 1 }, { x + 1, y - 1 }};

	std::vector<HexTile*> neighbors;

	for (auto& c : coords_to_check) {
		if (vec_min(c) >= 0) {
			HexTile* neighbor = map->matrix_at(c.x, c.y);
			if (neighbor->valid)
				neighbors.push_back(neighbor);
		}
	}
	return neighbors;
}
