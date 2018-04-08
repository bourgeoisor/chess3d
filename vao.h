#ifndef VAO_H
#define VAO_H

#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class Vao {
    public:
        static GLuint loadObj(std::string filename);
};

#endif