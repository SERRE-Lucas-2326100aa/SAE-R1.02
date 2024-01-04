#include "hdrs/globals.h"
#include "hdrs/utils.h"

Button::Button(const nsGraphics::Vec2D& position, const int& width,
               const int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
               const nsGraphics::RGBAcolor& border_col) : Shape(fill_col, border_col)
      {
        first_pos = position;
        second_pos = {position.getX() + width, position.getY() + height};
        text_content = content;
};
void Button::draw(MinGL& window) const
{
    // On règle la couleur du rectangle
    nsGraphics::RGBAcolor inColor = (is_in(glob_blob::cursor.x,glob_blob::cursor.y))
                                        ? nsGraphics::RGBAcolor{173,216,230,255} : getFillColor();

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

    nsGui::Text txt(first_pos,text_content,nsGraphics::KRed,
                           nsGui::GlutFont::BITMAP_9_BY_15, nsGui::Text::ALIGNH_CENTER);

    txt.setPosition({first_pos.getX() + ((second_pos.getX()-first_pos.getX())/2.f), second_pos.getY() - first_pos.getY()/2.f});


    if (is_in(glob_blob::cursor.x,glob_blob::cursor.y))
    {
        glutSetCursor(GLUT_CURSOR_INFO);
    }
    else
    {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }

    //std::cout << glob_blob::cursor.is_clicking << std::endl;

    window << txt;
};
