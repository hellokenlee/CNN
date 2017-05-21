/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "Texture.h"
#define nDEBUG

Texture::Texture(const char* filename, texture_type type, GLenum image_format, GLint internal_format,
                 GLint level, GLint border, GLenum wrap, GLenum filter) {
    texID = 0;
    this->type = type;
    //图片格式
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //图片数据
    BYTE *bits(0);
    //图片指针
    FIBITMAP *image(0);
    //宽，高
    GLuint width, height;
    //检查格式
    fif = FreeImage_GetFileType(filename, 0);
    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename);
    if(fif == FIF_UNKNOWN) {
#ifdef DEBUG
        std::cout << "Texture Load Failed! Unknow Image Type!" << endl;
#endif // DEBUG
        return;
    }
    //载入图片
    if(FreeImage_FIFSupportsReading(fif))
        image = FreeImage_Load(fif, filename);
    if(!image) {
#ifdef DEBUG
        cout << "Texture Load Failed! Can not find Image!" << endl;
#endif // DEBUG
        return;
    }
    FIBITMAP *imagePtr = image;
    //检查是否PNG图片，是转换成32位
    if(fif == FIF_PNG) {
        imagePtr = FreeImage_ConvertTo32Bits(image);
    }
    FreeImage_FlipVertical(imagePtr);
    //获取数据
    bits = FreeImage_GetBits(imagePtr);
    width = FreeImage_GetWidth(imagePtr);
    height = FreeImage_GetHeight(imagePtr);
    if((bits == 0) || (width == 0) || (height == 0)) {
#ifdef DEBUG
        cout << "Texture Load Failed! Can not load Image!" << endl;
#endif // DEBUG
        return;
    }

    //生产纹理
    glGenTextures(1, &texID);
    //指定纹理类型
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    //载入图片数据到纹理
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, bits);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //释放图片
    if(imagePtr != image)
        FreeImage_Unload(imagePtr);
    FreeImage_Unload(image);
    //恢复状态机状态
    glBindTexture(GL_TEXTURE_2D, 0);
    //
}
Texture::~Texture() {
    //glDeleteTextures(1,&texID);
}
