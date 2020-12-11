#define GL_SILENCE_DEPRECATION
#include <string>
#include <fstream>
#include "Angel-yjc.h"
#include <vector>

typedef Angel::vec3 color3;
typedef Angel::vec3 point3;

std::vector<vec4> sphere_container;
std::vector<vec3> sphere_normals;
std::vector<vec3> smooth_normals;
vec4 *sphere_colors;
vec4 *sphere_points;
vec3 *sphere_normal;
vec4 *shadow_colors;
vec3 *sphere_smooth;
int sphere_numVertices;
int radius = 1;
float offset = 0.0;
float step = M_PI * radius / 180;
int Index = 0;
float rate = 2.0f;
mat4 accRotation = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));

#pragma region texture section
#define ImageWidth 32
#define ImageHeight 32
GLubyte Image[ImageHeight][ImageWidth][4];

#define stripeImageWidth 32
GLubyte stripeImage[4 * stripeImageWidth];
#pragma endregion

#pragma region keyboard Section
bool started = 0;
bool animation = 1;
bool pressup = 0;
int isWired = 0;
int hasShadow = 1;
int hasLight = 1;
#pragma endregion

#pragma region light settings
float isFlat = 1.0f;
float hasGlobalLight = 1.0f;
float hasDirectionalLight = 1.0f;
float hasPointLight = 0.0f;
float hasSpotLight = 1.0f;
float fogState = 0;
float shadowBlended = 1;
int groundTextured = 1;
int sphereTextured = 1; 
int texIsVertical = 1;
int texIsEyeFrame = 0;
float sphereLattice = 1;
float sphereIsLatticed = 0;

vec4 globalLightAmbient = vec4(1, 1, 1, 1);

vec4 directionalAmbient = vec4(0, 0, 0, 1);
vec4 directionalDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
vec4 directionalSpecular = vec4(0.2, 0.2, 0.2, 1.0);
vec3 lightDirection = vec3(0.1, 0.0, -1.0);

vec4 pointLightAmbient = vec4(0, 0, 0, 1);
vec4 pointLightDiffuse = vec4(1, 1, 1, 1);
vec4 pointLightSpecular = vec4(1, 1, 1, 1);

vec4 spotDirectionPoint = vec4(-6, 0, -4.5f, 1);
float const_att = 2.0f;
float linear_att = 0.01f;
float quadratic_att = 0.001f;
float material_shininess = 150.0f;

vec4 groundAmbient = vec4(0.2, 0.2, 0.2, 1.0);
vec4 groundDiffuse = vec4(0, 1.0, 0, 1.0);
vec4 groundSpecular = vec4(0, 0, 0, 1);

vec4 sphereAmbient = vec4(0.2, 0.2, 0.2, 1.0);
vec4 sphereDiffuse = vec4(1.0, 0.84, 0, 1.0);
vec4 sphereSpecular = vec4(1.0, 0.84, 0, 1.0);
float shininess = 125.0f;
float cosangle = cos(20 * M_PI / 180);
float expo = 15.0f;

vec4 ambient_product, diffuse_product, specular_product;
vec4 PAmbientProduct, PDiffuseProduct, PSpecularProduct;

#pragma endregion


#pragma region particle section
#define PARTICLE_NUM 300
vec3 particleVel[PARTICLE_NUM];
vec3 particleColor[PARTICLE_NUM];
GLuint particleBuffer;
GLuint particleProgram;
int particleOpen = 0;
float startTime;
#pragma endregion

void InitParticles(){
    for(int i=0; i<PARTICLE_NUM; i++){
        particleColor[i] = vec3((rand()%256)/256.0,(rand()%256)/256.0,(rand()%256)/256.0);
        particleVel[i] = vec3(2.0*((rand()%256)/256.0-0.5),1.2*2.0*((rand()%256)/256.0),2.0*((rand()%256)/256.0-0.5));
    }
    glGenBuffers(1, &particleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleVel)+sizeof(particleColor),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particleVel), particleVel);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(particleVel), sizeof(particleColor),
                    particleColor);
}

