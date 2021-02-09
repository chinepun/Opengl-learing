#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include <vector>
#include <iostream>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 155.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // // glad: load all OpenGL function pointers
    // // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // // configure global opengl state
    // // -----------------------------
    glEnable(GL_DEPTH_TEST );

    // // // build and compile shaders
    // // // -------------------------
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float cubeVertices[] = {
    //     // positions         
    //     -0.5f, -0.5f, -0.5f, 
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f,  0.5f, -0.5f,  
    //     -0.5f,  0.5f, -0.5f, 
    //     -0.5f, -0.5f, -0.5f, 

    //     -0.5f, -0.5f,  0.5f, 
    //      0.5f, -0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //     -0.5f,  0.5f,  0.5f, 
    //     -0.5f, -0.5f,  0.5f, 

    //     -0.5f,  0.5f,  0.5f, 
    //     -0.5f,  0.5f, -0.5f, 
    //     -0.5f, -0.5f, -0.5f, 
    //     -0.5f, -0.5f, -0.5f, 
    //     -0.5f, -0.5f,  0.5f, 
    //     -0.5f,  0.5f,  0.5f, 

    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  

    //     -0.5f, -0.5f, -0.5f, 
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f,  0.5f,  
    //      0.5f, -0.5f,  0.5f,  
    //     -0.5f, -0.5f,  0.5f, 
    //     -0.5f, -0.5f, -0.5f, 

    //     -0.5f,  0.5f, -0.5f, 
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //     -0.5f,  0.5f,  0.5f, 
    //     -0.5f,  0.5f, -0.5f, 
    // };
// float quadVertices[] = {
// // positions        // colors
//         -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//          0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//         -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

//         -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//          0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//          0.05f,  0.05f,  0.0f, 1.0f, 1.0f
// };//glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
    Shader asteroidShader("../github/10.3.asteroids.vs", "../github/10.3.asteroids.fs");
    Shader planetShader("../github/10.3.planet.vs", "../github/10.3.planet.fs");


Model rock("../model/rock/rock.obj"), planet("../model/planet/planet.obj");
// glm::vec2 translations[100];
// unsigned int index{0};
// constexpr float offset_trans{0.1};
// for (int y = -10; y < 10; y += 2)
//     for (int x = -10; x < 10; x += 2)
//         {
//             glm::vec2 translation;
//             translation.x = static_cast<float>(x / 10.0f + offset_trans);
//             translation.y = static_cast<float>(y / 10.0f + offset_trans);
//             translations[index++] = translation;
//         }
// unsigned int instanceVbo;
// glGenBuffers(1, &instanceVbo);
// glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
//    Shader normalShader("../github/9.3.normal_visualization.vs", "../github/9.3.normal_visualization.fs", "../github/9.3.normal_visualization.gs");
//stbi_set_flip_vertically_on_load(true);
//Model nanosuit("../model/backpack.obj");
// unsigned int quadVao, quadVbo;
// glGenVertexArrays(1, &quadVao);
// glGenBuffers(1, &quadVbo);
// glBindVertexArray(quadVao);
// glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
// glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
// // positions
// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// glEnableVertexAttribArray(0);
// // colors
// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
// glEnableVertexAttribArray(1);
// // instance data
// glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
// glEnableVertexAttribArray(2);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
// glVertexAttribDivisor(2, 1);

    // cube VAO
    // unsigned int cubeVAO, cubeVBO;
    // glGenVertexArrays(1, &cubeVAO);
    // glGenBuffers(1, &cubeVBO);
    // glBindVertexArray(cubeVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // configure a uniform buffer object
    // ---------------------------------
    // // first. We get the relevant block indices
    // unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.programID, "Matrices");
    // unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.programID, "Matrices");
    // unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.programID, "Matrices");
    // unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.programID, "Matrices");
    // // then we link each shader's uniform block to this uniform binding point
    // glUniformBlockBinding(shaderRed.programID, uniformBlockIndexRed, 0);
    // glUniformBlockBinding(shaderGreen.programID, uniformBlockIndexGreen, 0);
    // glUniformBlockBinding(shaderBlue.programID, uniformBlockIndexBlue, 0);
    // glUniformBlockBinding(shaderYellow.programID, uniformBlockIndexYellow, 0);
    // Now actually create the buffer
    // unsigned int uboMatrices;
    // glGenBuffers(1, &uboMatrices);
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // // define the range of the buffer that links to a uniform binding point
    // glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    // // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    // glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
    unsigned int amount = 100000;
    std::vector<glm::mat4> modelMatrices(amount);
    srand(glfwGetTime()); // initialize random seed	
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }
    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

// shader.use();
// for (int i = 0; i < 100; i++)
//     shader.setVec2(("offsets["+std::to_string(i)+"]"), translations[i]);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // configure transformation matrices

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        asteroidShader.use();
        asteroidShader.setMat4("projection", projection);
        asteroidShader.setMat4("view", view);
        planetShader.use();
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        
        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        planetShader.setMat4("model", model);
        planet.draw(planetShader);

        // draw meteorites
        asteroidShader.use();
        asteroidShader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        for (unsigned int i = 0; i < rock.meshes.size(); i++)
        {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }





       // // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
        // glm::mat4 view = camera.GetViewMatrix();
        // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        // glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        // glBindBuffer(GL_UNIFORM_BUFFER, 0);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &cubeVAO);
    // glDeleteBuffers(1, &cubeVBO);
    // glDeleteBuffers(1, &quadVao);
    // glDeleteBuffers(1, &quadVbo);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

