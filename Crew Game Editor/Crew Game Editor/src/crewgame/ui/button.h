#pragma once
#include "component.h"
#include <string>
#include "../../graphics/2d/font.h"

namespace zakix {
	class Button : public Component {
	private:
		std::string text;
		Font* font;
		unsigned int backgroundColor = 0xabababab;
		unsigned int foregroundColor = 0xffffffff;
		unsigned int fontColorBack = 0xffffffff, fontColorFore = 0xff000000;
		bool intersecting = false;
	public:
		Button(const vec2& position, const vec2& size, std::string text = "");

		virtual bool update(CrewGame* cg);
		virtual void render(BatchRenderer2D* renderer);

		const std::string& getText() const;
		void setText(const std::string& text);
		Font* getFont() const;
		void setFont(Font* font);
		const inline unsigned int getBackgroundColor() const { return backgroundColor; }
		void setBackgroundColor(unsigned int backgroundColor);
		const inline unsigned int getForegroundColor() const { return foregroundColor; }
		void setForegroundColor(unsigned int foregroundColor);
		const inline unsigned int getBackFontColor() const { return fontColorBack; }
		const inline unsigned int getForeFontColor() const { return fontColorFore; }
		void setFontColor(unsigned int fontColorBack, unsigned int fontColorFore);
		const inline bool isIntersectingMouse() const { return intersecting; }
	};
}