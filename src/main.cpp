#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_helpers.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double x_pos_int, double y_pos_in);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
GLuint load_and_create_texture(const char *filename, GLenum format, GLint wrap_type);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

bool first_mouse = true;
float last_x = 800.0f / 2.0f;
float last_y = 600.0f / 2.0f;

// timing
float delta_time = 0.0f;
float last_frame = 0.0f;

glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
float ambient_strength = 0.1f;
float specular_strength = 0.5f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
    float vertices[] = {
    //       pos
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
  };
  GLuint VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  GLuint main_shader = create_shader_program(
    "src/shaders/main.vert",
    "src/shaders/main.frag"
  );

  while (!glfwWindowShouldClose(window)) {
    float current_frame = (float)glfwGetTime();
    float u_time = (float)glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    light_pos = glm::vec3(2.0f * cos(u_time / 4.0f), 1.0f, 2.0f * sin(u_time / 4.0f));

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(main_shader);
    // set_vec3(lighting_shader, "object_color", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    // set_vec3(lighting_shader, "light_color", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    // set_vec3(lighting_shader, "light_pos", glm::value_ptr(light_pos));
    // set_vec3(lighting_shader, "view_pos", glm::value_ptr(camera.position));
    // set_float(lighting_shader, "ambient_strength", ambient_strength);
    // set_float(lighting_shader, "specular_strength", specular_strength);

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
    glm::mat4 model = glm::mat4(1.0f);

    set_mat4(main_shader, "view", glm::value_ptr(view));
    set_mat4(main_shader, "projection", glm::value_ptr(projection));
    set_mat4(main_shader, "model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glDeleteProgram(lighting_shader);
  // glDeleteProgram(light_cube_shader);

  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  // movement
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.process_keyboard_input(RIGHT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.process_keyboard_input(LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.process_keyboard_input(BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.process_keyboard_input(FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.process_keyboard_input(UP, delta_time);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.process_keyboard_input(DOWN, delta_time);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos_in, double ypos_in) {
  if (first_mouse) {
    last_x = xpos_in;
    last_y = ypos_in;
    first_mouse = false;
  }

  float xoffset = xpos_in - last_x;
  float yoffset = ypos_in - last_y;
  last_x = xpos_in;
  last_y = ypos_in;

  camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.process_scroll(yoffset);
}

