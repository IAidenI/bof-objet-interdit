#ifndef STYLE_H
#define STYLE_H

#include "geometry.hpp"
#include <string>
#include <cstdint>

// ---- Style pour les textes ----
struct TextStyle {
    const Font *font;
    string text;
    float fontSize;
    float spacing;
    Color color;
};

inline void DrawTextStyled(TextStyle data, Position position) {
    DrawTextEx(*data.font, data.text.c_str(), position, data.fontSize, data.spacing, data.color);
}

inline Size MeasureTextStyled(TextStyle data) {
    return MeasureTextEx(*data.font, data.text.c_str(), data.fontSize, data.spacing);
}

struct InfoSegment {
    TextStyle textStyle;
    size_t offset = SIZE_MAX;
};

#endif // STYLE_H
