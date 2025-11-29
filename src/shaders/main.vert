#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

void main() {
  FragPos = (aPos + vec3(1.0f, 1.0f, 0.0f)) / 2.0f;

  gl_Position = vec4(aPos, 1.0f);
}


