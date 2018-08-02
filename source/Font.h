/**
 * @file Font.h
 * @brief Font loading and text rendering
 */

#ifndef FONT_H_
#define FONT_H_

// Includes C/C++
#include <tr1/unordered_map>

// Own includes
#include "Texture.h"
#include "Text.h"

namespace AMG {

/**
 * @struct AMG_Glyph
 * @brief Holds a glyph
 */
typedef struct{
	float x;			/**< Glyph X position, in pixels */
	float y;			/**< Glyph Y position, in pixels */
	float width;		/**< Glyph width on texture, in pixels */
	float height;		/**< Glyph height on texture, in pixels */
	float normWidth;	/**< Glyph width at size 1 */
	float normHeight;	/**< Glyph height at size 1 */
	float xoffset;		/**< Glyph X offset, in pixels */
	float yoffset;		/**< Glyph Y offset, in pixels */
	float advance;		/**< How much do I need to advance the cursor, in pixels */
}AMG_Glyph;

/**
 * @class Font
 * @brief Font glyph loading and text rendering engine
 */
class Font: public Entity {
private:
	std::tr1::unordered_map<char, AMG_Glyph*> glyphs;
	float spaceSize;			/**< Space size */
	Texture *font;				/**< Font texture */
	AMG_Glyph *glyphBuffer;		/**< Buffer holding the font glyphs */
	float lineHeight;			/**< Height of one line */
	AMG_Glyph *getCharacter(char c);
public:
	float getLineHeight(){ return lineHeight; }

	Font(const char *tex, const char *fnt);
	Text *createText(char *text, float size, float width, float height, int *remaining);
	virtual ~Font();
};

}

#endif
