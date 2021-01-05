#pragma once
#include <seabass/base/opengl_loader.h>

#include <iostream>
#include <string>
#include <vector>
namespace sb {

class VertexBuffer  {
private:
    std::vector<float> data;
    int element_size;
    GLuint id;
    int num_per_vert;

public:
    VertexBuffer(const std::vector<float> &data, int num_per_vert)
        : data(data), num_per_vert(num_per_vert) {
        element_size = sizeof(float);
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, this->data.size() * element_size,
                     this->data.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    const std::vector<float> read() const { return data; }
    std::vector<float> read() { return data; }
    size_t count() const { return this->data.size() / num_per_vert; }
    void bind_to_vertex_arrray(std::string attribute_name,
                               GLuint attribute_index) const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glVertexAttribPointer(attribute_index, num_per_vert, GL_FLOAT, GL_FALSE,
                              0, 0);
        glEnableVertexAttribArray(attribute_index);
    }
};

class VertexBufferAttribute {
private:
    std::string attribute_name_;
    GLuint attribute_index_;
    sb::VertexBuffer *vbo_;

public:
    VertexBufferAttribute(std::string attribute_name, GLuint attribute_index,
                          sb::VertexBuffer *vbo)
        : attribute_name_(attribute_name),
          attribute_index_(attribute_index),
          vbo_(vbo) {}
    const std::string &attribute_name() const { return attribute_name_; }
    GLuint attribute_index() const { return attribute_index_; }
    const sb::VertexBuffer *vbo() const { return vbo_; }
    sb::VertexBuffer *vbo() { return vbo_; }
};
};  // namespace sb
