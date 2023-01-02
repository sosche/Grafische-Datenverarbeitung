#include <windows.h>
#include <GL/freeglut.h>         
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "SOIL.h"

//************TASTENBELEGUNG*********************
//Pfeiltaste-Oben: Nach vorne laufen
//Pfeiltaste-Unten: Zurueck laufen
//Pfeiltaste-Rechts: Nach rechts drehen
//Pfeiltaste-Links: Nach links drehen
//F1: +CameraX 
//F2: -CameraX 
//F3: +CameraZ
//F4: -CameraZ
//F5: CCW Rotation	!X und Z müssen bei 0 sein
//F6: CW Roation	!X und Z müssen bei 0 sein
//F7: Linker Schlag
//F8: Rechter Schlag

//Startposition des Objektes
static GLfloat xPos = 0;
static GLfloat zPos = 0;
static GLfloat yRot = 0;
//Variablen zur manipulierung der Kamera
GLfloat cameraX = 0;
GLfloat cameraY = 0;
GLfloat cameraZ = 0;
GLfloat centerX = 0;
GLfloat centerY = 0;
GLfloat centerZ = 0;
//Variablen fuer Kampfposition
float punchRotation = 0;
float punchPositionY = 0;
//Position des Boxsacks
GLfloat punchingBagPosX = 10;
GLfloat punchingBagPosZ = -20;
//Fuer Rechstklick Menue
int yExtra = 0;
//Arrays mit für Winkel der Arme und Beine
float legAngle[4] = { 0,0,0,0 };
float armAngle[4] = { 0,0,-120, -120 };

static bool rightLeg = true;
static bool rightLowerLeg = false;
static bool leftLeg = false;
static bool leftLowerLeg = true;

static bool upperRightArm = false;
static bool lowerRightArm = false;
static bool upperLeftArm = true;
static bool lowerLeftArm = true;

float punchingBagAngle[1] = { 0 };
GLfloat skinColor[3] = { 255.0f, 255.0f, 0.0f };
GLfloat armColor[3] = { 255.0f, 255.0f, 255.0f };

bool punchBagHit = false;
//Struct fuer Texturen
struct texture {
	std::string side[6] = { "front", "back", "right", "left", "up", "down" }; //front, back, right, left, up, down
	GLuint img[6] = { NULL, NULL, NULL, NULL, NULL, NULL, }; //front, back, right, left, up, down
};
texture emptyTexture;
//Variablen fuer TexturBilder
GLuint face, boxsack, headSide, topHead, skin, frontBody, bodySide;//roboter
GLuint shoeUp, shoeSide, shoeDown, shoeFrontBack; // Roboter Schuh
GLuint right, left, front, back, up, down;//map
GLuint body, nose;//boxsack
GLuint sideBanner, topBanner; //boxring
GLUquadricObj* gluQ;

//Würfel+Textur+Farbe
void Wuerfel(GLfloat fSeitenLX, GLfloat fSeitenLY, GLfloat fSeitenLZ, GLfloat color[], texture textu)
{
	//Vorderseite
	if (textu.side[0] == "front" && textu.img[0] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[0]);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 0); glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 0);	glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}

	if (textu.side[1] == "back" && textu.img[1] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[1]);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0); glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(1, 0);	glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);   //Rueckseite
		glColor4f(color[0], color[1], color[2], 1.0f);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glEnd();
	}

	if (textu.side[2] == "right" && textu.img[2] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[2]);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 0); glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 0);	glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);   //Rechte Seite
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}

	if (textu.side[3] == "left" && textu.img[3] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[3]);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 0); glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(0, 0);	glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);   //Linke Seite
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}

	if (textu.side[4] == "up" && textu.img[4] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[4]);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 0); glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(0, 0);	glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);   //Deckflaeche
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, +fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glEnd();
	}

	if (textu.side[5] == "down" && textu.img[5] != NULL) {
		gluQ = gluNewQuadric();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(gluQ, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, textu.img[5]);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 0); glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glTexCoord2f(1, 1);	glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 1);	glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glTexCoord2f(0, 0);	glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);   //Bodenflaeche
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, -fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(+fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glColor4f(color[0], color[1], color[2], 1.0);
		glVertex3f(-fSeitenLX / 2.0f, -fSeitenLY / 2.0f, +fSeitenLZ / 2.0f);
		glEnd();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //ROT

	return;
}
//Positionserkennung des Boxsackes
bool collisionDetection(GLfloat xPosNow, GLfloat xPosNext, GLfloat zPosNow, GLfloat zPosNext) {
	bool collisionOnX = false;
	bool collisionOnZ = false;

	collisionOnX = ((xPosNow >= (xPosNext - 6)) && (xPosNow <= (xPosNext + 6)));
	collisionOnZ = ((zPosNow >= (zPosNext - 7)) && (zPosNow <= (zPosNext + 7)));

	if (collisionOnX && collisionOnZ) {
		std::cout << "detection" << std::endl;
	}

	return collisionOnX && collisionOnZ;
}

void mainMenu(int item)
{
	switch (item)
	{
	case 1:
		std::cout << "Exit" << std::endl;
		exit(0);
	}
}

