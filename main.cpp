#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std; 

const string VERTEX_SHADER_PATH = "shaders/simple_vshader.glsl";
const string FRAGMENT_SHADER_PATH = "shaders/simple_fragshader.glsl";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);

string loadShader (const string &path);

void checkShaderCompile (int shader, string name);
void checkShaderLink (int program, string name);

int main (int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }    

    string vertexShaderSource = loadShader (VERTEX_SHADER_PATH);
    string fragmentShaderSource = loadShader (FRAGMENT_SHADER_PATH); 

    const GLchar *vertexSource_glstr = vertexShaderSource.c_str();
    const GLchar *fragmentSource_glstr = fragmentShaderSource.c_str();
    
    // tell opengl to use our shader
    unsigned int vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, &vertexSource_glstr, NULL);
    glCompileShader (vertexShader);
    checkShaderCompile (vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShader, 1, &fragmentSource_glstr, NULL);
    glCompileShader (fragmentShader);  
    checkShaderCompile (fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);
    glLinkProgram (shaderProgram);
    checkShaderLink (shaderProgram, "PROG1");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), 
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered 
    // VBO as the vertex attribute's bound vertex buffer object so afterwards 
    // we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally 
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs 
    // (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    while(!glfwWindowShouldClose(window)) {
        processInput (window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

string loadShader (const string &path) {
    // get vertex shader into string
    ifstream shaderFile(path);
    ostringstream buffer;
    buffer << shaderFile.rdbuf();
    string bufferStr = buffer.str();
    shaderFile.close();

    return bufferStr;
}

void checkShaderCompile (int shader, string name) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf (stderr, "ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n",
            name.c_str(), infoLog);
    }  
}

void checkShaderLink (int program, string name) {
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf (stderr, "ERROR::SHADER::%s::LINKING_FAILED\n%s\n",
            name.c_str(), infoLog);
    }
}