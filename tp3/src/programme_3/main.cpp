#include <iostream>
#include <fstream>
#include <chrono>

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
#include "camera.h"

GLuint program_id;
GLuint VAO;
GLuint n_elements;
GLuint n_points;
GLuint texture_id;


// Create 4 VBO : 2 for the positions (TF needs read and write), 1 for the normales, 1 for the texcoord
// You can use the enum
enum VBO_TYPE{POSITION0, POSITION1, NORMAL, TEXCOORD};
GLuint VBO[4]; 

Camera cam;

void init()
{
  program_id = glhelper::create_program_from_file("shaders/basic.vert", "shaders/texture.frag");

  // Creation of the main character
  Mesh m = Mesh::load_from_file("data/Frankie/Frankie.obj");
  auto rmat = glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  m.apply_matrix(rmat);
  m.compute_normales();
  m.normalize();
  texture_id = glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");
  auto pos = m.position();
  auto normal = m.normal();
  auto texcoord = m.texcoord();
  n_elements=m.size_element(); // number of indices in the mesh 
  n_points = pos.size()/3; // number of vertices in the mesh 

  // TODO
  // Create transform feedback shaders with the tf.vert code (you can read again the glhelper functions to inspire)
  // Create the program for the transform feedback, you MUST declare the varyings (pos) for the TF before linking!
  // Create, fill and specify variables of the VAO, VBOs and EBO 
  //  -> VBO MUST be as defined in the global variables : SEPARATE, 1 VBO for pos, 1 VBO for normal, 1 VBO for texcoord, 1 VBO for TF writting
  //  -> you can read again the Mesh::load_to_gpu() function but there is some changes
  //END TODO

  glEnable(GL_DEPTH_TEST);            
}

void set_uniform_mvp(GLuint program)
{
  GLint mvp_id = glGetUniformLocation(program, "MVP");
  GLint current_prog_id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current_prog_id);
  glUseProgram(program);
  if (mvp_id != -1)
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = cam.projection()*cam.view()*model;
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
  }
  glUseProgram(current_prog_id);
}


static void display_callback()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  // TODO
  // Disable rasterisation
  // Switch to tf program
  // Use the buffer to fill with the TF information -> glBindBufferBase()
  // Use the good VAO, be sure the pointers to data are valid
  // Start the TF and "draw" points
  // Wait for the buffer to be filled
  // Swap read and write buffers
  //END TODO



  glUseProgram(program_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBindVertexArray(VAO);
  // TODO 
  // Update VAO after TF to use the new VBO with correct pointers
  // END TODO

  set_uniform_mvp(program_id);
  glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0); 

  glBindVertexArray(0);
  glutSwapBuffers ();
}

static void keyboard_callback(unsigned char key, int, int)
{
  int viewport[4];

  switch (key)
  {
    case 'r':
      glDeleteProgram(program_id);
      program_id = glhelper::create_program_from_file("shaders/basic.vert", "shaders/texture.frag");
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
  glutPostRedisplay();
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);

  glViewport(0,0, width, height); 
  glutPostRedisplay();
}

static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);
  glutPostRedisplay();
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
  glutPostRedisplay();
}

static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(cam.width(), cam.height());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);
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