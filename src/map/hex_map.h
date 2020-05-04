#ifndef HEX_MAP_H
#define HEX_MAP_H

#include "defs/defs.h"
#include "geometry/hex_batch.h"

class HexTile;

class HexMap
{
public:
	HexMap(int width, int height, float radius = 1.0f);
	~HexMap();

	HexTile* hex_at(int x, int y);
	HexTile* matrix_at(int x, int y);
	HexTile* axial_at(int q, int r);
	void print(void);

	void batch_tiles(void);
	HexBatch* batch;

private:
	int m_width_buffer;
	int m_buffer_w, m_buffer_h, m_hex_w, m_hex_h;
	float m_hex_radius;

	std::vector<HexTile> m_data;
	std::unordered_map<int, HexTile*> m_axial_map;

	void init_tiles(void);
	void set_coordinates(void);
	bool load_map_data(void);
};

unsigned int distance(HexTile* h1, HexTile* h2);

#endif /* HEX_MAP_H */
