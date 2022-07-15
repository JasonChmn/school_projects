

# define M_PI          3.141592653589793238462643383279502884L /* pi */

#include "../include/Light.h"

Light::Light(glm::vec4 posLight, int indexLight, float intensity, float attenuation) {
	this->posLight[0] = posLight.x;
	this->posLight[1] = posLight.y;
	this->posLight[2] = posLight.z;
	this->posLight[3] = posLight[3];
	
	this->intensity = intensity;
	this->attenuation = attenuation;
	this->ambientValue = 0.1f;

	this->hasShadow = 0;
	this->isAreaLight = 0;
	this->numberDivisionLightArea = 1;
	this->posRightAreaLight = glm::vec4(0, 0, 0, 0);

	switch (indexLight) {
		case 0 :
			this->enumLight = GL_LIGHT0;
			break;
		case 1:
			this->enumLight = GL_LIGHT1;
			break;
		case 2:
			this->enumLight = GL_LIGHT2;
			break;
		case 3:
			this->enumLight = GL_LIGHT3;
			break;
		case 4:
			this->enumLight = GL_LIGHT4;
			break;
		case 5:
			this->enumLight = GL_LIGHT5;
			break;
	}

	glLightfv(this->enumLight, GL_POSITION, this->posLight);

	// create fbo
	glGenFramebuffers(1, &(this->fboLightView));
	glGenTextures(1, &this->depthTexture);

	glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, sizeTexture, sizeTexture, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Link texture and fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fboLightView);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, this->depthTexture, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}


void Light::glRender() {
	glLightfv(this->enumLight, GL_POSITION, this->posLight);
}


void Light::updateShadowMap( void (*drawScene)() )
{	
	glm::mat4 projection, modelView;
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboLightView);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.8, 0.8, 0.8, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_TEXTURE_2D);
		glDrawBuffer(GL_NONE);
		glCullFace(GL_FRONT);
		glPushAttrib(GL_VIEWPORT_BIT);
			glViewport(0, 0, sizeTexture, sizeTexture);
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				if (this->posLight[3] != 0.0f) {
					gluPerspective(this->angleView, (double)(sizeTexture) / (double)(sizeTexture), this->nearPlan, this->farPlan);
				}
				else {
					float value = 12.0f;
					glOrtho(-value, value, -value, value, this->nearPlan, this->farPlan);
				}
			glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(	this->posLight[0], this->posLight[1], this->posLight[2],
							0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f);
			drawScene();
		glPopAttrib();
		glDrawBuffer(GL_FRONT);
		glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	this->proj_ModelView_matrix = projection * modelView;
}


void Light::drawPositionLight(float r, float g, float b, float size)
{
	if (this->isAreaLight == 1) {
		// area light
		glColor3f(r, g, b);
		
		glPushMatrix();
			glTranslatef(posLight[0], posLight[1], posLight[2]);
				glutSolidSphere(size / 2.0f, 15, 15);
				glColor3f(0.8f, 0.8f, 0.8f);
				glBegin(GL_LINES);
					glLineWidth(0.5f);
					glVertex3f(0,0,0);
					glVertex3f(posRightAreaLight.x, posRightAreaLight.y, posRightAreaLight.z);
				glEnd();
				for (int i = 0; i < this->numberDivisionLightArea; i++) {
					glTranslatef(	posRightAreaLight.x / (float)this->numberDivisionLightArea, 
									posRightAreaLight.y / (float)this->numberDivisionLightArea, 
									posRightAreaLight.z / (float)this->numberDivisionLightArea);
					glColor3f(r / 1.20f, g / 1.20f, b / 1.20f);
					glutSolidSphere(size / 4.0f, 12, 12);
				}
		glPopMatrix();
	}
	
	glPushMatrix();
		glColor3f(r, g, b);
		glTranslatef(posLight[0], posLight[1], posLight[2]);
			glutWireCube(size);
	glPopMatrix();
	
}


void Light::drawSquareTextured()
{
	float size = 3;
	//setMaterial();
	printf("\nTest Draw : %i", this->depthTexture);
	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, this->depthTexture);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f + size, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f + size, size, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, size, 0.0f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Light::getIDTextureDepth()
{
	return depthTexture;
}


void Light::stateLight(bool activated)
{
	//printf("\nLight 0 before : %i", glIsEnabled(GL_LIGHT0));
	if (activated) {
		glEnable(this->enumLight);
	} else {
		glDisable(this->enumLight);
	}
	//printf(" -> After  : %i", glIsEnabled(GL_LIGHT0));
}

void Light::inverseStateLight()
{
	if (glIsEnabled(this->enumLight)) {
		glDisable(this->enumLight);
	}
	else {
		glEnable(this->enumLight);
	}
}

