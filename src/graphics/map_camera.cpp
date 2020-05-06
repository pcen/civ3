#include "map_camera.h"
#include "utils/math.h"

#include <Split>

MapCamera::MapCamera(glm::vec3 position, glm::ivec2 screen_size, float hex_radius)
	: m_position{ position }, m_screen_size{ screen_size }, m_mouse_world_pos{ glm::vec2(0.0f, 0.0f) },
	m_view_w{ (float)screen_size.x }, m_view_h{ (float)screen_size.y }, m_hex_radius{ hex_radius },
	m_mouse_on_screen{ false }, m_speed{ 3.0f }, m_zoom{ 0.004f }, m_move_threshold{ 40 }
{
	m_proj = glm::ortho(-m_view_w * m_zoom, m_view_w * m_zoom, -m_view_h * m_zoom, m_view_h * m_zoom, -2.0f, 2.0f);

	update_matrix();
	event_bus_subscribe();
}

MapCamera::~MapCamera() {}

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

/* Events
 */
void MapCamera::on_window_resize(Split::WindowResize& resize)
{
	m_screen_size = resize.dimensions();
	m_view_w = (float)m_screen_size.x;
	m_view_h = (float)m_screen_size.y;
	m_proj = glm::ortho(-m_view_w * m_zoom, m_view_w * m_zoom, -m_view_h * m_zoom, m_view_h * m_zoom, -2.0f, 2.0f);
	update_matrix();
}

void MapCamera::on_mouse_move(Split::MouseMove& mouse)
{
	Split::mouse_data& m = Split::Input::get_mouse();
	m_mouse_world_pos.x = m_position.x + (m.x - m_screen_size.x / 2.0f) * m_zoom * 2.0f;
	m_mouse_world_pos.y = m_position.y + (m_screen_size.y / 2.0f - m.y) * m_zoom * 2.0f;
}

void MapCamera::update_mouse_on_hex(void)
{
	/*  q = x / 2cos(30)  +  y/3  */
	float qf = (m_mouse_world_pos.x * 0.57735026919f + 0.33333333333f * m_mouse_world_pos.y) / m_hex_radius;
	/*  r = -2y / 3  */
	float rf = (-m_mouse_world_pos.y * 0.66666666666f) / m_hex_radius;
	float sf = -qf - rf;
	int q = int(std::round(qf));
	int r = int(std::round(rf));
	int s = int(std::round(sf));
	double dq = std::abs((float)q - qf);
	double dr = std::abs((float)r - rf);
	double ds = std::abs((float)s - sf);
	if (dq > dr && dq > ds)
		q = -r - s;
	else if (dr > ds)
		r = -q - s;
	m_mouse_on_hex.x = q;
	m_mouse_on_hex.y = r;
}

void MapCamera::on_mouse_click(Split::MousePress& mouse)
{
	update_mouse_on_hex();
	std::cerr << "mouse: " << m_mouse_world_pos.x << ", " << m_mouse_world_pos.y << "\n";
	std::cerr << "axial: " << m_mouse_on_hex.x << ", " << m_mouse_on_hex.y << "\n\n";
}

void MapCamera::on_key_press(Split::KeyPress& key)
{
	std::cerr << "MapCamera: " << key.keycode() << " pressed\n";
}

void MapCamera::on_mouse_window_border(Split::MouseWindowBorder& event) { m_mouse_on_screen = event.entered(); }

void MapCamera::event_bus_subscribe(void)
{
	callback_subscribe(&MapCamera::on_key_press);
	callback_subscribe(&MapCamera::on_mouse_move);
	callback_subscribe(&MapCamera::on_mouse_click);
	callback_subscribe(&MapCamera::on_window_resize);
	callback_subscribe(&MapCamera::on_mouse_window_border);
}