void DrawParticles(){
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
    GLuint vVelocity = glGetAttribLocation(particleProgram, "vVelocity");
    glEnableVertexAttribArray(vVelocity);
    glVertexAttribPointer(vVelocity, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(particleProgram, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3) * PARTICLE_NUM));

    glPointSize(3.0);
    glDrawArrays(GL_TRIANGLES, 0, PARTICLE_NUM);
    glDisableVertexAttribArray(vVelocity);
    glDisableVertexAttribArray(vColor);
}

void loadTrianglesFromFile(const std::string &filename)
{
    std::fstream ifs(filename);
    int length, polygon;
    float x, y, z;
    ifs >> length;
    for (int i = 0; i < length; i++)
    {
        ifs >> polygon;
        vec4 triangle[3];
        for (int j = 0; j < polygon; j++)
        {
            ifs >> x >> y >> z;
            sphere_container.push_back(vec4(x, y, z, 1.0));
            triangle[j] = vec4(x, y, z, 1.0);
            smooth_normals.push_back(vec3(x, y, z));
        }
        vec4 u = triangle[1] - triangle[0];
        vec4 v = triangle[2] - triangle[0];
        vec3 n = normalize(cross(u, v));
        for (int j = 0; j < 3; j++)
            sphere_normals.push_back(n);
    }
    ifs.close();
}

