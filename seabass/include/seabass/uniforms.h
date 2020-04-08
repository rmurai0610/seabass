#pragma once
#include <seabass/opengl_loader.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace sb {
class UniformBase {
public:
    virtual void use() const = 0;
    virtual ~UniformBase() {}
};

class Uniform1i : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    int v_;

public:
    Uniform1i(GLuint pid, std::string name, int v)
        : pid_(pid), name_(name), v_(v) {}
    void set(int v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform1iv(id, 1, &v_);
    }
};

class Uniform2i : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::ivec2 v_;

public:
    Uniform2i(GLuint pid, std::string name, glm::ivec2 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::ivec2 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform2iv(id, 1, glm::value_ptr(v_));
    }
};

class Uniform3i : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::ivec3 v_;

public:
    Uniform3i(GLuint pid, std::string name, glm::ivec3 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::ivec3 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform3iv(id, 1, glm::value_ptr(v_));
    }
};

class Uniform4i : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::ivec4 v_;

public:
    Uniform4i(GLuint pid, std::string name, glm::ivec4 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::ivec4 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform4iv(id, 1, glm::value_ptr(v_));
    }
};

class Uniform1f : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    float v_;

public:
    Uniform1f(GLuint pid, std::string name, float v)
        : pid_(pid), name_(name), v_(v) {}
    void set(float v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform1fv(id, 1, &v_);
    }
};

class Uniform2f : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::vec2 v_;

public:
    Uniform2f(GLuint pid, std::string name, glm::vec2 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::vec2 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform2fv(id, 1, glm::value_ptr(v_));
    }
};

class Uniform3f : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::vec3 v_;

public:
    Uniform3f(GLuint pid, std::string name, glm::vec3 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::vec3 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform3fv(id, 1, glm::value_ptr(v_));
    }
};

class Uniform4f : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::vec4 v_;

public:
    Uniform4f(GLuint pid, std::string name, glm::vec4 v)
        : pid_(pid), name_(name), v_(v) {}
    void set(glm::vec4 v) { v_ = v; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniform4fv(id, 1, glm::value_ptr(v_));
    }
};

class UniformMat2 : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::mat2 m_;

public:
    UniformMat2(GLuint pid, std::string name, glm::mat2 m)
        : pid_(pid), name_(name), m_(m) {}
    void set(glm::mat2 m) { m_ = m; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniformMatrix2fv(id, 1, GL_FALSE, glm::value_ptr(m_));
    }
};

class UniformMat3 : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::mat3 m_;

public:
    UniformMat3(GLuint pid, std::string name, glm::mat3 m)
        : pid_(pid), name_(name), m_(m) {}
    void set(glm::mat3 m) { m_ = m; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(m_));
    }
};

class UniformMat4 : public UniformBase {
private:
    GLint pid_;
    std::string name_;
    glm::mat4 m_;

public:
    UniformMat4(GLuint pid, std::string name, glm::mat4 m)
        : pid_(pid), name_(name), m_(m) {}
    void set(glm::mat4 m) { m_ = m; }
    void use() const override {
        GLint id = glGetUniformLocation(pid_, name_.c_str());
        glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m_));
    }
};

template <class T>
struct uniform_mapping;
template <>
struct uniform_mapping<int> {
    typedef Uniform1i type;
};
template <>
struct uniform_mapping<glm::ivec2> {
    typedef Uniform2i type;
};
template <>
struct uniform_mapping<glm::ivec3> {
    typedef Uniform3i type;
};
template <>
struct uniform_mapping<glm::ivec4> {
    typedef Uniform4i type;
};
template <>
struct uniform_mapping<float> {
    typedef Uniform1f type;
};
template <>
struct uniform_mapping<glm::vec2> {
    typedef Uniform2f type;
};
template <>
struct uniform_mapping<glm::vec3> {
    typedef Uniform3f type;
};
template <>
struct uniform_mapping<glm::vec4> {
    typedef Uniform4f type;
};
template <>
struct uniform_mapping<glm::mat2> {
    typedef UniformMat2 type;
};
template <>
struct uniform_mapping<glm::mat3> {
    typedef UniformMat3 type;
};
template <>
struct uniform_mapping<glm::mat4> {
    typedef UniformMat4 type;
};
};  // namespace sb
