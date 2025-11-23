#ifndef SHADER_HELPERS_H
#define SHADER_HELPERS_H

const char *read_file(const char *filename);
GLuint load_shader(const char *filename, GLenum shader_type);
GLuint create_shader_program(const char *vertex_fname, const char *fragment_fname);
void set_mat4(GLuint program, const GLchar *name, const GLfloat *value);
void set_float(GLuint program, const GLchar *name, const GLfloat value);

const char *read_file(const char *filename) {
  FILE *file = fopen(filename, "rb");       // Open in binary mode to read all bytes as-is
  if (!file) return NULL;

  fseek(file, 0, SEEK_END);                 // Move to end of file
  long size = ftell(file);                  // Get file size
  fseek(file, 0, SEEK_SET);                 // Move back to beginning

  char *buffer = (char *)malloc(size + 1);  // Allocate buffer with room for null terminator
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, size, file);        // Read file into buffer
  buffer[size] = '\0';                 // Null terminate

  fclose(file);
  return buffer;                       // Returned pointer is const char * for read-only use
}

GLuint load_shader(const char *filename, GLenum shader_type) {
  const char *shaderSource = read_file(filename);
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("ERROR::SHADER::COMPILATION_FAILED\n");
    printf("%s", infoLog);
  }

  free((void*)shaderSource);

  return shader;
}

GLuint create_shader_program(const char *vertex_fname, const char *fragment_fname) {
  GLuint vertexShader = load_shader(vertex_fname, GL_VERTEX_SHADER);
  GLuint fragmentShader = load_shader(fragment_fname, GL_FRAGMENT_SHADER);

  // link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    printf("%s", infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void set_vec3(GLuint program, const GLchar *name, const GLfloat *value) {
  GLuint loc = glGetUniformLocation(program, name);
  glUniform3fv(loc, 1, value);
}

void set_mat4(GLuint program, const GLchar *name, const GLfloat *value) {
  GLuint loc = glGetUniformLocation(program, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void set_float(GLuint program, const GLchar *name, const GLfloat value) {
  GLuint loc = glGetUniformLocation(program, name);
  glUniform1f(loc, value);
}

#endif // !SHADER_HELPERS_H
