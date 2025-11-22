// This program is the second program working with OpenGL.
// We use Jamie King's 3D graphics tutorial series for the updates.

#define GLM_FORCE_SWIZZLE
// Third party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// c++ standard library
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// our libraries
#include <Camera.hpp>
#include <ezVertex.hpp>
#include <ShapeGenerator.hpp>

#define EZ_W_NAME	"ez-project"

// "m", is for member.
struct App {
    int mScreenHeight = 480;
    int mScreenWidth = 640;
    SDL_Window *mGraphicsApplicationWindow = nullptr;
    SDL_GLContext mOpenGLContext = nullptr;
    // Main loop flag
    // If this quit = "true", then the program ends.
    bool mQuit = false; 

    // Shader
    // The following stores a unique id for the graphics pipeline
    // program object that will be used for our OpenGL draw calls.
    GLuint mGraphicsPipeLineShaderProgram = 0;

    Camera mCamera;
};

struct Transform {
    glm::mat4 mModelMatrix{glm::mat4(1.0f)};
};

struct Mesh3D {
    // Vertex Array Objects (VAO)
    // Vertex Array Objects encapsulate all of the items needed to render an object
    // For example, we may have mutliple vertex buffer objects (VBO) related
    // to rendering one object.
    // THe VAO allows us to setup the OpenGL state to render that object using
    // the correct layout and correct buffers with one call after being setup.
    GLuint mVertexArrayObject = 0;    

    // Vertex Buffer Object (VBO)
    // Vertex Buffer Objects store information relating to vertices (e.g. positions,
    // normals, textures.)
    // VBOs are our mechanism for arranging geometry on the GPU.
    GLuint mVertexBufferObject = 0;

    // Index buffer object (IBO) 
    // Used to store the array of indices that we want to draw from 
    // when we do indexed drawing.
    GLuint mIndexBufferObject = 0;

    // This is the graphics pipeline we will use
    GLuint mPipeline = 0;
    // At a minimum, every Modern OpenGL program needs a vertex and a
    // fragment shader.
    Transform mTransform;
};

// Globals
App gApp;
Mesh3D gMesh1;
GLuint gNumIndices;
//Mesh3D gMesh2;

// print some info
void getOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// initialize
void initializeProgram(App *app)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    // Setup the OpenGL context
    // use OpenGL 4.1 core or greater
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // create an application window using OpenGL that supports SDL	
    app->mGraphicsApplicationWindow = SDL_CreateWindow(EZ_W_NAME, 
                                      SDL_WINDOWPOS_UNDEFINED, 
                                      SDL_WINDOWPOS_UNDEFINED, 
                                      app->mScreenWidth,
                                              app->mScreenHeight, 
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    // error checking 
	if (app->mGraphicsApplicationWindow == nullptr) {
		std::cout << "The SDL_CreateWindow() API failed, exiting now..." 
                << SDL_GetError()  << std::endl;
        exit(1);
	}

    // create an OpenGL Graphics Context
    app->mOpenGLContext = SDL_GL_CreateContext(app->mGraphicsApplicationWindow);
    if (app->mOpenGLContext == nullptr) {
        std::cout << "The SDL_GL_CreateContext() API failed, exiting now..."
                << std::endl;
        exit(1);
    }

	// initialize the GLAD library. (It loads all the openGL function pointers)
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "glad was not initialized, exiting now..." << std::endl;
		exit(1);
	}	

    // This is our fn(), it prints out some info to stdout.
    getOpenGLVersionInfo();
}

