/*
    glQuickText hello world

    requires GLFW : www.glfw.org

    compile with: c++ -o hello -I. hello.cpp -lglfw -lGLU -lGL

 */

#include <GLFW/glfw3.h>
#include <glQuickText.h>

// entry
int main(
    int,
    char *[]
) {

    // init GLFW
    if(GLFW_FALSE==glfwInit()) {
        fprintf(stderr, "can't initialize GLFW\n");
        exit(1);
    }

    // open window
    auto w = 320;
    auto h = 100;
    auto win = glfwCreateWindow(
        w,
        h,
        "glQuickText hello world",
        NULL,
        NULL
    );
    glfwMakeContextCurrent(win);

    // configure OpenGL context
    glViewport(0, 0, w, h);
    glClearColor(0.2, 0.2, 0.2, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, double(w), 0.0, double(h), -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // render loop
    while(1) {

        // see if it's time to die
        if(glfwWindowShouldClose(win)) {
            break;
        }

        // clear window
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT);

        // render text
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        glQuickText::printfAt(
            50.0f,
            45.0f,
            0.0,
            1.5,
            "%s",
            "Hello, World!"
        );

        // show frame
        glFlush();
        glfwSwapBuffers(win);

        // check event queue
        glfwPollEvents();
    }

    // clean up
    glfwTerminate();

    // make some noise
    printf("exited cleanly\n");
    return 0;
}

