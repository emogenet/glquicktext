/*

    glQuickText demo

    requires GLFW : www.glfw.org

    compile with: c++ -o demo -I. demo.cpp -lglfw -lGLU -lGL -lm

    written 2021 by emogenet@gmail.com
    in the public domain

 */

#include <chrono>
#include <math.h>
#include <GLFW/glfw3.h>
#include <glQuickText.h>

static auto g_w = 0;
static auto g_h = 0;
static auto g_run = true;
static auto g_speed = 0.1;
static auto g_boxes = false;

// error callback for GLFW
static auto errCB(
    int err,
    const char *errMsg
) {
    fprintf(
        stderr,
        "glfw err %d (%s)\n",
        err,
        errMsg
    );
}

// key pressed callback for GLFW
static auto keyCB(
    GLFWwindow *w,
    int key,
    int scanCode,
    int action,
    int mods
) {
    if(GLFW_KEY_B==key && GLFW_PRESS==action) {
        g_boxes = !g_boxes;
    }
    if(GLFW_KEY_ESCAPE==key && GLFW_PRESS==action) {
        glfwSetWindowShouldClose(w, GLFW_TRUE);
    }
    if(GLFW_KEY_SPACE==key && GLFW_PRESS==action) {
        g_run = !g_run;
    }
    if(GLFW_KEY_EQUAL==key && GLFW_PRESS==action) {
        g_speed *= 1.1;
    }
    if(GLFW_KEY_MINUS==key && GLFW_PRESS==action) {
        g_speed /= 1.1;
    }
}

