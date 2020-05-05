#include "hex_batch.h"
#include "utils/math.h"
#include "terrain_texture_atlas.h"

#include <Split>

#define HEX_VERT_NUM   12
#define HEX_POINTS_NUM 6

#define cos30 0.86602540378f

const float HEX_VERTICIES[HEX_VERT_NUM] = {
	 0.0f,   1.0f, // v0
	 cos30,  0.5f, // v1
	 cos30, -0.5f, // v2
	 0.0f,  -1.0f, // v3
	-cos30, -0.5f, // v4
	-cos30,  0.5f, // v5
};

const unsigned int HEX_INDICES[12] = {
	1, 2, 4, // right
	1, 4, 5, // left
	0, 1, 5, // top
	2, 3, 4, // bottom
};

SingleHex::SingleHex(float x, float y, terrain_type type)
	: m_x{ x }, m_y{ y }, m_type{ type } {}

SingleHex::~SingleHex() {}

HexBatch::HexBatch(float radius) : m_radius{ radius }
{
	m_shader = new Split::Shader("resources\\vert.glsl", "resources\\pixel.glsl");
}

HexBatch::~HexBatch()
{
	
}

va_ref& HexBatch::get_va(void)
{
	if (m_va == nullptr)
		gen_va();
	return m_va;
}

void HexBatch::add(float x, float y, terrain_type type)
{
	m_hexs.push_back(SingleHex(x, y, type));
}

void HexBatch::gen_ib(void)
{
	m_ib = Split::create_index_buffer(index_data(), index_count(), STATIC_DRAW);
}

void HexBatch::gen_va(void)
{
	gen_ib();
	auto vb = Split::create_vertex_buffer(vertex_data(), vertex_count(), STATIC_DRAW);
	vb->attributes = {
		{ "points", 2 },
		{ "texture coordinates", 2 }
	};
	m_va = Split::create_vertex_array(m_ib, vb);
}

void HexBatch::generate_mesh(void)
{
	m_hex_in_mesh = 0;
	for (auto& h : m_hexs) {
		mesh_hex_add(h);
		indices_hex_add(h);
		m_hex_in_mesh++;
	}
}

void HexBatch::indices_hex_add(SingleHex& hex)
{
	for (auto& i : HEX_INDICES)
		m_indices.push_back(i + m_hex_in_mesh * HEX_POINTS_NUM);
}

void HexBatch::mesh_hex_add(SingleHex& hex)
{
	float* texture_coordinates = TerrainTextureAtlas::get_coordinates(hex.m_type);
	for (char i = 0; i < HEX_VERT_NUM; i += 2) {
		m_verticies.push_back(HEX_VERTICIES[i] * m_radius + hex.m_x);
		m_verticies.push_back(HEX_VERTICIES[i + 1] * m_radius + hex.m_y);
		/* TODO push texture coordinates according to
		 * terrain type and current texture atlas
		 */
		m_verticies.push_back(texture_coordinates[i]);
		m_verticies.push_back(texture_coordinates[i + 1]);
	}
	delete texture_coordinates;
}

int HexBatch::index_count(void)
{
	return m_indices.size();
}

unsigned int* HexBatch::index_data(void)
{
	return m_indices.data();
}

unsigned int HexBatch::vertex_count(void)
{
	return m_verticies.size();
}

float* HexBatch::vertex_data(void)
{
	return m_verticies.data();
}
