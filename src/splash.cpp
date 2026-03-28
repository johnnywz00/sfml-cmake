#include "sfmlApp.hpp"

#ifdef SPLASH_HPP

Time 		SplashScreen::duration = 	seconds(5);
StateType 	SplashScreen::nextState = 	StateType::MainMenu;

void SplashScreen::onCreate ()
{
	self_ = this;
	
	//bkgdImg.setTexture(gTexture("splashBkgd"));
	//centerOrigin(bkgdImg);
	//bkgdImg.setPosition(scrcx, scrcy); // viewCX?
	
//	splashTitle = Text(app->programName, gFont("splashTitle"), 160);
	//position...
	
	//signatureTxt, clickTxt...
}

void SplashScreen::onMouseDown (const vecF& pos, bool isLeft)
{
	app->switchToState(nextState);
}

void SplashScreen::onKeyPress (Keyboard::Key)
{
	app->switchToState(nextState);
}

void SplashScreen::update (const Time& time)
{
	if (time > duration)
		app->switchToState(nextState);
	
	// animation/effects
}

void SplashScreen::draw ()
{
//	rwin->draw(bkgdImg);
//	rwin->draw(splashTitle);
//	rwin->draw(signatureTxt);
//	rwin->draw(clickOrKeyTxt);
}

#endif
