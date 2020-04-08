#pragma once
#include <seabass/opengl_loader.h>
#include <seabass/uniforms.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sb {
class Program {
private:
    std::string vertex_shader_, fragment_shader_;
    GLuint v_id_, f_id_, id_;
    std::map<std::string, std::shared_ptr<UniformBase>> uniforms_;
    void init() {
        v_id_ = glCreateShader(GL_VERTEX_SHADER);
        f_id_ = glCreateShader(GL_FRAGMENT_SHADER);
        compile_shader(v_id_, vertex_shader_.c_str());
        compile_shader(f_id_, fragment_shader_.c_str());

        GLint result = GL_FALSE;
        int info_log_length;
        id_ = glCreateProgram();
        glAttachShader(id_, v_id_);
        glAttachShader(id_, f_id_);
        glLinkProgram(id_);
        glGetProgramiv(id_, GL_LINK_STATUS, &result);
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> error_msg(info_log_length + 1);
            glGetProgramInfoLog(id_, info_log_length, NULL, &error_msg[0]);
            std::cerr << &error_msg[0] << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void compile_shader(GLuint id, const char *code) {
        GLint result = GL_FALSE;
        int info_log_length;
        glShaderSource(id, 1, &code, nullptr);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> error_msg(info_log_length + 1);
            glGetShaderInfoLog(id, info_log_length, NULL, &error_msg[0]);
            std::cerr << &error_msg[0] << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

public:
    Program() {
        vertex_shader_ = R"(
            #version 330 core
            layout(location = 0) in vec3 in_vert;
            layout(location = 1) in vec3 in_color;
            uniform mat4 MVP;
            out vec3 frag_color;
            void main() {
              gl_Position = MVP * vec4(in_vert, 1);
              frag_color = in_color;
            }
        )";
        fragment_shader_ = R"(
            #version 330 core
            in vec3 frag_color;
            out vec4 color;
            void main(){
              color = vec4(frag_color, 1.f);
            }
        )";
        init();
    }

    Program(std::string vertex_shader, std::string fragment_shader)
        : vertex_shader_(vertex_shader), fragment_shader_(fragment_shader) {
        init();
    }
    template <class T>
    void set_uniform(std::string n, T m) {
        typedef typename uniform_mapping<T>::type U;
        if (uniforms_.count(n)) {
            std::static_pointer_cast<U>(uniforms_.at(n))->set(m);
        } else {
            std::shared_ptr<UniformBase> uniform =
                std::make_shared<U>(id_, n, m);
            uniforms_.insert({n, uniform});
        }
    }

    void use() const {
        glUseProgram(id_);
        for (const auto &[_, u] : uniforms_) {
            u->use();
        }
    }
};
};  // namespace sb
