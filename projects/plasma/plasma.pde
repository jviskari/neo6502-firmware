/**
 Plasmas with Palette Looping
 https://lodev.org/cgtutor/plasma.html#Plasmas_with_Palette_Looping_
 */

int pal[] = new int[128];
int[] buffer;
int r = 0, g = 0, b = 0;
int rd, gd, bd;

int magic = 2; 

void setup() {
  size(320, 240);
  frameRate(25);
  buffer = new int[width*height];
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      buffer[x+y*width] = int(((128+(128*DSIN(magic*x)))
        +(128+(128*DCOS(magic*y))) 
        +(128+(128*DSIN(magic*sqrt((x*x+y*y))))))/4);
    }
  }
}

float DCOS(float degrees)
{
    return cos(radians(degrees));
}


float DSIN(float degrees)
{
    return sin(radians(degrees));
}

void palette(int c,int r,int g,int b)
{
    pal[c] = color(r,g,b);
}


int getpoint(int x,int y) 
{  
    return buffer[x+y*width];
}

int plot(int x, int y,int c) 
{  
    return pixels[x+y*width] = c;
}

void draw() {
  int c;
  float s_1, s_2;
  int m = 180/30;
  int n = 180/60;
  int p = 180/4;

  int frame =frameCount;
  
  rd = 1;
  gd = 1;
  bd = 1;
  
  if ((r > 128) || (r < 0)) rd = -rd;
  if ((g > 128) || (g < 0)) gd = -gd;
  if ((b > 128) || (b < 0)) bd = -bd;

  r = r+rd;
  g = g+gd;  
  b = b+bd;
  
  for (int i = 0; i < 128; i++) {
  
    s_1 = DSIN(i*m);
    s_2 = DSIN(i*n+p);
    
    palette(i, int(r+s_1*128), int(g+s_2*128), int(b+s_1*128));
  }

  loadPixels();

  //for (int i = 0; i < width*height; i++) {                    
    //pixels[i] =  pal[(buffer[i]+frame)&127];
  //}
  
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) { 
     c=pal[(getpoint(x,y)+frame)&127];
     plot(x,y,c);
    }
  }  
  
  

  updatePixels();
}
