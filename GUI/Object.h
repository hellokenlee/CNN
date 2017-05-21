/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef OBJECT_H
#define OBJECT_H

#include "Camera.h"
#include "Shader.h"
#include <vector>
//	顶点的数据格式
enum vertex_format {POSITIONS, POSITIONS_COLORS, POSITIONS_TEXTURES, POSITIONS_COLORS_TEXTURES, POSITIONS_NORMALS, POSITIONS_NORMALS_TEXTURES};

//
//	物体类(多个三维顶点的集合)
//

//!TODO: 根据法线和位置自动计算TBN矩阵，以适应法线贴图
//!TODO: 把view和projection两个矩阵放入到全局的UBO中减少传输带宽
//!TODO: 改进： Object的Shader为空的时候连续调用draw会出错。
//!             draw()结束前不把当前shader置空；uniform传入需要重新设
class Object {
public:
    //!注意： 传入的的VertexShader必须要有 model,view,projection这三个uniform
    //构造函数
    Object(GLfloat *vertices, GLuint vNum, vertex_format vf, GLenum mode,
           GLuint *indices = nullptr, GLuint iNum = 0,
           Shader *_shader = nullptr, Camera *_cam = nullptr);
    //析构函数
    ~Object();
    //画出这个物体，主循环调用
    void draw();
    //设置Shader
    void setShader(Shader *_shader);
    //设置相机
    void setCamera(Camera *_cam);
    //设置位置
    void moveTo(glm::vec3 pos);
    //缩放
    void scaleTo(GLfloat s);
    //根据纹理贴图中的法线方向和顶点的纹理坐标计算切线和副切线
    void calcTangents(GLfloat *vertices);
    //复制一个相同形状的物体使用同一块显存(同一个VBO和EBO), 更节省显存和载入速度更快，但注意，如果EBO的顶点值改变了，所有拷贝都会发生改变
    Object* clone();
public:
    //着色器
    Shader *shader;
    //摄像机
    Camera *cam;
    //物体的3个O
    GLuint VAO, VBO, EBO;
    //TBN矩阵的VBO
    GLuint TBN_VBO;
    //model矩阵
    glm::mat4 model;
private:
    //禁止默认构造
    Object();
    Object(const Object& o);
    Object& operator=(const Object& o);
    //顶点个数
    GLuint vertexNum;
    //如果有EBO，
    GLuint indexNum;
    //绘制模式 如: GL_LINES
    GLenum	mode;
    //view矩阵，projection矩阵在Shader中的位置
    GLuint viewPos, projPos, modelPos;
    //顶点格式
    vertex_format vf;
    //记录位移数据
    glm::vec3 translation;
    //记录缩放
    GLfloat scale;
    //记录顶点数组指针
    //GLfloat *vertices;
    //记录下标数组指针
    //GLuint *indices;

};

#endif	//	OBJECT_H
