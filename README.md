# glquicktext
    glQuickText is an OpenGL text renderer with the following attributes:
        - C++14
        - "traditional" OpenGL
        - header-only (one .h file )
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

