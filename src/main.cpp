#define SPLIT_MAIN
#include <Split>
#include "geometry/hex_batch.h"
#include "map/hex_map.h"

#define set_application(app_name) Split::Application* Split::create_application(void) { return new app_name(); }

using namespace Split;

class App : public Split::Application {

	void run(void)
	{
		m_window->capture_cursor(true);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), m_window->aspect_ratio(), 0.01f, 200.0f);

		auto sh = create_shader("resources\\vert.glsl", "resources\\pixel.glsl");
		sh->bind();

		Split::Renderer r;
		r.set_clear_colour({ 0.0f, 0.0f, 0.3f });
		r.use_wireframe(false);

		auto hm = new HexMap(5, 5);
		hm->print();

		hm->batch_tiles();
		hm->batch->generate_mesh();
		auto va = hm->batch->get_va();

		auto tex = Texture2D("resources\\water.jpg", TextureFormat::JPEG, 4, TEXTURE_2D);
		tex.bind();

		while (m_running)
		{
			r.clear();
			r.begin(proj * m_camera->get_view_matrix());
			r.push(sh, va);
			update();
		}
	}
};

set_application(App)
