#pragma once
#include <SDL3/SDL_render.h>
class Circle
{
public:
	static void DrawCircle(SDL_Renderer*& renderer, int centerX, int centerY, int radius) {
		/*int r2 = radius * radius;
		int area = r2 << 2;
		int rr = radius << 1;

		for (int i = 0; i < area; i++) {
			int tx = (i % rr) - radius;
			int ty = (i / rr) - radius;

			if (tx * tx + ty * ty <= r2)
				SDL_RenderPoint(renderer, centerX + tx, centerY + ty);
		}*/

		int x = 0;
		int y = -radius;
		float p = -radius;

		while (x < -y) {
			if (p > 0) {
				y += 1;
				p += 2 * (x + y) + 1;
			}
			else
				p += 2 * x + 1;

			SDL_RenderPoint(renderer, centerX + x, centerY + y);
			SDL_RenderPoint(renderer, centerX - x, centerY + y);
			SDL_RenderPoint(renderer, centerX + x, centerY - y);
			SDL_RenderPoint(renderer, centerX - x, centerY - y);
			SDL_RenderPoint(renderer, centerX + y, centerY + x);
			SDL_RenderPoint(renderer, centerX + y, centerY - x);
			SDL_RenderPoint(renderer, centerX - y, centerY - x);
			SDL_RenderPoint(renderer, centerX - y, centerY + x);
		//	//SDL_RenderLine(renderer, centerX + x, centerY + y, centerX - x, centerY - y);
		//	//SDL_RenderLine(renderer, centerX - x, centerY + y, centerX + x, centerY - y);
		//	//SDL_RenderLine(renderer, centerX + x, centerY - y, centerX - x, centerY + y);
		//	//SDL_RenderLine(renderer, centerX - x, centerY - y, centerX + x, centerY + y);
		//	//SDL_RenderLine(renderer, centerX + y, centerY + x, centerX - y, centerY - x);
		//	//SDL_RenderLine(renderer, centerX + y, centerY - x, centerX - y, centerY + x);
		//	//SDL_RenderLine(renderer, centerX - y, centerY - x, centerX + y, centerY + x);
		//	//SDL_RenderLine(renderer, centerX - y, centerY + x, centerX + y, centerY - x);
			x += 1;
		}
	}
};