/*************************************************************
void image_set_up(void):
  generate checkerboard and stripe images. 

* Inside init(), call this function and set up texture objects
  for texture mapping.
  (init() is called from main() before calling glutMainLoop().)
***************************************************************/
void image_set_up(void)
{
    int i, j, c;

    /* --- Generate checkerboard image to the image array ---*/
    for (i = 0; i < ImageHeight; i++)
        for (j = 0; j < ImageWidth; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

            if (c == 1) /* white */
            {
                c = 255;
                Image[i][j][0] = (GLubyte)c;
                Image[i][j][1] = (GLubyte)c;
                Image[i][j][2] = (GLubyte)c;
            }
            else /* green */
            {
                Image[i][j][0] = (GLubyte)0;
                Image[i][j][1] = (GLubyte)150;
                Image[i][j][2] = (GLubyte)0;
            }

            Image[i][j][3] = (GLubyte)255;
        }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /*--- Generate 1D stripe image to array stripeImage[] ---*/
    for (j = 0; j < stripeImageWidth; j++)
    {
        /* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
        When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
      */
        stripeImage[4 * j] = (GLubyte)255;
        stripeImage[4 * j + 1] = (GLubyte)((j > 4) ? 255 : 0);
        stripeImage[4 * j + 2] = (GLubyte)0;
        stripeImage[4 * j + 3] = (GLubyte)255;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    /*----------- End 1D stripe image ----------------*/

    /*--- texture mapping set-up is to be done in 
      init() (set up texture objects),
      display() (activate the texture object to be used, etc.)
      and in shaders.
 ---*/

} /* end function */

GLuint Angel::InitShader(const char *vShaderFile, const char *fShaderFile);

GLuint program;      /* shader program object id */
GLuint floor_buffer; /* vertex buffer object id for floor */
GLuint axis_buffer;
GLuint sphere_buffer;
GLuint shadow_buffer;

vec3 routine[3];

// Projection transformation parameters
GLfloat fovy = 45;  // Field-of-view in Y direction angle (in degrees)
GLfloat aspect = 1; // Viewport aspect ratio
GLfloat zNear = 0.5, zFar = 15.0;

GLfloat angle = 0.0; // rotation angle in degrees
vec4 init_eye(7, 3, -10, 1);
vec4 eye(init_eye);
vec4 light_point(-14, 12, -3, 1);

const int floor_NumVertices = 6;      //(1 face)*(2 triangles/face)*(3 vertices/triangle)
vec4 floor_points[floor_NumVertices]; // positions for all vertices
vec4 floor_colors[floor_NumVertices]; // colors for all vertices
vec3 floor_normals[floor_NumVertices];
vec2 floor_texCoord[6] = {
    vec2(0, 0),
    vec2(0, 6),
    vec2(5, 6),

    vec2(5, 6),
    vec2(5, 0),
    vec2(0, 0)};

const int axis_NumVertices = 9;
vec4 axis_points[axis_NumVertices];
vec4 axis_colors[axis_NumVertices];

static GLuint texName;
static GLuint sphereTexName;
static GLuint sphereTexName2;

float distance(const vec3 &A, const vec3 &B)
{
    vec3 delta = A - B;
    return sqrt(dot(delta, delta));
}

void floor()
{
    floor_colors[0] = vec4(0, 1, 0, 1);
    floor_colors[1] = vec4(0, 1, 0, 1);
    floor_colors[2] = vec4(0, 1, 0, 1);
    floor_colors[3] = vec4(0, 1, 0, 1);
    floor_colors[4] = vec4(0, 1, 0, 1);
    floor_colors[5] = vec4(0, 1, 0, 1);

    floor_points[0] = vec4(-5, 0, 8, 1);
    floor_points[1] = vec4(-5, 0, -4, 1);
    floor_points[2] = vec4(5, 0, -4, 1);
    floor_points[3] = vec4(5, 0, -4, 1);
    floor_points[4] = vec4(5, 0, 8, 1);
    floor_points[5] = vec4(-5, 0, 8, 1);

    vec4 u = floor_points[1] - floor_points[0];
    vec4 v = floor_points[2] - floor_points[0];
    vec3 normal = normalize(cross(u, v));
    for (int i = 0; i < 6; i++)
        floor_normals[i] = normal;
}

void axis()
{
    axis_points[0] = vec4(0, 0, 0, 1);
    axis_points[1] = vec4(10, 0, 0, 1);
    axis_points[2] = vec4(20, 0, 0, 1);
    axis_points[3] = vec4(0, 0, 0, 1);
    axis_points[4] = vec4(0, 10, 0, 1);
    axis_points[5] = vec4(0, 20, 0, 1);
    axis_points[6] = vec4(0, 0, 0, 1);
    axis_points[7] = vec4(0, 0, 10, 1);
    axis_points[8] = vec4(0, 0, 20, 1);

    axis_colors[0] = vec4(1, 0, 0, 1);
    axis_colors[1] = vec4(1, 0, 0, 1);
    axis_colors[2] = vec4(1, 0, 0, 1);
    axis_colors[3] = vec4(1, 0, 1, 1);
    axis_colors[4] = vec4(1, 0, 1, 1);
    axis_colors[5] = vec4(1, 0, 1, 1);
    axis_colors[6] = vec4(0, 0, 1, 1);
    axis_colors[7] = vec4(0, 0, 1, 1);
    axis_colors[8] = vec4(0, 0, 1, 1);
}

void sphere()
{
    loadTrianglesFromFile("sphere1024.txt");
    sphere_points = new vec4[sphere_container.size()];
    sphere_colors = new vec4[sphere_container.size()];
    sphere_normal = new vec3[sphere_container.size()];
    shadow_colors = new vec4[sphere_container.size()];
    sphere_smooth = new vec3[sphere_container.size()];
    for (size_t ind = 0; ind < sphere_container.size(); ind++)
    {
        sphere_points[ind] = sphere_container[ind];
        sphere_colors[ind] = vec4(1, 0.84, 0, 1.0);
        shadow_colors[ind] = vec4(0.25, 0.25, 0.25, 0.65);
        sphere_normal[ind] = sphere_normals[ind];
        sphere_smooth[ind] = smooth_normals[ind];
        // sphere_colors[ind] = vec3(0);
    }
    sphere_numVertices = sphere_container.size();
}

void SetUp_Lighting_Uniform_Vars(mat4 mv)
{
    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);

    vec4 light_position_eyeFrame = mv * light_point;
    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position_eyeFrame);
    glUniform1f(glGetUniformLocation(program, "ConstAtt"), const_att);
    glUniform1f(glGetUniformLocation(program, "LinearAtt"), linear_att);
    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

    glUniform1d(glGetUniformLocation(program, "hasGlobalLight"), hasGlobalLight);
    glUniform1d(glGetUniformLocation(program, "hasDirectionalLight"), hasDirectionalLight);
}
//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
    routine[0] = vec3(-4, 1, 4);
    routine[1] = vec3(3, 1, -4);
    routine[2] = vec3(-3, 1, -3);

    image_set_up();
    floor();

    axis();
    glGenBuffers(1, &axis_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, axis_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_points) * 2,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(axis_points), axis_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(axis_points), sizeof(axis_colors),
                    axis_colors);

    sphere();

    InitParticles();

    glGenBuffers(1, &shadow_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, shadow_buffer);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec4)) * sphere_numVertices,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * sphere_numVertices, sphere_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * sphere_numVertices, sizeof(vec4) * sphere_numVertices,
                    shadow_colors);

    // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader53.glsl", "fshader42.glsl");
    particleProgram = InitShader("vshader.glsl", "fshader.glsl");

    glEnable(GL_DEPTH_TEST);
    // glClearColor(0.529, 0.807, 0.92, 0.0);
    glLineWidth(2.0f);
}

