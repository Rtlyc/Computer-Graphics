#include <stdio.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#ifdef __APPLE__  // include Mac OS X verions of headers
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glut.h>
#endif

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000
#define OPTIONC 0
#define OPTIOND 1
#define OPTIONE 2

struct Circle{
    int x;
    int y;
    int r;
    Circle(int x, int y, int r):x(x),y(y),r(r){}
};

std::vector<Circle*> circles;
int K = 100;
int COUNT = 0;
int MODE = OPTIONE;
int Cx, Cy, Cr;

void display(void);
void myinit(void);
void idle(void);

/* Function to handle file input; modification may be needed */
void file_in(std::string filename, std::vector<Circle*>& circles);


/*-----------------
The main function
------------------*/
int main(int argc, char **argv)
{
    std::cout << "PLEASE SELECT HOMEWORK PARTS\n" << "TYPE 'c' or 'd' or 'e'" << std::endl;
    std::cout.flush();
    char input;
    std::cin >> input;
    if(input == 'c')
        MODE = OPTIONC;
    else if(input == 'd')
        MODE = OPTIOND;
    else
        MODE = OPTIONE;

    if(MODE == OPTIONC){
        std::cout << "PLEASE ENTER THREE INTEGERS FOR x,y,r:" << std::endl;
        std::cout.flush();
        std::cin >> Cx >> Cy >> Cr;
    }
    
    glutInit(&argc, argv);

    /* Use both double buffering and Z buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(XOFF, YOFF);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("CS6533/CS4533 Assignment 1");
    glutIdleFunc(idle);
    glutDisplayFunc(display);

    /* Function call to handle file input here */
    file_in("../input.txt", circles);

    myinit();
    glutMainLoop();

    for(auto each:circles){
        delete each;
    }

    return 0;
}

void draw_circle(int x, int y, int r){
    int xstart = r;
    int ystart = 0;
    int dold = 1 - r;
    while(ystart <= r * 0.5 * sqrt(2)+1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold + 2 * ystart + 1;
        }else{
            dold = dold + 2 * ystart + 1 - 2 * xstart;
            xstart -= 1;
        }
        ystart += 1;
    }

    xstart = -r;
    ystart = 0;
    dold = 1 - r;
    while(ystart >= -r * 0.5 * sqrt(2)-1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold - 2 * ystart + 1;
        }else{
            dold = dold - 2 * ystart + 1 + 2 * xstart;
            xstart += 1;
        }
        ystart -= 1;
    }

    xstart = 0;
    ystart = r;
    dold = 1 - r;
    while(xstart <= r * 0.5 * sqrt(2)+1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold + 2 * xstart + 1;
        }else{
            dold = dold - 2 * ystart + 1 + 2 * xstart;
            ystart -= 1;
        }
        xstart += 1;
    }

    xstart = 0;
    ystart = -r;
    dold = 1 - r;
    while(xstart >= -r * 0.5 * sqrt(2)-1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold - 2 * xstart + 1;
        }else{
            dold = dold + 2 * ystart + 1 - 2 * xstart;
            ystart += 1;
        }
        xstart -= 1;
    }

    xstart = r;
    ystart = 0;
    dold = 1 - r;
    while(ystart >= - r * 0.5 * sqrt(2)-1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold - 2 * ystart + 1;
        }else{
            dold = dold - 2 * ystart + 1 - 2 * xstart;
            xstart -= 1;
        }
        ystart -= 1;
    }

    xstart = -r;
    ystart = 0;
    dold = 1 - r;
    while(ystart <= r * 0.5 * sqrt(2)+1){        
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold + 2 * ystart + 1;
        }else{
            dold = dold + 2 * ystart + 1 + 2 * xstart;
            xstart += 1;
        }
        ystart += 1;
    }

    xstart = 0;
    ystart = -r;
    dold = 1 - r;
    while(xstart <= r * 0.5 * sqrt(2)+1){
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold + 2 * xstart + 1;
        }else{
            dold = dold + 2 * ystart + 1 + 2 * xstart;
            ystart += 1;
        }
        xstart += 1;
    }

    xstart = 0;
    ystart = r;
    dold = 1 - r;
    while(xstart >= -r * 0.5 * sqrt(2)-1){
        glVertex2i(xstart + x,ystart + y);
        if(dold < 0){
            dold = dold - 2 * xstart + 1;
        }else{
            dold = dold - 2 * ystart + 1 - 2 * xstart;
            ystart -= 1;
        }
        xstart -= 1;
    }
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(std::string filename, std::vector<Circle*>& circles)
{
    std::fstream ifs(filename);
    int num, x, y, r;
    ifs >> num;
    while(num > 0){
        ifs >> x >> y >> r;
        Circle* each = new Circle(x,y,r);
        circles.push_back(each);
        num--;
    }
    ifs.close();
    int maximum = 0;
    for(auto each:circles){
        if(each->x > 0)
            maximum = std::max(each->x+each->r,maximum);
        else
            maximum = std::max(abs(each->x-each->r),maximum);
        if(each->y > 0)
            maximum = std::max(each->y+each->r,maximum);
        else
            maximum = std::max(abs(each->y-each->r),maximum);
    }

    int w = WINDOW_HEIGHT/2;
    if(maximum > w){
        for(auto each:circles){
                each->x = each->x * w / maximum;
                each->y = each->y * w / maximum;
                each->r = each->r * w / maximum;
            }
    }
    
}

void idle(void)
{
    COUNT = COUNT %= K;
    COUNT++;

    glutPostRedisplay(); // or call display()
}

/*---------------------------------------------------------------------
display(): This function is called once for _every_ frame. 
---------------------------------------------------------------------*/
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
    glPointSize(2.0);                     /* size of each point */

    glBegin(GL_POINTS);
    if(MODE == OPTIONC){
        draw_circle(Cx+ WINDOW_WIDTH/2,Cy+ WINDOW_HEIGHT/2,Cr);
    }else{
        if(MODE == OPTIOND)
            COUNT = K;
        for (auto each:circles)
        {
            draw_circle(each->x + WINDOW_WIDTH/2,each->y + WINDOW_HEIGHT/2,each->r*COUNT/K);
        }
    }

    glEnd();

    // std::cout << "DONE";
    // std::cout.flush();

    glFlush();                            /* render graphics */

    glutSwapBuffers();                    /* swap buffers */
}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
    glClearColor(0.0, 0.0, 0.92, 0.0);    /* blue background*/

    /* set up viewing */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    //glViewport(0, 0, WINDOW_WIDTH*2, WINDOW_HEIGHT*2);
    glMatrixMode(GL_MODELVIEW);
}