#pragma once
#include "Component.hpp"
#include <vector>
#include <string>

namespace KuangyeEngine
{

class Audio : public Component
{
    COMPONENT_DECLARATION(Component, Audio, 32);
    Audio(Entity *entity);

    void play(const std::string &file);
};
} // namespace KuangyeEngine