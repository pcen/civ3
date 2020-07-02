#include <Split>

// civ 3 headers
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
		m_window->use_vsync(true);

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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(m_window->raw_window(), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		bool gui_window_open = true;

		while (m_running) {
			r.clear();
			r.begin(m_camera->get_matrix());
			r.push(sh, va);
	
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (gui_window_open) {
				ImGui::Begin("Gui Window", &gui_window_open);
				if (ImGui::Button("Close"))
					gui_window_open = false;
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			update();
		}

		// cleanup imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		app_log_info("civ3 run end");
	}
};

int main(int argc, char* argv[])
{
	launch(argc, argv, new App());
}
