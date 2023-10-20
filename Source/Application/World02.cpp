#include "World02.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

//#define INTERLEAVE
#define INDEX
namespace nc
{
    bool World02::Initialize()
    {
        // shaders
        const char* vertexShader =
            "#version 430\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "layout (location=0) out vec3 ocolor;"
            "void main() {"
            "  ocolor = color;"
            "  gl_Position = vec4(position, 1.0);"
            "}";



        const char* fragmentShader =
            "#version 430\n"
            "layout (location=0) in vec3 color;"
            "out vec4 ocolor;"
            "void main() {"
            "  ocolor = vec4(color, 1);"
            "}";
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, NULL);
        glCompileShader(vs);
       
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, NULL);
        glCompileShader(fs);
        

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glUseProgram(program);

#ifdef INTERLEAVE
        // vertex data
        const float vertexData[] = {
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
        };
       
        GLuint m_vbo;
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, m_vbo, 0, 6 * sizeof(GLfloat));

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glVertexAttribBinding(1, 0);
#elif defined(INDEX)
        // vertex data
        const float vertexData[] = {
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
    };
        GLuint indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };
        // Vertex buffer object
        GLuint m_vbo;
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        // Index buffer object
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Vertex array object
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, m_vbo, 0, 6 * sizeof(GLfloat));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glVertexAttribBinding(1, 0);
#else
        float colorData[] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
        float positionData[] = {
           -0.8f, -0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.0f,  0.8f, 0.0f
        };
        
#endif
        return true;
    }

    void World02::Shutdown()
    {
    }

    void World02::Update(float dt)
    {
        m_angle += 90 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;
        m_time += dt;
    }

    void World02::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.SetColor(0, 0, 0, 0);
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);

#ifdef INDEX
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif
        // post-render
        renderer.EndFrame();
    }
}
