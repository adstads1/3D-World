

#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//For console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
//#include "gltools.cpp"
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//  Created by Adam Tadele. 

//can define any number of textures here - we just have 2 images
//Constant values for image referencing.
#define TEXTURE_COUNT 14
GLuint  textures[TEXTURE_COUNT];
GLfloat shine = 128;
GLfloat Wwidth;
GLfloat Wheight;
GLfloat skyBoxRotation = 0.0;
GLfloat score = 0;
//below is simply a character array to hold the file names

//Array storing the file name, file type and the objects in the array are called when binding a image/texture with a quadric or rectangle.
const char *textureFiles[TEXTURE_COUNT] = { "grass_diff(1).tga", "old_wall_texture_TGA.tga", "floor.tga", "FootballCompleteMap.tga", "targetBlue.tga", "targetGreen.tga", "targetRed.tga", "targetDull.tga", "orangeFlowerFinal5.tga", "stormydays_large.tga", "fillBarVerticalR.tga", "fillBarHorizontal.tga", "palmBranchA.tga", "yellowFlowerFinal.tga" };

GLfloat spotLight[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular light

//lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f,1.0f, 1.0f };
GLfloat  specularmat[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.7f, 0.7f, 0.7f, 0.0f };//For ambient light
GLfloat	 specularLightPos[] = { 250, 495.0f, 220.0f, 1.0f };//specular light position



//Used for texture loader
//Used for image formatting and the sizing of the image
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// Below is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

GLfloat vertexCubeD[108] = {
	// Cube is made up of trianges and each triange requires three vertices. So to create a square 6 vertices are needs to be created.
	//1st
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,

	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	//4
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	//5
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	//6
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f

};

GLfloat coordinatesTextureSkybox[72] = {//This array stores textured coordinates. Each coordinate is normalized and specifies a point on each trinagle on the cube.

//face 1
	0.75,0.33,      //    0,1,
	0.75,0.67,     //    1,1,
	0.5,0.33,     //    0,0,
	0.5,0.33,     //    0,0,
	0.75,0.67,    //    1,0,
	0.5,0.67,   //    1,1,

	//face 2
	0.5,1.0, //    1,1,
	0.25,1, //    0,1,
	0.5,0.67, //    1,0,
	0.5,0.67, //    1,0,
	0.25,1.0, //    0,1,
	0.25,0.67, //    1,1,
	//face 3
	0,0.67,//    1,1,
	0,0.33,//    0,1,
	0.25,0.67,//    1,0,
	0.25,0.67,//    1,0,
	0,0.33,//    0,1,
	0.25,0.33,//    0,0,
	//face 4
	0.25,0.0,//    0,1,
	0.5,0.0,//    1,1,
	0.25,0.33,//    0,0,
	0.25,0.33,//    0,0,
	0.5,0.0,//    1,1,
	0.5,0.33,//    0,0,
	//face 5
	0.5,0.67,//    1,0,
	0.25,0.67,//    0,0,
	0.5,0.33,//    1,1,
	0.5,0.33,//    1,1,
	0.25,0.67,//    0,0,
	0.25,0.33,//    0,1,
	//face 6
	0.75,0.33,//    1,1,
	1.0,0.33,//    0,1,
	0.75,0.67,//    1,0,
	0.75,0.67,//    1,0,
	1.0,0.33,//    0,1,
	1.0,0.67//    0,0


};

GLfloat skyboxNormalsVertex[108] =//This array stores values of the light normals and they are all facing out from each face
{
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f

};

//end of intialisation 

//This method resets the project with this - it simply pops back the previous projection
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void createSkybox() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[9]);
	glFrontFace(GL_CW); //texture the inside

	//bind your texture here

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, skyboxNormalsVertex);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexCubeD);
	//   glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, coordinatesTextureSkybox);
	// draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glFrontFace(GL_CCW);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void drawTexturedBackWall()
{
	//Added some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[1]);//Calls the array that stores the name of the tga file.
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);//Textured coordinates are normalized and specifies the position on the quad
	glVertex3f(-600.0, -100.0, -700.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(600.0, -100.0, -700.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(600.0, 200.0, -700.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-600.0, 200.0, -700.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawTexturedLeftWall()
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-600.0, -100.0, 200.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(-600.0, -100.0, -700.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(-600.0, 200.0, -700.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-600.0, 200.0, 200.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawTexturedRightWall()
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(1.0, 1.0, 1.0);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(600.0, -100.0, 200.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(600.0, -100.0, -700.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(600.0, 200.0, -700.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(600.0, 200.0, 200.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawOrgFlower(GLfloat ox1, GLfloat ox2)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Blended with an Alpha channel which removes everything around the flower so everything except for the detailed orange flower is transparent.

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(ox1, -100.0, -699.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(ox2, -100.0, -699.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(ox2, -40.0, -699.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(ox1, -40.0, -699.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}

void drawYellFlower(GLfloat ox1, GLfloat ox2)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Blended with an Alpha channel which removes everything around the flower so everything except for the detailed yellow flower is transparent.

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[13]);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(ox1, -100.0, -690.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(ox2, -100.0, -690.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(ox2, -40.0, -690.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(ox1, -40.0, -690.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}

void drawTexturedQuadFloor()
{

	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);

	//binds the texture 
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-600.0, -100.0, 400.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(600.0, -100.0, 400.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(600.0, -100.0, -700.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-600.0, -100.0, -700.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}
GLUquadricObj *quadric;
void ball(GLfloat x, GLfloat y, GLfloat z, GLfloat r)//Function creates the ball.
{
	glPushMatrix();
	glFrontFace(GL_CCW);
	glTranslatef(x, y, z);
	// Create and texture the ball
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glColor3f(0.5, 0.5, 0.5);
	quadric = gluNewQuadric();//Creates a new quadric object
	gluQuadricDrawStyle(quadric, GLU_FILL);//Allows you to see a filled ball
	gluQuadricNormals(quadric, GLU_SMOOTH);//Makes the bal smooth
	gluQuadricOrientation(quadric, GLU_OUTSIDE);//Normals are pointing away from the ball
	gluQuadricTexture(quadric, GL_TRUE);//Applies the texture
	gluSphere(quadric, r, 50, 35);//Creates the ball
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void redDot(void)//Red dot so I know where the centre is in the enviroment.
{

	glPushMatrix();
	glColor3f(1, 0, 0);
	glutSolidSphere(5, 100, 100);
	glPopMatrix();

}

void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//set up for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, Wwidth, 0, Wheight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -Wheight, 0);
	//set for drawing again
	glMatrixMode(GL_MODELVIEW);
}


void textDisplay(GLfloat x, GLfloat y, GLint r, GLint g, GLint b, const char *string, bool stroke) {
	GLint j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (GLint i = 0; i < j; i++)
	{
		if (stroke)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		}

	}
}

char ballScoreString[12];

void drawScoreText(GLfloat scale)
{
	glPushMatrix();

	//draws & translates the text.
	glTranslatef(470.0, 180.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(ballScoreString, "Score:%1.0f", score);

	//flip the text as it is upside down originally
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	textDisplay(500, 100, 0, 1, 0, ballScoreString, true);
	glPopMatrix();
}

void drawVerticalBar() {

	//this draws the height indicator 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[10]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(590.0, 540.0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(620.0, 540.0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(620.0, 450.0);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(590.0, 450.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	//glPopMatrix();
}

void drawHorizontalBar() {

	//this draws the football direction

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[11]);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(600.0, 480.0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(700.0, 480.0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(700.0, 450.0);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(600.0, 450.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

}

void Bush(GLfloat z) {//Creates bushes only for the right side near the wall with different z axis positions called in the RenderScene function many times.

	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[12]);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-590.0, -99.0, z);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(-490.0, -99.0, z);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(-490.0, 150.0, z);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-590.0, 150.0, z);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void Bushleft(GLfloat z1) {//Creates bushes only for the left side near the wall with different z axis positions called in the RenderScene function many times.

	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[12]);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(460, -99.0, z1);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(560, -99.0, z1);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(560, 150.0, z1);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(460, 150.0, z1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void drawBlueTwentyHoop(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2)//Creates the 20 mark hoops which is called many times in the RenderScene function with different x and y coordinates.
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, -699.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(x2, y1, -699.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, -699.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(x1, y2, -699.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}

void drawGreenTenHoop(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2)//Creates the 10 mark hoops which is called many times in the RenderScene function with different x and y coordinates.
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, -699.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(x2, y1, -699.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, -699.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(x1, y2, -699.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}

void drawRedThirty(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2)//Creates the 30 mark hoops which is called many times in the RenderScene function with different x and y coordinates.
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, -699.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(x2, y1, -699.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, -699.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(x1, y2, -699.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
}

// Called to draw scene*/

void RenderScene(void)
{

	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// view the scene
	gluLookAt(0.00, 0.0, 575.0,//eye
		0.00, 0.00, 0.00,//centre
		0.00, 1.00, 0.00);//up

	glPushMatrix();
	glRotatef(skyBoxRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 600.0f, 0.0f);
	glScalef(8400.0f, 8400.0f, 8400.0f);
	createSkybox();
	glPopMatrix();

	glPushMatrix();
	glFrontFace(GL_CW);
	drawTexturedRightWall();
	glFrontFace(GL_CCW);
	glPopMatrix();
	drawTexturedLeftWall();
	drawTexturedBackWall();
	drawTexturedQuadFloor();
	redDot();// Red dot at the centre
	ball(0, -75.0f, 255.0f, 25.0);//positioning the ball
	drawRedThirty(-300, -220, 90, 170);
	drawRedThirty(-580, -500, -40, 40);
	drawRedThirty(320, 400, 90, 170);
	drawRedThirty(450, 530, 0, 80);

	drawGreenTenHoop(-510, -350, 30, 175);
	drawGreenTenHoop(270, 420, -75, 70);

	drawBlueTwentyHoop(-210.0, -120.0, -60.0, 30.0);
	drawBlueTwentyHoop(10.0, 100.0, 50.0, 140.0);

	drawYellFlower(430, 510);
	drawYellFlower(200, 300);
	drawYellFlower(100, 200);
	drawYellFlower(-150, -50);
	drawYellFlower(-350, -300);
	drawYellFlower(-450, -400);
	drawYellFlower(-500, -450);

	drawOrgFlower(-580.0, -480.0);
	drawOrgFlower(-400.0, -300.0);
	drawOrgFlower(-300.0, -200.0);
	drawOrgFlower(-100.0, 0.0);
	drawOrgFlower(0.0, 100.0);
	drawOrgFlower(170.0, 270.0);
	drawOrgFlower(490.0, 590.0);
	drawOrgFlower(380.0, 480.0);


	Bushleft(-500);
	Bushleft(-350.0);
	Bushleft(-210.0);
	Bushleft(-70.0);

	Bush(-500.0);
	Bush(-350.0);
	Bush(-210.0);
	Bush(-100.0);

	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	glBegin(GL_LINES);//Creates the vertical line for the bar
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(1215.0, 691.0);
	glVertex2f(1255.0, 691.0);
	glEnd();
	glBegin(GL_LINES);//The two red lines creates the horizontal lines for the bar
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(70.0, 10.0);
	glVertex2f(70.0, 20.0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(70.0, 50.0);
	glVertex2f(70.0, 60.0);
	glEnd();
	drawScoreText(1.0f);

	glPushMatrix();
	glTranslatef(630.0, 150.0, 0.0);
	drawVerticalBar();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-580.0, -430.0, 0.0);
	drawHorizontalBar();
	glPopMatrix();
	glPopMatrix();

	resetPerspectiveProjection();

	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures
	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//targa file (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//The four statement declare the way the images are put onto the quadrics or rectangles.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		free(pBytes0);
	}
	//enable textures
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    

	glEnable(GL_CULL_FACE);		// Do not calculate inside

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightLessBright);//Generic ambient light with no position set so the entire envioremnet is lit up with no position.

	//Specular spotlight created with a diffusion light to give of a natural lighting reflection from the ball.
	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);//Diffues light created so when combined with the specular light, it creates a natural light with natural reflection from the ball.
	glLightfv(GL_LIGHT2, GL_POSITION, specularLightPos);//Sets the position of the light.
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotLight);//Specular light.
	glEnable(GL_LIGHT2);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, spotLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glEnable(GL_NORMALIZE);

	// A dark black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}


void TimerFunc(int value)//Timer function.
{
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
	skyBoxRotation += 0.025f;
	if (skyBoxRotation > 360.0f)
	{
		skyBoxRotation = 0.0f;

	}
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevents a divide by zero
	Wwidth = w;
	Wheight = h;

	if (h == 0)
		h = 1;
	//Used for going back to projection form.

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculates aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 90000.0);

	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);// a 16:9 ratio
	glutCreateWindow("3D world");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}






