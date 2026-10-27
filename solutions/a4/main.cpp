#include "texture.hpp"

int main(int argc, char** argv)
{
    try
    {
        App app(argc, argv, "A4 - Textured world");

        Program program(
            std::string(SHADER_DIR) + "/scene.vert",
            std::string(SHADER_DIR) + "/scene.frag"
        );

        Mesh house, quad;
        createMesh(house, houseVertices(), houseIndices());
        createTexturedQuad(quad);

        auto world = makeWorld(house);
        Camera camera;

        Texture atlas, checker;
        createTexture(atlas, loadPng(std::string(COURSE_ROOT) + "/assets/atlas.png"));
        createTexture(checker, loadPng(std::string(COURSE_ROOT) + "/assets/checker.png"));

        bool linear = false, repeat = true;
        textureSettings(atlas, linear, false);
        textureSettings(checker, linear, repeat);

        std::vector<Sprite> sprites{
            {{&quad, {-1.2f, 0}, 0, {1.4f, 1.4f}}, &atlas, 0},
            {{&quad, {1.2f, 0}, 0, {1.4f, 1.4f}}, &atlas, 5}
        };

        Sprite wrapDemo{{&quad, {0, -1.2f}, 0, {2, .8f}}, &checker, 0};

        while (app.begin())
        {
            glViewport(0, 0, app.width, app.height);
            updateCamera(camera, app);

            glUseProgram(program.id);
            uploadCamera(program.id, camera, app.width, app.height);

            if (app.pressed(GLFW_KEY_F)) linear = !linear;
            if (app.pressed(GLFW_KEY_V)) repeat = !repeat;
            if (app.pressed(GLFW_KEY_F) || app.pressed(GLFW_KEY_V)) {
                textureSettings(atlas, linear, false);
                textureSettings(checker, linear, repeat);
            }
            // Restore color-only mode before drawing houses on every frame.
            glProgramUniform1i(program.id, glGetUniformLocation(program.id, "uTextured"), 0);

            for (const auto& o : world)
                drawObject(o, program.id);

            for (const auto& s : sprites)
                drawSprite(s, program.id, atlasRect(s.tile, 4, 2, atlas.width, atlas.height));

            drawSprite(wrapDemo, program.id, {{0, 0}, {3, 3}});

            if (app.pressed(GLFW_KEY_F) || app.pressed(GLFW_KEY_V))
                std::printf(
                    "Filter: %s | checker wrap: %s\n",
                    linear ? "LINEAR" : "NEAREST",
                    repeat ? "REPEAT" : "CLAMP_TO_EDGE"
                );

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
