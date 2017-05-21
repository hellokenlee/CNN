/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/

#include "Object.h"
#define DEBUG
Object::Object(GLfloat *vertices, GLuint vNum, vertex_format _vf, GLenum _mode,
               GLuint *indices, GLuint iNum,
               Shader *_shader, Camera *_cam) {
    VAO = VBO = EBO = 0;
    translation = glm::vec3(0, 0, 0);
    scale = 1;
    mode = _mode;
    vertexNum = vNum;
    indexNum = iNum;
    shader = _shader;
    cam = _cam;
    model = glm::mat4();
    modelPos = projPos = viewPos = 0;
    vf = _vf;
    this->TBN_VBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if(indexNum != 0)
        glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    if(indexNum != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }
    switch(vf) {
    case POSITIONS: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        break;
    }
    case POSITIONS_COLORS:
    case POSITIONS_NORMALS: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        break;
    }
    case POSITIONS_TEXTURES: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 5 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        break;
    }
    case POSITIONS_COLORS_TEXTURES:
    case POSITIONS_NORMALS_TEXTURES: {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        break;
    }
    }
    glBindVertexArray(0);

    if(shader != nullptr) {
        modelPos = glGetUniformLocation(shader->programID, "model");
        projPos = glGetUniformLocation(shader->programID, "projection");
        viewPos = glGetUniformLocation(shader->programID, "view");
    }
}

Object::Object() {
    VAO = VBO = EBO = 0;
    translation = glm::vec3(0, 0, 0);
    scale = 1;
    mode = GL_LINES;
    vertexNum = 0;
    indexNum = 0;
    shader = nullptr;
    cam = nullptr;
    model = glm::mat4();
    modelPos = projPos = viewPos = 0;
    vf = POSITIONS;
}

void Object::calcTangents(GLfloat *vertices){
    if(this->vf != POSITIONS_NORMALS_TEXTURES){
        // 若无纹理坐标，返回错误
        std::cout<<"Object::calcTangents() only support POSITIONS_NORMALS_TEXTURES mode\n";
        return;
    }
    if(this->mode != GL_TRIANGLES){
        // 暂时只支持三角形的计算
        std::cout<<"Object::calcTangents() can not calc tangents from non-triangle-mesh\n";
        return;
    }
    if(this->TBN_VBO != 0){
        // 检查是否已经计算过了
        std::cout<<"Object::calcTangents() tangents have already been calced\n";
        return;
    }

    int stride = 8;

    if(EBO == 0){
        std::vector<glm::vec3> TBN;
        for(int i = 0; i < vertexNum; i += 3){
            // 取出对应顶点下标
            int index1 = i + 0;
            int index2 = i + 1;
            int index3 = i + 2;
            // 取出对应顶点位置坐标
            glm::vec3 pos1(vertices[index1 * stride + 0], vertices[index1 * stride + 1], vertices[index1 * stride + 2]);
            glm::vec3 pos2(vertices[index2 * stride + 0], vertices[index2 * stride + 1], vertices[index2 * stride + 2]);
            glm::vec3 pos3(vertices[index3 * stride + 0], vertices[index3 * stride + 1], vertices[index3 * stride + 2]);
            // 取出法线
            glm::vec3 normal1(vertices[index1 * stride + 3], vertices[index1 * stride + 4], vertices[index1 * stride + 5]);
            glm::vec3 normal2(vertices[index2 * stride + 3], vertices[index2 * stride + 4], vertices[index2 * stride + 5]);
            glm::vec3 normal3(vertices[index3 * stride + 3], vertices[index3 * stride + 4], vertices[index3 * stride + 5]);
            // 取出对应顶点纹理坐标
            glm::vec2 uv1, uv2, uv3;
            // 3, 3, 2
            uv1 = glm::vec2(vertices[index1 * stride + 6], vertices[index1 * stride + 7]);
            uv2 = glm::vec2(vertices[index2 * stride + 6], vertices[index2 * stride + 7]);
            uv3 = glm::vec2(vertices[index3 * stride + 6], vertices[index3 * stride + 7]);
            // 计算边和差值
            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;
            //
            GLfloat f = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV2.x * deltaUV1.y));
            // 计算TBN
            glm::vec3 T, B, N;
            T.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            T.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            T.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            T = glm::normalize(T);

            B.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            B.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            B.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            B = glm::normalize(B);

            //N = glm::normalize(normal1 + normal2 + normal3);
#ifdef DDEBUG
            printf("Point (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f):\n", pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, pos3.x, pos3.y, pos3.z);
            printf("have T(%.2f, %.2f, %.2f), B(%.2f, %.2f, %.2f), N(%.2f, %.2f, %.2f)\n", T.x, T.y, T.z, B.x, B.y, B.z, N.x, N.y, N.z);
            glm::vec3 p1p2 = pos1 - pos2;
            glm::vec3 p2p3 = pos2 - pos3;
            printf("Dot Product: T · B = %f\n", glm::dot(T, B));
            printf("Dot Product: B · N = %f\n", glm::dot(B, N));
            printf("Dot Product: T · N = %f\n", glm::dot(T, N));
            printf("Dot Product: p1p2 · N = %f\n", glm::dot(p1p2, N));
            printf("Dot Product: p2p3 · N = %f\n\n", glm::dot(p2p3, N));
#endif // DEBUG
            // 三个顶点都要插入TBN
            TBN.push_back(T);
            TBN.push_back(B);
           // TBN.push_back(N);

            TBN.push_back(T);
            TBN.push_back(B);
          //  TBN.push_back(N);

            TBN.push_back(T);
            TBN.push_back(B);
           // TBN.push_back(N);

        }
        // 绑定TBN
        // 根据类型判断已经绑定的location
        GLuint attribLocation = 3;
