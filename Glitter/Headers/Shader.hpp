#ifndef SHADER_H
#define SHADER_H

// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class Shader{
public:
	// The program ID
	GLuint Program;
	// Constructor reads and builds the shader
	Shader(string vs_path, string fs_path){
		// Retrieve shader code
		string vs_str = readShaderFile(vs_path);
		string fs_str = readShaderFile(fs_path);
		const char* vs_src = vs_str.c_str();
		const char* fs_src = fs_str.c_str();

		// Compile shader 
		GLint success;
		GLchar infoLog[512];
		GLuint vshader, fshader;
		vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, &vs_src, NULL);
		glCompileShader(vshader);
		glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(vshader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILAION_FAILED\n" << infoLog << endl;
		}
		
		fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader, 1, &fs_src, NULL);
		glCompileShader(fshader);
		glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(fshader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILAION_FAILED\n" << infoLog << endl;
		}
		
		// Link shader program
		Program = glCreateProgram();
		glAttachShader(Program, vshader);
		glAttachShader(Program, fshader);
		glLinkProgram(Program);
		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success){
			glGetProgramInfoLog(Program, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}

		// Delete the shaders as they're linked into program
		glDeleteShader(vshader);
		glDeleteShader(fshader);
	}
	// Use the program 
	void Use(){
		glUseProgram(this->Program);
	}

private:
	// Read the code of shaderfile
	string readShaderFile(string path){
		ifstream file(path, ifstream::in);
		string buf;
		if (file){
			// Old Version
			//string tmp;
			//while (getline(file, tmp))
			//	buf += tmp + '\n';

			// New Version(learning OpenGL)
			stringstream ss;
			ss << file.rdbuf();
			buf = ss.str();
		}
		else
			cout << "failed to open shader: " << path << endl;
		file.close();
		return buf;
	}
};

#endif