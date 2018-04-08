#include "vao.h"

// Reads a .obj model and stores the vertices and normals in an OpenGL object.
GLuint Vao::loadObj(std::string filename) {
    std::ifstream inputfile(filename);

    std::vector<std::vector<float>> vertices, normals, textures;
    std::vector<unsigned int> vertex_indices, normal_indices, texture_indices;

    std::string str;
    while (std::getline(inputfile, str)) {
        if (str.size() > 2) {
            std::istringstream iss(str);
            std::string token;
            float vec[3];

            // Reads a vertex.
            if (str[0] == 'v' && str[1] == ' ') {
                std::getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    std::getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }

                vertices.push_back({vec[0], vec[1], vec[2]});
            }
            // Reads a normal.
            else if (str[0] == 'v' && str[1] == 'n') {
                getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }

                normals.push_back({vec[0], vec[1], vec[2]});
            }
            // Reads a texture.
            else if (str[0] == 'v' && str[1] == 't') {
                getline(iss, token, ' ');
                
                for (int i = 0; i < 2; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }
                
                textures.push_back({vec[0], vec[1]});
            }
            // Reads a vertex/texture/normal index list.
            else if (str[0] == 'f' && str[1] == ' ') {
                getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    size_t index;

                    if ((index = token.find("//")) != std::string::npos) {
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 2, token.size() - index);

                        vertex_indices.push_back(std::stoi(a) - 1);
                        normal_indices.push_back(std::stoi(b) - 1);
                    } 
                    else {
                        index = token.find("/");
                        size_t index2 = token.find("/", index + 1);
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 1, index2 - index - 1);
                        std::string c = token.substr(index2 + 1, token.size() - index2 + 1);
                        

                        vertex_indices.push_back(std::stoi(a) - 1);
                        texture_indices.push_back(std::stoi(b) - 1);
                        normal_indices.push_back(std::stoi(c) - 1);
                    }
                }
            }
        }
    }

    // Initializes the object.
    GLuint object = glGenLists(1);
    glNewList(object, GL_COMPILE);
    glPushMatrix();
    {
        glPointSize(1.0);

        // Draws the triangles of the object.
        glBegin(GL_TRIANGLES);
        {
            glColor3f(1.0,0.23,0.27);

            for (int i = 0; i < vertex_indices.size(); i+=3) {
                glNormal3f(normals[normal_indices[i]][0], normals[normal_indices[i]][1], normals[normal_indices[i]][2]);
                glVertex3f(vertices[vertex_indices[i]][0], vertices[vertex_indices[i]][1], vertices[vertex_indices[i]][2]);

                glNormal3f(normals[normal_indices[i+1]][0], normals[normal_indices[i+1]][1], normals[normal_indices[i+1]][2]);
                glVertex3f(vertices[vertex_indices[i+1]][0], vertices[vertex_indices[i+1]][1], vertices[vertex_indices[i+1]][2]);

                glNormal3f(normals[normal_indices[i+2]][0], normals[normal_indices[i+2]][1], normals[normal_indices[i+2]][2]);
                glVertex3f(vertices[vertex_indices[i+2]][0], vertices[vertex_indices[i+2]][1], vertices[vertex_indices[i+2]][2]);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();

    return object;
}