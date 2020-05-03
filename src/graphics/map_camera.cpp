#include "map_camera.h"

#include <Split>

MapCamera::MapCamera(glm::vec3 position, glm::ivec2 screen_size)
	: m_position{ position }, m_screen_area{ screen_size.x * screen_size.y },
	m_screen_size{ screen_size }, m_view_w{ (float)screen_size.x }, m_view_h{ (float)screen_size.y },
	m_mouse_on_screen{ false }, m_speed{ 3.0f }, m_zoom{ 0.004f }, m_move_threshold{ 40 }
{
	m_proj = glm::ortho(-m_view_w * m_zoom, m_view_w * m_zoom, -m_view_h * m_zoom, m_view_h * m_zoom, -2.0f, 2.0f);

	update_matrix();
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
	float speed = m_speed * (float)dt;
	Split::mouse_data& mouse = Split::Input::get_mouse();

	bool has_moved = false;
	if (move_in_x(mouse)) {
		m_position.x += (mouse.x > m_screen_size.x / 2) ? speed : -speed;
		has_moved = true;
	}
	if (move_in_y(mouse)) {
		m_position.y += (mouse.y < m_screen_size.y / 2) ? speed : -speed;
		has_moved = true;
	}
	if (has_moved)
		update_matrix();
}

void MapCamera::update_matrix(void)
{
	m_view = glm::inverse(glm::translate(glm::mat4(1.0f), m_position));
	m_view_proj = m_proj * m_view;
}

glm::mat4& MapCamera::get_matrix(void) { return m_view_proj; }

float MapCamera::aspect_ratio(void) { return (float)m_screen_size.x / (float)m_screen_size.y; }

/* Events
 */
void MapCamera::on_window_resize(Split::WindowResize& resize)
{
	m_screen_size = resize.dimensions();
	m_screen_area = m_screen_size.x * m_screen_size.y;
	m_view_w = (float)m_screen_size.x;
	m_view_h = (float)m_screen_size.y;
	m_proj = glm::ortho(-m_view_w * m_zoom, m_view_w * m_zoom, -m_view_h * m_zoom, m_view_h * m_zoom, -2.0f, 2.0f);
	update_matrix();
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
