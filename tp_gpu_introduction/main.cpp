#include <iostream>
#include <fstream>
#include <vector>

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int VBO, VAO, EBO, VBO_couleurs;
int programme;
int programme_squirrel;
int usep=1;


// Renvoi le contenu d'un fichier
std::string lit_fichier(const std::string& filename)
{
  std::ifstream ifs;
  ifs.open(filename.c_str());
  if (!ifs)
  {
    std::cout << "ERROR: FILE " << filename << " NOT FOUND" << std::endl;
  }
  return std::string((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));
}

// Creation d'une image
void load_image(const std::string& filename, unsigned char*& image, int& w, int& h, bool& rgb)
{
  int comp;
  image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);
  if(image == nullptr)
    std::cout << "ERROR: IMAGE " << filename << std::endl;

  if(comp == 3)
    rgb = true;
  else
  {
    rgb = false;
  }
}

// Creation texture et envoie sur GPU, bind la texture dans la machine d'état
void load_texture(std::string filename)
{
  unsigned int texture_id;
  unsigned char* im;
  int w;
  int h;
  bool rgb;
  load_image(filename, im, w, h, rgb);

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if(rgb)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, im);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, im);
  stbi_image_free(im);
}

// Creation d'un shader
// shader_source : code GLSL
// shader_type : enum représentant le type de shader (GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER)
// Renvoie l'identifiant du shader
int compile_shader(const char* shader_source, int shader_type)
{
  int success;
  char log[128];

  // TODO:
  // Créer un shader vide -> glCreateShader(GLenum)
  // Mettre le code voulu dans le shader
  //  -> glShaderSource(GLuint, 1, const GLchar * const *, NULL);
  // Compiler le shader -> glCompileShader(GLuint);
  int shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_source, NULL);
  glCompileShader(shader_id);

  
  // FIN TODO
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader_id, 128, nullptr, log);
    std::cout << "ERROR: SHADER COMPILATION FAILED\n" << log << std::endl;
  }
  return shader_id;
}

// Creation programme GPU (vertex + fragment)
// vertex_shader : Contenu du shader
// fragment_shader : Contenu du shader
// Renvoie l'identifiant du programme
int creation_programme(const std::string& vertex_shader, const std::string& fragment_shader)
{
  int success;
  char log[128];

  // TODO:
  // Récupérer le vertex et le fragment shader compilé (utilisez std::string::c_str)
  // Créer un nouveau programmme vide GPU -> glCreateProgram()
  // Lier les deux shader au programme GPU -> glAttachShader(GLuint, GLuint)
  // Lier le programme a la CG -> glGetLinkProgram()
  // glCreateProgram();
  // glAttachShader(GLuint, GLuint);
  // glGetLinkProgram();
  

  int vertex_id = compile_shader(vertex_shader.c_str(),GL_VERTEX_SHADER);
  int fragment_id = compile_shader(fragment_shader.c_str(),GL_FRAGMENT_SHADER);
  int shaderProgram = glCreateProgram();


  glAttachShader(shaderProgram,vertex_id);
  glAttachShader(shaderProgram,fragment_id);

  glLinkProgram(shaderProgram);
  // FIN TODO

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 128, NULL, log);
    std::cout << "ERROR: SHADER LINKING FAILED\n" << log << std::endl;
  }

  // TODO
  // Supprimer les deux shaders -> glDeleteShader(GLuint)
  // glDeleteShader(GLuint);
  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);

  // FIN TODO

  return shaderProgram;
}