#ifdef DEBUG
        std::cout<<"TBN Matrix Size: "<<TBN.size()<<std::endl;
        std::cout<<"attribStartLocation: "<<attribLocation<<std::endl;
#endif // DEBUG
        glGenBuffers(1, &TBN_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBN_VBO);
            glBufferData(GL_ARRAY_BUFFER, TBN.size() * sizeof(glm::vec3), &TBN[0], GL_STATIC_DRAW);
            glBindVertexArray(VAO);
                glEnableVertexAttribArray(attribLocation + 0);
                glVertexAttribPointer(attribLocation + 0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(0 * sizeof(glm::vec3)));
                glEnableVertexAttribArray(attribLocation + 1);
                glVertexAttribPointer(attribLocation + 1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(1 * sizeof(glm::vec3)));
                //glEnableVertexAttribArray(attribLocation + 2);
                //glVertexAttribPointer(attribLocation + 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (GLvoid*)(2 * sizeof(glm::vec3)));
            glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }else{
        std::cout<<"Object::calcTangents() can not calc tangents since I am lazy!\n";
    }

}

Object* Object::clone() {
    Object *res = new Object();
    res->VAO = 0;
    res->translation = glm::vec3(0, 0, 0);
    res->scale = this->scale;
    res->modelPos = res->projPos = res->viewPos = 0;
    res->mode = this->mode;
    res->EBO = this->EBO;
    res->VBO = this->VBO;
    res->vertexNum = this->vertexNum;
    res->indexNum = this->indexNum;
    res->shader = this->shader;
    res->cam = this->cam;
    res->model = glm::mat4();
    res->vf = this->vf;
    res->TBN_VBO = this->TBN_VBO;

    glGenVertexArrays(1, &(res->VAO));
    glBindVertexArray(res->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, res->VBO);
    switch(res->vf) {
    case POSITIONS: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        break;
    }
    case POSITIONS_COLORS:
    case POSITIONS_NORMALS: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        break;
    }
    case POSITIONS_TEXTURES: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        break;
    }
    case POSITIONS_COLORS_TEXTURES:
    case POSITIONS_NORMALS_TEXTURES: {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        break;
    }
    }
    glBindVertexArray(0);

    if(res->shader != nullptr) {
        res->modelPos = glGetUniformLocation(shader->programID, "model");
        res->projPos = glGetUniformLocation(shader->programID, "projection");
        res->viewPos = glGetUniformLocation(shader->programID, "view");
    }
    return res;
}

Object::~Object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if(indexNum > 0)
        glDeleteBuffers(1, &EBO);
}

void Object::setCamera(Camera *_cam) {
    cam = _cam;
}

void Object::setShader(Shader *_sha) {
    shader = _sha;
    if(shader != nullptr) {
        modelPos = glGetUniformLocation(shader->programID, "model");
        projPos = glGetUniformLocation(shader->programID, "projection");
        viewPos = glGetUniformLocation(shader->programID, "view");
    }
}

void Object::moveTo(glm::vec3 pos) {
    translation = pos;
    model = glm::mat4();
    model = glm::translate(model, translation);
    model = glm::scale(model, glm::vec3(scale));
}

void Object::scaleTo(GLfloat s) {
    scale = s;
    model = glm::mat4();
    model = glm::translate(model, translation);
    model = glm::scale(model, glm::vec3(scale));
}

void Object::draw() {
    if(shader != nullptr) {
        shader->use();
        glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));
    }
    if(shader != nullptr && cam != nullptr) {
        glUniformMatrix4fv(viewPos, 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(projPos, 1, GL_FALSE, cam->getProjectionMatrixVal());
    }
    glBindVertexArray(VAO);
    if(EBO != 0) {
        glDrawElements(mode, indexNum, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, vertexNum);
    }
    glBindVertexArray(0);
}
