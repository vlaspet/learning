#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void movements(GLFWwindow *window, int& x, int& y);

//settings
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";
//330 - version
//core - explicitly mentions we're using core profile functionality
//layout (location = 0) sets the location of the input variable
//in vec3 aPos; declares all the input vertex attributes 
//in the vertex shader
//gl_Position - sets the output of the vertex shader 
//we have to assign the position data
//source code stores in a const C string

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
//out vec4 FragColor - defines the final color output
//out declare output values
//vec4 assigns a vec4 to the color output

int main()
{
    glfwInit();
    //initializing GLFW 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint sets hints for the next call to glfwCreateWindow
    //GLFW_CONTEXT_VERSION_MAJOR
    //set the client API version that the created context must be compatible with
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //GLFW_CONTEXT_VERSION_MINOR the same thing as privious only for minor.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //we want to explicitly use the core-profile


    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //gets called each time the window is resized

    //glfwGetProcAddress returns the address of the specified
    //OpenGL core or extension function
    //gladLoadGLLoader - initializing GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //creates the shader with a type GL_VERTEX_SHADER
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //the first argement takes the shader object to compile to
    //the second argument how many strings we're passing as source code
    //the third argument is the actual source code
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if ( !success ) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if ( !success ) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if ( !success ) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f
    // };

    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    //vertex buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //generates a buffer object
    //new
    glGenBuffers(1, &EBO);
    //new
    //bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //binds a buffer object to the current buffer type target
    //GL_ARRAY_BUFFER - type of a vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //copies the previously defined vertex data into the buffer's memory
    //the first argument the type of the buffer we want to copy data into
    //the second argument the size of the data (in bytes) we want 
    //to pass to the buffer
    //the third argument the actual data we want to send
    //the fourth argument how to manage the given data
    //GL_STREAM_DRAW: the data is set only once and used be the GPU 
    //at most a few times
    //GL_STATIC_DRAW: the data is set only once and used many times
    //GL_DYNAMIC_DRAW: the data is changed a lot and used many times

    //new
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    //new

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //the first parameter (layout (location = 0))
    //the second the size of the vertex attribute is a vec3 => 3
    //the third the type of the data which is GL_FLOAT (a vec* in GLSL = float)
    //the fourth if we want the data to be normalized. If we're inputting integer
    //then we've set this to GL_TRUE AND I THINK ITS WILL BE CONVERTED TO FLOAT
    //otherwise GL_FALSE
    //the fifth is of type void* this is the offset of where the position data
    //begins in the buffer. Since the position data is at the start of the data
    //array this value is just 0.
    glEnableVertexAttribArray(0);
    //obtaining the vertex attribute location as its argument

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //we can safely unbind

    glBindVertexArray(0);
    //unbinding the VAO afterwards so other VAO calls won't accidentally 
    //modify this VAO

    //wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //the first to apply it to the front and back of all triangles
    //the second tells us to draw them as lines

    //glfwWindowShouldClose returns GL_TRUE if the window has
    //been instructed to close
    int x = 0, y = 0;
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        movements(window, x, y);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glViewport(x, y, SRC_WIDTH, SRC_HEIGHT);
        //new
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //new
        //the third is a type of the indices
        //the fourth to specify an offset in the EBO or pass in an index
        //array, but that is when you're not using element buffer objects

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //the first is mode
        //the second is the starting index of the vertex array
        //the third how many vertices we want to draw

        glfwSwapBuffers(window);
        //swaps the front and backs buffers of the specified
        //window's double-buffer
        glfwPollEvents();
        //checks if any events are triggered (like keyboard input or
        //mouse movement events)  
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //new
    glDeleteBuffers(1, &EBO);
    //new
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    //cleans up all the resources and properly exit the application
    return 0;
}

void processInput(GLFWwindow *window) {
    if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, true);
    }
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //the first two parameters set the location of the lower
    //left corner of the window
}  

void movements(GLFWwindow *window, int& x, int& y) {
    int speed = 2;
    if ( glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) {
        x -= speed;
    }
    if ( glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ) {
        x += speed;
    }
    if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ) {
        y += speed;
    }
    if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ) {
        y -= speed;
    }
}