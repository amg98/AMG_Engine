/**
 * @file Font.cpp
 * @brief Font loading and text rendering
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

// Own includes
#include "Font.h"
#include "Debug.h"

namespace AMG {

/**
 * @brief Constructor for a Font
 */
Font::Font(const char *tex, const char *fnt){

	// Load the texture
	glyphBuffer = NULL;
	font = new Texture(tex, 0.0f);
	font->setDependency(true);

	// Create the glyph map
	glyphs = std::tr1::unordered_map<char, AMG_Glyph*>();

	// Open the font file
	FILE *f = fopen(getFullPath(fnt, AMG_FONT), "rb");
	if(f == NULL) Debug::showError(FILE_NOT_FOUND, (void*)fnt);

	// Read the header lines
	float fontSize = 0.0f;
	char line[256];
	fgets(line, 256, f);
	sscanf(line, "info face=%*s size=%f", &fontSize);
	fgets(line, 256, f);
	sscanf(line, "common lineHeight=%f", &lineHeight);
	lineHeight += 8;
	lineHeight /= fontSize;

	// Get the number of characters
	do{
		fgets(line, 256, f);
	}while(strncmp(line, "chars", 5) != 0);
	int nchars = 0;
	sscanf(line, "chars count=%d", &nchars);

	// Create the glyph buffer
	glyphBuffer = (AMG_Glyph*) malloc (nchars * sizeof(AMG_Glyph));

	// Read each glyph
	for(int i=0;i<nchars;i++){
		AMG_Glyph *glyph = &glyphBuffer[i];
		fgets(line, 256, f);
		int id;
		sscanf(line+5, "id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f",
						&id, &glyph->x, &glyph->y, &glyph->width, &glyph->height, &glyph->xoffset, &glyph->yoffset, &glyph->advance);

		glyph->xoffset /= fontSize;
		glyph->yoffset /= fontSize;
		glyph->advance /= fontSize;
		glyph->normWidth = glyph->width / fontSize;
		glyph->normHeight = glyph->height / fontSize;
		glyph->x /= font->getWidth();
		glyph->width /= font->getWidth();
		glyph->y /= font->getHeight();
		glyph->height /= font->getHeight();

		// Add the glyph onto the map
		glyphs[id] = glyph;
	}

	// Set the space size
	this->spaceSize = glyphs[' ']->advance;

	// Close the font file
	fclose(f);
}

/**
 * @brief Get a character from the map
 * @param c The character to retrieve
 * @return The glyph associated to this character
 */
AMG_Glyph *Font::getCharacter(char c){
	std::tr1::unordered_map<char, AMG_Glyph*>::const_iterator got = glyphs.find(c);
	if(got == glyphs.end()) Debug::showError(14, (void*)&c);
	return got->second;
}

/**
 * @brief Create a text object using this font
 * @param text The text to be rendered, in extended ASCII format
 * @param size Size of the text
 * @param width Textbox width, in pixels
 * @param height Textbox height, in pixels
 * @param remaining Position where it stopped writing text
 */
Text *Font::createText(char *text, float size, float width, float height, int *remaining){

	// Get size of the string
	int nchars = strlen(text);
	int writeLen = 0;
	int written = 0;

	// Organize the text in words
	char *tbuf = (char*) malloc ((nchars + 1) * sizeof(char));
	strcpy(tbuf, text);
	std::vector<char*> words = std::vector<char*>();
	std::vector<int> wordsSize = std::vector<int>();
	char *pch = strtok (tbuf, " ");
	while (pch != NULL){
		int l = strlen(pch);
		wordsSize.push_back(l);
		words.push_back(pch);
		writeLen += l;
		pch = strtok (NULL, " ");
	}

	// Create buffers
	int bufferSize = writeLen * 6 * 2;
	int textBufferSize = 0;
	float *vertices = (float*) calloc (bufferSize, sizeof(float));
	float *texcoords = (float*) calloc (bufferSize, sizeof(float));
	float cursorX = 0.0f;
	float cursorY = 0.0f;

	// Write every word
	for(unsigned int i=0;i<words.size();i++){

		// Get word information
		char *word = words[i];
		int wordLen = wordsSize[i];

		// Check if the word fits horizontally
		float length = 0.0f;
		for(int j=0;j<wordLen;j++){
			AMG_Glyph *character = getCharacter(word[j]);
			length += character->advance;
		}

		// If it doesn't fit in the text box, put it in the next line
		if((length * size) > (width - cursorX)){
			cursorY -= lineHeight * size;
			cursorX = 0.0f;
		}

		// Check if the word fits vertically
		if(cursorY < -height){
			*remaining = written + i;
			break;
		}

		// For each character
		for(int j=0;j<wordLen;j++){

			// Check new line
			if(word[j] == '\n'){
				cursorY -= lineHeight * size;
				cursorX = 0.0f;
				continue;
			}

			// Get the glyph data
			AMG_Glyph *character = getCharacter(word[j]);

			// Calculate the coordinates for this character
			int offset = written * 12;
			float x = cursorX + character->xoffset * size;
			float y = cursorY - character->yoffset * size;
			float maxX = x + character->normWidth * size;
			float maxY = y - character->normHeight * size;
			float texX = character->x;
			float texY = 1.0f - character->y;
			float texMaxX = character->x + character->width;
			float texMaxY = 1.0f - character->y - character->height;

			// Write primitive data
			vertices[offset + 0] = x;
			vertices[offset + 1] = y;
			vertices[offset + 2] = maxX;
			vertices[offset + 3] = y;
			vertices[offset + 4] = maxX;
			vertices[offset + 5] = maxY;
			vertices[offset + 6] = x;
			vertices[offset + 7] = y;
			vertices[offset + 8] = maxX;
			vertices[offset + 9] = maxY;
			vertices[offset + 10] = x;
			vertices[offset + 11] = maxY;
			texcoords[offset + 0] = texX;
			texcoords[offset + 1] = texY;
			texcoords[offset + 2] = texMaxX;
			texcoords[offset + 3] = texY;
			texcoords[offset + 4] = texMaxX;
			texcoords[offset + 5] = texMaxY;
			texcoords[offset + 6] = texX;
			texcoords[offset + 7] = texY;
			texcoords[offset + 8] = texMaxX;
			texcoords[offset + 9] = texMaxY;
			texcoords[offset + 10] = texX;
			texcoords[offset + 11] = texMaxY;
			textBufferSize += 12;

			// Advance to the next character
			cursorX += character->advance * size;
			written ++;
		}

		// Advance a space
		cursorX += this->spaceSize * size;
	}

	// Create text object
	Text *t = new Text(vertices, texcoords, textBufferSize * sizeof(float), this->font);

	// Free data
	free(vertices);
	free(texcoords);
	free(tbuf);
	words.clear();
	wordsSize.clear();

	// Return the created text
	return t;
}

/**
 * @brief Destructor for a Font
 */
Font::~Font() {
	if(glyphBuffer) free(glyphBuffer);
	glyphs.clear();
	if(font) delete font;
}

}
