#include "camera.hpp"

int main(int argc, char** argv)
{
    try
    {
        App app(argc, argv, "A3 - World and camera");

        Program program(
            std::string(SHADER_DIR) + "/scene.vert",
            std::string(SHADER_DIR) + "/scene.frag"
        );

        Mesh house;
        createMesh(house, houseVertices(), houseIndices());

        auto world = makeWorld(house);
        Camera camera;

        while (app.begin())
        {
            // TODO A3.7: update viewport with framebuffer dimensions and update camera state.
            // app.width / app.height are refreshed every frame, including after resize.

            glUseProgram(program.id);
            uploadCamera(program.id, camera, app.width, app.height);

            constexpr bool enableClickPlacement = false;

            if (enableClickPlacement && app.leftClick && app.width > 0 && app.height > 0)
            {
                int w, h;
                double x, y;

                glfwGetWindowSize(app.window, &w, &h);
                glfwGetCursorPos(app.window, &x, &y);

                if (w > 0 && h > 0)
                    world.push_back({
                        &house,
                        cursorToWorld(camera, x, y, w, h, float(app.width) / app.height),
                        0,
                        {.7f, .7f}
                    });
            }

            for (const auto& o : world)
                drawObject(o, program.id);

            app.end();
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        std::fprintf(stderr, "%s\n", e.what());
        return 1;
    }
}