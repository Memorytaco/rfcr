#ifndef RFCSHADER
#define RFCSHADER

#include "core.h"

class shader
{
  public:
    // load from file
    shader(std::string name, std::string path, GLenum tp);
    ~ shader() {};
    // get shader handle
    GLuint get_shader() const { return id; }
    bool get_status() const { return status; }
    GLenum get_type() const { return type; }
    std::string get_type_str() const;
    bool compile(std::string& src);
    // TODO: build status info query function
    /* std::string& query_status() const; */
  private:
    GLuint id {};
    GLenum type;      // shader type vertex or fragment etc...
    std::string name; // shader arbitrary name
    std::string path; // shader file path
    bool status {false};  // shader avalible status
};

#endif
