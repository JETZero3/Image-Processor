/*
 * TextureBMP.cpp
 *
 * Loosely based on the example checker.c 
 * (Example 9-1, OpenGL Programming Guide, 3rd edition) 
 * 
 * Read an image from a bitmap (.bmp) file, and draw it as a texture 
 * map on a quad.  A cpp class RgbImage is used to read a texture
 * map from a bitmap (.bmp) file.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book.
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */

#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include <math.h>
#include <string>
#include <cstring>
#define MENU_LOAD 1
#define MENU_SAVE 2
#define MENU_QUANTIZE 3
#define MENU_BRIGHTEN 4
#define MENU_SATURATION 5
#define MENU_SCALE 6
#define MENU_ROTATE 7
#define ANGLE 3.14159624/4
#define IMAGE_SCALER 450

int menu_status = 0;		//determines which function should be run
char* loadedFile;			//char pointer that keeps track of which file was loaded
RgbImage imageSave;			//global RgbImage for saving later
double originalRows = 0;
double originalCols = 0;
double modifiedRows = 0;
double modifiedCols = 0;

void RenderString(float x, float y, std::string text)
{
	glRasterPos2f(x, y);
	for (int i = 0; i < text.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

GLuint GenerateTexture(const RgbImage& theTexMap)
{
	GLuint texture; // for storing our texture id
	glGenTextures( 1, &texture );
	
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,theTexMap.GetNumCols(), theTexMap.GetNumRows(),
					 GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
    return texture;
}

GLuint loadTextureFromFile(char *filename)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );	
	
	originalRows = theTexMap.GetNumRows();
	originalCols = theTexMap.GetNumCols();
	
	return GenerateTexture(theTexMap);
}

GLuint textureID1;
GLuint textureID2;

void saveTextureToFile(char *filename)
{
	imageSave.WriteBmpFile(filename);		//calls the save function
}

/*
 * Creates the popup menu to allow different operations to be done
 */

std::string cmdText;		//string to be output to display
std::string inputText;		//string that the user types in for inputs
std::string reminderText = "Right click to bring up the menu. To change a value to quantize/brighten/saturate, select the function again from the menu. ";

void processMenuEvents(int option)		//changes menu_status based on the menu item selected
{
	switch(option)
	{
		case MENU_LOAD:
			cmdText = "Please enter the file you wish to load: ";
			menu_status = 1;
			break;
		case MENU_SAVE:
			cmdText = "Please enter the file name for the saved file: ";
			menu_status = 2;
			break;
		case MENU_QUANTIZE:
			cmdText = "Please enter number of levels for quantization: ";
			menu_status = 3;
			break;
		case MENU_BRIGHTEN:
			cmdText = "Please enter the brightness scale factor: ";
			menu_status = 4;
			break;
		case MENU_SATURATION:
			cmdText = "Please enter the saturation scale factor: ";
			menu_status = 5;
			break;
		case MENU_SCALE:
			cmdText = "The image will be scaled by a factor of 2. Please press -enter- to continue. ";
			menu_status = 6;
			break;
		case MENU_ROTATE:
			cmdText = "The image will be rotated counter-clockwise by 45 degrees. Please press -enter- to continue. ";
			menu_status = 7;
			break;
	}
	inputText = "";
	glutPostRedisplay();
}

void createGLUTMenus()
{
	//Create menu
	int menuID = glutCreateMenu(processMenuEvents);
	//Create items
	glutAddMenuEntry("Load an image", MENU_LOAD);
	glutAddMenuEntry("Save the changed image", MENU_SAVE);
	glutAddMenuEntry("Quantize the image", MENU_QUANTIZE);
	glutAddMenuEntry("Brighten the image", MENU_BRIGHTEN);
	glutAddMenuEntry("Saturate the image", MENU_SATURATION);
	glutAddMenuEntry("Scale the image", MENU_SCALE);
	glutAddMenuEntry("Rotate the image", MENU_ROTATE);
	//Attach to right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*
 * Draw the texture in the OpenGL graphics window
 */
 
void draw1()		//scales the quad to the dimensions of the original image (divided by an image scaler so it fits in the window
{
   	glBindTexture(GL_TEXTURE_2D, textureID1);

   	glBegin(GL_QUADS);
    
   	glTexCoord2f(0.0, 0.0); 
  	glVertex3f(-originalCols/IMAGE_SCALER, -0.45, 0.0);
   
   	glTexCoord2f(0.0, 1.0); 
   	glVertex3f(-originalCols/IMAGE_SCALER, -0.45 + (originalRows/IMAGE_SCALER), 0.0);
   
   	glTexCoord2f(1.0, 1.0); 
   	glVertex3f(0.0, -0.45 + (originalRows/IMAGE_SCALER), 0.0);  
   
    glTexCoord2f(1.0, 0.0); 
   	glVertex3f(0.0, -0.45, 0.0);	
   	
   	glEnd();
}
void draw2()
{
   	glBindTexture(GL_TEXTURE_2D, textureID2);
	
	glBegin(GL_QUADS);
  
  	glTexCoord2f(0.0, 0.0); 
  	glVertex3f(0.0, -0.45, 0.0);
   
   	glTexCoord2f(0.0, 1.0); 
   	glVertex3f(0.0, -0.45 + (modifiedRows/IMAGE_SCALER), 0.0);
   
   	glTexCoord2f(1.0, 1.0); 
   	glVertex3f(0.0 + (modifiedCols/IMAGE_SCALER), -0.45 + (modifiedRows/IMAGE_SCALER), 0.0);
   
   	glTexCoord2f(1.0, 0.0); 
   	glVertex3f(0.0 + (modifiedCols/IMAGE_SCALER), -0.45, 0.0);   
   	   
   	glEnd();
}
 
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
	draw1();
	draw2();

    glDisable(GL_TEXTURE_2D);
    RenderString(-0.5f, -0.5f, cmdText);
    RenderString(-0.5f, -0.55f, inputText);
    RenderString(-0.95f, 0.90f, reminderText);
    glFlush();

}


