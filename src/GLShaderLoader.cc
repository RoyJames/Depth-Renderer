#include "GLShaderLoader.h"

#include <iostream>

const char* fshader_default = 
"uniform vec4 viewer, lightpos;\n"
"uniform vec4 light_amb, light_diff, light_spec;\n"
"uniform vec4 mat_amb, mat_diff, mat_spec;\n"
"uniform float phong;\n"
"\n"
"varying vec3 norm_old;\n"
"varying vec3 position;\n"
"\n"
"// product of components, which we will use for shading calculations:\n"
"vec4 product(vec4 a, vec4 b) {\n"
"    return vec4(a[0]*b[0], a[1]*b[1], a[2]*b[2], a[3]*b[3]);\n"
"}\n"
"\n"
"void main() \n"
"{\n"
"    vec4 ambientColor = vec4(0,0,0,0),\n"
"     	 diffuseColor = vec4(0,0,0,0),\n"
"     	 specularColor = vec4(0,0,0,0);\n"
"    vec3 norm = normalize(norm_old);\n"
"\n"
"    vec3 lightVector = normalize(lightpos.xyz - position);\n"
"    vec3 ray = normalize(position - viewer.xyz);\n"
"    vec3 hVector = normalize(lightVector - ray);\n"
"\n"
"    float l_n = dot(lightVector, norm);\n"
"    float h_n = dot(hVector, norm);\n"
"    float phong_val = pow(max(0, h_n), phong);\n"
"\n"
"    ambientColor = product(mat_amb, light_amb);\n"
"	diffuseColor = product(vec4(1,1,1,0), light_diff) * max(0, l_n);\n"
"    specularColor = product(mat_spec, light_spec) * phong_val;\n"
"\n"
"    gl_FragColor = ambientColor + diffuseColor + specularColor;\n"
"}\n";

const char* vshader_default =
"attribute vec4 vPosition;\n"
"attribute vec4 vNorm;\n"
"attribute vec2 uv;\n"
"uniform mat4 ctm;\n"
"\n"
"varying vec3 norm_old;\n"
"varying vec3 position;\n"
"\n"
"void main() \n"
"{\n"
"    gl_Position = vPosition * ctm;\n"
"    position = vPosition.xyz;\n"
"    norm_old = vNorm.xyz;\n"
"}";

const char* default_shaders[2] = {vshader_default, fshader_default};

// Create a NULL-terminated string by reading the provided file
char* readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "r");

	if ( fp == NULL ) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	if(fread(buf, 1, (size_t) size, fp) != 1) {
//		perror("Invalid read");
	}

	buf[size] = '\0';
	fclose(fp);

	return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader {
		const char*  filename;
		GLenum       type;
		GLchar*      source;
	}  shaders[2] = {
		{ vShaderFile, GL_VERTEX_SHADER, NULL },
		{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();

	for ( int i = 0; i < 2; ++i ) {
		Shader& s = shaders[i];

		char* fileResult = readShaderSource( s.filename );
		if (fileResult) {
			s.source = fileResult;
		} else {
			s.source = (char*) (default_shaders[i]);
		}

		GLuint shader = glCreateShader( s.type );
		glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
		glCompileShader( shader );

		GLint  compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
			char* logMsg = new char[logSize];
			glGetShaderInfoLog( shader, logSize, NULL, logMsg );
			std::cerr << logMsg << std::endl;
			delete [] logMsg;

			exit( EXIT_FAILURE );
		}

		if(!fileResult)
			delete [] s.source;

		glAttachShader( program, shader );
	}

    /* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv( program, GL_LINK_STATUS, &linked );
	if ( !linked ) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		std::cerr << logMsg << std::endl;
		delete [] logMsg;

		exit( EXIT_FAILURE );
	}

    /* use program object */
	glUseProgram(program);

	return program;
}
