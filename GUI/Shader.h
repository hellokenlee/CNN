/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

//读取类别：从文件还是从字符串
enum shader_modes {SHADER_FROM_FILE, SHADER_FROM_STRING};
//
// 	Shader类: 读取，编译，链接GLSL
//
class Shader {
public:
    //opengl 中的shader程序ID
    GLuint programID;
public:
    //构造函数：从文件构造
    Shader(const GLchar* vShaderFilePath, const GLchar* fShaderFilePath, const bool debug = true, shader_modes mode = SHADER_FROM_FILE);
    //析构函数 释放Shader程序空间
    ~Shader();
    //使用该Shader程序
    void use();
    //从文件中读取Shader代码
    static std::string readShaderFile(const GLchar* shaderFilePath);
    //增加可选着色器
    bool addOptionalShader(const GLchar* shaderFilePath, GLenum shaderType);
private:
    Shader();
    void compileCheck(const char* sName, GLuint shaderID);
    void linkCheck();
    GLuint fShader, vShader;
};


#endif	//	SHADER_H
