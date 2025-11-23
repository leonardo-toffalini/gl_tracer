#ifndef CAMERA_H
#define CAMERA_H

enum direction {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class Camera {
public:
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right;

  // roll is not supported (yet)
  float yaw = -90.0f;
  float pitch = 0.0f;
  float speed = 2.5f;
  float sensitivity = 0.1f;
  float fov = 45.0f;

  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : position(position), up(up), yaw(yaw), pitch(pitch) {
    update_vectors();
  }
  Camera(glm::vec3 position) : position(position) {
    update_vectors();
  }

  glm::mat4 get_view_matrix() {
    return glm::lookAt(position, position + front, up);
  }

  void process_keyboard_input(direction d, float dt) {
    float vel = speed * dt;
    switch (d) {
      case FORWARD:
        position += vel * front;
        break;
      case BACKWARD:
        position -= vel * front;
        break;
      case LEFT:
        position -= vel * glm::normalize(glm::cross(front, up));
        break;
      case RIGHT:
        position += vel * glm::normalize(glm::cross(front, up));
        break;
      case UP:
        position += vel * up;
        break;
      case DOWN:
        position -= vel * up;
        break;
      default:
        break;
    }
  }

  void process_mouse_movement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;
    pitch = fmin(fmax(-89.0f, pitch), 89.0f);

    update_vectors();
  }

  void process_scroll(double yoffset) {
    fov -= (float)yoffset;
    fov = fmin(fmax(1.0f, fov), 45.0f);
  }

  void update_vectors() {
    front = glm::normalize(glm::vec3(
      cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
      sin(glm::radians(pitch)),
      sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));

    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));

  }

};

#endif // !CAMERA_H

