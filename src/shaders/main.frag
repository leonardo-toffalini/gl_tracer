#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform float ambient_strength;
uniform float specular_strength;

void main() {
  vec3 frag_coord = FragPos;
  frag_coord.y = frag_coord.y;
  frag_coord = (frag_coord + vec3(0.5f, 0.5f, 0.0f));
  vec3 ray_direction = frag_coord - vec3(0.0f);
  vec3 unit_dir = ray_direction / (dot(ray_direction, ray_direction));
  float a = 0.5f * (unit_dir.y + 1.0f);

  vec3 frag_color = (1.0f - a) * vec3(1.0f) + a * vec3(0.5f, 0.5f, 1.0f);
  FragColor = vec4(frag_color, 1.0f);
}

