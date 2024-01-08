#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <mingl/graphics/idrawable.h>
#include <mingl/shape/shape.h>
#include <mingl/graphics/rgbacolor.h>
#include <mingl/graphics/vec2d.h>
#include <mingl/gui/text.h>


/**
 * Clase Button personelle
 */

class Button : public nsShape::Shape
{
public:
    /**
     * @brief Constructeur de la classe
     * @fn Button(const nsGraphics::Vec2D& position, const int& width,
           const int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
           const nsGraphics::RGBAcolor& border_col = nsGraphics::KTransparent);
     *
     */
    Button(const nsGraphics::Vec2D& position, const int& width,
           const int& height, const std::string& content, const nsGraphics::RGBAcolor& fill_col,
           const nsGraphics::RGBAcolor& border_col = nsGraphics::KTransparent);
    /**
     * @brief Destructeur de la classe Button
     * @fn ~Button();
     */
    virtual ~Button() override = default;

    /**
     * @brief fonction virtuelle de la classe IDrawEtidable
     * @fn void draw(MinGL& window) const override;
     */
    virtual void draw(MinGL& window) const override;

    /**
     * @brief fonction qui vérifie si les coordonée sont dans le boutton
     * @fn is_in(int x, int y) const
     */
    bool is_in(int x, int y) const
    {
        if (x >= first_pos.getX() && x <= second_pos.getX() && y >= first_pos.getY() && y <= second_pos.getY())
            return true;
        return false;
    };
    /**
     * @brief fonction à appeler en cas de clique
     * @fn void();
     */
    std::function<void()> on_click;

private:
    /**
     * @brief first_pos : position début du rectangle;
     */
    nsGraphics::Vec2D first_pos;
    /**
     * @brief second_pos : position fin du rectangle;
     */
    nsGraphics::Vec2D second_pos;
    /**
     * @brief text_content : Contenu du texte à l'intérieur du bouton;
     */
    std::string text_content;
};

#endif // BUTTON_H
