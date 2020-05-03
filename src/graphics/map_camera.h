#ifndef MAP_CAMERA_H
#define MAP_CAMERA_H

#include "events/event_bus_client.h"
#include "rendering/cameras/camera.h"

/* Forward declarations
 */
namespace Split
{
	struct mouse_data; class WindowResize;
	class MouseWindowBorder; class KeyPress;
};

class MapCamera : public Split::Camera, Split::EventBusListener
{
public:
	MapCamera(glm::vec3 position, glm::ivec2 screen_size);
	~MapCamera();

	glm::mat4& get_matrix(void) override;
	void update(double dt) override;

	float m_speed;

private:
	glm::mat4 m_view, m_proj, m_view_proj;
	glm::vec3 m_position;
	glm::ivec2 m_screen_size;
	int m_screen_area;

	float m_view_w, m_view_h, m_zoom;
	bool m_mouse_on_screen;
	int m_move_threshold;

	bool move_in_x(Split::mouse_data& mouse);
	bool move_in_y(Split::mouse_data& mouse);

	float aspect_ratio(void);
	void update_matrix(void);
	
	void on_window_resize(Split::WindowResize& resize);
	void on_mouse_window_border(Split::MouseWindowBorder& event);
	void on_key_press(Split::KeyPress& key);
	void event_bus_subscribe(void) override;
};

#endif /* MAP_CAMERA_H */
