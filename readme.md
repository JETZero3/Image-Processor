# Image Processor

##### About
This is an image processor for .bmp files. It can change image properties, such as brightness, scaling, saturation, and color bits. 

It was created in university to apply the knowledge of pixel properties. It only currently works with Linux system right now, but a Windows version may come up. 

##### Dependencies
The image processor requires: 
- OpenGL Utility Toolkit (GLUT)

##### Instructions for running:
1. make all
2. ./assign1

Sidenote: make clean is also implemented. 

######Instructions for use:
- Bringing up the menu:
	1. Right-click on the mouse. 
		
- Loading:
	1. Select "Load an image" from the menu.
	2. Type in the name of the file you wish to open and press Enter. 
	3. The loaded image should appear on the left side of the screen. 
		
- Quantizing:
	1. Select "Quantize the image" from the menu. 
	2. Enter the number of levels you want to quantize the image by and press Enter.
		
- Brightness:
	1. Select "Brighten the image" from the menu. 
	2. Enter the brightness scale factor (as a number) and press Enter.

- Saturation:
	1. Select "Saturate the image" from the menu. 
	2. Enter the saturation scale factor (as a number) and press Enter.
		
- Scaling:
	1. Select "Scale the image" from the menu. 
	2. Press Enter. 
		
- Rotate:
	1. Select "Rotate the image" from the menu. 
	2. Press Enter. 
		
With all of these operations, you should see the modified image on the right hand side of the screen. 
	
- Saving the changed image:
	1. Perform quantization, brightness, saturation, scaling, or rotation on the original image. 
	2. Select "Save the changed image" from the menu. 
	3. Enter the name you wish to save the file as (in the format "filename.bmp") and press Enter. 
	
- Exiting:
	1. Hit "Esc".

##### Functions borrowed from university tutorials:
- void RenderString(float x, float y, std::string text)
- GLuint GenerateTexture(const RgbImage& theTexMap)
- GLuint loadTextureFromFile(char *filename)
- void resizeWindow(int w, int h)
- int main(int argc, char** argv)