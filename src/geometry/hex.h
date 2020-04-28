#ifndef HEX_H
#define HEX_H

#include <vector>
#include <memory>

struct SingleHex
{
	SingleHex(float x, float y, float z = 0.0f);
	~SingleHex();

	float m_x, m_y, m_z;
};

/* Forward declarations
 */
namespace Split
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
};

typedef std::shared_ptr<Split::VertexArray> va_ref;
typedef std::shared_ptr<Split::IndexBuffer> ib_ref;
typedef std::shared_ptr<Split::VertexBuffer> vb_ref;

class HexBatch
{
public:
	HexBatch();
	~HexBatch();

	va_ref& get_va(void);

	void generate_mesh(void);

private:
	va_ref m_va = nullptr;
	ib_ref m_ib = nullptr;
	Split::Shader* m_shader = nullptr;

	unsigned int vertex_size(void);
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
