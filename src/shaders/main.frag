#version 330 core
out vec4 FragColor;

in vec3 FragPos;

float hit_sphere(vec3 center, float radius, vec3 ro, in vec3 rd) {
  vec3 oc = center - ro;
  float a = dot(rd, rd);
  float b = -2.0 * dot(rd, oc);
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return -1.0;
  else
    return (-b - sqrt(discriminant)) / (2.0 * a);
}

vec3 ray_at(vec3 ro, vec3 rd, float t) {
  return ro + t * rd;
}

void main() {
  vec3 camera_center = vec3(0.5, 0.5, 0.0);
  vec3 ro = camera_center;
  vec3 rd = vec3(FragPos.xy, -1.0) - camera_center;
  vec3 unit_direction = normalize(rd);
  float a = unit_direction.y + 0.5;
  vec3 cbg = (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);

  vec3 sphere_center = vec3(0.5, 0.5, -2.0);
  float t = hit_sphere(sphere_center, 0.5, ro, rd);

  if (t > 0.0) {
    vec3 N = normalize(ray_at(ro, rd, t) - sphere_center);
    vec3 cn = 0.5 * (N + vec3(1,1,1));

    FragColor = vec4(cn, 1.0);
  }
  else
    FragColor = vec4(cbg, 1.0f);
}

