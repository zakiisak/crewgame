#include "button.h"
#include "../crewgame.h"

namespace zakix {

	Button::Button(const vec2& position, const vec2& size, std::string text) {
		this->position = position;
		this->size     = size;
		this->text     = text;
	}

	bool Button::update(CrewGame* cg) {
		Window* w = cg->getWindow();
		
		double x, y;
		w->getMousePosition(x, y);
		if (contains(vec2((float)x, (float)y))) {
			intersecting = true;
			if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				w->setMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, false);
				return true;
			}
		}
		else {
			intersecting = false;
		}
		return false;
	}

	void Button::render(BatchRenderer2D* renderer) {
		const vec2 pos = position + offset;
		renderer->submit(pos, size, intersecting ? foregroundColor : backgroundColor);
		if(!intersecting) renderer->batchString(text, vec3(pos.x + size.x / 2 - font->getWidth(text) / 2, pos.y + size.y / 2 + font->getSize() / 2 + 1, 0), font, 0xff000000);
		renderer->batchString(text, vec3(pos.x + size.x / 2 - font->getWidth(text) / 2, pos.y + size.y / 2 + font->getSize() / 2, 0), font, intersecting ? fontColorFore : fontColorBack);
		
	}

	const std::string& Button::getText() const {
		return text;
	}

	void Button::setText(const std::string& text) {
		this->text = text;
	}

	Font* Button::getFont() const {
		return font;
	}

	void Button::setFont(Font* font) {
		this->font = font;
	}

	void Button::setBackgroundColor(unsigned int backgroundColor) {
		this->backgroundColor = backgroundColor;
	}

	void Button::setForegroundColor(unsigned int foregroundColor) {
		this->foregroundColor = foregroundColor;
	}

	void Button::setFontColor(unsigned int fontColorBack, unsigned int fontColorFore) {
		this->fontColorBack = fontColorBack;
		this->fontColorFore = fontColorFore;
	}
}