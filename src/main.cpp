#define SPLIT_MAIN
#include <Split>
#include "geometry/hex.h"
#include "map/hex_map.h"

class App : public Split::Application {

	typedef std::shared_ptr<Split::VertexArray> vertex_array_ptr;

	void run(void)
	{
		m_window->capture_cursor(true);
		m_window->use_vsync(false);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), m_window->aspect_ratio(), 0.01f, 100.0f);

		auto sh = Split::create_shader("resources\\vert.glsl", "resources\\pixel.glsl");
		sh->bind();

		Split::Renderer r;
		r.set_clear_colour({ 0.0f, 0.0f, 0.5f });
		r.use_wireframe(true);

		auto map = new HexBatch();
		auto va = map->get_va();

		while (m_running)
		{
			r.clear();
			r.begin(proj * m_camera->get_view_matrix());
			r.push(sh, va);
			update();
		}

		delete map;

		auto hm = new HexMap(5, 10);
		hm->print();
	}
};

Split::Application* Split::create_application(void) { return new App(); }
