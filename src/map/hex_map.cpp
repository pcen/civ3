﻿#include "hex_map.h"
#include "utils/math.h"

#define cos30 0.86602540378f

/* HexMap implementation
 */
HexMap::HexMap(int width, int height, float radius)
	: m_hex_w{ width }, m_hex_h{ height }, batch{ new HexBatch(radius) }, m_hex_radius{ radius }
{
	/* first 2 hex rows stack evenly before column drops off rectangular edge
	 */
	m_width_buffer = (int)std::ceil((float)(height - 2) / 2.0f);
	m_buffer_w = width + m_width_buffer;
	m_buffer_h = height;
	init_tiles();
	set_coordinates();
}

HexMap::~HexMap()
{
	delete batch;
}

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
	if (x < 0 || y < 0 || y * m_buffer_w + x >= m_data.size())
		std::cerr << "ERROR: invalid matrix index: (" << x << ", " << y << ")\n";
	return &m_data.at(y * m_buffer_w + x);
}

HexTile* HexMap::axial_at(int q, int r)
{
	if (m_axial_map.count(dovetail(q, r)))
		return m_axial_map[dovetail(q, r)];
	else
		return nullptr;
}

/* Create vertex buffer for terrain hex
 */
void HexMap::batch_tiles(void)
{
	bool even = true;
	float y_factor = 1.5f * m_hex_radius;
	float x_factor = 2 * cos30 * m_hex_radius;
	float dx = 0.0f;

	for (int y = 0; y < m_buffer_h; y++) {
		int null = 0;
		for (int x = 0; x < m_buffer_w; x++) {
			dx = even ? 0.0f : -cos30 * m_hex_radius;

			auto h = matrix_at(x, y);
			if (!h->valid)
				null++;
			else
				batch->add((x - null) * x_factor + dx, y * y_factor);
		}
		even = !even;
	}
}

void HexMap::init_tiles(void)
{
	m_data = std::vector<HexTile>();
	for (int y = 0; y < m_buffer_h; y++) {
		for (int x = 0; x < m_buffer_w; x++)
			m_data.push_back(HexTile(this, x, y));
	}
}

void HexMap::set_coordinates(void)
{
	int i = m_width_buffer;
	bool even = false;

	/* invalidate unused buffer indices
	 */
	for (int y = 0; y < m_buffer_h; y++) {
		for (int j = 0; j < i; j++)
			matrix_at(j, y)->valid = false;
		for (int k = 1; k < m_width_buffer - i + 1; k++)
			matrix_at(-k, y)->valid = false;
		if (even)
			i -= 1;
		even = !even;
	}

	/* adjust offset coordinates and generate axial ones
	 */
	for (int y = 0; y < m_buffer_h; y++) {
		int null = 0;
		for (int x = 0; x < m_buffer_w; x++) {
			auto h = matrix_at(x, y);
			if (!h->valid) {
				null++;
			}
			else {
				h->x -= (m_hex_w / 2 + null);
				h->y -= (m_hex_h / 2);
				h->set_axial_coordinates();
				m_axial_map[dovetail(h->q, h->r)] = h;
			}
		}
	}
}

bool HexMap::load_map_data(void)
{
	std::cerr << "ERROR: map data loading not implemented.\n";
	return false;
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

/* HexTile implementation
 */
HexTile::HexTile(HexMap* parent, int x, int y, bool is_valid)
	: map{ parent }, x{ x }, y{ y }, q{ 0 }, r{ 0 }, s{ 0 }, valid{ is_valid } {}

std::string HexTile::str(void)
{
	std::stringstream ss;
	if (valid) ss << " (" << q << ", " << r << ")";
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
		{x - 1, y + 1}, {x + 1, y - 1}};

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

unsigned int distance(HexTile* h1, HexTile* h2)
{
	if (!h1 || !h2) {
		std::cerr << "ERROR: nullptr hex in distance computation.\n";
		return 0;
	}
	return (std::abs(h1->q - h2->q) + std::abs(h1->s - h2->s) + std::abs(h1->r - h2->r)) / 2;
}
