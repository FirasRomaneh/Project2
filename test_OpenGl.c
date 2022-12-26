#include<GL/glu.h>
#include<GL/glut.h>

float p1 = 10, p2 = 10;
float deltax = 0.005;
float deltay = .01;
float c = 0;

void Display(){



glClearColor(1,1,1,0);
glClear(GL_COLOR_BUFFER_BIT);
glLineWidth(3);
glBegin(GL_LINE_LOOP);
glVertex2f(5,5);
glVertex2f(195,5);
glVertex2f(195,70);
glVertex2f(5,70);

glVertex2f(5,75);
glVertex2f(195,75);
glVertex2f(195,150);
glVertex2f(5,150);

glVertex2f(5,155);
glVertex2f(195,155);
glVertex2f(195,195);
glVertex2f(5,195);

glEnd();

glPointSize(15);
glBegin(GL_POINTS);
glColor3f(1,0,0);
glVertex2f(90,p1);

glColor3f(0,0,1);
glVertex2f(100,p2);
glColor3f(0,1,0);
glVertex2f(50,50);

glColor3f(0,1,0);
glVertex2f(130,50);

glColor3f(0,1,0);
glVertex2f(60,50);

glColor3f(0,1,0);
glVertex2f(80,50);

glEnd();
glFlush();

p1=p1+deltax;

if (p1 > 200 || p1 <=10 ){
 deltax=-deltax;
}
   

p2=p2+=deltay;

if (p2 > 200 || p2 <=10 ){
 deltay=-deltay;
} 
   

glutPostRedisplay();

}

int main(int argC,char *argV[]){
    
    glutInit(&argC,argV);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    
    glutCreateWindow("Points");
    
    gluOrtho2D(0,200,0,200);
    
    glutDisplayFunc(Display);
    // glutDisplayFunc(Display2);
    glutMainLoop();
    // return 0;


}