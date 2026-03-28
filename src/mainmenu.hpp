#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "state.hpp"

class MainMenuState: public BaseState
{
public:
	static MainMenuState* self () { return self_; }
	
	void onCreate () override;
	
	//	void activate () override;
	
	void onMouseDown (const vecF& pos, bool isLeft) override;
	
	//	void onMouseUp (const vecF& pos, bool isLeft) override;
	
	void onMouseMove (const vecF& pos) override;
	
	//	void onMouseWheel(float delta, bool isVertical) override;
	
	void onKeyPress (Keyboard::Key) override;
	
	//	void onKeyRelease (Keyboard::Key) override;
	
	//	bool handleTextEvent (optional<Event>) override;
	
	void update (const Time& time) override;
	
	void draw () override;
	
	//	void deactivate () override;
	
private:
	static inline MainMenuState* self_ = nullptr;
	//title, buttons, thumbnails, instructions, signature
	
	RectangleShape r;///////////
	
};


#endif




