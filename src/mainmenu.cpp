
#include "sfmlApp.hpp"

#ifdef MAINMENU_HPP

void MainMenuState::onCreate ()
{
	self_ = this;

}

void MainMenuState::onMouseDown (const vecF& pos, bool isLeft)
{

}

void MainMenuState::onMouseMove (const vecF& pos)
{
	
}

void MainMenuState::onKeyPress(Keyboard::Key k)
{
	switch(k) {
			
		case Keyboard::Key::Escape:
			app->close();
			break;
				
		default:
			break;
	}
}

void MainMenuState::update (const Time& time)
{

}

void MainMenuState::draw ()
{
//	rwin->draw();
}

#endif
