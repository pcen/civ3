#include "hex.h"

#include <Split>
#include <cmath>

#define STATIC_DRAW 0x88E4

#define cos(angle) ((float)std::cos(glm::radians((angle))))
#define sin(angle) ((float)std::sin(glm::radians((angle))))

#define HEX_VERT_NUM   18
#define HEX_POINTS_NUM 6

const float HEX_VERTICIES[HEX_VERT_NUM] = {
	 0.0f,        1.0f,       0.0f, // vert 0
	 cos(30.0f),  sin(30.0f), 0.0f, // vert 1
	 cos(30.0f), -sin(30.0f), 0.0f, // vert 2
	 0.0f,       -1.0f,       0.0f, // vert 3
	-cos(30.0f), -sin(30.0f), 0.0f, // vert 4
	-cos(30.0f),  sin(30.0f), 0.0f  // vert 5
};

const unsigned int HEX_INDICES[12] = {
	1, 2, 4, // right
	1, 4, 5, // left
	0, 1, 5, // top
	2, 3, 4  // bottom
};

SingleHex::SingleHex(float x, float y, float z)
	: m_x{x}, m_y{y}, m_z{z} {}

SingleHex::~SingleHex() {}

HexBatch::HexBatch()
{
	m_hexs.push_back(SingleHex(4, 4));
	m_shader = new Split::Shader("resources\\vert.glsl", "resources\\pixel.glsl");
	generate_mesh();
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

void HexBatch::gen_ib(void)
{
	m_ib = Split::create_index_buffer(index_data(), index_count(), STATIC_DRAW);
}

void HexBatch::gen_va(void)
{
	gen_ib();
	auto vb = Split::create_vertex_buffer(vertex_data(), vertex_size(), STATIC_DRAW);
	vb->attributes = { {"points", 3} };
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
	for (char i = 0; i < HEX_VERT_NUM; i += 3) {
		m_verticies.push_back(HEX_VERTICIES[i] + hex.m_x);
		m_verticies.push_back(HEX_VERTICIES[i + 1] + hex.m_y);
		m_verticies.push_back(HEX_VERTICIES[i + 2] + hex.m_z);
	}
}

int HexBatch::index_count(void)
{
	return m_indices.size();
}

unsigned int* HexBatch::index_data(void)
{
	return m_indices.data();
}

unsigned int HexBatch::vertex_size(void)
{
	return m_verticies.size() * sizeof(float);
}

float* HexBatch::vertex_data(void)
{
	return m_verticies.data();
}