void drawObj(GLuint buffer, int num_vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * num_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * num_vertices + sizeof(vec3) * num_vertices));

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vTexCoord);
}

void drawObj(GLuint buffer, int num_vertices, bool lighting)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    GLuint vColor, vNormal;
    if (!lighting)
    {
        vColor = glGetAttribLocation(program, "vColor");
        glEnableVertexAttribArray(vColor);
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                              BUFFER_OFFSET(sizeof(vec4) * num_vertices));
        // the offset is the (total) size of the previous vertex attribute array(s)
    }
    else
    {
        vNormal = glGetAttribLocation(program, "vNormal");
        glEnableVertexAttribArray(vNormal);
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                              BUFFER_OFFSET(sizeof(vec4) * num_vertices));
    }
    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    if (!lighting)
        glDisableVertexAttribArray(vColor);
    else
        glDisableVertexAttribArray(vNormal);
}

//----------------------------------------------------------------------------

mat4 shadowMatrix(vec4 lightpoint)
{
    mat4 shadow_matrix;
    shadow_matrix[0][0] = lightpoint[1];
    shadow_matrix[0][1] = -lightpoint[0];
    shadow_matrix[2][1] = -lightpoint[2];
    shadow_matrix[2][2] = lightpoint[1];
    shadow_matrix[3][1] = -1;
    shadow_matrix[3][3] = lightpoint[1];
    return shadow_matrix;
}


void create_sphere_buffer()
{
    if (!isWired && (hasGlobalLight + hasDirectionalLight + hasPointLight + hasSpotLight > 0) && hasLight) //light mode
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &sphereTexName);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_1D, sphereTexName);

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &sphereTexName2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, sphereTexName2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
        if (isFlat)
        {
            glGenBuffers(1, &sphere_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
            glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3)) * sphere_numVertices, NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * sphere_numVertices, sphere_points);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * sphere_numVertices, sizeof(vec3) * sphere_numVertices,
                            sphere_normal);
        }
        else
        {
            glGenBuffers(1, &sphere_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
            glBufferData(GL_ARRAY_BUFFER, (sizeof(vec4) + sizeof(vec3)) * sphere_numVertices, NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * sphere_numVertices, sphere_points);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * sphere_numVertices, sizeof(vec3) * sphere_numVertices,
                            sphere_smooth);
        }
    }
    else //color mode
    {
        glGenBuffers(1, &sphere_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * sphere_numVertices * 2,
                     NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * sphere_numVertices, sphere_points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * sphere_numVertices, sizeof(vec4) * sphere_numVertices, sphere_colors);
    }
}

