#define SPLIT_MAIN
#include <Split>
#include "geometry/hex_batch.h"
#include "map/hex_map.h"
#include "graphics/map_camera.h"

using namespace Split;

class App : public Split::Application {

	void run(void)
	{
		set_camera(new MapCamera({0, 0, 1}, m_window->get_size()));
		m_window->capture_cursor(false);

		auto sh = create_shader("resources\\vert.glsl", "resources\\pixel.glsl");
		sh->bind();

		Split::Renderer r;

		auto hm = new HexMap(5, 5);
		hm->batch_tiles();
		hm->batch->generate_mesh();
		auto va = hm->batch->get_va();

		auto tex = Texture2D("resources\\water.jpg", TextureFormat::JPEG, 4, TEXTURE_2D);
		tex.bind();

		while (m_running)
		{
			r.clear();
			r.begin(m_camera->get_matrix());
			r.push(sh, va);
			update();
		}
	}
};

set_split_app(App)