void lighting(int item)
{
	switch (item)
	{
	case 1:
	{
		glEnable(GL_LIGHT0);
		// RenderScene aufrufen.
		glutPostRedisplay();
		break;
	}
	case 2:
	{
		glDisable(GL_LIGHT0);
		// RenderScene aufrufen.
		glutPostRedisplay();
		break;
	}
	}
}

void yPOS(int item)
{
	switch (item)
	{
	case 1:
	{
		yExtra += 5;
		glutPostRedisplay();
		break;
	}
	case 2:
	{
		yExtra -= 5;
		glutPostRedisplay();
		break;
	}
	}
}

void Init()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Unter-Menu
	int submenu1;
	submenu1 = glutCreateMenu(lighting);
	glutAddMenuEntry("Light enable", 1);
	glutAddMenuEntry("Light disable", 2);
	// Unter-Menu
	int submenu2;
	submenu2 = glutCreateMenu(yPOS);
	glutAddMenuEntry("Up", 1);
	glutAddMenuEntry("Down", 2);
	// Haupt-Menu
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Y-Pos", submenu2);
	glutAddSubMenu("Light", submenu1);
	glutAddMenuEntry("Exit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	//lighting:
	GLfloat Light_Position[] = { -20,20,0,1 };
	GLfloat Light_Ambient[] = { 10.0,10.0,0.7,0 };
	GLfloat Light_Spot_direction[] = { 0,0,0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light_Spot_direction);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	gluQ = gluNewQuadric();

	//roboter texture
	glPushMatrix();
	face = SOIL_load_OGL_texture("img//mikeTyson.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	skin = SOIL_load_OGL_texture("img//skin.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	headSide = SOIL_load_OGL_texture("img//side.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	topHead = SOIL_load_OGL_texture("img//top.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	frontBody = SOIL_load_OGL_texture("img//mikeTysonFrontBody.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	bodySide = SOIL_load_OGL_texture("img//BodySide.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	shoeUp = SOIL_load_OGL_texture("img//shoeUp.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	shoeDown = SOIL_load_OGL_texture("img//shoeDown.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	shoeSide = SOIL_load_OGL_texture("img//shoeSide.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	shoeFrontBack = SOIL_load_OGL_texture("img//shoeFrontBack.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glPopMatrix();


	//punchingbag texture
	glPushMatrix();
	boxsack = SOIL_load_OGL_texture("img//boxsack.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glPopMatrix();

	//boxing Ring texture
	glPushMatrix();
	sideBanner = SOIL_load_OGL_texture("img//sideBanner.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	topBanner = SOIL_load_OGL_texture("img//topBanner.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glPopMatrix();

	//map texture
	glPushMatrix();
	right = SOIL_load_OGL_texture("img//sideWall3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	left = SOIL_load_OGL_texture("img//windowWall.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	front = SOIL_load_OGL_texture("img//entranceWall.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	back = SOIL_load_OGL_texture("img//sideWall2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	up = SOIL_load_OGL_texture("img//roof.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	down = SOIL_load_OGL_texture("img//floor.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glPopMatrix();
}

void map() {
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(gluQ, GL_TRUE);

	glTranslatef(0, 25, 0);
	glRotated(180, 0, 1, 0);
	glScalef(450, 50, 450);
	//front
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(1, 1);	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0, 1);	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0, 0);	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0, 1);	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1, 0);	glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
	glPopMatrix();

	//right
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(0.3, -0.5, -0.5);
	glTexCoord2f(1, 1);	glVertex3f(0.3, 0.5, -0.5);
	glTexCoord2f(0, 1);	glVertex3f(0.3, 0.5, 0.5);
	glTexCoord2f(0, 0);	glVertex3f(0.3, -0.5, 0.5);
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-0.3, -0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(-0.3, 0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(-0.3, 0.5, -0.5);
	glTexCoord2f(0, 0); glVertex3f(-0.3, -0.5, -0.5);
	glEnd();
	glPopMatrix();

	//up
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, up);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(0, 1); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0, 0); glVertex3f(-0.5, 0.5, 0.5);
	glEnd();
	glPopMatrix();

	//down
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, down);
	glBegin(GL_QUADS);
	/*glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, -0.5);*/

	GLfloat x = -0.5;
	GLfloat z = -0.5;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++) {
			//while(x >= -0.5 && x <= 0.5){
			glTexCoord2f(1, 0); glVertex3f((x + 0.2), -0.5, z);
			glTexCoord2f(1, 1); glVertex3f((x + 0.2), -0.5, (z + 0.2));
			glTexCoord2f(0, 1); glVertex3f(x, -0.5, (z + 0.2));
			glTexCoord2f(0, 0); glVertex3f(x, -0.5, z);
			x += 0.2;
		}
		z += 0.2;
		x = -0.5;
		/*glTexCoord2f(1, 0); glVertex3f(-0.3, -0.5, z);
		glTexCoord2f(1, 1); glVertex3f(-0.3, -0.5, z + 0.2);
		glTexCoord2f(0, 1); glVertex3f(-0.5, -0.5, z + 0.2);
		glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, z);

		glTexCoord2f(1, 0); glVertex3f(-0.1, -0.5, z);
		glTexCoord2f(1, 1); glVertex3f(-0.1, -0.5, z + 0.2);
		glTexCoord2f(0, 1); glVertex3f(-0.3, -0.5, z + 0.2);
		glTexCoord2f(0, 0); glVertex3f(-0.3, -0.5, z);

		glTexCoord2f(1, 0); glVertex3f(0.1, -0.5, z);
		glTexCoord2f(1, 1); glVertex3f(0.1, -0.5, z + 0.2);
		glTexCoord2f(0, 1); glVertex3f(-0.1, -0.5, z + 0.2);
		glTexCoord2f(0, 0); glVertex3f(-0.1, -0.5, z);

		glTexCoord2f(1, 0); glVertex3f(0.3, -0.5, z);
		glTexCoord2f(1, 1); glVertex3f(0.3, -0.5, z + 0.2);
		glTexCoord2f(0, 1); glVertex3f(0.1, -0.5, z + 0.2);
		glTexCoord2f(0, 0); glVertex3f(0.1, -0.5, z);

		glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, z);
		glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, z + 0.2);
		glTexCoord2f(0, 1); glVertex3f(0.3, -0.5, z + 0.2);
		glTexCoord2f(0, 0); glVertex3f(0.3, -0.5, z);

		x += 0.2;
		z += 0.2;*/
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

