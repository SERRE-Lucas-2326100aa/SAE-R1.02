#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <mingl/graphics/idrawable.h>
#include <mingl/shape/shape.h>
#include <mingl/graphics/rgbacolor.h>
#include <mingl/graphics/vec2d.h>
#include <mingl/gui/text.h>

// GUI Utility
class Button : public nsShape::Shape
{
public:
    Button(const nsGraphics::Vec2D& position, const int& width,
           const int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
           const nsGraphics::RGBAcolor& border_col = nsGraphics::KTransparent);
    virtual ~Button() override = default;

    virtual void draw(MinGL& window) const override;

    bool is_in(int x, int y) const
    {
        if (x >= first_pos.getX() && x <= second_pos.getX() && y >= first_pos.getY() && y <= second_pos.getY())
            return true;
        return false;
    };

    std::function<void()> on_click;

private:
    nsGraphics::Vec2D first_pos;
    nsGraphics::Vec2D second_pos;
    std::string text_content;
};

#endif // BUTTON_H