// This is effectively our constructor
// Setup your geometry during the vertex specification step per
// mesh
// Setup which shader pipeline you'll use with your mesh
void meshCreate(Mesh3D *mesh)
{

    //ShapeData shape = ShapeGenerator::makeTriangle();
    ShapeData shape = ShapeGenerator::makeCube();
    // Vertex Arrays Object (VAO) setup
    // note: we can think of the VAO as a 'wrapper around' all of the Vertex
    // buffer obects; In the sense that it encapsulates all VBO state that we
    // are setting up. Thus, it is also important that we glBindVertexArray
    // (i.e select the VAO we want to use.) before our vertex buffer object
    // operations.
    glGenVertexArrays(1, &mesh->mVertexArrayObject);
    
    // We bind (i.e. select) to the Vertex Array Object (VAO) that we want to
    // work with.
    glBindVertexArray(mesh->mVertexArrayObject);

    // Vertex Buffer Object (VBO) creation
    // Create a new vertex buffer object
    // note: we'll see this pattern of code often in OpenGL, that of creating
    // and binding to a buffer.
    glGenBuffers(1, &mesh->mVertexBufferObject);

    // Next we will do glBindBuffer.
    // Bind is the equivalent to 'selecting the active buffer object' that we 
    // want to work with in OpenGL.
    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);

    // now, in our curretly binded buffer, we populate the data from our 'vertexPositions'
    // (which lives on the CPU), onto a buffer that will live on the GPU
    glBufferData(GL_ARRAY_BUFFER, // kind of buffer we are working with 
                shape.vertexBufferSize(), // size of data in bytes
                shape.vertices, // raw array of data
                GL_STATIC_DRAW); // how we intend to use the data
            
    glGenBuffers(1, &mesh->mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                mesh->mIndexBufferObject);
    // populate our index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                shape.indexBufferSize(),
                shape.indices,
                GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    // the 3 is for the x, y, z
    glVertexAttribPointer(0,
                          3,    // the number of components
                          GL_FLOAT, // type
                          false, // is the data normalized
                          sizeof(float) * 6, // stride
                          (void *)0 // offset, in reference to the start of the vector
                          ); 
           
    // now linking up the color attributes in our VBO
    glVertexAttribPointer(1,
                          3, // RGB colors
                          GL_FLOAT,
                          false,
                          sizeof(float) * 6,
                          (GLvoid *)(sizeof(GLfloat) * 3)
                          );

    glEnableVertexAttribArray(1);

    // unbind our currently bound vertex array object
    glBindVertexArray(0);
    // disable any attributes we opened in our vertex attribute
    // array, we don't want to leave them open.
    glDisableVertexAttribArray(0); // the literal is a reference to an index?
    glDisableVertexAttribArray(1);
    
    // get the number of indices for the shape
    gNumIndices = shape.numIndices;
    // delete the memory allocated for the triangle
    shape.cleanup();
}

void meshDelete(Mesh3D *mesh)
{
    glDeleteBuffers(1, &mesh->mVertexBufferObject);
    glDeleteVertexArrays(1, &mesh->mVertexArrayObject);
}

// need to set the graphics pipeline before we draw
void meshSetPipeline(Mesh3D *mesh, GLuint pipeline)
{
    mesh->mPipeline = pipeline;
}

// meshUpdate:
// Typically we will use this for setting some sort of 'state' for our mesh.
// note: some of the calls may take place at different stages (post-processing)
//       of the pipeline.
void meshUpdate(Mesh3D *mesh)
{
    ;
}

// Returns the location of a uniform variable based on its name
int findUniformLocation(GLuint pipeline, const GLchar *name)
{

    GLint location = glGetUniformLocation(pipeline, name);
    if (location < 0) {
        std::cerr << "Could not find " << name <<
                    "maybe the spelling is wrong?\nExiting now" << std::endl;
        exit(EXIT_FAILURE);
    }

    return location;
}

// meshDraw
// The render function gets called once per loop.
// Typically this includes 'glDraw' related calls, and the relevant setup
// of buffers for thos calls.
//
// Note: We draw per mesh, choose the graphics pipeline that we want to use,
//       generally not very efficient, update later.
void meshDraw(Mesh3D *mesh)
{
    if (mesh == nullptr) {
        exit(1);
    }
    
    // setup which graphics pipeline we are going to use
    glUseProgram(mesh->mPipeline);

    // get the location of the uniform variables we are using for this
    // program.
    
    // model-matrix-related uniform
    GLint u_ModelMatrixLocation = findUniformLocation(gApp.mGraphicsPipeLineShaderProgram,
                                        "u_ModelMatrix");
    // send it!
    // The 1 is because we are currently using 1 matrix.
    // The false is the predicate for the matrix-transpose operation.
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, 
                        &mesh->mTransform.mModelMatrix[0][0]);

    // view-matrix-related uniform
    glm::mat4 view = gApp.mCamera.GetViewMatrix();
    GLint u_ViewLocation = findUniformLocation(gApp.mGraphicsPipeLineShaderProgram,
                                        "u_ViewMatrix");
    // send it!
    glUniformMatrix4fv(u_ViewLocation, 1, false, &view[0][0]);

    // projection-matrix-related uniform
    glm::mat4 perspective = gApp.mCamera.getProjectionMatrix();

    GLint u_ProjectionLocation = findUniformLocation(gApp.mGraphicsPipeLineShaderProgram,
                                        "u_Projection");
    // send it!
    glUniformMatrix4fv(u_ProjectionLocation, 1, false, &perspective[0][0]);
    
    // lets add some color via uniform
    //glm::vec3 dominatingColor(1.0f, 0.0f, 0.0f);
    // lets find the location of the color uniform
   // GLint u_dominatingColor = findUniformLocation(gApp.mGraphicsPipeLineShaderProgram,
    //                                    "dominatingColor");
    // send it!
    // I think its how the vec3 is implemented, it overloads the 
    // index operator.
    //glUniform3fv(u_dominatingColor, 1, &dominatingColor[0]);

    // enable our attributes
    glBindVertexArray(mesh->mVertexArrayObject);

    // select the vertex buffer object we want to enabel
    // also we wrap this with the macro we defined
    //
    // we commented this out, don't need to rebind it,
    // the above statement already does this
   // GLCheck(glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);)

    // render data
    // changed it to 6 vertices, for our quad
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // clear the depth buffer, already done in the input()
    //glClear(GL_DEPTH_BUFFER_BIT);
    // we draw using indices
    // Render data
    // 3 indices will draw us the triangle, paramter 2 in this fn()
    glDrawElements(GL_TRIANGLES, gNumIndices, GL_UNSIGNED_SHORT, 0);
                    
    
    // stop using our current graphics pipeline
    // note: this is not necessary if we only have one graphics pipeline
    glUseProgram(0);
}

