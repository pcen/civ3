#include <Split>

class App : public Split::Application {

	typedef std::shared_ptr<Split::VertexArray> vertex_array_ptr;

	void run(void)
	{
		m_window->capture_cursor(false);
		m_window->use_vsync(false);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), m_window->aspect_ratio(), 0.01f, 100.0f);

		auto sh = Split::create_shader("resources\\vert.glsl", "resources\\pixel.glsl");
		sh->bind();

		vertex_array_ptr va = Split::create_cube();

		Split::Renderer r;
		r.set_clear_colour({ 0.0f, 0.0f, 0.0f });
		r.use_wireframe(true);

		// glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -3.0f));

		while (m_running)
		{
			r.clear();
			r.begin(proj * m_camera->get_view_matrix());
			r.push(sh, va);
			update();
		}
	}

};

Split::Application* Split::create_application(void) { return new App(); }
