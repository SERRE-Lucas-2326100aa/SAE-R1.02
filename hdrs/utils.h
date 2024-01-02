#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include "mingl/graphics/idrawable.h"
#include "mingl/shape/shape.h"
#include "mingl/graphics/rgbacolor.h"
#include "mingl/graphics/vec2d.h"
#include "mingl/gui/text.h"
using CMatrice = std::vector<std::vector<unsigned short>>; // grille


// GUI Utility
class Button : public nsShape::Shape
{
public:
    Button(const nsGraphics::Vec2D& position, const unsigned int& width,
           const unsigned int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
           const nsGraphics::RGBAcolor& border_col = nsGraphics::KTransparent);
    virtual ~Button() override = default;

    virtual void draw(MinGL& window) const override;

    void set_hover(bool is_hov)
    {
        is_hovered = is_hov;
    }

    void set_pressed(bool is_press)
    {
        is_pressed = is_press;
    }

    bool is_in(int x, int y) const
    {
        if (x >= first_pos.getX() && x <= second_pos.getX() && y >= first_pos.getY() && y <= second_pos.getY())
            return true;
        return false;
    };

private:
    bool is_hovered, is_pressed;
    nsGraphics::Vec2D first_pos;
    nsGraphics::Vec2D second_pos;
    std::string text_content;
};


#endif // UTILS_H
