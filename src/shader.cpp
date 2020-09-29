#include "shader.h"

using std::string;
using std::fstream;
using std::stringstream;
using std::endl;
using std::cout;

shader::shader(string name, string path, GLenum tp) :
  name{name}, path{path}, type{tp}
{
  stringstream buf{};
  try {
    fstream fs{path, std::ios_base::in};
    buf << fs.rdbuf();
    fs.close();
  } catch (std::ios_base::failure) {
    status = false;
    throw;
  }

  string content {buf.str()};
  status = compile(content);
}

/* shader::~shader() */
/* { */
/* } */

string shader::get_type_str() const
{
  switch (type) {
    case GL_VERTEX_SHADER:
      return std::string("vertex");
    case GL_FRAGMENT_SHADER:
      return std::string("fragment");
    case GL_GEOMETRY_SHADER:
      return std::string("geometry");
    default:
      return std::string("nothing");
  }
}

bool shader::compile(std::string& src)
{
  id = glCreateShader(type);
  const GLchar *code = src.c_str();
  glShaderSource(id, 1, &code, NULL);
  glCompileShader(id);

  GLint res{};
  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE) {
    cout << get_type_str() << " shader compiled faild" << endl;
    GLint len{};
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
    char *buf = new char[len];
    glGetShaderInfoLog(id, len, NULL, buf);
    cout << buf << endl;
    delete[] buf;
    return false;
  }
  return true;
}

/* string& shader::query_status() const */
/* { */
/* } */
