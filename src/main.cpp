#define SPLIT_MAIN
#include <Split>
#include "geometry/hex_batch.h"
#include "map/hex_map.h"
#include "graphics/map_camera.h"

using namespace Split;

class App : public Application {

	void run(void)
	{
		app_log_info("civ3 run start");

		set_camera(new MapCamera({0, 0}, m_window->get_size()));
		m_window->capture_cursor(false);

		auto sh = create_shader("resources\\vert.glsl", "resources\\pixel.glsl");
		sh->bind();

		Renderer r;
		r.set_clear_colour({ 0.0f, 0.0f, 0.0f });

		auto hm = new HexMap(96, 60);
		hm->batch_tiles();
		hm->batch->generate_mesh();
		auto va = hm->batch->get_va();

		auto tex = Texture2D("resources\\atlas.jpg", TextureFormat::JPEG, 4, TEXTURE_2D);
		tex.bind();

		while (m_running)
		{
			r.clear();
			r.begin(m_camera->get_matrix());
			r.push(sh, va);
			update();
		}

		app_log_info("civ3 run end");
	}
};

set_split_app(App)
