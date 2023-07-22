#pragma once

namespace Manisonik {
    class MMaterial
    {
    public:
        MMaterial();
        ~MMaterial();

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
}