//void roboter(float xPos, float zPos, float yRot, float s)
//{
//
//	glEnable(GL_TEXTURE_2D);
//	gluQuadricTexture(gluQ, GL_TRUE);
//
//	glPushMatrix(); //1
//	glTranslatef(xPos, 6 + punchPositionY, zPos);
//
//	glPushMatrix(); //2
//	glRotatef(yRot, 0, 1, 0);
//	glRotatef(-90 + punchRotation, 0, 1, 0);
//	glScaled(s, s, s);
//	glTranslatef(0, 0.3, 0);	
//
//	glPushMatrix(); //3
//	glTranslated(0.0, 0.7, 0.0); //Bewege ganzes Objekt
//	glRotated(270.0, 0.0, 1.0, 0.0); //Objektwinkel zu rücken
//
//	texture skinTexture;
//	skinTexture.img[0] = skin;
//	skinTexture.img[1] = skin;
//	skinTexture.img[2] = skin;
//	skinTexture.img[3] = skin;
//	skinTexture.img[4] = skin;
//	skinTexture.img[5] = skin;
//
//	texture gloves;
//	gloves.img[0] = topHead;
//	gloves.img[1] = topHead;
//	gloves.img[2] = topHead;
//	gloves.img[3] = topHead;
//	gloves.img[4] = skin;
//	gloves.img[5] = topHead;
//
//	texture texHead;
//	texHead.img[0] = face;
//	texHead.img[1] = headSide;
//	texHead.img[2] = headSide;
//	texHead.img[3] = headSide;
//	texHead.img[4] = topHead;
//	texHead.img[5] = skin;
//	 
//	//------Objekt Anfang
//	// 
//	//*********************************************
//	// head
//	glPushMatrix();
//	glTranslatef(0.0, 0.45, 0.0);
//	Wuerfel(0.2, 0.2, 0.15, skinColor, texHead);
//	glPopMatrix();
//
//	//*********************************************
//	// body / torso
//	glPushMatrix();
//	glScalef(1.5, 2., 1.5);
//	glTranslatef(0., 0.0, 0.0);
//	Wuerfel(0.3, 0.3, 0.15, skinColor, emptyTexture);
//	glPopMatrix();
//
//	//*********************************************
//	// left upper Leg
//	glPushMatrix();
//	glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);
//	glScalef(0.5, 1.0, 0.3);
//	glTranslatef(0.2, -0.40, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, skinColor, skinTexture);
//
//	// left lower Leg
//	glPushMatrix();
//	glRotatef(legAngle[2], 1.0f, 0.0f, 0.0f);
//	glScalef(1.0, 1.0, 1.0);
//	glTranslatef(0.0, -0.23, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, skinColor, skinTexture);
//	glPopMatrix();
//
//	// right upper Leg
//	glPushMatrix();
//	glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
//	glScalef(0.5, 1.0, 0.3);
//	glTranslatef(-0.2, -0.40, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, skinColor, skinTexture);
//
//	// right lower Leg
//	glPushMatrix();
//	glRotatef(legAngle[3], 1.0f, 0.0f, 0.0f);
//	glScalef(1.0, 1.0, 1.0);
//	glTranslatef(0.0, -0.23, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, skinColor, skinTexture);
//	glPopMatrix();
//
//	//*********************************************
//
//	// Right Upperarm
//	glPushMatrix();
//	glTranslatef(-0.47, 0.20, 0.0);
//	glRotatef(armAngle[0], -1.0f, 0.0f, 0.0f);
//	glScalef(0.4, 1.0, 0.3);
//	glTranslatef(0.47, -0.05, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, armColor, skinTexture);
//
//	// Right Underarm 
//	glPushMatrix();
//	glTranslatef(0.0, -0.44, 0.0);
//	//glRotatef(/*armAngle[3]*/0, -1.0f, 0.0f, 0.0f);
//	glTranslatef(0.0, 0.22, 0.0);	
//	glScalef(0.9, 1.0, 1.0);
//	Wuerfel(0.2, 0.2, 0.30, armColor, skinTexture);
//	// Right hand
//	glPushMatrix();
//	glScalef(0.9, 0.4, 1.0);
//	glTranslatef(0.00, -0.40, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, armColor, gloves);
//	glPopMatrix();
//
//
//	//*********************************************
//	// Left Upperarm
//	glPushMatrix();
//	glTranslatef(0.47, 0.20, 0.0);
//	glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
//	glScalef(0.4, 1, 0.3);
//	glTranslatef(-0.47, -0.05, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, armColor, skinTexture);
//
//	// Left Underarm
//	glPushMatrix();
//	//glRotatef(armAngle[3], 1.0f, 0.0f, 0.0f);
//	glScalef(0.9, 1.0, 1.0);
//	glTranslatef(0.0, -0.22, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, armColor, skinTexture);
//	// Left hand
//	glPushMatrix();
//	glScalef(0.9, 0.4, 1.0);
//	glTranslatef(0.00, -0.40, 0.0);
//	Wuerfel(0.2, 0.2, 0.3, armColor, gloves);
//	glPopMatrix();
//
//
//	//*********************************************
//	// 
//	//------Objekt Ende
//
//	glPopMatrix(); //3
//	glPopMatrix(); //2
//	glPopMatrix(); //1
//	glColor3f(1, 1, 1);
//}

