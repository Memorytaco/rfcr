// include the core usefull header
#include "core.h"
#include "shader.h"
#include "rfcdoc.h"
#include "font.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::endl;
using std::cout;
using std::string;
using std::map;

class ShaderStock {
  ShaderStock() {}
  ~ShaderStock() {}
};

#define RFCDOCVIEWER "0.0.1-r1"

void render_text(GLuint program, std::map<char, FontTexture>& codemap, unsigned int &VAO, unsigned int &VBO, float x, float y, float scale, glm::vec3 color, const string &text) {
  glUseProgram(program);
  glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    FontTexture font = codemap[*c];
    float xpos = x + font.bearing.x * scale;
    float ypos = y - (font.size.y - font.bearing.y) * scale;
    float w = font.size.x * scale;
    float h = font.size.y * scale;
    float vertices[6][4] = {
          { xpos,     ypos + h,   0.0f, 0.0f },
          { xpos,     ypos,       0.0f, 1.0f },
          { xpos + w, ypos,       1.0f, 1.0f },

          { xpos,     ypos + h,   0.0f, 0.0f },
          { xpos + w, ypos,       1.0f, 1.0f },
          { xpos + w, ypos + h,   1.0f, 0.0f }
      };
    glBindTexture(GL_TEXTURE_2D, font.id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (font.advance >> 6) * scale;
  }
}

/*
  global control data here
*/

int page = 1;
int baseline = 1;
int maxpage;
int maxline;
int fontheight = 0;
int fontbeary = 0;
int width;
int height;
float scale = 1.0f;

// call back functions
void framebuffer_size_callback(GLFWwindow* win, int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
  /* cout << "Got xoffset " << xoffset << ", got yoffset " << yoffset << endl; */
  if (yoffset < 0) {
    baseline = baseline <= 0? 0 : baseline - 1;
  } else {
    baseline += 1;
  }
}

void keys(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_EQUAL:
        scale += 0.1f;
        break;
      case GLFW_KEY_MINUS:
        scale = scale <= 0.1f? 0.1f: scale-0.1f;
        break;
      case GLFW_KEY_F:
        page = page >= maxpage? maxpage : page + 1;
        baseline = 0;
        break;
      case GLFW_KEY_B:
        page = page <= 1? 1 : page - 1;
        baseline = 0;
        break;
      case GLFW_KEY_D:
        baseline += 1;
        break;
      case GLFW_KEY_U:
        baseline = baseline <= 0? 0 : baseline - 1;
        break;
    }
  }
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    cout<< argv[0] << " [rfc num]" << endl;
    exit(0);
  }

  RFConfig conf{"/home/eilliot/.rfc/repo/text/", "/home/eilliot/.local/share/fonts/", "/home/eilliot/.rfc/shaders/"};
  RFCIndex indx{conf, "rfc-index.txt"};

  // prepare the rfc document
  int num = std::stoi(argv[1]);
  indx.consoleprint(num);
  RFCDoc document{conf, num};
  maxpage = document.page_num();
  maxline = document.line_num();

  FontResLib fontlib;
  fontlib.add_fontface(conf.fontdir, "CamingoCode-Regular.ttf", 0);
  fontlib.set_char_size(0, 0, 24);

  // initialize gl here
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "RFC Reader", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, keys);
  glfwSetScrollCallback(window, scroll);

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // setting up shader
  shader vertex{"font_vertex", conf.shaderdir + "font.vert", GL_VERTEX_SHADER};
  shader fragment{"font_fragment", conf.shaderdir + "font.frag", GL_FRAGMENT_SHADER};
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex.get_shader());
  glAttachShader(program, fragment.get_shader());
  glLinkProgram(program);
  GLint status = false;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    cout << "program link failed" << endl;
    GLint len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    char *buf = new char[len];
    glGetProgramInfoLog(program, len, NULL, buf);
    cout << buf << endl;
    exit(1);
  }

  glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f);
  glUseProgram(program);
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  // font map
  std::map<char, FontTexture> codemap;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (unsigned char c = 0; c < 128; c++) {
    if (fontlib.load_char(0, c))
      cout << "Error on loading char " << c << endl;
    FT_Face &face = fontlib.get_face(0);
    FontTexture tex{face};
    codemap[(char)c] = tex;
  }
  fontheight = 8 + codemap['W'].size.y;
  fontbeary = codemap['W'].bearing.y;

  // vao and vbo
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::vec3 color{0.92f, 0.85f, 0.69f};
  int line = 20;
  while (!glfwWindowShouldClose(window)) {
    projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // clear the surface
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.156f, 0.156f, 0.156f, 1.0f);

    // to render the page
    int line;
    const Range &pagerange = document.get_page(page);
    for (line = pagerange.begin + baseline; line <=pagerange.end; line++) {
      int y = height - fontbeary - (line - pagerange.begin - baseline) * fontheight;
      render_text(program, codemap, VAO, VBO, 0, y, scale, color, document.at(line));
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
