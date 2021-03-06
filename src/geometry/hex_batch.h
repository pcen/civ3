#ifndef HEX_H
#define HEX_H

#include "map/terrain_types.h"

#include "defs/defs.h"

struct SingleHex
{
	SingleHex(float x, float y, terrain_type type);
	~SingleHex();
	float m_x, m_y;
	terrain_type m_type;
};

class HexBatch
{
public:
	HexBatch(float radius = 1.0f);
	~HexBatch();

	va_ref& get_va(void);
	void add(float x, float y, terrain_type type);
	void generate_mesh(void);

private:
	float m_radius;
	va_ref m_va = nullptr;
	ib_ref m_ib = nullptr;
	Split::Shader* m_shader = nullptr;

	unsigned int vertex_count(void);
	int index_count(void);
	float* vertex_data(void);
	unsigned int* index_data(void);
	void gen_ib(void);
	void gen_va(void);

	void indices_hex_add(SingleHex& hex);
	void mesh_hex_add(SingleHex& hex);

	unsigned int m_hex_in_mesh = 0;
	std::vector<SingleHex> m_hexs;
	std::vector<float> m_verticies;
	std::vector<unsigned int> m_indices;

};

#endif /* HEX_H */
