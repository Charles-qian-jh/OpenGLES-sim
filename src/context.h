#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
#include <GLES3/gl3.h>

#include "type.h"
#include "GPU/driver.h"

struct attribute
{
    GLboolean   enable;
    GLint       size;
    GLenum      type;
    GLboolean   normalized;
    GLsizei     stride;
    const GLvoid * ptr;
};

struct drawCommand
{
    char		name[20];
    GLenum      mode;
    GLint       first;
    GLsizei     count;
    const GLvoid  * indices;
};

struct viewPort
{
    GLint       x,y;
    GLint       w,h;

    GLfloat     n;
    GLfloat     f;

    inline viewPort() {
        w = 1024;
        h = 1024;
        x = 0;
        y = 0;
        n = 0.0f;
        f = 1.0f;
    }
};

struct textureState
{
    GLboolean		genMipmap;
    GLenum      	minFilter;
    GLenum      	magFilter;
    GLenum      	wrapS;
    GLenum      	wrapT;
    GLubyte			baseLevel;
    GLubyte			maxLevel;
    GLuint			texBindID;

    GLuint      	texArrayNum;

    inline textureState() {
        genMipmap = GL_FALSE;
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
        magFilter = GL_LINEAR;
        wrapS = GL_REPEAT;
        wrapT = GL_REPEAT;
        baseLevel = 0;
        maxLevel = 12;
    }
};

struct shaderObject
{
	GLboolean isCompiled;
	GLboolean delFlag;
	GLsizei count;
	GLenum type;

	std::string src;
	std::string asmSrc;
	std::vector<GLuint> attachList;

	inline shaderObject()
	{
		isCompiled = GL_FALSE;
		delFlag = GL_FALSE;
		count = 0;
		type = 0;
	}
};

struct symbol
{
	std::string name;
	std::string declareType;
	unsigned char idx;
	unsigned char element;

	inline symbol()
	{
		name.clear();
		declareType.clear();
		idx = 0;
		element = 0;
	}

	void Print()
	{
		printf("%s %s Idx:%d element:%d\n",
				declareType.c_str(),
				name.c_str(),
				idx,
				element );
	}
};

struct programObject
{
	GLuint sid4VS;	///Shader ID for Vertex Shader
	GLuint sid4FS;	///Shader ID for Fragment Shader
	GLboolean isLinked;
	GLboolean delFlag;

	///Resource Statistic
	int VSinCnt, VSoutCnt, VSuniformCnt;
	int FSinCnt, FSoutCnt, FSuniformCnt;
	int uniformCnt;

	std::string	linkInfo;

	///Naming Table <glsl variable name, asm symbol attribute>
	std::map<std::string, symbol> srcVSin;
	std::map<std::string, symbol> srcVSout;
	std::map<std::string, symbol> srcFSin;
	std::map<std::string, symbol> srcFSout;
	std::map<std::string, symbol> srcUniform;

	///Index Table <asm uniform index, asm symbol attribute>
	std::map<int, symbol> asmVSIdx;
	std::map<int, symbol> asmFSIdx;

	//Building the asm texture's index table is perhaps needed.
	//std::map<int, symbol> asmVStexIdx;
	//std::map<int, symbol> asmFStexIdx;

	///Custom Instruction Format pool
	std::vector<instruction> VSinstructionPool;
	std::vector<instruction> FSinstructionPool;

	inline programObject()
	{
		sid4VS = 0;
		sid4FS = 0;
		isLinked = GL_FALSE;
		delFlag = GL_FALSE;
		VSinCnt = 0;
		VSoutCnt = 0;
		VSuniformCnt = 0;
		FSinCnt = 0;
		FSoutCnt = 0;
		FSuniformCnt = 0;
	}