void create_floor_buffer()
{
    if (hasLight)
    {
        if (groundTextured)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glGenTextures(1, &texName);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texName);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);

            glGenBuffers(1, &floor_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_normals) + sizeof(floor_texCoord),
                         NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_normals), floor_normals);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_normals), sizeof(floor_texCoord), floor_texCoord);
        }
        else
        {
            glGenBuffers(1, &floor_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_normals),
                         NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_normals),
                            floor_normals);
        }
    }
    else
    {
        glGenBuffers(1, &floor_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
                     NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
                        floor_colors);
    }
}

void display(void)
{
#pragma region Setup ModelView and Projection
    glClearColor(0.529, 0.807, 0.92, 0.0);
    GLuint model_view; // model-view matrix uniform shader variable location
    GLuint projection; // projection matrix uniform shader variable location

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program); // Use the shader program

    model_view = glGetUniformLocation(program, "ModelView");
    projection = glGetUniformLocation(program, "Projection");
    mat4 p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

    /*---  Set up and pass on Model-View matrix to the shader ---*/
    // eye is a global variable of vec4 set to init_eye and updated by keyboard()
    vec4 VPN(-7, -3, 10, 0);
    vec4 at(0, 0, 0, 1);
    vec4 up(0.0, 1.0, 0.0, 0.0);

    mat4 mv = LookAt(eye, at, up);

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major

    glUniform1f(glGetUniformLocation(program, "fogState"), fogState);
    glUniform1i(glGetUniformLocation(program, "ground_Textured"),0);
    glUniform1i(glGetUniformLocation(program, "sphere_Textured"),0);
    glUniform1f(glGetUniformLocation(program, "sphereLattice"),0);
    glUniform1i(glGetUniformLocation(program, "ground_Textured"), 0);
    glUniform1i(glGetUniformLocation(program, "sphere_Textured"),0);
    glUniform1f(glGetUniformLocation(program, "sphereLattice"),0);
#pragma endregion

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(axis_buffer, axis_NumVertices, false); // draw axis

