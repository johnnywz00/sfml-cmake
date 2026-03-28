#ifndef SPLASH_HPP
#define SPLASH_HPP

#include "state.hpp"


class SplashScreen: public BaseState
{
public:
	static SplashScreen* self () { return self_; }
	void onCreate () override;
	void onMouseDown (const vecF& pos, bool isLeft) override;
	void onKeyPress (Keyboard::Key) override;
	void update (const Time& time) override;
	void draw () override;
	
private:
	static inline SplashScreen*	self_ = nullptr;
	static Time					duration;
	static StateType			nextState;
//	Sprite						bkgdImg;
//	Text						splashTitle
//								, signatureTxt
//								, clickOrKeyTxt
//	;
	
};

#endif