void roboterTwo(float xPos, float zPos, float yRot, float s) {
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(gluQ, GL_TRUE);

	glPushMatrix(); //1
	glTranslatef(xPos, 6.2 + punchPositionY, zPos);

	glPushMatrix(); //2
	glRotatef(yRot, 0, 1, 0);
	glRotatef(-90 + punchRotation, 0, 1, 0);
	glScaled(s, s, s);
	glTranslatef(0, 0.3, 0);

	glPushMatrix(); //3
	glTranslated(0.0, 0.7, 0.0); //Bewege ganzes Objekt
	glRotated(270.0, 0.0, 1.0, 0.0); //Objektwinkel zu rücken

	texture skinTexture;
	skinTexture.img[0] = skin;
	skinTexture.img[1] = skin;
	skinTexture.img[2] = skin;
	skinTexture.img[3] = skin;
	skinTexture.img[4] = skin;
	skinTexture.img[5] = skin;

	texture gloves;
	gloves.img[0] = topHead;
	gloves.img[1] = topHead;
	gloves.img[2] = topHead;
	gloves.img[3] = topHead;
	gloves.img[4] = skin;
	gloves.img[5] = topHead;

	texture texHead;
	texHead.img[0] = face;
	texHead.img[1] = headSide;
	texHead.img[2] = headSide;
	texHead.img[3] = headSide;
	texHead.img[4] = topHead;
	texHead.img[5] = skin;

	texture shoeTexture;
	shoeTexture.img[0] = shoeFrontBack;
	shoeTexture.img[1] = shoeFrontBack;
	shoeTexture.img[2] = shoeSide;
	shoeTexture.img[3] = shoeSide;
	shoeTexture.img[4] = shoeUp;
	shoeTexture.img[5] = shoeDown;

	texture bodyTexture;
	bodyTexture.img[0] = frontBody;
	bodyTexture.img[1] = bodySide;
	bodyTexture.img[2] = bodySide;
	bodyTexture.img[3] = bodySide;
	bodyTexture.img[4] = skin;
	bodyTexture.img[5] = shoeDown;

	//------Objekt Anfang
	//*********************************************
	glPushMatrix();
		glPushMatrix();

		//Kopf
		glPushMatrix();
		glTranslatef(0.0, 0.45, 0.0);
		Wuerfel(0.2, 0.2, 0.15, skinColor, texHead);
		glPopMatrix();

		//Körper
		glPushMatrix();
		glTranslatef(0.0, 0.10, 0.0);
		Wuerfel(0.4, 0.5, 0.15, skinColor, bodyTexture);
		glPopMatrix();

			glPushMatrix();
			glTranslatef(-0.25, 0.28, 0);
			glRotatef(armAngle[0], -1, 0, 0);
			glTranslatef(0.25, -0.28, 0);

			glPushMatrix();
			//Schulter
			glTranslatef(-0.25, 0.28, 0);
			Wuerfel(0.10, 0.10, 0.10, armColor, skinTexture);
			glPopMatrix();

			glPushMatrix();
			//RechterOberarm
			glTranslatef(-0.25, 0.19, 0);
			Wuerfel(0.09, 0.20, 0.09, armColor, skinTexture);
			glPopMatrix();

				glPushMatrix();
				//Translation Rotatio
				glTranslatef(-0.25, 0.05, 0.0);
				glRotatef(armAngle[2], 1, 0, 0);
				glTranslatef(0.25, -0.05, 0.0);

					glPushMatrix();
					//Gelenk
					glTranslatef(-0.25, 0.05, 0.0);
					Wuerfel(0.080, 0.075, 0.080, armColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//RechterUnterarm
					glTranslatef(-0.25, -0.080, 0.0);
					Wuerfel(0.075, 0.22, 0.075, armColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//RechteHand
					glTranslatef(-0.25, -0.17, 0);
					Wuerfel(0.10, 0.10, 0.10, armColor, gloves);
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.25, 0.28, 0);
		glRotatef(armAngle[1], 1, 0, 0);
		glTranslatef(-0.25, -0.28, 0);

			glPushMatrix();
			//Schulter
			glTranslatef(0.25, 0.28, 0);
			Wuerfel(0.10, 0.10, 0.10, armColor, skinTexture);
			glPopMatrix();

			glPushMatrix();
			//LinkerOberarm
			glTranslatef(0.25, 0.19, 0);
			Wuerfel(0.09, 0.20, 0.09, armColor, skinTexture);
			glPopMatrix();

				glPushMatrix();
				//Translation Rotatio
				glTranslatef(0.25, 0.05, 0.0);
				glRotatef(armAngle[3], 1, 0, 0);
				glTranslatef(-0.25, -0.05, 0.0);

				glPushMatrix();
				//Gelenk
				glTranslatef(0.25, 0.05, 0.0);
				Wuerfel(0.080, 0.075, 0.080, armColor, skinTexture);
				glPopMatrix();

				glPushMatrix();
				//LinkerUnterarm
				glTranslatef(0.25, -0.080, 0.0);
				Wuerfel(0.075, 0.22, 0.075, armColor, skinTexture);
				glPopMatrix();

				glPushMatrix();
				//LinkeHand
				glTranslatef(0.25, -0.17, 0);
				Wuerfel(0.10, 0.10, 0.10, armColor, gloves);
				glPopMatrix();
				glPopMatrix();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.09, -0.25, 0.0);
		glRotatef(legAngle[0], 1, 0, 0);
		glTranslatef(0.09, 0.25, 0.0);

			glPushMatrix();
			//RechtesOberbein
			glTranslatef(-0.09, -0.25, 0.0);
			Wuerfel(0.12, 0.22, 0.12, skinColor, skinTexture);
			glPopMatrix();

				glPushMatrix();
				//Rotation Unterbein
				glTranslatef(-0.09, -0.39, 0.0);
				glRotatef(legAngle[2], 1, 0, 0);
				glTranslatef(0.09, 0.39, 0.0);

					glPushMatrix();
					//Rechtes Knie Gelenk
					glTranslatef(-0.09, -0.39, 0.0);
					Wuerfel(0.10, 0.08, 0.10, skinColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//Rechtes Unterbein
					glTranslatef(-0.09, -0.56, 0.0);
					Wuerfel(0.10, 0.27, 0.10, skinColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//Rechter Fuß
					glTranslatef(-0.09, -0.73, 0.04);
					Wuerfel(0.10, 0.06, 0.19, skinColor, shoeTexture);
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();

			glPushMatrix();
			glTranslatef(0.09, -0.25, 0.0);
			glRotatef(legAngle[1], 1, 0, 0);
			glTranslatef(-0.09, 0.25, 0.0);

				glPushMatrix();
				//LinkesOberbein
				glTranslatef(0.09, -0.25, 0.0);
				Wuerfel(0.12, 0.22, 0.12, skinColor, skinTexture);
				glPopMatrix();

				glPushMatrix();
				//Rotation Unterbein
				glTranslatef(0.09, -0.39, 0.0);
				glRotatef(legAngle[3], 1, 0, 0);
				glTranslatef(-0.09, 0.39, 0.0);

					glPushMatrix();
					//Linkes Knie Gelenk
					glTranslatef(0.09, -0.39, 0.0);
					Wuerfel(0.10, 0.08, 0.10, skinColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//Linkes Unterbein
					glTranslatef(0.09, -0.56, 0.0);
					Wuerfel(0.10, 0.27, 0.10, skinColor, skinTexture);
					glPopMatrix();

					glPushMatrix();
					//Linker Fuß
					glTranslatef(0.09, -0.73, 0.04);
					Wuerfel(0.10, 0.06, 0.19, skinColor, shoeTexture);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//*********************************************
	//------Objekt Ende

	glPopMatrix(); //3
	glPopMatrix(); //2
	glPopMatrix(); //1
	glColor3f(1, 1, 1);
}

void punchingBag() {

	glPushMatrix();
	gluQuadricTexture(gluQ, GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glTranslated(punchingBagPosX, 20.0, punchingBagPosZ + 2);

	glPushMatrix();
	glTranslatef(0, 15, 0);
	glRotated(punchingBagAngle[0], -1, 0, 0);
	glTranslatef(0, -15, 0);

	glPushMatrix();
	texture boxSack;
	boxSack.img[0] = boxsack;
	boxSack.img[1] = boxsack;
	boxSack.img[2] = boxsack;
	boxSack.img[3] = boxsack;
	boxSack.img[4] = NULL;
	boxSack.img[5] = NULL;
	Wuerfel(5, 15, 5, armColor, boxSack);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void boxingRing() {

	texture ringTextur;
	ringTextur.img[0] = sideBanner;
	ringTextur.img[1] = sideBanner;
	ringTextur.img[2] = sideBanner;
	ringTextur.img[3] = sideBanner;
	ringTextur.img[4] = topBanner;

	glPushMatrix();
	gluQuadricTexture(gluQ, GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	glTranslated(0, -10.5, -20);
	glScalef(10, 10, 10);

	glPushMatrix();
	glTranslated(0, 1.5, 0);

	glPushMatrix();

	Wuerfel(7, 0.8, 7, skinColor, ringTextur);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Puffer loeschen
	glLoadIdentity();

	//gluLookAt (0.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Kamera von vorne oben
	gluLookAt(xPos + cameraX,		//eyeX
		20 + yExtra + cameraY,		//eyeY
		zPos + 40 + cameraZ,		//eyeZ
		xPos + centerX,				//centerX
		20 + yExtra + centerY,		//centerY
		zPos + centerZ,				//centerZ
		0,							//upX
		1,							//upY
		0);							//upZ

	//roboter(xPos, zPos, yRot, 10);
	map();
	punchingBag();
	boxingRing();
	//roboter(xPos, zPos, yRot, 10);
	roboterTwo(xPos, zPos, yRot, 10);

	glutSwapBuffers();
	glutPostRedisplay();//jedes mal display durchführen
}

//Bewegung beim gehen
void movement()
{
	if (upperRightArm)
		armAngle[0] = armAngle[0] + 2.0f;
	else
		armAngle[0] = armAngle[0] - 2.0f;

	if (armAngle[0] >= 16) {
		upperRightArm = false;
	}
	if (armAngle[0] <= -16) {
		upperRightArm = true;
	}

	if (lowerRightArm)
	{
		armAngle[2] = armAngle[2] + 2.0f;
	}
	else
	{
		armAngle[2] = armAngle[2] - 2.0f;
	}
	if (armAngle[2] >= -90)
	{
		lowerRightArm = false;
	}
	if (armAngle[2] <= -120)
	{
		lowerRightArm = true;
	}

	//-------------------------------------
	if (upperLeftArm)
		armAngle[1] = armAngle[1] - 2.0f;
	else
		armAngle[1] = armAngle[1] + 2.0f;

	if (armAngle[1] >= 16) {
		upperLeftArm = true;
	}
	if (armAngle[1] <= -16) {
		upperLeftArm = false;
	}

	if (lowerLeftArm)
	{
		armAngle[3] = armAngle[3] + 2.0f;
	}
	else
	{
		armAngle[3] = armAngle[3] - 2.0f;
	}
	if (armAngle[3] >= -90)
	{
		lowerLeftArm = false;
	}
	if (armAngle[3] <= -120)
	{
		lowerLeftArm = true;
	}
	//------------------------------------

	if (leftLeg)
		legAngle[0] = legAngle[0] + 2.0f;
	else
		legAngle[0] = legAngle[0] - 2.0f;

	if (legAngle[0] >= 16)
		leftLeg = false;
	if (legAngle[0] <= -16)
		leftLeg = true;

	if (leftLowerLeg)
		legAngle[3] = legAngle[3] + 2.5f;
	else
		legAngle[3] = legAngle[3] - 2.5f;

	if (legAngle[3] >= 32)
		leftLowerLeg = false;
	if (legAngle[3] <= -10)
		leftLowerLeg = true;

	std::cout << "legAngle[2]: " << legAngle[2] << std::endl;
	//------------------------------------

	if (rightLeg)
		legAngle[1] = legAngle[1] + 2.0f;
	else
		legAngle[1] = legAngle[1] - 2.0f;

	if (legAngle[1] >= 16)
		rightLeg = false;
	if (legAngle[1] <= -16)
		rightLeg = true;

	if (rightLowerLeg)
		legAngle[2] = legAngle[2] + 2.5f;
	else
		legAngle[2] = legAngle[2] - 2.5f;

	if (legAngle[2] >= 32)
		rightLowerLeg = false;
	if (legAngle[2] <= -10)
		rightLowerLeg = true;
}

//Rechter Schlag
void rightPunch(bool bagGotHit) {

	if (bagGotHit)
	{
		punchRotation = 15;
		punchPositionY = -0.2;

		if (upperRightArm)
			armAngle[0] = armAngle[0] + 10.0f;
		else
			armAngle[0] = armAngle[0] - 10.0f;

		if (armAngle[0] >= 100) {
			upperRightArm = false;
			punchBagHit = true;
		}
		if (armAngle[0] <= 0) {
			upperRightArm = true;
			punchBagHit = false;
		}

		if (lowerRightArm)
			armAngle[2] = armAngle[2] + 12.5f;
		else
			armAngle[2] = armAngle[2] - 12.5f;

		if (armAngle[2] <= -120) {
			lowerRightArm = true;
			//std::cout << "Winkel 1: " << armAngle[2] << std::endl;
		}
		if (armAngle[2] >= -5) {
			lowerRightArm = false;
			//std::cout << "Winkel 2: " << armAngle[2] << std::endl;
		}

		if (punchBagHit)
			punchingBagAngle[0] = punchingBagAngle[0] + 1.5f;
		else
			punchingBagAngle[0] = punchingBagAngle[0] - 1.5f;

		if (punchingBagAngle[0] >= 0)
			punchBagHit = false;
		if (punchingBagAngle[0] <= -20)
			punchBagHit = true;

		legAngle[0] = -20;
		legAngle[1] = 20;
		legAngle[2] = 20;
		legAngle[3] = 20;
	}
	else {
		punchRotation = 15;
		punchPositionY = -0.2;

		if (upperRightArm)
			armAngle[0] = armAngle[0] + 10.0f;
		else
			armAngle[0] = armAngle[0] - 10.0f;

		if (armAngle[0] >= 100) {
			upperRightArm = false;
		}
		if (armAngle[0] <= 0) {
			upperRightArm = true;
		}

		if (lowerRightArm)
			armAngle[2] = armAngle[2] + 12.5f;
		else
			armAngle[2] = armAngle[2] - 12.5f;

		if (armAngle[2] <= -120) {
			lowerRightArm = true;
			//std::cout << "Winkel 1: " << armAngle[2] << std::endl;
		}
		if (armAngle[2] >= -5) {
			lowerRightArm = false;
			//std::cout << "Winkel 2: " << armAngle[2] << std::endl;
		}

		legAngle[0] = -20;
		legAngle[1] = 20;
		legAngle[2] = 20;
		legAngle[3] = 20;
	}
}

//Linker Schlag
void leftPunch(bool bagGotHit) {

	if (bagGotHit)
	{
		punchRotation = -15;
		punchPositionY = -0.2;

		if (upperLeftArm)
			armAngle[1] = armAngle[1] + 10.0f;
		else
			armAngle[1] = armAngle[1] - 10.0f;

		if (armAngle[1] >= 0) {
			upperLeftArm = false;
			punchBagHit = false;
		}
		if (armAngle[1] <= -100) {
			upperLeftArm = true;
			punchBagHit = true;
		}

		if (lowerLeftArm)
			armAngle[3] = armAngle[3] + 12.5f;
		else
			armAngle[3] = armAngle[3] - 12.5f;

		if (armAngle[3] <= -120) {
			lowerLeftArm = true;
		}
		if (armAngle[3] >= -5) {
			lowerLeftArm = false;
		}

		if (punchBagHit)
			punchingBagAngle[0] = punchingBagAngle[0] + 1.5f;
		else
			punchingBagAngle[0] = punchingBagAngle[0] - 1.5f;

		if (punchingBagAngle[0] >= 0)
			punchBagHit = false;
		if (punchingBagAngle[0] <= -20)
			punchBagHit = true;

		legAngle[0] = 20;
		legAngle[1] = -20;
		legAngle[2] = 20;
		legAngle[3] = 20;
	}
	else {
		punchRotation = -15;
		punchPositionY = -0.2;

		if (upperLeftArm)
			armAngle[1] = armAngle[1] + 10.0f;
		else
			armAngle[1] = armAngle[1] - 10.0f;

		if (armAngle[1] >= 0) {
			upperLeftArm = false;
		}
		if (armAngle[1] <= -100) {
			upperLeftArm = true;
		}

		//std::cout << "armAngle[1]: " << armAngle[1] << std::endl;

		if (lowerLeftArm)
			armAngle[3] = armAngle[3] + 12.5f;
		else
			armAngle[3] = armAngle[3] - 12.5f;

		if (armAngle[3] <= -120) {
			lowerLeftArm = true;
		}
		if (armAngle[3] >= -5) {
			lowerLeftArm = false;
		}

		legAngle[0] = 20;
		legAngle[1] = -20;
		legAngle[2] = 20;
		legAngle[3] = 20;
	}
}

//Aktion wenn Tasten losgelassen werden
void SpecialKeysUp(int key, int x, int y)
{
	armAngle[0] = 0;
	upperRightArm = false;
	armAngle[1] = 0;
	upperLeftArm = true;
	armAngle[2] = -120;
	lowerRightArm = false;
	armAngle[3] = -120;
	lowerLeftArm = false;
	legAngle[0] = 0;
	leftLeg = false;
	legAngle[1] = 0;
	rightLeg = true;
	legAngle[2] = 0;
	leftLowerLeg = true;
	legAngle[3] = 0;
	rightLowerLeg = false;
	punchingBagAngle[0] = 0;
	punchBagHit = false;
	punchRotation = 0;
	punchPositionY = 0;
	glutPostRedisplay();
}

//Aktionen wenn Tasten gedrueckt werden
void SpecialKeys(int key, int x, int y)
{
	collisionDetection(xPos, punchingBagPosX, zPos, punchingBagPosZ);

	/*std::cout << "Roboter Position (x,z): (" << xPos << ", " << zPos << ")" << std::endl;
	std::cout << "PunchingBag Position (x,z): (" << punchingBagPosX << ", " << punchingBagPosZ << ")" << std::endl;*/

	std::cout << "Key: " << key << std::endl;
	if (key == GLUT_KEY_UP) {
		if (collisionDetection(xPos - sin(3.14 * (yRot / 180)), punchingBagPosX, zPos - cos(3.14 * (yRot / 180)), punchingBagPosZ)) {
			std::cout << "Object hit" << std::endl;
			return;
		}
		xPos -= sin(3.14 * (yRot / 180));
		zPos -= cos(3.14 * (yRot / 180));

		movement();
	}

	if (key == GLUT_KEY_DOWN) {

		if (collisionDetection(xPos + sin(3.14 * (yRot / 180)), punchingBagPosX, zPos + cos(3.14 * (yRot / 180)), punchingBagPosZ)) {
			std::cout << "Object hit" << std::endl;
			return;
		}

		xPos += sin(3.14 * (yRot / 180));
		zPos += cos(3.14 * (yRot / 180));

		//movementBackwards();
		movement();
	}

	if (key == GLUT_KEY_F7) {
		leftPunch(collisionDetection(xPos - sin(3.14 * (yRot / 180)), punchingBagPosX, zPos - cos(3.14 * (yRot / 180)), punchingBagPosZ));
	}

	if (key == GLUT_KEY_F8) {
		rightPunch(collisionDetection(xPos - sin(3.14 * (yRot / 180)), punchingBagPosX, zPos - cos(3.14 * (yRot / 180)), punchingBagPosZ));
	}

	if (key == GLUT_KEY_LEFT)	yRot += 30;
	if (key == GLUT_KEY_RIGHT)	yRot -= 30;

	if (xPos > 34.3) xPos = 34.3;
	if (xPos < -34.3) xPos = -34.3;
	if (zPos > 15.2) zPos = 15.2;
	if (zPos < -53) zPos = -53;

	//X-Achse
	if (key == GLUT_KEY_F1) {
		cameraX += 5;
		std::cout << "CameraX: " << cameraX << std::endl;
	}
	if (key == GLUT_KEY_F2) {
		cameraX += -5;
		std::cout << "CameraX: " << cameraX << std::endl;
	}

	//Z-Achse
	if (key == GLUT_KEY_F3) {
		cameraZ += 5;
		std::cout << "CameraZ: " << cameraZ << std::endl;
	}
	if (key == GLUT_KEY_F4) {
		cameraZ += -5;
		std::cout << "CameraZ: " << cameraZ << std::endl;
	}

	//CCW Rotation
	if (key == GLUT_KEY_F6) {

		if (cameraX >= 0 && cameraX <= 40 &&
			cameraZ <= 0 && cameraZ >= -40) {
			cameraX += 5;
			cameraZ += -5;
		}
		if (cameraX <= 40 && cameraX >= 0
			&& cameraZ <= -40 && cameraZ >= -80)
		{
			cameraX += -5;
			cameraZ += -5;
		}
		if (cameraX <= 0 && cameraX >= -40
			&& cameraZ >= -80 && cameraZ <= -40) {
			cameraX += -5;
			cameraZ += 5;
		}
		if (cameraX <= 0 && cameraX >= -40 &&
			cameraZ <= 0 && cameraZ >= -40) {
			cameraX += 5;
			cameraZ += 5;
		}
		/*std::cout << "CameraX: " << cameraX << std::endl;
		std::cout << "CameraZ: " << cameraZ << std::endl << std::endl;*/
	}
	//CW Rotation
	if (key == GLUT_KEY_F5) {
		if (cameraX <= 0 && cameraX >= -40 &&
			cameraZ <= 0 && cameraZ >= -40) {
			cameraX += -5;
			cameraZ += -5;
		}
		if (cameraX <= 0 && cameraX >= -40
			&& cameraZ >= -80 && cameraZ <= -40) {
			cameraX += 5;
			cameraZ += -5;
		}
		if (cameraX <= 40 && cameraX >= 0
			&& cameraZ <= -40 && cameraZ >= -80)
		{
			cameraX += 5;
			cameraZ += 5;
		}
		if (cameraX >= 0 && cameraX <= 40 &&
			cameraZ <= 0 && cameraZ >= -40) {
			cameraX += -5;
			cameraZ += 5;
		}
		/*std::cout << "CameraX: " << cameraX << std::endl;
		std::cout << "CameraZ: " << cameraZ << std::endl << std::endl;*/
	}

	glutPostRedisplay();
}

static void play(void) { glutPostRedisplay(); }
void Reshape(int width, int height) {

	// Hier: Reaktionen auf eine Veraenderung der Groesse des Graphikfensters 
	glMatrixMode(GL_PROJECTION); // Matrix fuer Transf. Frustum->Viewport
	glLoadIdentity(); // Projection-Matrix als Einheitsmatrix initialisieren
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW); // Modellierungs/Viewing-Matrix
	glLoadIdentity();// Reset The Modelview Matrix

}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Jusufovic755481-Scheraz755737 - Mike Tyson");
	Init();
	glutDisplayFunc(RenderScene);
	glutIdleFunc(play);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKeys); //keyboard call back function
	glutSpecialUpFunc(SpecialKeysUp);
	glutMainLoop();
	return 0;
}