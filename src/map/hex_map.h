#ifndef HEX_MAP_H
#define HEX_MAP_H

#include <Split>

struct HexTile;


class HexMap
{
public:
	HexMap(int width, int height);
	~HexMap();

	HexTile* hex_at(int x, int y);
	HexTile* matrix_at(int x, int y);
	void print(void);

private:
	int m_width_buffer;
	int m_buffer_w, m_buffer_h;
	int m_hex_w, m_hex_h;

	std::vector<HexTile> m_data;

	bool init_tiles(void);
	bool delineate_map(void);
	bool load_map_data(void);
};

struct HexTile
{
	HexTile(HexMap* parent, float x, float y, bool is_valid = true);
	
	float x, y;
	HexMap* map;
	bool valid;

	void invalidate(void);
	std::string str(void);
	std::vector<HexTile*> get_neighbors(void);
};

#endif /* HEX_MAP_H */
