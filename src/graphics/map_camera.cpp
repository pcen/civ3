#include "map_camera.h"
#include "utils/math.h"

#include <Split>

MapCamera::MapCamera(glm::vec2 position, glm::ivec2 screen_size, float hex_radius)
	: m_position{ position }, m_screen_size{ screen_size }, m_mouse_world_pos{ glm::vec2(0.0f, 0.0f) },
	m_view{ glm::vec2((float)screen_size.x, (float)screen_size.y) }, m_hex_radius{ hex_radius },
	m_mouse_on_screen{ false }, m_speed{ 3.0f }, m_zoom{ 0.004f }, m_move_threshold{ 40 },
	m_zoom_min{ 0.0015f }, m_zoom_max{ 0.01f }
{
	update_ortho();
	update_matrix();
	event_bus_subscribe();
}

MapCamera::~MapCamera() {}

glm::mat4& MapCamera::get_matrix(void)
{
	return m_view_proj_matrix;
}

bool MapCamera::move_in_x(Split::mouse_data& mouse)
{
	return m_mouse_on_screen && ((int)mouse.x < m_move_threshold || m_screen_size.x - (int)mouse.x < m_move_threshold);
}

bool MapCamera::move_in_y(Split::mouse_data& mouse)
{
	return m_mouse_on_screen && ((int)mouse.y < m_move_threshold || m_screen_size.y - (int)mouse.y < m_move_threshold);
}

glm::vec2 MapCamera::screen_to_world(glm::vec2 point)
{
	return glm::vec2(point.x * m_zoom * 2.0f, point.y * m_zoom * 2.0f);
}

glm::vec2 MapCamera::get_map_mouse(void)
{
	Split::mouse_data& mouse = Split::Input::get_mouse();
	return glm::vec2(mouse.x - m_screen_size.x / 2.0f, m_screen_size.y / 2.0f - mouse.y);
}

bool MapCamera::check_world_bounds(void)
{
	return false;
}

void MapCamera::update(double dt)
{
	float speed = m_speed * (float)dt;
	Split::mouse_data& mouse = Split::Input::get_mouse();

	bool moved = false;
	if (move_in_x(mouse)) {
		m_position.x += (mouse.x > m_screen_size.x / 2) ? speed : -speed;
		moved = true;
	}
	if (move_in_y(mouse)) {
		m_position.y += (mouse.y < m_screen_size.y / 2) ? speed : -speed;
		moved = true;
	}
	if (check_world_bounds())
		moved = true;

	if (moved)
		update_matrix();
}

void MapCamera::update_ortho(void)
{
	m_proj_matrix = glm::ortho(-m_view.x * m_zoom, m_view.x * m_zoom, -m_view.y * m_zoom, m_view.y * m_zoom, -2.0f, 2.0f);
}

void MapCamera::update_matrix(void)
{
	m_view_matrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 1.0f)));
	m_view_proj_matrix = m_proj_matrix * m_view_matrix;
}

void MapCamera::update_mouse_on_hex(void)
{
	m_mouse_world_pos = m_position + screen_to_world(get_map_mouse());

	float q = (m_mouse_world_pos.x * 0.57735026919f + 0.33333333333f * m_mouse_world_pos.y) / m_hex_radius;
	float r = (-m_mouse_world_pos.y * 0.66666666666f) / m_hex_radius;
	float s = -q - r;

	glm::ivec3 cubic(int(std::round(q)), int(std::round(r)), int(std::round(s)));
	glm::dvec3 dif(std::abs((float)cubic.x - q), std::abs((float)cubic.y - r), std::abs((float)cubic.z - s));

	if (dif.x > dif.y && dif.x > dif.z)
		cubic.x = -cubic.y - cubic.z;
	else if (dif.y > dif.z)
		cubic.y = -cubic.x - cubic.z;

	m_mouse_on_hex = glm::ivec2(cubic.x, cubic.y);
}

/* Events
 */
void MapCamera::on_window_resize(Split::WindowResize& resize)
{
	m_screen_size = resize.dimensions();
	m_view = glm::vec2((float)m_screen_size.x, (float)m_screen_size.y);
	update_ortho();
	update_matrix();
}

void MapCamera::on_mouse_scroll(Split::MouseScroll& scroll)
{
	m_zoom -= scroll.dy() * 0.0005f;

	if (m_zoom < m_zoom_min)
		m_zoom = m_zoom_min;
	else if (m_zoom > m_zoom_max)
		m_zoom = m_zoom_max;

	update_ortho();
	update_matrix();
	update_mouse_on_hex();
}

void MapCamera::on_mouse_move(Split::MouseMove& mouse)
{
	update_mouse_on_hex();
}

void MapCamera::on_mouse_click(Split::MousePress& mouse)
{
	update_mouse_on_hex();
	core_log_info("mouse: ({0}, {1})", m_mouse_world_pos.x, m_mouse_world_pos.y);
	core_log_info("hex: ({0}, {1})\n", m_mouse_on_hex.x, m_mouse_on_hex.y);
}

void MapCamera::on_key_press(Split::KeyPress& key)
{
	core_log_info("MapCamera key pressed: {0}", key.keycode());
}

void MapCamera::on_mouse_window_border(Split::MouseWindowBorder& event) { m_mouse_on_screen = event.entered(); }

void MapCamera::event_bus_subscribe(void)
{
	callback_subscribe(&MapCamera::on_mouse_scroll);
	callback_subscribe(&MapCamera::on_key_press);
	callback_subscribe(&MapCamera::on_mouse_move);
	callback_subscribe(&MapCamera::on_mouse_click);
	callback_subscribe(&MapCamera::on_window_resize);
	callback_subscribe(&MapCamera::on_mouse_window_border);
}
