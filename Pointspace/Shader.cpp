#include "Shader.h"

Shader::Shader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath)
{
	ProgramID = glCreateProgram();

	std::string VertShaderSource;

	try {
		VertShaderSource = ReadFile(_VertexShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Vertex Shader file " << std::endl;
	}

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* VertSourceCode = VertShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertSourceCode, NULL);

	GLint CompileStatus = 0;
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(VertexShaderID);
	}

	glAttachShader(ProgramID, VertexShaderID);
	glDeleteShader(VertexShaderID);

	//----------------------------------

	std::string FragShaderSource;

	try {
		FragShaderSource = ReadFile(_FragmentShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Fragment Shader file " << std::endl;
	}

	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* FragSourceCode = FragShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragSourceCode, NULL);

	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(FragmentShaderID);
	}

	glAttachShader(ProgramID, FragmentShaderID);
	glDeleteShader(FragmentShaderID);

	GLint LinkStatus = 0;
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &LinkStatus);
	if (!LinkStatus) {
		PrintProgramLog(ProgramID);
	}
}

Shader::Shader(const GLchar* _VertexShaderPath, const GLchar* _GeometryShaderPath, const GLchar* _FragmentShaderPath)
{
	ProgramID = glCreateProgram();

	std::string VertShaderSource;

	try {
		VertShaderSource = ReadFile(_VertexShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Vertex Shader file " << std::endl;
	}

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* VertSourceCode = VertShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertSourceCode, NULL);

	GLint CompileStatus = 0;
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(VertexShaderID);
	}

	glAttachShader(ProgramID, VertexShaderID);
	glDeleteShader(VertexShaderID);

	//----------------------------------

	std::string GeometryShaderSource;

	try {
		GeometryShaderSource = ReadFile(_GeometryShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Geometry Shader file " << std::endl;
	}

	GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* GeometrySourceCode = GeometryShaderSource.c_str();
	glShaderSource(GeometryShaderID, 1, &GeometrySourceCode, NULL);

	CompileStatus = 0;
	glCompileShader(GeometryShaderID);
	glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(GeometryShaderID);
	}

	glAttachShader(ProgramID, GeometryShaderID);
	glDeleteShader(GeometryShaderID);
	
	//----------------------------------

	std::string FragShaderSource;

	try {
		FragShaderSource = ReadFile(_FragmentShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Fragment Shader file " << std::endl;
	}

	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* FragSourceCode = FragShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragSourceCode, NULL);

	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(FragmentShaderID);
	}

	glAttachShader(ProgramID, FragmentShaderID);
	glDeleteShader(FragmentShaderID);

	GLint LinkStatus = 0;
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &LinkStatus);
	if (!LinkStatus) {
		PrintProgramLog(ProgramID);
	}
}

