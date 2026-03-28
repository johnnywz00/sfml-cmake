
#include "sfmlApp.hpp"

void CoreState::onCreate ()
{
	self_ = this;
	
	debugTxt.setPosition({8, 9});
	debugTxt.setFillColor(AZURE);
	
	initAddMouseSprite(&spr, {{
//		.texture = &gTexture("deh"),
		 .position = {100, 100}
		, .rotation = 90
		},
		{.draggable = true, .drawingOrder=105,
			.clickFunc = [this](const vecF& pos){ spr.setColor(ORANGE);}}
	});
	initAddMouseText(&txt, {{
		.charSize = 60,
		.color = Color::Black,
		.text = "HEBISBH",
		.outlineThickness = 10,
		.outlineColor = PURPLE,
		.position = {200, 200},
		.scale = {.5, 2},
		.rotation = 17
	}, {
		.draggable = true,
		.rightDraggable = true,
		.mouseEnterFunc = []() { FullscreenOnlyApp::setRedrawColor(MUTEDGRASS); },
		.mouseExitFunc = []() { FullscreenOnlyApp::setRedrawColor(Color::Black); }
	}});
	initAddMouseRect(&rect, {{
		.size = {49, 78}, .color = ORANGE, .position = {300,300},
		.outlineThickness=2
	}, {.draggable=true,
		.rightClickFunc = [this](const vecF& pos){ rect.shape.setScale({2, 2}); } }});
	initAddMouseCircle(&circ, {{
		.radius = 35, .color = CAPPUCCINO, .position = {400,400},
		.outlineThickness=6, .outlineColor= decreaseBrightness(Color::White, 12)
	}, {.draggable=true,
		.clickFunc = [this](const vecF& pos){ if (isShiftPressed())circ.pendingDeletion = true; }}});
	
	emplaceMouseSprite({{
		.texture = &gTexture("deh"),
		.color=Color::Green,
		.position = {100, 100}
		, .rotation = 90
	},
		{.draggable = true, .drawingOrder=106 }
	});
	emplaceMouseText({{
		.font = &gFont("textbox"),
		.charSize = 60,
		.color = Color::Green,
		.text = "PHOHEBISBH",
		.outlineThickness = 10,
		.outlineColor = Color::Black,
		.position = {200, 200},
		.rotation = 170
	}, {.draggable = true, .rightDraggable = true}});
	emplaceMouseRect({{
		.size = {49, 78}, .color = Color::Green, .position = {300,300},
		.outlineThickness=2
	}, {.draggable=true}});
	emplaceMouseCircle({{
		.radius = 35, .color = Color::Green, .position = {400,400},
		.outlineThickness=6, .outlineColor= decreaseBrightness(Color::White, 12)
	}, {.draggable=true}});
	
	
	reset();
}

void CoreState::activate () { }

void CoreState::onMouseDown (const vecF& pos, bool isLeft)
{
//	if (tbox.tbox.gGB().contains(pos)) {
//		if (activeTbox)
//			activeTbox->setActive(false);
//		tbox.setActive(true);
//		activeTbox = &tbox;
//		return;
//	}
//	else if (tbox.isActive) {
//		tbox.setActive(false);
//		activeTbox = nullptr;
//		return;
//	}
	
	if (isLeft) {
		for (auto& resp : responders) {
			if (resp && !resp->onMouseDown_(pos))
				break;
		}
	}
	else {
		for (auto& resp : responders) {
			if (!resp->onRightMouseDown_(pos))
				break;
		}
	}
}

void CoreState::onMouseUp (const vecF& pos, bool isLeft)
{
	for (auto& resp : responders){
		if (isLeft)
			resp->onMouseUp_(pos);
		else resp->onRightMouseUp(pos);
	}
}

void CoreState::onMouseMove (const vecF& pos)
{
	for (auto& resp : responders)
		resp->onMouseMove_(pos);
}

void CoreState::onMouseWheel(const vecF& pos, float delta, bool isVertical)
{

}

void CoreState::onKeyPress(Keyboard::Key k)
{
	switch(k) {
			
		case Keyboard::Key::Escape:
#ifdef MAINMENU_HPP
			app->switchToState(StateType::MainMenu);
#else
			app->close();
#endif
			break;
						
		default:
			break;
	}
}

void CoreState::onKeyRelease(Keyboard::Key k)
{
	switch(k) {
			
//		case Keyboard::Key::
//			break;
			
		default:
			break;
	}
}

bool CoreState::handleTextEvent (optional<Event> event)
{
	if (activeTbox) {
		if (const auto* textEvt = event->getIf<Event::TextEntered>()) {
			if (textEvt->unicode == 8) { /* Backspace */
				if (isShiftPressed())
					activeTbox->clear();
				else activeTbox->deleteLastChar();
			}
			else if (textEvt->unicode == 9) /* Tab */
				/* Potentially focus next text field */
				; /* don't write the \t */
			else activeTbox->appendText(textEvt->unicode);
		}
		else if (const auto* keyDown = event->getIf<Event::KeyPressed>()) {
			if (keyDown->code == Keyboard::Key::Enter
				&& !activeTbox->returnKeyDeactivates) {
				activeTbox->processReturn();
			}
			else if (keyDown->code == Keyboard::Key::Escape
					 || keyDown->code == Keyboard::Key::Enter) {
				 activeTbox->setActive(false);
				 activeTbox = nullptr;
			 }
		}
		return true;
	}
	return false;
}

void CoreState::update (const Time& time)
{
	timedMgr.fireReadyEvents(time);
//	animMgr.update(time);
	
	//====  DEBUG PRINTING  =====
	ostringstream oss;
	oss << mouseVec.x << ", " << mouseVec.y << endl
	<< vecfStr(txt.txt.gP()) << endl << responders.size()
	;
	debugTxt.setString(oss.str());
}

void CoreState::draw ()
{
	for (auto* resp : responderDrawPtrs) {
		if (resp && !(resp->needsExplicitDrawCall))
			rwin->draw(*resp);
	}
	
	rwin->draw(debugTxt);
}

void CoreState::deactivate ()
{
	timedMgr.reset();
}

void CoreState::reset ()
{
	
}
