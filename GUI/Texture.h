/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <FreeImage.h>

//
//	纹理类（用于Model类）
//
enum texture_type {TEXTURE_DIFFUSE, TEXTURE_SPECULAR, TEXTURE_REFLECT};

class Texture {
public:
    //参数构造
    Texture(
        const char* filepath,				//路径
        texture_type type,                  //贴图类型，漫反射贴图or光照贴图
        GLenum image_format = GL_BGR,		//图片数据格式
        GLint internal_format = GL_RGB,		//内部数据格式
        GLint level = 0,					//多级纹理(Mipmapping) level
        GLint border = 0,					//边界宽度
        GLenum wrap = GL_REPEAT,			//环绕模式(处理越界访问)
        GLenum filter = GL_LINEAR			//过滤模式(处理上采样)
    );
    //析构
    ~Texture();
public:
    //纹理ID
    GLuint texID;
    //类型
    texture_type type;
private:
    //私有默认构造
    Texture();
//	Texture(const Texture &);
//	Texture& operator=(const Texture &);

};

#endif	//	TEXTURE_H
