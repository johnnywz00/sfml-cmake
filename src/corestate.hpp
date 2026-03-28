#ifndef CORESTATE_HPP
#define CORESTATE_HPP

#include "state.hpp"


class CoreState: public BaseState
{
public:
	static CoreState* self () { return self_; }
	
	void onCreate () override;
	
	void activate () override;
	
	void onMouseDown (const vecF& pos, bool isLeft) override;
	
	void onMouseUp (const vecF& pos, bool isLeft) override;
	
	void onMouseMove (const vecF& pos) override;
	
	void onMouseWheel(const vecF& pos, float delta, bool isVertical) override;
	
	void onKeyPress (Keyboard::Key) override;
	
	void onKeyRelease (Keyboard::Key) override;
	
	bool handleTextEvent (optional<Event>) override;
	
	void update (const Time& time) override;
	
	void draw () override;
	
	void deactivate () override;
	
private:
	void reset ();

	static inline CoreState*	self_ = nullptr;
	
	Textbox*				activeTbox = nullptr;
	Text					debugTxt {Resources::getDefaultFont(), "", 13};
	
}; //end class CoreState

#endif