#pragma region Z - buffer Algorithms
    glDepthMask(GL_FALSE);
    create_floor_buffer();
    if (hasGlobalLight + hasDirectionalLight + hasPointLight + hasSpotLight > 0 && hasLight)
    {
        glUniform4fv(glGetUniformLocation(program, "GlobalAmbientProduct"), 1, groundAmbient * globalLightAmbient);
        glUniform1f(glGetUniformLocation(program, "hasGlobalLight"), hasGlobalLight);

        ambient_product = groundAmbient * directionalAmbient;
        diffuse_product = groundDiffuse * directionalDiffuse;
        specular_product = groundSpecular * directionalSpecular;
        glUniform1f(glGetUniformLocation(program, "hasDirectionalLight"), hasDirectionalLight);
        glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
        glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
        glUniform3fv(glGetUniformLocation(program, "lightdirection"), 1, lightDirection);
        glUniform1f(glGetUniformLocation(program, "Shininess"), shininess);

        glUniform1f(glGetUniformLocation(program, "hasPointLight"), hasPointLight);
        PAmbientProduct = groundAmbient * pointLightAmbient;
        PDiffuseProduct = groundDiffuse * pointLightDiffuse;
        PSpecularProduct = groundSpecular * pointLightSpecular;
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, mv * light_point);
        glUniform4fv(glGetUniformLocation(program, "PAmbientProduct"), 1, PAmbientProduct);
        glUniform4fv(glGetUniformLocation(program, "PDiffuseProduct"), 1, PDiffuseProduct);
        glUniform4fv(glGetUniformLocation(program, "PSpecularProduct"), 1, PSpecularProduct);
        glUniform1f(glGetUniformLocation(program, "ConstAtt"), const_att);
        glUniform1f(glGetUniformLocation(program, "LinearAtt"), linear_att);
        glUniform1f(glGetUniformLocation(program, "QuadAtt"), quadratic_att);

        glUniform1f(glGetUniformLocation(program, "hasSpotLight"), hasSpotLight);
        glUniform4fv(glGetUniformLocation(program, "SpotlightPoint"), 1, mv * spotDirectionPoint);
        glUniform1f(glGetUniformLocation(program, "cosangle"), cosangle);
        glUniform1f(glGetUniformLocation(program, "expo"), expo);

        mat3 normal_matrix = NormalMatrix(mv, 1);
        glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // drawObj(floor_buffer, floor_NumVertices, true); // draw the floor
        glUniform1i(glGetUniformLocation(program, "texture_2D"), 0);
        glUniform1i(glGetUniformLocation(program, "texture_1D"), 1);
        glUniform1i(glGetUniformLocation(program, "ground_Textured"), groundTextured);


        drawObj(floor_buffer, 6);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawObj(floor_buffer, floor_NumVertices, false);
    }

    glUniform1f(glGetUniformLocation(program, "hasGlobalLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasDirectionalLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasPointLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasSpotLight"), 0.0);
    glUniform1i(glGetUniformLocation(program, "ground_Textured"), 0);

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

    if (started && animation)
        accRotation = Rotate(rate, rolling[0], rolling[1], rolling[2]) * accRotation;

    mat4 model_move = Translate(position) * accRotation;

    mv = LookAt(eye, at, up) * shadowMatrix(light_point) * model_move;

    //draw shadow
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if (!isWired)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (hasShadow > 0)
    {
        if (shadowBlended)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glUniform1f(glGetUniformLocation(program, "sphereLattice"),sphereLattice*sphereIsLatticed);
        drawObj(shadow_buffer, sphere_container.size(), false);
        if (shadowBlended)
        {
            glDisable(GL_BLEND);
        }
    }
    glUniform1f(glGetUniformLocation(program, "sphereLattice"),0);

    glDepthMask(GL_TRUE); 
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, LookAt(eye, at, up));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (hasLight && groundTextured)
        drawObj(floor_buffer, floor_NumVertices);
    else if (hasLight)
        drawObj(floor_buffer, floor_NumVertices, hasGlobalLight + hasDirectionalLight + hasPointLight + hasSpotLight);
    else
        drawObj(floor_buffer, floor_NumVertices, false);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

