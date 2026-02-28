#ifndef VC_SHADER_H
#define VC_SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char* shaderReadFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存
    char* buffer = (char*)malloc(size + 1);
    if (!buffer)
    {
        printf("Failed to allocate memory for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    // 读取文件
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

static unsigned int shaderCompile(unsigned int type, const char* src)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    // 检查错误
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(id, 512, NULL, log);
        printf("Failed Compile Shader: %s\n", log);
        return 0;
    }
    return id;
}

unsigned int shaderLoad(const char* vertex_path, const char* fragment_path)
{
    // 读取源码
    char* vertex_src = shaderReadFile(vertex_path);
    char* fragment_src = shaderReadFile(fragment_path);
    if (!vertex_src || !fragment_src)
    {
        printf("Failed Load Shader File\n");
        return 0;        
    }

    // 编译
    unsigned int vertex = shaderCompile(GL_VERTEX_SHADER, vertex_src);
    unsigned int fragment = shaderCompile(GL_FRAGMENT_SHADER, fragment_src);
    free(vertex_src);
    free(fragment_src);
    if (!vertex || !fragment)
    {
        if (vertex) glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        return 0;
    }

    // 链接
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // 检查链接错误
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("Failed Link Shader: %s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    // 清理
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

// 使用shader
void shaderUse(unsigned int id)
{
    glUseProgram(id);
}

// 删除shader
void shaderDelete(unsigned int id)
{
    glDeleteProgram(id);
}

// 设置uniform
void shaderSetBool(unsigned int id, const char* name, bool value) 
{         
    glUniform1i(glGetUniformLocation(id, name), (int)value); 
}

void shaderSetInt(unsigned int id, const char* name, int value) 
{ 
    glUniform1i(glGetUniformLocation(id, name), value); 
}

void shaderSetFloat(unsigned int id, const char* name, float value) 
{ 
    glUniform1f(glGetUniformLocation(id, name), value); 
}

void shaderSetVec2(unsigned int id, const char* name, const glm::vec2 &value) 
{ 
    glUniform2fv(glGetUniformLocation(id, name), 1, &value[0]); 
}
void shaderSetVec2(unsigned int id, const char* name, float x, float y) 
{ 
    glUniform2f(glGetUniformLocation(id, name), x, y); 
}

void shaderSetVec3(unsigned int id, const char* name, const glm::vec3 &value) 
{ 
    glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]); 
}
void shaderSetVec3(unsigned int id, const char* name, float x, float y, float z) 
{ 
    glUniform3f(glGetUniformLocation(id, name), x, y, z); 
}

void shaderSetVec4(unsigned int id, const char* name, const glm::vec4 &value) 
{ 
    glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]); 
}
void shaderSetVec4(unsigned int id, const char* name, float x, float y, float z, float w) 
{ 
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w); 
}

void shaderSetMat2(unsigned int id, const char* name, const glm::mat2 &mat) 
{
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

void shaderSetMat3(unsigned int id, const char* name, const glm::mat3 &mat) 
{
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

void shaderSetMat4(unsigned int id, const char* name, const glm::mat4 &mat) 
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &mat[0][0]);
}

#endif