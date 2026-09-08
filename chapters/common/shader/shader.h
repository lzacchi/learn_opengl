#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader {
  public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shaders
    // Shader(const char *vertexPath, const char *fragmentPath);
    // use/activate the shader
    // void use();
    // utility uniform functions
    // void setBool(const std::string &name, bool value) const;
    // void setInt(const std::string &name, int value) const;
    // void setFloat(const std::string &name, float value) const;

    // Definitions. TODO: Move to cpp file
    Shader(const char *vertexPath, const char *fragmentPath);

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;
};

#endif // SHADER_H