#pragma endregion

    mv = LookAt(eye, at, up) * model_move;
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    create_sphere_buffer();
    if (!isWired && (hasGlobalLight + hasDirectionalLight + hasPointLight + hasSpotLight > 0) && hasLight)
    {

        glUniform4fv(glGetUniformLocation(program, "GlobalAmbientProduct"), 1, sphereAmbient * globalLightAmbient);
        glUniform1f(glGetUniformLocation(program, "hasGlobalLight"), hasGlobalLight);

        ambient_product = sphereAmbient * directionalAmbient;
        diffuse_product = sphereDiffuse * directionalDiffuse;
        specular_product = sphereSpecular * directionalSpecular;
        glUniform1f(glGetUniformLocation(program, "hasDirectionalLight"), hasDirectionalLight);
        glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
        glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
        glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
        glUniform3fv(glGetUniformLocation(program, "lightdirection"), 1, lightDirection);
        glUniform1f(glGetUniformLocation(program, "Shininess"), shininess);

        glUniform1f(glGetUniformLocation(program, "hasPointLight"), hasPointLight);
        PAmbientProduct = sphereAmbient * pointLightAmbient;
        PDiffuseProduct = sphereDiffuse * pointLightDiffuse;
        PSpecularProduct = sphereSpecular * pointLightSpecular;
        glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, LookAt(eye, at, up) * light_point);
        glUniform4fv(glGetUniformLocation(program, "PAmbientProduct"), 1, PAmbientProduct);
        glUniform4fv(glGetUniformLocation(program, "PDiffuseProduct"), 1, PDiffuseProduct);
        glUniform4fv(glGetUniformLocation(program, "PSpecularProduct"), 1, PSpecularProduct);
        glUniform1f(glGetUniformLocation(program, "ConstAtt"), const_att);
        glUniform1f(glGetUniformLocation(program, "LinearAtt"), linear_att);
        glUniform1f(glGetUniformLocation(program, "QuadAtt"), quadratic_att);

        glUniform1f(glGetUniformLocation(program, "hasSpotLight"), hasSpotLight);
        glUniform4fv(glGetUniformLocation(program, "SpotlightPoint"), 1, LookAt(eye, at, up) * spotDirectionPoint);
        glUniform1f(glGetUniformLocation(program, "cosangle"), cosangle);
        glUniform1f(glGetUniformLocation(program, "expo"), expo);

        mat3 normal_matrix = NormalMatrix(mv, 1);
        glUniformMatrix3fv(glGetUniformLocation(program, "Normal_Matrix"), 1, GL_TRUE, normal_matrix);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUniform1i(glGetUniformLocation(program, "texture_2D"), 3);
        glUniform1i(glGetUniformLocation(program, "texture_1D"), 1);
        glUniform1i(glGetUniformLocation(program, "sphere_Textured"),sphereTextured);
        glUniform1i(glGetUniformLocation(program, "texIsVertical"), texIsVertical);
        glUniform1i(glGetUniformLocation(program, "texIsEyeFrame"), texIsEyeFrame);
        glUniform1f(glGetUniformLocation(program, "sphereLattice"),sphereLattice*sphereIsLatticed);
        drawObj(sphere_buffer, sphere_numVertices, true); // draw the sphere
    }
    else
    {
        if (!isWired)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawObj(sphere_buffer, sphere_container.size(), false);
    }
    glUniform1f(glGetUniformLocation(program, "hasGlobalLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasDirectionalLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasPointLight"), 0.0);
    glUniform1f(glGetUniformLocation(program, "hasSpotLight"), 0.0);
    glUniform1i(glGetUniformLocation(program, "sphere_Textured"),0);
    glUniform1f(glGetUniformLocation(program, "sphereLattice"),0);

    if(particleOpen == 1){
        glUseProgram(particleProgram);
        model_view = glGetUniformLocation(particleProgram, "model_view");
        projection = glGetUniformLocation(particleProgram, "projection");
        glUniformMatrix4fv(projection, 1, GL_TRUE, Perspective(fovy, aspect, zNear, zFar)); 
        glUniformMatrix4fv(model_view, 1, GL_TRUE, LookAt(eye,at,up));
        float temp = (float)glutGet(GLUT_ELAPSED_TIME)-startTime;
        float t = (int)temp%5000;
        glUniform1f(glGetUniformLocation(particleProgram, "t"),t);
        glPointSize(3.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        DrawParticles();
    }

    glutSwapBuffers();
}
void myexit()
{
    delete[] sphere_points;
    delete[] sphere_normal;
    delete[] sphere_colors;
    delete[] sphere_smooth;
    delete[] shadow_colors;
    exit(0);
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
        myexit();
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
        break;
    
    case 'o':
    case 'O':
        texIsEyeFrame = 0;
        break;
    
    case 'e':
    case 'E':
        texIsEyeFrame = 1;
        break;
    
    case 'v':
    case 'V':
        texIsVertical = 1;
        break;
    
    case 's':
    case 'S':
        texIsVertical = 0;
        break;

    case 'u':
    case 'U':
        sphereLattice = 1;
        break;
    
    case 't':
    case 'T':
        sphereLattice = 2;
        break;
    
    case 'l':
    case 'L':
        sphereIsLatticed = 1-sphereIsLatticed;
        break;

    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (pressup)
    {
        pressup = 1 - pressup;
        return;
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (animation)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(idle);
        animation = 1 - animation;
        pressup = 1;
    }
}

