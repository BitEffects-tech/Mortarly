#pragma once

class Image;

typedef struct {
    float x;
    float y;
    float w;
    float h;
} Rectangle;

namespace gui {
    enum LayoutDirection {
        Horizontal,
        Vertical
    };

    enum Style {
        Visibility,
        Display,
        MinWidth,
        MaxWidth,
        MinHeight,
        MaxHeight,
        HAlignContent,
        VAlignContent,
        Padding,
        Margin,
        BackgroundColor,
        Gradient,
        Color,
        BorderWidth,
        BorderColor,
    };

    typedef std::variant<std::string, float, bool> StyleValue;
    typedef std::map<Style, StyleValue> StyleMap;

    typedef struct {
        LayoutDirection direction;
        Rectangle rect;
        StyleMap styles;
        float pWidth;
        float pHeight;
    } ControlState;

    std::shared_ptr<ControlState> begin(LayoutDirection direction, float width = 0, float height = 0);

    void image(const Image &img);
    void text(const std::string &text);

    void style(Style key, const std::string &value);
    void style(Style key, const float value);
    void style(Style key, const bool value);
    
    bool onClick();
    bool onMouseDown();
    bool onHover();

    void end();
}

