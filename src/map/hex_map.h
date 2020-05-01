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

	void init_tiles(void);
	void delineate_map(void);
	bool load_map_data(void);
};

struct HexTile
{
	HexTile(HexMap* parent, int x, int y, bool is_valid = true);
	
	int x, y; // offset coordinates
	HexMap* map;
	bool valid;

	std::string str(void);
	std::vector<HexTile*> get_neighbors(void);
};

#endif /* HEX_MAP_H */
