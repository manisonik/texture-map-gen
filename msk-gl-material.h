#pragma once

namespace TexGen {
    class TgMaterial
    {
    public:
        TgMaterial();
        ~TgMaterial();

    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_shininess;
    };
}

