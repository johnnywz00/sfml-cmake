#ifndef SFMLAPP_HPP
#define SFMLAPP_HPP

//#include "splash.hpp"
//#include "mainmenu.hpp"
#include "corestate.hpp"

enum class StateType
{
	Splash, MainMenu, Core, Editor
};

class FullscreenOnlyApp
{
public:
    FullscreenOnlyApp ();
	
	FullscreenOnlyApp (const FullscreenOnlyApp& foa) = delete;
	FullscreenOnlyApp& operator= (const FullscreenOnlyApp& foa) = delete;
	
	static void initialize (FullscreenOnlyApp* foa)
	{
		self_ = foa;
	}
	
	void run ();
	
	void update ();
	
	void render ();
	
	void close ()
	{
		window.close();
		isDone = true;
	}
	
	static void setRedrawColor (const Color& c) { redrawColor_ = c; }
	
	static Color redrawColor () { return redrawColor_; }
	
	static void showCursor (bool show) { self_->window.setMouseCursorVisible(show); }
	
	void switchToState (StateType st);

	void dbgPauseAfterDraw () { dbgPauseAfterDraw_ = true; }
	
	static inline FullscreenOnlyApp*
							self_ = nullptr;
	static string			programName;
	static unsigned int		framerateLimit;
	static Color            redrawColor_ ;
	static Keyboard::Key 	debugUnpauseKey;

	RenderWindow     		window;
	unordered_map<StateType, unique_ptr<BaseState>>
							states;
	BaseState*				curState = nullptr;
	Clock            		clock;
	Time             		elapsed;
	Image                   icon;
	bool					isFocused = true;
	bool					isDone = false;
	bool					dbgPauseAfterDraw_ = false;
};

#endif /* sfmlApp_hpp */
