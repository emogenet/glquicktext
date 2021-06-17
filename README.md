# glquicktext

    glQuickText is an OpenGL text renderer with the following attributes:

        neat:
            - C++14
            - header-only (one .h file)
            - straightforward printf-like API
            - very few dependencies: C++ std lib, C std lib
            - it might even compile on windows, who knows...
            - can compute the bounding box of a rendered string
            - font glyphs are "true" 3D objects and behave well when rendered in 3D environments

        less neat:
            - "traditional" OpenGL code ... uses glVertex a lot
            - therefore: speed is OK but geometry buffers it ain't
            - you don't get to choose the font: you get a monospaced, courier-like and that's that

        good to know:
            - renders glyphs as individual, transparent, textured quads
            - uses a densely packed 256x256 lum+alpha texture (inlined in .h file)
            - in particular, the rendered string behaves reasonably wrt current transform / shading stack

        basic use:

            glQuickText::printfAt(
                xPos,
                yPos,
                zPos,
                textScale,
                "sin(%.5f) = %.5f",
                234.0,
                sin(234.0)
            );

        two examples are provided:

            hello.cpp   :   shows very basic use of glQuickText
            demo.cpp    :   shows more advanced uses of glQuickText

            examples both require GLFW but glQuickText itself does not

        here's what that should look like:

![alt text](https://github.com/emogenet/glquicktext/blob/master/hello.png?raw=true)

