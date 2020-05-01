#include "map_camera.h"

#include <Split>

MapCamera::MapCamera(glm::vec3 position, glm::ivec2 screen_size)
	: m_position{ position }, m_screen_size{ screen_size }
{
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_speed = 3.0f;
	m_sensitivity = 0.5f;
	m_yaw = -90.0f;
	m_real_speed = 0.0f;
	m_mouse_on_screen = false;
	m_move_threshold = 40;
	update_view_matrix();
	event_bus_subscribe();
}

MapCamera::~MapCamera()
{

}

bool MapCamera::move_in_x(Split::mouse_data& mouse)
{
	return m_mouse_on_screen && ((int)mouse.x < m_move_threshold || m_screen_size.x - (int)mouse.x < m_move_threshold);
}

bool MapCamera::move_in_y(Split::mouse_data& mouse)
{
	return m_mouse_on_screen && ((int)mouse.y < m_move_threshold || m_screen_size.y - (int)mouse.y < m_move_threshold);
}

void MapCamera::update(double dt)
{
	m_real_speed = m_speed * (float)dt;
	Split::mouse_data& mouse = Split::Input::get_mouse();

	bool has_moved = false;
	if (move_in_x(mouse)) {
		m_position.x += (mouse.x > m_screen_size.x / 2) ? m_real_speed : -m_real_speed;
		has_moved = true;
	}
	if (move_in_y(mouse)) {
		m_position.y += (mouse.y < m_screen_size.x / 2) ? m_real_speed : -m_real_speed;
		has_moved = true;
	}
	if (has_moved)
		update_view_matrix();
}

void MapCamera::update_view_matrix(void)
{
	m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void MapCamera::on_window_resize(Split::WindowResize& resize)
{
	m_screen_size = resize.dimensions();
}

void MapCamera::on_key_press(Split::KeyPress& key)
{
	std::cerr << "MapCamera: " << key.keycode() << " pressed\n";
}

void MapCamera::on_mouse_window_border(Split::MouseWindowBorder& event)
{
	m_mouse_on_screen = event.entered();
}

void MapCamera::event_bus_subscribe(void)
{
	callback_subscribe(&MapCamera::on_key_press);
	callback_subscribe(&MapCamera::on_window_resize);
	callback_subscribe(&MapCamera::on_mouse_window_border);
}

glm::mat4& MapCamera::get_view_matrix(void) { return m_view_matrix; }

void MapCamera::set_sensitivity(float sensitivity) { m_sensitivity = sensitivity; }

void MapCamera::set_speed(float speed) { m_speed = speed; }
