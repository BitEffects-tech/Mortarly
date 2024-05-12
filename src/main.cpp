#include "gui/GUI.h"

// #include <OpenGl_GraphicDriver.hxx>
// #include <AIS_InteractiveContext.hxx>
// #include <BRepPrimAPI_MakeBox.hxx>

// #include <V3d_View.hxx>
// #include <AIS_Shape.hxx>

int main(void) {
    try
    {
        initDefaultExceptionHandler();
        
        if (!glfwInit()) {
            // Initialization failed
            return -1;
        }

        // Create a windowed mode window and its OpenGL context
        GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window) {
            // Window or OpenGL context creation failed
            glfwTerminate();
            return -1;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Initialize Open CASCADE Graphic Driver
        // Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
        // Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);
        // Handle(V3d_Viewer) viewer = new V3d_Viewer(graphicDriver);
        // Handle(V3d_View) view = viewer->CreateView();
        // Handle(AIS_InteractiveContext) context = new AIS_InteractiveContext(viewer);

        // // Create a simple box
        // TopoDS_Shape box = BRepPrimAPI_MakeBox(10., 20., 30.).Shape();
        // Handle(AIS_Shape) aisBox = new AIS_Shape(box);
        // // Set display mode to shaded to see the colors
        // aisBox->SetDisplayMode(AIS_Shaded);

        // // Set color
        // Quantity_Color aColor(1.0, 0.0, 0.0, Quantity_TOC_RGB);
        // aisBox->SetColor(aColor);

        // // Set material
        // aisBox->SetMaterial(Graphic3d_NOM_GOLD);

        // // Ensure the shape is updated
        // context->Display(aisBox, Standard_True);
        // context->UpdateCurrentViewer();

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            // Poll for and process events
            glfwPollEvents();

            // Clear the screen to black
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Main Window
            gui::begin(gui::LayoutDirection::Vertical, 1, 1);
                // Menu Bar
                gui::begin(gui::LayoutDirection::Horizontal, 1);
                    gui::begin(gui::LayoutDirection::Horizontal);
                        if (gui::onClick()) /* do something */{}
                        gui::text("File");
                    gui::end();
                    gui::begin(gui::LayoutDirection::Horizontal);
                        if (gui::onClick()) /* do something */{}
                        gui::text("Edit");
                    gui::end();
                    gui::begin(gui::LayoutDirection::Horizontal);
                        if (gui::onClick()) /* do something */{}
                        gui::text("Help");
                    gui::end();
                gui::end();

                // Ribbon
                gui::begin(gui::LayoutDirection::Horizontal, 1);
                gui::end();

                // Main Content Area
                gui::begin(gui::LayoutDirection::Vertical, 1, 1);
                gui::end();

                // Status Bar
                gui::begin(gui::LayoutDirection::Horizontal, 1);
                    gui::begin(gui::LayoutDirection::Horizontal);
                        gui::text("status");
                    gui::end();

                    gui::begin(gui::LayoutDirection::Horizontal, 1); gui::end();

                    gui::begin(gui::LayoutDirection::Horizontal);
                    gui::end();
                gui::end();
            gui::end();

            // view->Camera()->SetEye(gp_Pnt(50, 50, 50));
            // view->Camera()->SetCenter(gp_Pnt(0, 0, 0));
            // view->Camera()->SetUp(gp_Dir(0, 0, 1));
            // view->Camera()->SetScale(1);

            // // Redraw the view
            // view->Redraw();

            // Swap front and back buffers
            glfwSwapBuffers(window);
        }

        glfwTerminate();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
        printStackTrace();
        
        return 1;
    }

    return 0;
}
