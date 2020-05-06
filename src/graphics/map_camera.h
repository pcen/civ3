#ifndef MAP_CAMERA_H
#define MAP_CAMERA_H

#include "events/event_bus_client.h"
#include "rendering/cameras/camera.h"

/* forward declarations
 */
namespace Split
{
	struct mouse_data; class WindowResize;
	class MousePress; class MouseMove; class MouseScroll;
	class MouseWindowBorder; class KeyPress;
};

class MapCamera : public Split::Camera, Split::EventBusListener
{
public:
	MapCamera(glm::vec2 position, glm::ivec2 screen_size, float hex_radius = 1.0f);
	~MapCamera();

	glm::mat4& get_matrix(void) override;
	void update(double dt) override;

	float m_speed;

private:
	glm::mat4 m_view_matrix, m_proj_matrix, m_view_proj_matrix;
	glm::vec2 m_position;
	glm::ivec2 m_screen_size;
	float m_hex_radius;
	glm::vec2 m_mouse_world_pos;
	glm::ivec2 m_mouse_on_hex;
	float m_zoom_min, m_zoom_max;
	glm::vec2 m_view;
	float m_zoom;
	bool m_mouse_on_screen;
	int m_move_threshold;

	void update_matrix(void);
	void update_ortho(void);

	void update_mouse_on_hex(void);

	bool move_in_x(Split::mouse_data& mouse);
	bool move_in_y(Split::mouse_data& mouse);
	bool check_world_bounds(void);
	
	glm::vec2 screen_to_world(glm::vec2 point);
	glm::vec2 get_map_mouse(void);

	/* event callbacks
	 */
	void on_mouse_scroll(Split::MouseScroll& scroll);
	void on_mouse_move(Split::MouseMove& mouse);
	void on_mouse_click(Split::MousePress& mouse);
	void on_window_resize(Split::WindowResize& resize);
	void on_mouse_window_border(Split::MouseWindowBorder& event);
	void on_key_press(Split::KeyPress& key);
	void event_bus_subscribe(void) override;
};

#endif /* MAP_CAMERA_H */
