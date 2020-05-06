#include "hex_map.h"
#include "hex_tile.h"
#include "terrain_types.h"
#include "utils/math.h"

#include <Split>

HexMap::HexMap(int width, int height, float radius)
	: m_hex_w{ width }, m_hex_h{ height }, batch{ new HexBatch(radius) }, m_hex_radius{ radius }
{
	m_width_buffer = (int)std::ceil((float)(height - 2) / 2.0f);
	m_buffer_w = width + m_width_buffer;
	m_buffer_h = height;
	init_tiles();
	set_coordinates();
	app_log_info("{0} by {1} hex map generated with hex radius {2}", width, height, radius);
}

HexMap::~HexMap()
{
	delete batch;
	app_log_info("Hex map destructed");
}

HexTile* HexMap::matrix_at(int x, int y)
{
	if (x < 0)
		x += m_buffer_w;
	if (y < 0)
		y += m_buffer_h;
	if (x < 0 || y < 0 || y * m_buffer_w + x >= m_data.size()) {
		app_log_error("Invalid hex-matrix index: ({0}, {1})", x, y);
		return nullptr;
	}
	return &m_data.at(y * m_buffer_w + x);
}

HexTile* HexMap::axial_at(int q, int r)
{
	if (m_axial_map.count(dovetail(q, r)) != 0) {
		return m_axial_map[dovetail(q, r)];
	}
	else {
		app_log_warn("Axial coordinates q = {0}, r = {1} not in map", q, r);
		return nullptr;
	}
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
			dx = even ? 0.0f : cos30 * m_hex_radius;
			auto h = matrix_at(x, y);
			if (!h->valid) {
				null++;
			}
			else {
				batch->add((x - null) * x_factor + dx, y * y_factor, h->type);
				matrix_at(x, y)->set_world_coords((x - null) * x_factor + dx, y * y_factor);
			}
		}
		even = !even;
	}
}

void HexMap::init_tiles(void)
{
	app_log_trace("Initializing hex map tiles");
	m_data = std::vector<HexTile>();
	for (int y = 0; y < m_buffer_h; y++) {
		for (int x = 0; x < m_buffer_w; x++) {
			/* TODO write terrain generation with random noise function
			 */
			terrain_type type = y & 1 ? terrain_type::GRASS : terrain_type::OCEAN;
			m_data.push_back(HexTile(this, x, y, type));
		}
	}
}

void HexMap::set_coordinates(void)
{
	app_log_trace("Setting hex map coordinates");
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
				h->x -= null;
				h->set_axial_coordinates();
				m_axial_map[dovetail(h->q, h->r)] = h;
			}
		}
	}
}

bool HexMap::load_map_data(void)
{
	app_log_error("Hex map data loading not implemented");
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
