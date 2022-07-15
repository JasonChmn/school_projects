
#include "../include/LoaderShader.h"

unsigned long LoadShader::getFileLength(std::ifstream* file)
{
	if (!file->good()) return 0;

	unsigned long pos = file->tellg();
	file->seekg(0, file->end);
	unsigned long len = file->tellg();
	file->seekg(file->beg);

	return len;
}

int LoadShader::loadshader(char* filename, GLchar** ShaderSource, unsigned long* len)
{
	std::ifstream file;
	file.open(filename, file.in);
	if (!file) {
		printf("\nFile (%s) not found", filename);
		return -1;
	} else {
		printf("\nFile (%s) opened", filename);
	}
	*len = getFileLength(&file);

	if (len == 0) {
		return -2;   // Error: Empty File 
	}

	*ShaderSource = new GLchar[*len];
	if (*ShaderSource == 0) {
		return -3;   // can't reserve memory
	}

	printf(" -> len(%i)", *len);

	unsigned int i = 0;
	while (file.good())
	{
		(*ShaderSource)[i] = file.get();       // get character from file.
		if (!file.eof()) {
			i++;
		}
	}
	(*ShaderSource)[i] = 0;

	file.close();

	return 0; // No Error
}


int LoadShader::unloadshader(GLubyte** ShaderSource)
{
	if (*ShaderSource != 0) {
		delete[] * ShaderSource;
	}
	*ShaderSource = 0;
	return 0;
}