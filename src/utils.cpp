#include "hdrs/utils.h"


Button::Button(const nsGraphics::Vec2D& position, const unsigned int& width,
               const unsigned int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
               const nsGraphics::RGBAcolor& border_col) : Shape(fill_col, border_col)
      {
        //ZzShape(fill_col,border_col);
        first_pos = position;
        second_pos = {position.getX() + width, position.getY() + height};
};
void Button::draw(MinGL& window) const
{

    // On règle la couleur du rectangle
    const nsGraphics::RGBAcolor inColor = getFillColor();
    glColor4ub(inColor.getRed(), inColor.getGreen(), inColor.getBlue(), inColor.getAlpha());

    // Affiche un rectangle via la routine OpenGL
    glRecti(first_pos.getX(), first_pos.getY(), second_pos.getX(), second_pos.getY());

    if (getBorderColor() != nsGraphics::KTransparent) {
        // On a une bordure, on l'affiche
        const nsGraphics::RGBAcolor borderColor = getBorderColor();
        glColor4ub(borderColor.getRed(), borderColor.getGreen(), borderColor.getBlue(), borderColor.getAlpha());
        glLineWidth(1.f);

        glBegin(GL_LINE_LOOP);
        glVertex2i(first_pos.getX(), first_pos.getY());
        glVertex2i(first_pos.getX(), second_pos.getY());
        glVertex2i(second_pos.getX(), second_pos.getY());
        glVertex2i(second_pos.getX(), first_pos.getY());
        glEnd();
    }
};
