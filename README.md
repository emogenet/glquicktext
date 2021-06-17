# glquicktext

    glQuickText is an OpenGL text renderer with the following attributes:
        - C++14
        - "traditional" OpenGL
        - header-only (one .h file )
        - has almost no dependencies (C++ std lib, C std lib)
        - fixed font: monospaced, courier-like
        - renders glyphs as individual, transparent, textured quads
        - uses a densely packed 256x256 lum+alpha texture (inlined in .h file)
        - computes the bounding box of a rendered string
        - straightforward printf-like API:

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

        hello.cpp   :   very basic use of glQuickText
        demo.cpp    :   shows off more advanced uses of glQuickText

        examples both require GLFW but glQuickText does not

