#include <string>
#include <fstream>
#include "Angel-yjc.h"
#include <vector>

typedef Angel::vec3 color3;
typedef Angel::vec3 point3;

std::vector<vec3> sphere_container;
vec3 *sphere_colors;
vec3 *sphere_points;
int sphere_numVertices;
int radius = 1;
float offset = 0.0;
float step = M_PI * radius / 180;
int Index = 0;
float rate = 1.0f;
mat4 accRotation = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));

#pragma region keyboard Section
bool started = 0;
bool animation = 1;
bool pressup = 0;

#pragma endregion

void loadTrianglesFromFile(const std::string &filename)
{
    std::fstream ifs(filename);
    int length, polygon;
    float x, y, z;
    ifs >> length;
    for (int i = 0; i < length; i++)
    {
        ifs >> polygon;
        for (int j = 0; j < polygon; j++)
        {
            ifs >> x >> y >> z;
            sphere_container.push_back(point3(x, y, z));
        }
    }
    ifs.close();
}

/************************************************************
 * Handout: rotate-cube-new.cpp (A Sample Code for Shader-Based OpenGL ---
                                 for OpenGL version 3.1 and later)
 * Originally from Ed Angel's textbook "Interactive Computer Graphics" 6th Ed
              sample code "example3.cpp" of Chapter 4.
 * Moodified by Yi-Jen Chiang to include the use of a general rotation function
   Rotate(angle, x, y, z), where the vector (x, y, z) can have length != 1.0,
   and also to include the use of the function NormalMatrix(mv) to return the
   normal matrix (mat3) of a given model-view matrix mv (mat4).

   (The functions Rotate() and NormalMatrix() are added to the file "mat-yjc-new.h"
   by Yi-Jen Chiang, where a new and correct transpose function "transpose1()" and
   other related functions such as inverse(m) for the inverse of 3x3 matrix m are
   also added; see the file "mat-yjc-new.h".)

 * Extensively modified by Yi-Jen Chiang for the program structure and user
   interactions. See the function keyboard() for the keyboard actions.
   Also extensively re-structured by Yi-Jen Chiang to create and use the new
   function drawObj() so that it is easier to draw multiple objects. Now a floor
   and a rotating cube are drawn.

** Perspective view of a color cube using LookAt() and Perspective()

** Colors are assigned to each vertex and then the rasterizer interpolates
   those colors across the triangles.
**************************************************************/

GLuint Angel::InitShader(const char *vShaderFile, const char *fShaderFile);

GLuint program;      /* shader program object id */
GLuint floor_buffer; /* vertex buffer object id for floor */
GLuint axis_buffer;
GLuint sphere_buffer;

vec3 routine[3];

// Projection transformation parameters
GLfloat fovy = 45;  // Field-of-view in Y direction angle (in degrees)
GLfloat aspect = 1; // Viewport aspect ratio
GLfloat zNear = 0.5, zFar = 15.0;

GLfloat angle = 0.0; // rotation angle in degrees
vec4 init_eye(7, 3, -10, 1);
vec4 eye(init_eye);

const int floor_NumVertices = 6;        //(1 face)*(2 triangles/face)*(3 vertices/triangle)
point3 floor_points[floor_NumVertices]; // positions for all vertices
color3 floor_colors[floor_NumVertices]; // colors for all vertices

const int axis_NumVertices = 9;
point3 axis_points[axis_NumVertices];
color3 axis_colors[axis_NumVertices];

float distance(const vec3 &A, const vec3 &B)
{
    vec3 delta = A - B;
    return sqrt(dot(delta, delta));
}

void floor()
{
    floor_colors[0] = color3(0, 1, 0);
    floor_colors[1] = color3(0, 1, 0);
    floor_colors[2] = color3(0, 1, 0);
    floor_colors[3] = color3(0, 1, 0);
    floor_colors[4] = color3(0, 1, 0);
    floor_colors[5] = color3(0, 1, 0);

    floor_points[0] = point3(5, 0, 8);
    floor_points[1] = point3(-5, 0, -4);
    floor_points[2] = point3(-5, 0, 8);
    floor_points[3] = point3(5, 0, 8);
    floor_points[4] = point3(-5, 0, -4);
    floor_points[5] = point3(5, 0, -4);
}

void axis()
{
    axis_points[0] = vec3(0, 0, 0);
    axis_points[1] = vec3(10, 0, 0);
    axis_points[2] = vec3(20, 0, 0);
    axis_points[3] = vec3(0, 0, 0);
    axis_points[4] = vec3(0, 10, 0);
    axis_points[5] = vec3(0, 20, 0);
    axis_points[6] = vec3(0, 0, 0);
    axis_points[7] = vec3(0, 0, 10);
    axis_points[8] = vec3(0, 0, 20);

    axis_colors[0] = vec3(1, 0, 0);
    axis_colors[1] = vec3(1, 0, 0);
    axis_colors[2] = vec3(1, 0, 0);
    axis_colors[3] = vec3(1, 0, 1);
    axis_colors[4] = vec3(1, 0, 1);
    axis_colors[5] = vec3(1, 0, 1);
    axis_colors[6] = vec3(0, 0, 1);
    axis_colors[7] = vec3(0, 0, 1);
    axis_colors[8] = vec3(0, 0, 1);
}