// Méthode d'initialisation pour afficher un carre qui recouvre la fenêtre
void init()
{
  load_texture("squirel.png");
  // TODO :
  // Lire les fichiers contenant les programmes des shaders puis les utiliser pour créer le programme
  std::string fs = lit_fichier("color.fs"); // C'est notre programme  que l'on veut run 
  std::string vs = lit_fichier("color.vs");

  std::string squirrel_fs = lit_fichier("color_sommet.fs"); // C'est notre programme  que l'on veut run 
  std::string squirrel_vs = lit_fichier("color_sommet.vs");

  programme = creation_programme(vs,fs);
  programme_squirrel = creation_programme(squirrel_vs,squirrel_fs);
  // Créer un tableau de float contenant les sommets à afficher
  std::vector<float> sommets = {
      // Positions des sommets (x, y, z) avec des couleurs
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,  // Sommet 1 (bas-gauche)
      0.5f, -0.5f, 0.0f,   1.0f, 1.0f,   // Sommet 2 (bas-droite)
      0.5f,  0.5f, 0.0f,   1.0f, 0.0f,   // Sommet 3 (haut-droite)
      -0.5f,  0.5f, 0.0f,  0.0f, 0.0f   // Sommet 4 (haut-gauche)
  };

  std::vector<float> couleurs = {
    1.0f, 0.0f, 0.0f,  // Couleur pour le sommet 1 (bas-gauche)
    0.0f, 1.0f, 0.0f,  // Couleur pour le sommet 2 (bas-droite)
    0.0f, 0.0f, 1.0f,  // Couleur pour le sommet 3 (haut-droite)
    1.0f, 1.0f, 0.0f   // Couleur pour le sommet 4 (haut-gauche)
};

  // Créer un ficher d'entier non signé contenant les indices de sommets
  std::vector<unsigned int> indices = {
    0, 1, 2,
    2, 3, 0  // Les trois sommets du triangle
  };


  // Créer un VAO -> glGenVertexArrays(GLsizei, GLuint *)
  glGenVertexArrays(1,&VAO); // VAO contient les liens entre VBO et EBO
  // Créer un VBO puis un EBO -> glGenBuffers(GLsizei, GLuint *)
  glGenBuffers(1,&VBO); // VBO contient les donnees des sommets 
  glGenBuffers(1,&EBO); // EBO contient les indices des sommets
  // Mettre le VAO en actif dans la machine d'état -> glBindVertexArray(GLuint)
  glBindVertexArray(VAO);
  // Remplir le VBO puis l'EBO en utilisant la machine d'état.
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

  
  //  -> glBindBuffer(GLenum, GLuint) et glBufferData(GLenum, GLsizeiptr, const GLvoid*, GL_STATIC_DRAW);
  // Specifier comment parcourir les buffers crées (utilise le dernier VBO de type GL_ARRAY_BUFFER) :
  glBufferData(GL_ARRAY_BUFFER, sommets.size() * sizeof(float), sommets.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  //  -> glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, glsizei, const void*)
  //      Pour l'indice, se référer au vertex shader !

  ///////////////// POUR LES POINTS DANS L'ESPACE
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0*sizeof(float)));
  // Activer notre tableau de vertices : glEnableVertexAttribArray(GLuint)
  glEnableVertexAttribArray(0);

  ///////////////// POUR LA TEXTURE
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
  // Activer notre tableau de vertices : glEnableVertexAttribArray(GLuint)
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &VBO_couleurs);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_couleurs);
  glBufferData(GL_ARRAY_BUFFER, couleurs.size() * sizeof(float), couleurs.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(2);


  //
  // Le VAO permet de ne pas répéter les étapes de lecture des buffers à chaque affichage !
  // Les VAO peuvent être obligatoires dans certains cas.

  // END TODO


  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Pas nécessaire mais permet de contrôler un peu ce qui est fait...
  // Globalement tous les GlEnable...
  // Décommenter et essayer de comprendre
  // glEnable(GL_CULL_FACE);
  // glFrontFace(GL_CCW);
  // glCullFace(GL_FRONT);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
}

// Boucle d'affichage
static void display_callback()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO :
  // Specifier le programme -> glUseProgram(GLuint)
  init();
  if (usep==0){
    glUseProgram(programme);
  }
  else if(usep==1){
    glUseProgram(programme_squirrel);
    GLint radiusLocation = glGetUniformLocation(programme_squirrel, "circleRadius");
    float radius = 0.2f; // Rayon du cercle (20% de l'image)
    glUniform1f(radiusLocation, radius);
  }
  // Specifier le VAO à utiliser -> glBindVertexArray(GLuint)
  glBindVertexArray(VAO);
  // Demander affichage -> glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // END TODO
  glBindVertexArray(0);
  glutSwapBuffers ();
  // Pour afficher de nouveau
  glutPostRedisplay();
}

void restart_programme(){
      glDeleteProgram(programme);

      std::cout<<"programme delete "<<std::endl;
      std::string fs = lit_fichier("color.fs"); // C'est notre programme  que l'on veut run 
      std::string vs = lit_fichier("color.vs");
      programme = creation_programme(vs,fs);

}

void switch_programme(){
      if (usep==0){
        usep=1;
      }
      else if (usep==1){
        usep=0;
      }
}

static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'q':
    case 'Q':
    case 'r': 
      restart_programme();
      break;
    case 's': 
      switch_programme();
      break;
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

  init();

  glutMainLoop();

  return 0;
}
