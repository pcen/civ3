#ifndef HEX_MAP_H
#define HEX_MAP_H

#include <Split>
#include "geometry/hex_batch.h"

struct HexTile;

class HexMap
{
public:
	HexMap(int width, int height, float radius = 1.0f);
	~HexMap();

	HexTile* hex_at(int x, int y);
	HexTile* matrix_at(int x, int y);
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
	void delineate_map(void);
	void populate_axial_mapping(void);
	bool load_map_data(void);
};

struct HexTile
{
	HexTile(HexMap* parent, int x, int y, bool is_valid = true);
	
	void set_axial_coordinates(void);

	int x, y; // offset coordinates
	int q, r, s; // axial (cube) coordinates
	HexMap* map;
	bool valid;

	std::string str(void);
	std::vector<HexTile*> get_neighbors(void);
};

#endif /* HEX_MAP_H */
