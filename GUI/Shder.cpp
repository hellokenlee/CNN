/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/

#include "Shader.h"

// 构造函数
Shader::Shader(const GLchar* vShaderSourcePath, const GLchar* fShaderSourcePath, const bool debug, shader_modes mode) {
    this->programID = 0;
    const GLchar* vShaderCode;
    const GLchar* fShaderCode;
    std::string vShaderSource;
    std::string fShaderSource;
    if(mode == SHADER_FROM_FILE) {
        vShaderSource = readShaderFile(vShaderSourcePath);
        fShaderSource = readShaderFile(fShaderSourcePath);
        vShaderCode = vShaderSource.c_str();
        fShaderCode = fShaderSource.c_str();
    } else if(mode == SHADER_FROM_STRING) {
        vShaderCode = vShaderSourcePath;
        fShaderCode = fShaderSourcePath;
    }
    //编译Shader
    vShader = 0;
    fShader = 0;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, nullptr);
    glCompileShader(vShader);
    //输出错误
    if(debug)
        compileCheck("VertexShader", vShader);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCode, nullptr);
    glCompileShader(fShader);
    //输出错误
    if(debug)
        compileCheck("FragmentShader", fShader);
    //链接Shader程序
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vShader);
    glAttachShader(this->programID, fShader);
    glLinkProgram(this->programID);
    if(debug)
        linkCheck();
}

// 从文件读入
std::string Shader::readShaderFile(const char* shaderFilePath) {
    std::string shaderSourceString;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //从文件读入
        shaderFile.open(shaderFilePath);
        std::stringstream shaderSourceStream;
        shaderSourceStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderSourceString = shaderSourceStream.str();
    } catch(std::ifstream::failure e) {
        //处理异常
        std::cout << "Error::Shader(const char*,const char*)::Fail to find shader source file!" << std::endl;
        std::cout << e.what() << std::endl;
        return "";
    }
    return shaderSourceString;
}

// 析构函数
Shader::~Shader() {
    //释放program空间,包括attached的shaders
    if(programID != 0)
        glDeleteProgram(this->programID);
}

// 使用
void Shader::use() {
    glUseProgram(this->programID);
}

//检查GLSL编译情况
void Shader::compileCheck(const char* sName, GLuint shaderID) {
    GLint isSuccess;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccess);
    if(!isSuccess) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cout << "ERROR: Comlile " << sName << " Error:" << std::endl << infoLog << std::endl;
    }
}
//检查多Shader链接情况
void Shader::linkCheck() {
    GLint isSuccess;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &isSuccess);
    if(!isSuccess) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cout << "ERROR: Link Program Error:" << std::endl << infoLog << std::endl;
    }
}

//
bool Shader::addOptionalShader(const GLchar* shaderFilePath, GLenum shaderType) {
    std::string shaderSourceString = readShaderFile(shaderFilePath);
    GLuint shader = glCreateShader(shaderType);
    const GLchar *shaderCode = shaderSourceString.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    compileCheck("Optional Shader", shader);

    glAttachShader(this->programID, shader);
    glLinkProgram(this->programID);
    linkCheck();
    return true;
}
