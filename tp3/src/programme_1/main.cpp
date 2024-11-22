#include <iostream>
#include <fstream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif
#include "glhelper.h"
#include "mesh.h"
#include "glm/gtx/transform.hpp"

unsigned int VAO;
int program_id;

// Méthode d'initialisation pour afficher un carre qui recouvre la fenêtre
void init()
{
    program_id = glhelper::create_program_from_file("shaders/textured_quad.vert", "shaders/flou_shaderToy.frag");

  Mesh m = Mesh::create_grid(2);
  auto rmat = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  m.apply_matrix(rmat);
  VAO = m.load_to_gpu();

  glhelper::load_texture("data/squirel.png");
}

// Boucle d'affichage
static void display_callback()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

  glBindVertexArray(0);
  glutSwapBuffers ();
  glutPostRedisplay();
}

static void keyboard_callback(unsigned char key, int, int)
{
  int viewport[4];

  switch (key)
  {
    case 'r':
      glDeleteProgram(program_id);
      program_id = glhelper::create_program_from_file("shaders/textured_quad.vert", "shaders/texture.frag");
      break;
    case 'p':
      glGetIntegerv(GL_VIEWPORT, viewport);
      glhelper::print_screen(viewport[2], viewport[3]);
      break;
    case 'q':
    case 'Q':
    case 27:
              exit(0);
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glewExperimental=true;
  glewInit();

  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  std::cout << "OpenGL Version: " << major <<"."<< minor << std::endl;

  init();
  glutMainLoop();

  return 0;
}