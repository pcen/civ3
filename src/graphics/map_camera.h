#ifndef MAP_CAMERA_H
#define MAP_CAMERA_H

#include "events/event_bus_client.h"
#include "rendering/cameras/camera.h"

/* Forward declarations
 */
namespace Split
{
	struct mouse_data;
	class WindowResize;
	class MouseWindowBorder;
	class KeyPress;
};

class MapCamera : public Split::Camera, Split::EventBusListener
{
public:
	MapCamera(glm::vec3 position, glm::ivec2 screen_size);
	~MapCamera();

	glm::mat4& get_view_matrix(void) override;

	void set_sensitivity(float sensitivity);
	void set_speed(float speed);

	void update(double dt) override;

private:
	glm::mat4 m_view_matrix, m_proj_matrix;
	glm::vec3 m_position, m_front, m_up;
	glm::ivec2 m_screen_size;
	float m_pitch, m_yaw, m_roll;
	float m_sensitivity, m_speed, m_real_speed;
	float m_fov, m_ratio;
	bool m_mouse_on_screen;
	int m_move_threshold;

	bool move_in_x(Split::mouse_data& mouse);
	bool move_in_y(Split::mouse_data& mouse);

	float aspect_ratio(void);
	void on_window_resize(Split::WindowResize& resize);
	void on_mouse_window_border(Split::MouseWindowBorder& event);
	void on_key_press(Split::KeyPress& key);
	void event_bus_subscribe(void) override;
	void update_view_matrix(void);

};

#endif /* MAP_CAMERA_H */
