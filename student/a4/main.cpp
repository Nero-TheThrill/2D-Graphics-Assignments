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

        std::vector<Sprite> sprites;

        // TODO A4.8: create two static sprites sharing quad and atlas.
        // Hint: Add Sprite entries with sprites.push_back or vector initialization. Use &quad for the Object mesh and &atlas for the texture.
        // Use different tile numbers (0..7) and positions visible to the initial camera.

        Sprite wrapDemo{{&quad, {0, -1.5f}, 0, {2, 1}}, &checker, 0};

        while (app.begin())
        {
            glViewport(0, 0, app.width, app.height);
            updateCamera(camera, app);

            glUseProgram(program.id);
            uploadCamera(program.id, camera, app.width, app.height);

            // TODO A4.9: F toggles filtering for both textures; V toggles checker wrapping.
            // Hint: Use app.pressed(GLFW_KEY_F) / app.pressed(GLFW_KEY_V) to toggle the bool values, then call textureSettings.
            // Apply the new settings with textureSettings; keep atlas wrapping clamped.
            // Use pressed for toggles; down would toggle every frame.
            // Provided: restore color-only mode before drawing the houses.

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
