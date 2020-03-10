#ifndef IMGUI_SFML_H
#define IMGUI_SFML_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "imgui-SFML_export.h"
#include "kage2dutil/atlas.h"

namespace sf
{
    class Event;
    class RenderTarget;
    class RenderWindow;
    class Sprite;
    class Texture;
    class Window;
}

namespace ImGui
{
    namespace SFML
    {
        IMGUI_SFML_API void Init(sf::RenderWindow& window, bool loadDefaultFont = true);
        IMGUI_SFML_API void Init(sf::Window& window, sf::RenderTarget& target, bool loadDefaultFont = true);

        IMGUI_SFML_API void ProcessEvent(const sf::Event& event);

        IMGUI_SFML_API void Update(sf::RenderWindow& window, sf::Time dt);
        IMGUI_SFML_API void Update(sf::Window& window, sf::RenderTarget& target, sf::Time dt);
        IMGUI_SFML_API void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt);

        IMGUI_SFML_API void Render(sf::RenderTarget& target);

        IMGUI_SFML_API void Shutdown();

        IMGUI_SFML_API void UpdateFontTexture();
        IMGUI_SFML_API sf::Texture& GetFontTexture();

        // joystick functions
        IMGUI_SFML_API void SetActiveJoystickId(unsigned int joystickId);
        IMGUI_SFML_API void SetJoytickDPadThreshold(float threshold);
        IMGUI_SFML_API void SetJoytickLStickThreshold(float threshold);

        IMGUI_SFML_API void SetJoystickMapping(int action, unsigned int joystickButton);
        IMGUI_SFML_API void SetDPadXAxis(sf::Joystick::Axis dPadXAxis, bool inverted = false);
        IMGUI_SFML_API void SetDPadYAxis(sf::Joystick::Axis dPadYAxis, bool inverted = false);
        IMGUI_SFML_API void SetLStickXAxis(sf::Joystick::Axis lStickXAxis, bool inverted = false);
        IMGUI_SFML_API void SetLStickYAxis(sf::Joystick::Axis lStickYAxis, bool inverted = false);
    }

    // custom ImGui widgets for SFML stuff

    // Image overloads
    IMGUI_SFML_API void Image(const sf::Texture& texture,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const sf::Texture& texture, const sf::Vector2f& size,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& textureRect,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);

    IMGUI_SFML_API void Image(const sf::Sprite& sprite,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const sf::Sprite& sprite, const sf::Vector2f& size,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const sf::RenderTexture& rt,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    IMGUI_SFML_API void Image(const kage::Atlas& atlas,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);

    // ImageButton overloads
    IMGUI_SFML_API bool ImageButton(const sf::Texture& texture, const int framePadding = -1,
        const sf::Color& bgColor = sf::Color::Transparent,
        const sf::Color& tintColor = sf::Color::White);
    IMGUI_SFML_API bool ImageButton(const sf::Texture& texture, const sf::Vector2f& size, const int framePadding = -1,
        const sf::Color& bgColor = sf::Color::Transparent, const sf::Color& tintColor = sf::Color::White);

    IMGUI_SFML_API bool ImageButton(const sf::Sprite& sprite, const int framePadding = -1,
        const sf::Color& bgColor = sf::Color::Transparent,
        const sf::Color& tintColor = sf::Color::White);
    IMGUI_SFML_API bool ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, const int framePadding = -1,
        const sf::Color& bgColor = sf::Color::Transparent,
        const sf::Color& tintColor = sf::Color::White);

    IMGUI_SFML_API bool ImageButton(const kage::Atlas& atlas, const int framePadding = -1,
        const sf::Color& bgColor = sf::Color::Transparent,
        const sf::Color& tintColor = sf::Color::White);

    // Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
    IMGUI_SFML_API void DrawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col, float thickness = 1.0f);
    IMGUI_SFML_API void DrawRect(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F, float thickness = 1.0f);
    IMGUI_SFML_API void DrawRectFilled(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F);
}

#endif //# IMGUI_SFML_H