// Translates a mesh --- updating its model matrix
void meshTranslate(Mesh3D *mesh, float x, float y, float z)
{

    
    // Create a "model-matrix, that begins with a translate-matrix?"
    mesh->mTransform.mModelMatrix = glm::translate(mesh->mTransform.mModelMatrix,
                                    glm::vec3(x,
                                              y, 
                                              z));

}

// rotates a mesh about the axis parameter
void meshRotate(Mesh3D *mesh, float angle, glm::vec3 axis)
{
    mesh->mTransform.mModelMatrix = glm::rotate(mesh->mTransform.mModelMatrix,
                                            glm::radians(angle),
                                            axis);
}

// scales a mesh in a non-uniform way
void meshScale(Mesh3D *mesh, float x, float y, float z) 
{
    mesh->mTransform.mModelMatrix = glm::scale(mesh->mTransform.mModelMatrix,
                                            glm::vec3(x, y, z));
}
/*
    // rotate the object w/o input
    mesh->muRotate -= 0.05f;
    std::cout << "gMesh1.muRotate: " << mesh->muRotate << std::endl;
    // update our model matrix by applying a rotation and a scale-operation
    model = glm::rotate(model, glm::radians(mesh->muRotate), glm::vec3(0.0f, 1.0f, 0.0f));  
    model = glm::scale(model, glm::vec3(mesh->muScale, mesh->muScale, mesh->muScale));

*/

// erorr handling routines

static void GLClearAllErrors()
{
    // the glGetError() will reset the error flag after returning it,
    // essentially 'popping' it off(i.e. its clearing all the error-flags)
    while (glGetError() != GL_NO_ERROR) {
        ; // null statement
    }

}

// returns true if we have an error
static bool GLCheckErrorStatus(const char *function, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL error: "  << error 
                << "\tLine: " << line
                << "\tfunction: " << function << std::endl; 
        return true;
    }
    return false;

}

// error macro
#define GLCheck(x) \
            do { \
                GLClearAllErrors(); \
                x; \
                GLCheckErrorStatus(#x, __LINE__); \
            } while (false);

// Read in shader files.
std::string loadShaderAsString(const std::string &filename)
{
    std::string result = "";

    std::string line = "";
    std::ifstream myFile(filename.c_str());
    
    if (myFile.is_open()) {
        while (std::getline(myFile, line)) {
            result += line + "\n";
        }
        myFile.close();
    }

    return result;
}
    

GLuint compileShader(GLuint type, const std::string &source)
{
    // compile shaders
    GLuint shaderObject = 0;

    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char *src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);
    
    // retrieve the result of our compilation
    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char *errorMessages = new char[length];
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (type == GL_VERTEX_SHADER) {
            std::cout << "error: GL_VERTEX_SHADER compilation failed!\n"
                    << errorMessages << "\n";
        } else if (type == GL_FRAGMENT_SHADER) {
            std::cout << "error: GL_FRAGMENT_SHADER compilation failed!\n" 
                    << errorMessages <<"\n";
        }

        // reclaim our memory
        delete[] errorMessages;

        // delete our broken shader
        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

GLuint createShaderProgram(const std::string &vertexShaderSource,
                        const std::string &fragmentShaderSource)
{
    // create a new program object
    GLuint programObject = glCreateProgram();

    // compile our shaders
    GLuint myVertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = compileShader(GL_FRAGMENT_SHADER, 
                                             fragmentShaderSource);

    // Link our two shader program together.
    // Could think of this like two source files being linked into one executable.
    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);
    
    // validate our program
    glValidateProgram(programObject);

    // once our final program object has been created, we can detach
    // and then delete our individual shaders.
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);
    // delete the individual shaders once we are done
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject; 
}

void createGraphicsPipeLine()
{
    // Read in the shader files.
    std::string vertexShaderSource = loadShaderAsString("../shaders/vert.glsl");
    std::string fragmentShaderSource = loadShaderAsString("../shaders/frag.glsl");

    gApp.mGraphicsPipeLineShaderProgram = createShaderProgram(vertexShaderSource,
                                                    fragmentShaderSource);

}