	///Initialize the elements which is related for program linkage
	void LinkInit()
	{
		isLinked = GL_FALSE;
		VSinCnt = 0;
		VSoutCnt = 0;
		VSuniformCnt = 0;
		FSinCnt = 0;
		FSoutCnt = 0;
		FSuniformCnt = 0;
		linkInfo.clear();
		srcVSin.clear();
		srcVSout.clear();
		srcFSin.clear();
		srcFSout.clear();
		srcUniform.clear();
		asmVSIdx.clear();
		asmFSIdx.clear();
		VSinstructionPool.clear();
		FSinstructionPool.clear();
	}
};

class Context
{
public:

    Context();
    ~Context();

    void                SetCurrent(bool current);
    static void         SetCurrentContext(Context * context);
    static Context *    GetCurrentContext();
    void                RecordError(GLenum error);
    void                DumpImage();

//OpenGL ES 2.0 API
	void 		AttachShader(GLuint program, GLuint shader);
	void 		ActiveTexture(GLenum texture);
	void 		BindTexture (GLenum target, GLuint texture);
    void 		Clear (GLbitfield mask);
    void 		ClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void 		ClearDepthf (GLfloat depth);
    void		CompileShader(GLuint shader);
    GLuint 		CreateProgram (void);
    GLint		CreateShader (GLenum type);
    void		DeleteProgram (GLuint program);
    void		DeleteShader (GLuint shader);
    void 		DeleteTextures (GLsizei n, const GLuint* textures);
    void 		DepthRangef (GLfloat n, GLfloat f);
    void 		DetachShader (GLuint program, GLuint shader);
    void 		Disable (GLenum cap);
    void 		DrawArrays (GLenum mode, GLint first, GLsizei count);
    void 		Enable (GLenum cap);
    void 		EnableVertexAttribArray (GLuint index);
    void 		GenerateMipmap(GLenum target);
    void 		GenTextures (GLsizei n, GLuint* textures);
    int			GetAttribLocation (GLuint program, const GLchar* name);
    GLenum		GetError (void);
    void		GetProgramiv (GLuint program, GLenum pname, GLint* params);
    void		GetShaderiv (GLuint shader, GLenum pname, GLint* params);
    int			GetUniformLocation (GLuint program, const GLchar* name);
    GLboolean	IsProgram (GLuint program);
    GLboolean	IsShader (GLuint shader);
    GLboolean	IsTexture (GLuint texture);
	void 		LinkProgram (GLuint program);
    void 		ShaderSource (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    void 		TexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void 		TexParameteri (GLenum target, GLenum pname, GLint param);
    void		UseProgram (GLuint program);
    void 		ValidateProgram (GLuint program);
    void 		VertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
    void 		Viewport (GLint x, GLint y, GLsizei width, GLsizei height);


/// @fixme (elvis#1#): dirty buffer setting before buffer management is ready
    void           *drawBuffer[2]; //0 - color buffer, 1 - depth buffer

    viewPort        vp;
    floatVec4		clearColor;
    GLfloat			clearDepth;
    GLboolean		clearStat;
    GLuint		 	clearMask;

    GLboolean       blendEnable;
    GLboolean       depthTestEnable;

	textureState	texContext[2];
    attribute       vertexAttrib[8];
    drawCommand     drawCmd;

	///the Program ID called in UseProgram()
    GLuint			usePID;

	/**
	 *	All created program/shader/texImage objects will push into
	 *	programPool/shaderPool/texImagePool respectly. And their ID genreated
	 *	from their created function will also be used as std::map key value.
	 */
    std::map<GLuint, textureImage> texImagePool;
	std::map<GLuint, programObject> programPool;
    std::map<GLuint, shaderObject> shaderPool;

    /**
	 *	All specified uniform value will be stored in uniformPool and it's ID
	 *	queried from getLocation funtion will be used as its std::map key value.
	 */
    std::map<GLuint, floatVec4> uniformPool;

private:
	bool            m_current;
	GLubyte			activeTexture;

	std::stack<GLenum> errorStack;
};

#endif // CONTEXT_H_INCLUDED

