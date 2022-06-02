#include "Texture.h"

void Texture::free_() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	w = h = 0;
}

void Texture::load(string filename, uint nRows, uint nCols) {
	SDL_Surface* tempSurface = IMG_Load(filename.c_str());
	//if (tempSurface == nullptr) throw SDL_ERROR("Error loading surface from " + filename);
	//printf("ERRORTEXTURASSURFACE: %s\n",strerror(errno));
	free_();
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	//printf("ErrorTEXTURASTEX: %s\n",strerror(errno));
	//if (texture == nullptr) throw except("Error loading texture from " + filename);
	numRows = nRows;
	numCols = nCols;
	w = tempSurface->w;
	h = tempSurface->h;
	fw = w / numCols;
	fh = h / numRows;
	SDL_FreeSurface(tempSurface);
}

void Texture::render(const SDL_Rect& destRect, SDL_RendererFlip flip) const {
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = fw; srcRect.h = fh;
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, flip);
}

void Texture::renderFrame(const SDL_Rect& destRect, int row, int col, int angle, SDL_RendererFlip flip) const {
	SDL_Rect srcRect;
	srcRect.x = fw*col; // CABLEADISIMO  lo que había antes era fw*col
	srcRect.y = fh*row; // fh * row
	srcRect.w = fw;
	srcRect.h = fh;
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, 0, flip);
}
