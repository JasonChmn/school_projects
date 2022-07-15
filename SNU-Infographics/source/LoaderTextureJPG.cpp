#include "..\include\LoaderTextureJPG.h"

int LoaderTextureJPG::createNewTexture(const char * pathTextureJPG, GLint paramWrap, GLint paramFilter)
{
	// load an image file directly as a new OpenGL texture
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		pathTextureJPG,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	// check for an error during the load process
	if (0 == tex_2d)
	{
		printf("\nSOIL loading error: '%s' with folder : '%s'\n", SOIL_last_result(), pathTextureJPG);
		Sleep(10000);
		exit(1);
	}
	glBindTexture(GL_TEXTURE_2D, tex_2d);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramFilter);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex_2d;

}
