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

GLuint VAO;
GLuint n_elements;

// camera
Camera cam;

// Current program id
GLuint program_id;
std::vector<GLuint> programmes_id;
int id_n = 0;


void init()
{
    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);
  
    program_id = glhelper::create_program_from_file("shaders/basic.vert", "shaders/basic.frag");
    programmes_id.push_back(program_id);
    program_id = glhelper::create_program_from_file("shaders/texture.vert", "shaders/texture.frag");
    programmes_id.push_back(program_id);
    
    program_id = glhelper::create_program_from_file("shaders/texture.vert", "shaders/texture_mouv.frag");
    programmes_id.push_back(program_id);

    program_id = glhelper::create_program_from_file_with_geom("shaders/texture_6.vert", 
                                                            "shaders/texture.frag",
                                                            "shaders/explode.geom");
    programmes_id.push_back(program_id);

   
    

    Mesh m = Mesh::load_from_file("data/Frankie/Frankie.obj");
    m.compute_normales();
    n_elements= m.size_element();
    VAO = m.load_to_gpu();

    glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

void set_uniform_time(GLuint program)
{
    static auto t_start = std::chrono::high_resolution_clock::now(); // Temps de départ, mémorisé

    GLint time_id = glGetUniformLocation(program, "time"); // Récupère l'emplacement de l'uniforme
    GLint current_prog_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_prog_id);
    glUseProgram(program);
  
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float>(t_now - t_start).count(); // Temps écoulé en secondes
    glUniform1f(time_id, time); // Passe la valeur au shader
  
    glUseProgram(current_prog_id); // Restaure le programme précédent
}

static void display_callback()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program_id);
    glBindVertexArray(VAO);
    set_uniform_mvp(program_id);
    set_uniform_time(program_id);

    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);




    glBindVertexArray(0);
    glutSwapBuffers ();
}

static void switch_programme(){
        id_n = (id_n + 1) % programmes_id.size();
        program_id = programmes_id[id_n];
        std::cout<<"programme : "<<id_n<<std::endl;
}


static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'p':
      glhelper::print_screen(cam.width(), cam.height());
      break;
    case 'q':
    case 'Q':
    case 'i':
      switch_programme();
      break;
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
  glutInitWindowSize(cam.height(), cam.width());
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
  //Position camera pour culling
  std::cout << "Camera at : " << glm::to_string(cam.position()) << std::endl;
  glutMainLoop();

  return 0;
}
