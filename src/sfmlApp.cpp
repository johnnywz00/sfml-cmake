#include "sfmlApp.hpp"

//================================================
// 	Customize per project:
string 		  FullscreenOnlyApp::programName = 		"SFMLApp";
Color 		  FullscreenOnlyApp::redrawColor_ = 	Color::White;
unsigned int  FullscreenOnlyApp::framerateLimit = 	60;
Keyboard::Key FullscreenOnlyApp::debugUnpauseKey = 	Keyboard::Key::Grave;
//================================================

FullscreenOnlyApp::FullscreenOnlyApp ()
{
	VideoMode mode = VideoMode::getDesktopMode();
	auto state = State::Windowed;
	auto fsmodes = VideoMode::getFullscreenModes();
	if (fsmodes.size()) {
		mode = fsmodes[0];
		state = State::Fullscreen;
	}
	window.create(mode, programName, state);
	
	window.setFramerateLimit(framerateLimit);
	
	if (!icon.loadFromFile((resourcePath() / "images" / "icon.png").string()))
		cerr << "Couldn't load icon. " << endl;
	else
		window.setIcon(icon);
	
    srand(unsigned(time(nullptr)));
		
#ifdef SPLASH_HPP
	states.emplace(StateType::Splash, make_unique<SplashScreen>());
#endif
#ifdef MAINMENU_HPP
	states.emplace(StateType::MainMenu, make_unique<MainMenuState>());
#endif
	states.emplace(StateType::Core, make_unique<CoreState>());
	/* Any custom States here */
	
	for (auto& kv : states) {
		kv.second->app = this;
		kv.second->rwin = &window;
		kv.second->onCreate();
	}
	
#if defined(SPLASH_HPP)
	switchToState(StateType::Splash);
#elif defined(MAINMENU_HPP)
	switchToState(StateType::MainMenu);
#else
	switchToState(StateType::Core);
#endif

    clock.restart();
}

void FullscreenOnlyApp::run ()
{
	cout << "Entering app.run()\n";
	while (!isDone) {
		update();
		render();
		elapsed += clock.restart();
	}
	cout << "Leaving app.run()\n";
}

void FullscreenOnlyApp::update ()
{
	vecF adjCoords;
    while (const optional<Event> event = window.pollEvent()) {
				
		const auto* textEntd = event->getIf<Event::TextEntered>();
		const auto* keyDown = event->getIf<Event::KeyPressed>();
		if (textEntd || keyDown) {
			if (!curState->handleTextEvent(event)
				&& keyDown) {
				// F5 toggle fullscreen?
				curState->onKeyPress(keyDown->code);
			}
		}

		if (const auto* mouseMove = event->getIf<Event::MouseMoved>()) {
			adjCoords = window.mapPixelToCoords(vecI(mouseMove->position.x,
													 mouseMove->position.y));
			curState->onMouseMove_(adjCoords);
		}
		
		else if (const auto* keyUp = event->getIf<Event::KeyReleased>())
				curState->onKeyRelease(keyUp->code);
				
			/* Here, only distinguishing between left button and not-left button */
		else if (const auto* mouseDown = event->getIf<Event::MouseButtonPressed>()) {
			adjCoords = window.mapPixelToCoords(vecI(mouseDown->position.x,
													 mouseDown->position.y));
			curState->onMouseDown(adjCoords,
								  mouseDown->button == Mouse::Button::Left);
		}
		
		else if (const auto* mouseUp = event->getIf<Event::MouseButtonReleased>()) {
			adjCoords = window.mapPixelToCoords(vecI(mouseUp->position.x,
													 mouseUp->position.y));
			curState->onMouseUp(adjCoords,
								  mouseUp->button == Mouse::Button::Left);
		}
		
		else if (const auto* mouseScroll = event->getIf<Event::MouseWheelScrolled>()) {
			adjCoords = window.mapPixelToCoords(vecI(mouseScroll->position.x,
													 mouseScroll->position.y));
			curState->onMouseWheel(adjCoords, mouseScroll->delta,
								   mouseScroll->wheel == Mouse::Wheel::Vertical);
		}

		else if (event->is<Event::FocusLost>())
				isFocused = false;
				
		else if (event->is<Event::FocusGained>())
				isFocused = true;
				// curState-> reset mouseVec?
				
		else if (event->is<Event::Closed>())
				close();
    }
    curState->update_(elapsed);
}

void FullscreenOnlyApp::render ()
{
	window.clear(redrawColor_);
	curState->draw();
	window.display();
	if (dbgPauseAfterDraw_) {
		while (!Keyboard::isKeyPressed(debugUnpauseKey))
			dbgPauseAfterDraw_ = false;
	}
}

void FullscreenOnlyApp::switchToState (StateType st)
{
	if (curState)
		curState->deactivate();
	curState = states[st].get();
	curState->activate_();
	window.setView(curState->vw);
}


int main (int argc, char* argv[])
{
/* XCode quirk: two instances of the program are launched if
 * we customize the working directory. We can cause the extraneous
 * instance to silently quit by giving it a relative
 * path that it can't find.
 */
#ifdef DEBUG
    Image img;
	if (!img.loadFromFile("resources/images/icon.png")) {
		cerr << "DEBUG preprocessor symbol is defined, but icon.png can't be found through relative path.\n";
		return EXIT_FAILURE;
	}
#endif
	
	cout << "Initializing Resources class...\n";
	Resources::initialize(argc, argv);

	FullscreenOnlyApp app;
	FullscreenOnlyApp::initialize(&app);
	
	try {
		cout << "Preparing to launch game loop for " << app.programName << endl;
		app.run();
		cout << "Exited " << app.programName << " loop" << endl;
	}
	catch (runtime_error& e) {
		cerr << "Caught runtime exception: " << e.what() << endl;
	}
	catch (exception& e) {
		cerr << "Caught exception: " << e.what() << endl;
	}
	catch (...) {
		cerr << "Caught unknown exception\n";
	}
	
	Resources::cleanup();
}