void resizeWindow(int w, int h)
{
	float viewWidth = 1.1;
	float viewHeight = 1.1;
	glViewport(0, 0, w, h);
	h = (h==0) ? 1 : h;
	w = (w==0) ? 1 : w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ( h < w ) {
		viewWidth *= (float)w/(float)h; 
	}
	else {
		viewHeight *= (float)h/(float)w;
	}
	glOrtho( -viewWidth, viewWidth, -viewHeight, viewHeight, -1.0, 1.0 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void quantize(int levels)										//quantizing function
{
	RgbImage modified = RgbImage(loadedFile);
	double r, g, b;
	for (int x = 0; x < modified.GetNumRows(); x++)
	{
		for (int y = 0; y < modified.GetNumCols(); y++)
		{					
			modified.GetRgbPixel(x, y, &r, &g, &b);				//gets original values for r, g, b
			r = r * 255; 								
			g = g * 255; 	
			b = b * 255;
			r = 255 * (floor(r*(levels - 1)/255)/(levels-1));
			g = 255 * (floor(g*(levels - 1)/255)/(levels-1));
			b = 255 * (floor(b*(levels - 1)/255)/(levels-1));
			r = r/255;
			g = g/255;
			b = b/255;
			modified.SetRgbPixelf(x, y, r, g, b);				//sets after the function
		}
	}
	textureID2 = GenerateTexture(modified);
	glutPostRedisplay();
	imageSave = modified;
	modifiedRows = modified.GetNumRows();
	modifiedCols = modified.GetNumCols();
}

void brighten(double s) //s = brightness value
{
	int x = 0, y = 0;
	RgbImage black( loadedFile );								//loads an RgbImage that will be completely black
	for (x = 0; x < black.GetNumRows(); x++)
	{
		for (y = 0; y < black.GetNumCols(); y++)
		{					
			black.SetRgbPixelf(x, y, 0, 0, 0);
		}
	}
	RgbImage modified( loadedFile );
	for (x = 0; x < modified.GetNumRows(); x++)
	{
		for (y = 0; y < modified.GetNumCols(); y++)
		{					
			double r1, r2, g1, g2, b1, b2;
			modified.GetRgbPixel(x, y, &r1, &g1, &b1);			//gets the RGB values from the original texture
			black.GetRgbPixel(x, y, &r2, &g2, &b2);				//gets the RGB values from the black image
			double newR = ((1-s)*r2) + (s*r1);					//blends the red values
			double newG = ((1-s)*g2) + (s*g1);					//blends the blue values
			double newB = ((1-s)*b2) + (s*b1);					//blends the green values
			modified.SetRgbPixelf(x, y, newR, newG, newB);		//sets the blended RGB values
		}
	}
	textureID2 = GenerateTexture(modified);
	glutPostRedisplay();
	imageSave = modified;
	modifiedRows = modified.GetNumRows();
	modifiedCols = modified.GetNumCols();
}

void saturation(double s)
{
	int x = 0, y = 0;
	RgbImage luminance( loadedFile );
	for (x = 0; x < luminance.GetNumRows(); x++)
	{
		for (y = 0; y < luminance.GetNumCols(); y++)
		{					
			double r, g, b;
			luminance.GetRgbPixel(x, y, &r, &g, &b);
			luminance.SetRgbPixelf(x, y, 0.3*r, 0.59*g, 0.11*b);//sets the values for the correct luminance
		}
	}
	RgbImage modified( loadedFile );
	for (x = 0; x < modified.GetNumRows(); x++)					
	{
		for (y = 0; y < modified.GetNumCols(); y++)
		{					
			double r1, r2, g1, g2, b1, b2;
			modified.GetRgbPixel(x, y, &r1, &g1, &b1);			//gets the RGB values from the original texture
			luminance.GetRgbPixel(x, y, &r2, &g2, &b2);			//gets the RGB values from the luminance
			double newR = ((1-s)*r2) + (s*r1);
			double newG = ((1-s)*g2) + (s*g1);					//blends the values
			double newB = ((1-s)*b2) + (s*b1);
			modified.SetRgbPixelf(x, y, newR, newG, newB);		//sets the blended RGB values
		}
	}
	textureID2 = GenerateTexture(modified);
	glutPostRedisplay();
	imageSave = modified;
	modifiedRows = modified.GetNumRows();
	modifiedCols = modified.GetNumCols();
}
			
void scale()
{
	RgbImage original( loadedFile );
	int rows = original.GetNumRows();
	int cols = original.GetNumCols() * 2;
	RgbImage modified(rows, cols);
	double r, g, b;
	for(int x = 0; x < original.GetNumRows(); x++)
	{
		for (int y = 0; y < original.GetNumCols(); y++)
		{
			original.GetRgbPixel(x, y, &r, &g, &b);
			modified.SetRgbPixelf(x, 2*y, r, g, b);		//scales the horizontal by 2		
		}
	}
	textureID2 = GenerateTexture(modified);
	glutPostRedisplay();
	imageSave = modified;
	modifiedRows = modified.GetNumRows();
	modifiedCols = modified.GetNumCols();
}
			
void rotate()
{
	double r, g, b;
	RgbImage original( loadedFile );
	int rows = original.GetNumRows();
	int cols = original.GetNumCols();
	RgbImage modified (rows, cols);
	for(int x = 0; x < original.GetNumRows(); x++)
	{
		for (int y = 0; y < original.GetNumCols(); y++)
		{
			original.GetRgbPixel(x, y, &r, &g, &b);
			int newX = (x*cos(-ANGLE) - y*sin(-ANGLE));
			int newY = (x*sin(-ANGLE) + y*cos(-ANGLE));
			if (newX > 0 && newY > 0 && newX < rows && newY < cols)
				modified.SetRgbPixelf(newX, newY, r, g, b);
		}
	}
	textureID2 = GenerateTexture(modified);
	glutPostRedisplay();
	imageSave = modified;
	modifiedRows = modified.GetNumRows();
	modifiedCols = modified.GetNumCols();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 27:					//exits
			free(loadedFile);
			loadedFile = NULL;
			exit(0);
			break;
		case 13:					//else-if statement that runs a function depending on what menu_status is
			if (menu_status == 1)
			{
				char* filename = new char [inputText.length() - 1];
				std::strcpy(filename, inputText.c_str());
				loadedFile = filename;
				textureID1 = loadTextureFromFile(filename);
				glutPostRedisplay();
				menu_status = 0;		
			}
			else if (menu_status == 2)
			{
				char* filename = new char [inputText.length() - 1];
				std::strcpy(filename, inputText.c_str());
				saveTextureToFile(filename);
				cmdText = "The image has been saved. ";
				glutPostRedisplay();
				menu_status = 0;
			}
			else if (menu_status == 3)
			{
				char* levelsString = new char [inputText.length() - 1];
				std::strcpy(levelsString, inputText.c_str());
				int levelsInt = atoi(levelsString);
				quantize(levelsInt);
				glutPostRedisplay();
				menu_status = 0;
			}
			else if (menu_status == 4)
			{
				char* levelsString = new char [inputText.length() - 1];
				std::strcpy(levelsString, inputText.c_str());
				double scaleFactor = atof(levelsString);
				brighten(scaleFactor);
				glutPostRedisplay();
				menu_status = 0;
			}
			else if (menu_status == 5)
			{
				char* levelsString = new char [inputText.length() - 1];
				std::strcpy(levelsString, inputText.c_str());
				double scaleFactor = atof(levelsString);
				saturation(scaleFactor);
				glutPostRedisplay();
				menu_status = 0;
			}
			else if (menu_status == 6)
			{
				scale();
				glutPostRedisplay();
				menu_status = 0;
			}
			else if (menu_status == 7)
			{
				rotate();
				glutPostRedisplay();
				menu_status = 0;
			}
			break;
		default:				//adds to the inputText string
			inputText += key;
			glutPostRedisplay();
			break;
   }
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1920, 1200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	createGLUTMenus();
	
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0; 
}
