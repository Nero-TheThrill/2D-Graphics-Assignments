#include "objects.hpp"

int main(int argc, char** argv)
{
    try
    {
        App app(argc, argv, "A2 - Objects and model transforms");

        Program program(
            std::string(SHADER_DIR) + "/scene.vert",
            std::string(SHADER_DIR) + "/scene.frag"
        );

        Mesh house;
        createMesh(house, houseVertices(), houseIndices());

        auto objects = makeObjects(house);

        size_t selected = 0;
        bool rts = false;

        while (app.begin())
        {
            glViewport(0, 0, app.width, app.height);
            glUseProgram(program.id);

            for (int i = 0; i < 3; ++i)
            {
                if (app.pressed(GLFW_KEY_1 + i) && size_t(i) < objects.size())
                    selected = size_t(i);
            }

            if (app.pressed(GLFW_KEY_O))
                rts = !rts;

            if (!objects.empty())
                updateObject(objects[selected], app);

            for (const auto& o : objects)
                drawObject(o, program.id, rts);

            if (app.pressed(GLFW_KEY_O))
                std::printf("Matrix order: %s\n", rts ? "R*T*S" : "T*R*S");

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