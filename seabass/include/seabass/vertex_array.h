#pragma once
#include <seabass/opengl_loader.h>
#include <seabass/program.h>
#include <seabass/vertex_buffer.h>

#include <iostream>
#include <string>
#include <vector>

namespace sb {
class VertexArray {
private:
    GLuint id_;
    const sb::Program *program_;
    std::vector<sb::VertexBufferAttribute> vbos_;

public:
    VertexArray(const sb::Program *program,
                const std::vector<sb::VertexBufferAttribute> &vbos)
        : program_(program), vbos_(vbos) {
        glGenVertexArrays(1, &id_);
    }
    void render(GLuint type) const {
        size_t count = UINT32_MAX;
        glBindVertexArray(id_);
        for (auto &&vbo : vbos_) {
            vbo.vbo()->bind_to_vertex_arrray(vbo.attribute_name(),
                                             vbo.attribute_index());
            count = std::min(count, vbo.vbo()->count());
        }
        program_->use();
        glDrawArrays(type, 0, count);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void render(GLuint type, std::vector<uint32_t> indices) const {
        size_t count = UINT32_MAX;
        glBindVertexArray(id_);
        for (auto &&vbo : vbos_) {
            vbo.vbo()->bind_to_vertex_arrray(vbo.attribute_name(),
                                             vbo.attribute_index());
            count = std::min(count, vbo.vbo()->count());
        }
        program_->use();
        glDrawElements(type, indices.size(), GL_UNSIGNED_INT, indices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
};  // namespace sb