void sphere()
{
    loadTrianglesFromFile("sphere128.txt");
    sphere_points = new point3[sphere_container.size()];
    sphere_colors = new color3[sphere_container.size()];
    for (size_t ind = 0; ind < sphere_container.size(); ind++)
    {
        sphere_points[ind] = sphere_container[ind];
        sphere_colors[ind] = vec3(1,0.84,0);
        // sphere_colors[ind] = vec3(0);
    }
    sphere_numVertices = sphere_container.size();
}
//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{

    routine[0] = vec3(-4, 1, 4);
    routine[1] = vec3(3, 1, -4);
    routine[2] = vec3(-3, 1, -3);

    floor();
    glGenBuffers(1, &floor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
                    floor_colors);

    axis();
    glGenBuffers(1, &axis_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, axis_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_points) * 2,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(axis_points), axis_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(axis_points), sizeof(axis_colors),
                    axis_colors);

    sphere();
    glGenBuffers(1, &sphere_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * sphere_numVertices * 2,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * sphere_numVertices, sphere_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * sphere_numVertices, sizeof(vec3) * sphere_numVertices,
                    sphere_colors);

    // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader42.glsl", "fshader42.glsl");

    glEnable(GL_DEPTH_TEST);
    // glClearColor(0.529, 0.807, 0.92, 0.0);
    glLineWidth(2.0f);
}

void drawObj(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(point3) * num_vertices));
    // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}
//----------------------------------------------------------------------------
void display(void)
{
    glClearColor(0.529, 0.807, 0.92, 0.0);
    GLuint model_view; // model-view matrix uniform shader variable location
    GLuint projection; // projection matrix uniform shader variable location

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program); // Use the shader program

    model_view = glGetUniformLocation(program, "model_view");
    projection = glGetUniformLocation(program, "projection");
    mat4 p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

    /*---  Set up and pass on Model-View matrix to the shader ---*/
    // eye is a global variable of vec4 set to init_eye and updated by keyboard()
    vec4 VPN(-7, -3, 10, 0);
    vec4 at(0,0,0,1);
    vec4 up(0.0, 1.0, 0.0, 0.0);

    mat4 mv = LookAt(eye, at, up);

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj(floor_buffer, floor_NumVertices); // draw the floor
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(axis_buffer, axis_NumVertices);


    float movement = step * angle;
    if (movement > distance(routine[(Index + 1) % 3], routine[Index]))
    {
        Index = (Index + 1) % 3;
        angle = 0;
        movement = 0;
    }
    vec3 direction = normalize(routine[(Index + 1) % 3] - routine[Index]);
    vec3 rolling = cross(vec3(0, 1, 0), direction);

    vec3 position = routine[Index] + movement * direction;

    if(started && animation)
        accRotation = Rotate(rate, rolling[0], rolling[1], rolling[2]) * accRotation;

    mv = mv * Translate(position) * accRotation;
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(sphere_buffer, sphere_container.size());

    glutSwapBuffers();
}

void idle(void)
{
    //angle += 0.02;
    if (started)
        angle += rate; //YJC: change this value to adjust the cube rotation speed.
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 033: // Escape Key
    case 'q':
    case 'Q':
        exit(EXIT_SUCCESS);
        break;

    case 'X':
        eye[0] += 1.0;
        break;
    case 'x':
        eye[0] -= 1.0;
        break;
    case 'Y':
        eye[1] += 1.0;
        break;
    case 'y':
        eye[1] -= 1.0;
        break;
    case 'Z':
        eye[2] += 1.0;
        break;
    case 'z':
        eye[2] -= 1.0;
        break;

    case 'b':
    case 'B':
        started = 1;
        glutIdleFunc(idle);
        animation = 1;
    // case 'a':
    // case 'A': // Toggle between animation and non-animation
    //     animationFlag = 1 - animationFlag;
    //     if (animationFlag == 1)
    //         glutIdleFunc(idle);
    //     else
    //         glutIdleFunc(NULL);
    //     break;

    // case 'c':
    // case 'C': // Toggle between filled and wireframe cube
    //     cubeFlag = 1 - cubeFlag;
    //     break;

    // case 'f':
    // case 'F': // Toggle between filled and wireframe floor
    //     floorFlag = 1 - floorFlag;
    //     break;

    // case ' ': // reset to initial viewer/eye position
    //     eye = init_eye;
    //     break;
    // }
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if(pressup){
        pressup = 1 - pressup;
        return;
    }
    if(button == GLUT_RIGHT_BUTTON){
        if(animation) glutIdleFunc(NULL);
        else glutIdleFunc(idle);
        animation = 1 - animation;
        pressup = 1;
    }

}

void menu(int id){
    glutIdleFunc(idle);
    animation = 1;
    switch (id)
    {
    case 1:
        eye = init_eye;
        break;
    case 2:
        exit(0);
        break;
    
    default:
        break;
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
#ifdef __APPLE__ // Enable core profile of OpenGL 3.2 on macOS.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
    glutInitWindowSize(512 * 2, 512 * 2);
    glutCreateWindow("Color Cube");

#ifdef __APPLE__ // on macOS
    // Core profile requires to create a Vertex Array Object (VAO).
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#else // on Linux or Windows, we still need glew
    /* Call glewInit() and error checking */
    int err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Error: glewInit failed: %s\n", (char *)glewGetErrorString(err));
        exit(1);
    }
#endif

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutCreateMenu(menu);
    glutAddMenuEntry("Default View Point",1);
    glutAddMenuEntry("Quit",2);
    glutAttachMenu(GLUT_LEFT_BUTTON);

    init();
    glutMainLoop();
    return 0;
}
