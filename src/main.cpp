#include "GLGEGraphic/GLGEGraphic.h"

int main()
{
    Window win("Hello World!", uivec2(600,600));

    while (!win.isClosingRequested()) {
        glge_Graphic_MainTick();
    }
}