bool Light::getStateLight()
{
	return glIsEnabled(this->enumLight);
}

void Light::setPosLight(glm::vec3 posLight) {
	this->posLight[0] = posLight.x;
	this->posLight[1] = posLight.y;
	this->posLight[2] = posLight.z;
	glLightfv(this->enumLight, GL_POSITION, this->posLight);
}

void Light::setLightColor(glm::vec3 lightColor, float ambientValue, float alpha)
{
	this->lightColor[0] = (GLfloat)lightColor.x;
	this->lightColor[1] = (GLfloat)lightColor.y;
	this->lightColor[2] = (GLfloat)lightColor.z;
	this->lightColor[3] = (GLfloat)alpha;
	this->ambientValue = ambientValue;

	float lightAmbient[4] = { lightColor[0] * ambientValue, lightColor[1] * ambientValue, lightColor[2] * ambientValue, lightColor[0] };
	glLightfv(this->enumLight, GL_AMBIENT, lightAmbient);
	glLightfv(this->enumLight, GL_DIFFUSE, this->lightColor);
	glLightfv(this->enumLight, GL_SPECULAR, this->lightColor);
}

void Light::setStateShadow(int hasShadow)
{
	this->hasShadow = hasShadow;
}

void Light::setStateAreaLight(int isAreaLight, int numberDivisionLightArea, glm::vec3 posRight)
{
	this->isAreaLight = isAreaLight;
	this->numberDivisionLightArea = numberDivisionLightArea;
	this->posRightAreaLight = glm::vec4(posRight, this->posLight[3]);
}


// To implement
void Light::setIntensity(float intensity)
{
}
// To implement
float Light::setAttenuation(float attenuation)
{
	return 0.0f;
}

glm::mat4x4 Light::getMatrix_Proj_ModelView()
{
	glm::mat4x4 projection, modelView, MVPmatrix;
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (this->posLight[3] != 0.0f) {
			gluPerspective(this->angleView, (double)(sizeTexture) / (double)(sizeTexture), this->nearPlan, this->farPlan);
		} else {
			float value = 12.0f;
			glOrtho(-value, value, -value, value, this->nearPlan, this->farPlan);
		}
		projection = get_modelViewMatrix();

		glLoadIdentity();
		gluLookAt(	this->posLight[0], this->posLight[1], this->posLight[2],
					0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f);
		modelView = get_modelViewMatrix();

	glPopMatrix();
	MVPmatrix = projection * modelView;
	return MVPmatrix;
}

float * Light::getPtrPosLight()
{
	return this->posLight;
}


Light::LightFormatShader Light::getLightFormatShader()
{
	LightFormatShader myFormat;
	myFormat.position = { this->posLight[0], this->posLight[1], this->posLight[2], this->posLight[3] };
	myFormat.colorLight = { this->lightColor[0], this->lightColor[1], this->lightColor[2] };
	myFormat.attenuation = this->attenuation;
	myFormat.intensity = this->intensity;
	myFormat.ambientValue = this->ambientValue;
	switch (this->enumLight) {
		case GL_LIGHT0:
			myFormat.indexLight = 0;
			break;
		case GL_LIGHT1:
			myFormat.indexLight = 1;
			break;
		case GL_LIGHT2:
			myFormat.indexLight = 2;
			break;
		case GL_LIGHT3:
			myFormat.indexLight = 3;
			break;
		case GL_LIGHT4:
			myFormat.indexLight = 4;
			break;
		case GL_LIGHT5:
			myFormat.indexLight = 5;
			break;
	}
	myFormat.MVPmatrix = getMatrix_Proj_ModelView();
	myFormat.isAreaLight = this->isAreaLight;
	myFormat.numberDivisionLightArea = this->numberDivisionLightArea;
	myFormat.positionRight = glm::vec4(this->posLight[0], this->posLight[1], this->posLight[2], 0) + this->posRightAreaLight;
	//printf("\nPos Light : %f, %f, %f, %f", this->posLight[0], this->posLight[1], this->posLight[2], this->posLight[3]);
	//printf("\nPos Right : %f, %f, %f, %f", myFormat.positionRight[0], myFormat.positionRight[1], myFormat.positionRight[2], myFormat.positionRight[3]);
	myFormat.hasShadow = this->hasShadow;
	return myFormat;
}


glm::mat4x4 Light::get_projectionMatrix() {
	float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	glm::mat4x4 projectionMatrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			projectionMatrix[col][row] = m[col * 4 + row];
		}
	}
	return projectionMatrix;
}

glm::mat4x4 Light::get_modelViewMatrix() {
	float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glm::mat4x4 modelViewMatrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			modelViewMatrix[col][row] = m[col * 4 + row];
		}
	}
	return modelViewMatrix;
}