Shader::Shader(const GLchar* _VertexShaderPath, const GLchar* _TCShaderPath, const GLchar* _TEShaderPath, const GLchar* _GeometryShaderPath, const GLchar* _FragmentShaderPath)
{
	ProgramID = glCreateProgram();

	std::string VertShaderSource;

	try {
		VertShaderSource = ReadFile(_VertexShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Vertex Shader file " << std::endl;
	}

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* VertSourceCode = VertShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertSourceCode, NULL);

	GLint CompileStatus = 0;
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(VertexShaderID);
	}

	glAttachShader(ProgramID, VertexShaderID);
	glDeleteShader(VertexShaderID);

	//----------------------------------

	std::string TessControlShaderSource;

	try {
		TessControlShaderSource = ReadFile(_TCShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Tessellation Control Shader file " << std::endl;
	}

	TessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);

	const GLchar* TCShaderSource = TessControlShaderSource.c_str();
	glShaderSource(TessControlShaderID, 1, &TCShaderSource, NULL);

	CompileStatus = 0;
	glCompileShader(TessControlShaderID);
	glGetShaderiv(TessControlShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(TessControlShaderID);
	}

	glAttachShader(ProgramID, TessControlShaderID);
	glDeleteShader(TessControlShaderID);

	//----------------------------------

	std::string TessEvalShaderSource;

	try {
		TessEvalShaderSource = ReadFile(_TEShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Tessellation Evaluation Shader file " << std::endl;
	}

	TessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);

	const GLchar* TEShaderSource = TessEvalShaderSource.c_str();
	glShaderSource(TessEvalShaderID, 1, &TEShaderSource, NULL);

	CompileStatus = 0;
	glCompileShader(TessEvalShaderID);
	glGetShaderiv(TessEvalShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(TessEvalShaderID);
	}

	glAttachShader(ProgramID, TessEvalShaderID);
	glDeleteShader(TessEvalShaderID);

	//----------------------------------

	std::string GeometryShaderSource;

	try {
		GeometryShaderSource = ReadFile(_GeometryShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Geometry Shader file " << std::endl;
	}

	GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* GeometrySourceCode = GeometryShaderSource.c_str();
	glShaderSource(GeometryShaderID, 1, &GeometrySourceCode, NULL);

	CompileStatus = 0;
	glCompileShader(GeometryShaderID);
	glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(GeometryShaderID);
	}

	glAttachShader(ProgramID, GeometryShaderID);
	glDeleteShader(GeometryShaderID);

	//----------------------------------

	std::string FragShaderSource;

	try {
		FragShaderSource = ReadFile(_FragmentShaderPath);
	}
	catch (std::ifstream::failure _error) {
		std::cout << "Error: Unable to read Fragment Shader file " << std::endl;
	}

	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* FragSourceCode = FragShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragSourceCode, NULL);

	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		PrintShaderLog(FragmentShaderID);
	}

	glAttachShader(ProgramID, FragmentShaderID);
	glDeleteShader(FragmentShaderID);

	GLint LinkStatus = 0;
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &LinkStatus);
	if (!LinkStatus) {
		PrintProgramLog(ProgramID);
	}
}

Shader::~Shader()
{
	Clear();
}

std::string Shader::ReadFile(const GLchar* _FilePath)
{
	std::ifstream FileParser;
	std::stringstream FileBuffer;
	FileParser.exceptions(std::ifstream::badbit);
	FileParser.open(_FilePath);
	FileBuffer << FileParser.rdbuf();
	FileParser.close();
	std::string FileContent = FileBuffer.str();
	return FileContent;
}

void Shader::Clear()
{
	glUseProgram(0);
	glDeleteProgram(ProgramID);
}

void Shader::PrintProgramLog(GLuint _ProgramID)
{
	if (glIsProgram(_ProgramID)) {
		int InfoLogLength = 0;
		int MaxLength = InfoLogLength;

		glGetProgramiv(_ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);

		char* InfoLog = new char[MaxLength];

		glGetProgramInfoLog(_ProgramID, MaxLength, &InfoLogLength, InfoLog);
		if (InfoLogLength > 0) {
			fprintf(stdout, "%s\n", InfoLog);
		}

		delete[] InfoLog;
	}
	else {
		fprintf(stdout, "%d is not a valid program ID\n", _ProgramID);
	}
}

void Shader::PrintShaderLog(GLuint _ShaderID)
{
	if (glIsShader(_ShaderID)) {
		int InfoLogLength = 0;
		int MaxLength = InfoLogLength;

		glGetShaderiv(_ShaderID, GL_INFO_LOG_LENGTH, &MaxLength);

		char* InfoLog = new char[MaxLength];

		glGetShaderInfoLog(_ShaderID, MaxLength, &InfoLogLength, InfoLog);
		if (InfoLogLength > 0) {
			fprintf(stdout, "%s\n", InfoLog);
		}

		delete[] InfoLog;
	}
	else {
		fprintf(stdout, "%d is not a valid shader ID\n", _ShaderID);
	}
}