// window resized callback for GLFW
static auto winSizeCB(
    GLFWwindow *win,
    int w,
    int h
) {

    g_w = w;
    g_h = h;
    glfwMakeContextCurrent(win);

    glViewport(0, 0, g_w, g_h);
    glClearColor(0.2, 0.2, 0.2, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, double(g_w), 0.0, double(g_h), -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// render fixed reticle at center of window
static auto renderReticle(
    double cx,
    double cy
) {
    glPushMatrix();
        glTranslatef(cx,  cy, 0.0f);
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        glBegin(GL_LINES);
            glVertex2i(-10000, 0);
            glVertex2i( 10000, 0);
            glVertex2i(0, -10000);
            glVertex2i(0,  10000);
        glEnd();
    glPopMatrix();
}

// render keyboard help text in lower left corner
static auto renderHelpText() {
    glColor4ub(0x00, 0x00, 0x00, 0xFF);
    glQuickText::printfAt(
         10.0f,
        125.0f,
        0.0,
        1.0,
        "%s",
        "  b  : toggle char boxes\n"
        "  +  : speed up animation\n"
        "  -  : speed down animation\n"
        "SPACE: pause/restart animation\n"
        "ESC  : exit\n"
    );
}

// render current frame rate in upper left corner
static auto renderFrameRate(
    double frameRate
) {
    glColor4ub(0xFF, 0x4F, 0x4F, 0xFF);
    glQuickText::printfAt(
        10.0f,
        (g_h - 25.0),
        0.0,
        1.0,
        "FPS: %6.2f",
        frameRate
    );
}

// render animated text box
static auto renderAnimatedText(
    double cx,
    double cy
) {

    // time-driven parameter
    static auto mu = 0.75;
    if(g_run) {
        mu += (0.01 * g_speed);
    }

    // various animated parameters
    auto scale = (2.0 + sin(mu));
    auto angle = 45.0 * sin(2.0*mu);
    auto r = uint8_t((1 + sin(1234.0 + mu)) * 0.5 * 255.0);
    auto g = uint8_t((1 + sin(4321.0 + mu)) * 0.5 * 255.0);
    auto b = uint8_t((1 + sin(1111.0 + mu)) * 0.5 * 255.0);

    // main text to feed to printf
    static auto str =
        "glQuickText is a C++-14, printf-like,\n"
        "minimalist, single-header, fixed-font\n"
        "OpenGL text renderer\n"
        "rgb = (0x%02x 0x%02x 0x%02x)\n"
        "scale = %.2f\n"
        "speed = %.2f\n"
        "angle = %.2f%s"
        ;

    // compute text box
    double box[4];
    glQuickText::stringBox(
        box,
        scale,
        str,
        r,
        g,
        b,
        scale,
        g_run ? g_speed : 0.0,
        angle,
        (angle<0.0 ? "\nangle is negative" : "")
    );

    // window reticle is below everything else
    renderReticle(cx, cy);

    auto tw = (box[2] - box[0]);
    auto th = (box[3] - box[1]);
    glPushMatrix();

        // rotate around window center
        glTranslatef( cx,  cy, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glTranslatef(-cx, -cy, 0.0f);

        // center text to window center
        glTranslatef(cx - tw/2.0, cy + th/2.0, 0.0f);

        // draw transparent text box
        glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColor4ub(0x2F, 0x3F, 0x4F, 0xCF);
            glBegin(GL_QUADS);
                glVertex3d(box[0], box[1], 0.0);
                glVertex3d(box[2], box[1], 0.0);
                glVertex3d(box[2], box[3], 0.0);
                glVertex3d(box[0], box[3], 0.0);
            glEnd();
        glDisable(GL_BLEND);

        // draw a red reticle at text string geometric origin
        glColor4ub(0xFF, 0x2F, 0x2F, 0xFF);
        glBegin(GL_LINES);
            glVertex2i(-10000, 0);
            glVertex2i( 10000, 0);
            glVertex2i(0, -10000);
            glVertex2i(0,  10000);
        glEnd();

        // draw text string itself
        glColor4ub(r, g, b, 0xFF);
        glQuickText::printfAt(
            0.0f,
            0.0f,
            0.0,
            scale,
            str,
            r,
            g,
            b,
            scale,
            g_run ? g_speed : 0.0,
            angle,
            angle<0.0 ? "\nangle is negative" : ""
        );

    glPopMatrix();
}

// overlay a transparent text watermark on everything
static auto renderWaterMark(
    double cx,
    double cy
) {

    // compute watermark box with scale = 1.0
    double box[4];
    auto str = "glQuickText";
    glQuickText::stringBox(
        box,
        1.0,
        "%s",
        str
    );

    // derive scale so that box quasi-fills window horizontally
    auto boxLeft = box[0];
    auto boxRight = box[2];
    auto boxWidth = (boxRight - boxLeft);
    auto scale = 0.98 * (g_w / double(boxWidth));

    // re-compute box with derived scale
    glQuickText::stringBox(
        box,
        scale,
        "%s",
        str
    );

    // redo centering with scaled box
    auto boxTop = box[3];
    auto boxBottom = box[1];
    auto boxHeight = (boxTop - boxBottom);

    boxLeft = box[0];
    boxRight = box[2];
    boxWidth = (boxRight - boxLeft);

    // draw transparent (alpha=0.5) watermak text
    glColor4ub(0xFF, 0xFF, 0xFF, 0x7F);
    glQuickText::printfAt(
        (g_w - boxWidth)/2.0,
        -boxBottom + (g_h/2.0) - (boxHeight/2.0),
        0.0,
        scale,
        "%s",
        str
    );
}

// render everything
static auto render(
    GLFWwindow *w
) {

    // window center
    auto cx = (g_w / 2.0);
    auto cy = (g_h / 2.0);

    // compute frame rate every 20 frames
    static auto frameCount = 0;
    static auto frameRate = -1.0;
    static auto startTime = std::chrono::steady_clock::now();

    ++frameCount;
    if(20==frameCount) {
        auto endTime = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(endTime-startTime).count();
        if(0.0<elapsed) {
            frameRate = (frameCount / elapsed);
        }
        startTime = endTime;
        frameCount = 0;
    }

    // clear window
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT);

    // render normal elements
    renderHelpText();
    renderReticle(cx, cy);
    renderFrameRate(frameRate);

    // render elements with potentially debug boxes on
    glQuickText::setDebugBoxes(g_boxes);
        renderAnimatedText(cx, cy);
        renderWaterMark(cx, cy);
    glQuickText::setDebugBoxes(false);

    // show drawing
    glFlush();
    glfwSwapBuffers(w);
}

// fairly standard GLFW event loop
static auto evLoop(
    GLFWwindow *w
) {
    while(1) {
        if(glfwWindowShouldClose(w)) {
            break;
        }
        render(w);
        glfwPollEvents();
    }
}

// fairly standard GLFW harness
static auto show() {

    glfwSetErrorCallback(errCB);
    if(GLFW_FALSE==glfwInit()) {
        return;
    }

    constexpr auto winW = 1800;
    constexpr auto winH = 1200;
    auto win = glfwCreateWindow(
        winW,
        winH,
        "glQuickText demo",
        NULL,
        NULL
    );
    if(NULL!=win) {

        glfwSetFramebufferSizeCallback(win, winSizeCB);
        glfwSetKeyCallback(win, keyCB);

        int w;
        int h;
        glfwGetWindowSize(win, &w, &h);
        winSizeCB(win, w, h);
        evLoop(win);

    }
    glfwTerminate();
}

// entry
int main(
    int,
    char *[]
) {
    // run demo
    show();

    // make some noise before leaving
    printf("exited cleanly\n");
    return 0;
}