void menu(int id)
{
    glutIdleFunc(idle);
    animation = 1;
    switch (id)
    {
    case 1:
        eye = init_eye;
        break;
    case 2: //shadow
        hasShadow = 1 - hasShadow;
        break;
    case 3: //wireFram
        isWired = 1 - isWired;
        break;
    case 4:
        myexit();
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void LightingMenu(int id)
{
    switch (id)
    {
    case 1: //No lighting
        hasLight = 0;
        break;
    case 2: //Enable lighting
        hasLight = 1;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void ShadingMenu(int id)
{
    switch (id)
    {
    case 1: //Flat shading
        isFlat = 1;
        break;
    case 2: //Smooth shading
        isFlat = 0;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void LightSourceMenu(int id)
{
    switch (id)
    {
    case 1: //Spot Light
        hasSpotLight = 1.0f;
        hasPointLight = 0.0f;
        break;

    case 2: //Point Source
        hasSpotLight = 0.0f;
        hasPointLight = 1.0f;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void FogMenu(int id)
{
    switch (id)
    {
    case 0: //no fog
        fogState = 0;
        break;

    case 1: //linear
        fogState = 1;
        break;

    case 2: //exponential
        fogState = 2;
        break;

    case 3: //double expo
        fogState = 3;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void ShadowBlendMenu(int id)
{
    switch (id)
    {
    case 0: //No
        shadowBlended = 0;
        break;

    case 1: //Yes
        shadowBlended = 1;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void GroundTextureMenu(int id)
{
    switch (id)
    {
    case 0:
        groundTextured = 0;
        break;
    case 1:
        groundTextured = 1;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void SphereTextureMenu(int id){
    switch (id)
    {
    case 0: //No sphere texture
        sphereTextured = 0;
        break;
    
    case 1: //Contour Lines
        sphereTextured = 1;
        break;
    
    case 2: //CheckerBoard
        sphereTextured = 2;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


void ParticleOpenMenu(int id){
    switch (id)
    {
    case 0: //particle is closed
        particleOpen = 0;
        break;

    case 1: //particle is open
        if(particleOpen==0){
            startTime = (float)glutGet(GLUT_ELAPSED_TIME);
            particleOpen = 1;
        }
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

    int lightingmenu = glutCreateMenu(LightingMenu);
    glutAddMenuEntry("No", 1);
    glutAddMenuEntry("Yes", 2);

    int shadingmenu = glutCreateMenu(ShadingMenu);
    glutAddMenuEntry("Flat Shading", 1);
    glutAddMenuEntry("Smooth Shading", 2);

    int lightsourcemenu = glutCreateMenu(LightSourceMenu);
    glutAddMenuEntry("Spot Light", 1);
    glutAddMenuEntry("Point Source", 2);

    int fogmenu = glutCreateMenu(FogMenu);
    glutAddMenuEntry("No Fog", 0);
    glutAddMenuEntry("Linear", 1);
    glutAddMenuEntry("Exponential", 2);
    glutAddMenuEntry("Exponential Square", 3);

    int shadowblendmenu = glutCreateMenu(ShadowBlendMenu);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    int groundtexturemenu = glutCreateMenu(GroundTextureMenu);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    int spheretexturemenu = glutCreateMenu(SphereTextureMenu);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes - Contour Lines", 1);
    glutAddMenuEntry("Yes - Checkerboard", 2);

    int particlemenu = glutCreateMenu(ParticleOpenMenu);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    glutCreateMenu(menu);
    glutAddMenuEntry("Default View Point", 1);
    glutAddMenuEntry("Shadow", 2);
    glutAddMenuEntry("Wire-Frame", 3);

    glutAddSubMenu("Enable Lighting", lightingmenu);
    glutAddSubMenu("Shading Option", shadingmenu);
    glutAddSubMenu("Light Source", lightsourcemenu);
    glutAddSubMenu("Fog Setting", fogmenu);
    glutAddSubMenu("Shadow Blended", shadowblendmenu);
    glutAddSubMenu("Texture Mapped Ground", groundtexturemenu);
    glutAddSubMenu("Texture Mapped Sphere", spheretexturemenu);
    glutAddSubMenu("Firework", particlemenu);

    glutAddMenuEntry("Quit", 4);

    glutAttachMenu(GLUT_LEFT_BUTTON);

    init();
    glutMainLoop();
    return 0;
}