void input() 
{
    //DataVertexSpecification Store the currunt mouse position
    static int mouseX = gApp.mScreenWidth / 2;
    static int mouseY = gApp.mScreenHeight / 2;

    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    
    // Handles events on queue
    while (SDL_PollEvent(&e) != 0) {
        // If users posts an event to quit;
        // An example is clicking the 'x' in the corner of the window.
        if (e.type == SDL_QUIT) {
            std::cout << "Goodbye, leaving main application loop"
                    << std::endl;
            gApp.mQuit = true;
        } 

        if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gApp.mCamera.MouseLook(mouseX, mouseY);
        }

    }
    
    // todo: use some other key to move our object
    // guOffset

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    float speed = 0.003f;
    if (state[SDL_SCANCODE_UP]) {
        gApp.mCamera.MoveForward(speed);
    }

    if (state[SDL_SCANCODE_DOWN]) {
        gApp.mCamera.MoveBackward(speed);
       //guOffset -= 0.01f;
        //std::cout << "guOffset: " << guOffset << std::endl;
    }

    if (state[SDL_SCANCODE_LEFT]) {
        gApp.mCamera.MoveLeft(speed);
        
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        gApp.mCamera.MoveRight(speed);
    }

    if (state[SDL_SCANCODE_ESCAPE]) {
        gApp.mQuit = true;
    }

}

void mainLoop()
{
    // starts mouse in the middle
    SDL_WarpMouseInWindow(gApp.mGraphicsApplicationWindow, gApp.mScreenWidth / 2,
                        gApp.mScreenHeight / 2);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gApp.mQuit) {
        // handle input
        input();

    // I enabled the depth test.
        // disable depth test and face culling
     //   glDisable(GL_DEPTH_TEST);
        glEnable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);

        // initialize clear color
        // this is the background of the screen.
        glViewport(0, 0, gApp.mScreenWidth, gApp.mScreenHeight);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        // clear color buffer and depth buffer
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        static float rotate = 0.05f;
        meshRotate(&gMesh1, rotate, glm::vec3(1.0f, 0.0f, 0.0f));
//        meshRotate(&gMesh2, -rotate, glm::vec3(0.0f, 1.0f, 0.0f));

        // update our meshes before drawing

        // Draw calls in OpenGL
        // When we draw in OpenGL, this activates the graphics pipeline,
        // i.e. when we use glDrawElements or glDrawArrays, the pipeline that
        //      is utilized is whatever glUseProgram is currently binded.
        // 
        meshUpdate(&gMesh1);
        meshDraw(&gMesh1);

     //   meshUpdate(&gMesh2);
     //   meshDraw(&gMesh2);

        // update the screen of our specified windnow
        SDL_GL_SwapWindow(gApp.mGraphicsApplicationWindow);
    }

}

void cleanUp()
{
    // Destroy our SDL2 Window
    SDL_DestroyWindow(gApp.mGraphicsApplicationWindow);
    gApp.mGraphicsApplicationWindow = nullptr;

    meshDelete(&gMesh1);
    // Delete our OpenGL objects

    // delete our graphics pipeline
    glDeleteProgram(gApp.mGraphicsPipeLineShaderProgram);

    // Quit SDL subsystems
    SDL_Quit();
}

// start of main
int main(int argc, char *argv[])
{
    // 1. set up the graphics program, here we set up entities that 
    //    live on the CPU
    initializeProgram(&gApp);
    
    // Setup our camera
    gApp.mCamera.setProjectionMatrix(glm::radians(45.0f),
                    (float)gApp.mScreenWidth / (float)gApp.mScreenHeight,
                    0.1f,
                    10.0f);

    // 2. set up our geometry, this set up is on a per object basis,
    //    these entities live on the GPU?
    meshCreate(&gMesh1);
    meshTranslate(&gMesh1, 0.0f, 0.0f, -3.0f);
    meshScale(&gMesh1, 1.0f, 1.0f, 1.0f);

 //   meshCreate(&gMesh2);
  //  meshTranslate(&gMesh2, 0.0f, 0.0f, -4.0f);
  //  meshScale(&gMesh2, 1.0f, 2.0f, 1.0f);

    // 3. create our graphics pipeline,
    //    at a minimum this means create the
    //    vertex and fragment shader. Here we set up how we processes the
    //    objects we created.
    createGraphicsPipeLine();

    // 3.5 for each of our meshes, set them to a pipeline
    meshSetPipeline(&gMesh1, gApp.mGraphicsPipeLineShaderProgram);
//    meshSetPipeline(&gMesh2, gApp.mGraphicsPipeLineShaderProgram);

    // 4. call the main application loop
    mainLoop();

    // 5. call the cleanup fn() when our program ends
    cleanUp();
   

    return 0;
}
