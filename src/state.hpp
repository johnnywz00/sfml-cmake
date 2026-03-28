#ifndef BASESTATE_HPP
#define BASESTATE_HPP

#include "resourcemanager.hpp"
#include "timedeventmanager.hpp"
#include "animations.hpp"
#include "MouseResponder.hpp"

using namespace jwz;

class FullscreenOnlyApp;
enum class StateType;


struct ResponseOrderLess
{
	bool operator() (const Pointer<MouseResponder>& a, const Pointer<MouseResponder>& b) const
	{
		if (a->responseOrder != b->responseOrder)
			return a->responseOrder < b->responseOrder;
		/* If responseOrders are the same, we don't want the set
		 * to reject the insertion if they are pointers to
		 * different objects
		 */
		return a.get() < b.get();
	}
};

struct DrawingOrderLess
{
	bool operator() (MouseResponder* a, MouseResponder* b) const
	{
		if (a->drawingOrder != b->drawingOrder)
			return a->drawingOrder < b->drawingOrder;
		return a < b; // See above
	}
};


class BaseState
{
public:
	virtual ~BaseState ()
	{
		responderDrawPtrs.clear();
		responders.clear();
	}

//==============  MOUSE RESPONDER SECTION  ========================
	
	void initAddMouseSprite (MouseResponder* resp, const MouseSpriteConfig& cfg)
	{
		initMouseSprite(resp, cfg);
		registerMouseResponder(resp);
	}
	
	void initAddMouseText (MouseResponder* resp, const MouseTextConfig& cfg)
	{
		initMouseText(resp, cfg);
		registerMouseResponder(resp);
	}
	
	void initAddMouseRect (MouseResponder* resp, const MouseRectConfig& cfg)
	{
		initMouseRect(resp, cfg);
		registerMouseResponder(resp);
	}
	
	void initAddMouseCircle (MouseResponder* resp, const MouseCircleConfig& cfg)
	{
		initMouseCircle(resp, cfg);
		registerMouseResponder(resp);
	}
	
	void registerMouseResponderCommon (MouseResponder* resp)
	{
		responderDrawPtrs.insert(resp);
	}
	
	void registerMouseResponder (MouseResponder* resp)
	{
		registerMouseResponderCommon(resp);
		responders.emplace(resp);
	}
	
	template<typename T>
	void registerMouseResponder (shared_ptr<T> resp)
	{
		static_assert(std::is_base_of_v<MouseResponder, T>, "T must be a subclass of MouseResponder.");
		registerMouseResponderCommon(resp.get());
		responders.emplace(shared_ptr<MouseResponder>(std::move(resp)));
	}
	
	template<typename T>
	void registerMouseResponder (weak_ptr<T> resp)
	{
		static_assert(std::is_base_of_v<MouseResponder, T>, "T must be a subclass of MouseResponder.");
		registerMouseResponderCommon(resp.lock().get());
		responders.emplace(weak_ptr<MouseResponder>(std::move(resp)));
	}
	
	void unregisterMouseResponder (MouseResponder* resp)
	{
		erase_if(responders, [resp](auto& x) { return x.get() == resp; });
		responderDrawPtrs.erase(resp);
	}

	void emplaceMouseSprite (const MouseSpriteConfig& cfg)
	{
		unique_ptr<MouseResponder> spr = make_unique<MouseRespondingSprite>(cfg);
		registerMouseResponderCommon(spr.get());
		responders.emplace(std::move(spr));
	}
	
	void emplaceMouseText (const MouseTextConfig& cfg)
	{
		unique_ptr<MouseResponder> txt = make_unique<MouseRespondingText>(cfg);
		registerMouseResponderCommon(txt.get());
		responders.emplace(std::move(txt));
	}
	
	void emplaceMouseRect (const MouseRectConfig& cfg)
	{
		unique_ptr<MouseResponder> rect = make_unique<MouseRespondingShape<RectangleShape>>(cfg);
		registerMouseResponderCommon(rect.get());
		responders.emplace(std::move(rect));
	}
	
	void emplaceMouseCircle (const MouseCircleConfig& cfg)
	{
		unique_ptr<MouseResponder> circ = make_unique<MouseRespondingShape<CircleShape>>(cfg);
		registerMouseResponderCommon(circ.get());
		responders.emplace(std::move(circ));
	}
//==============================================================
	
	/* Hooks for derived states */
	virtual void onCreate () { }
	
	virtual void activate () { }
	
	virtual void onMouseDown (const vecF& pos, bool isLeft) { }
	
	virtual void onMouseUp (const vecF& pos, bool isLeft) { }
	
	virtual void onMouseMove (const vecF& pos) { }
	
	virtual void onMouseWheel(const vecF& pos, float delta, bool isVertical) { }
	
	virtual void onKeyPress (Keyboard::Key) { }
	
	virtual void onKeyRelease (Keyboard::Key) { }
	
	virtual bool handleTextEvent (optional<Event>) { }
	
	virtual void update (const Time& time) { }
	
	virtual void draw () { }
	
	virtual void deactivate () { }
	
	/* Methods called by app class: handle busywork */
	void activate_ ()
	{
		mouseVec = Mouse::getPosition(*rwin);
		oldMouse = mouseVec;
		activate();
	}
	
	void onMouseMove_ (const vecF& pos)
	{
		mouseVec = toVecI(pos);
		onMouseMove(pos);
	}
	
	void update_ (const Time& time)
	{
		oldMouse = mouseVec;
		erase_if(responderDrawPtrs, [](auto* x){ return x->pendingDeletion; });
		erase_if(responders, [](auto& x){ return x->pendingDeletion; });
		update(time);
	}
	
	View							vw {FloatRect({0.f, 0.f},
												  {scrw, scrh})};
	RenderWindow*  		 			rwin;
	FullscreenOnlyApp* 				app;
	TimedEventManager      			timedMgr;
	AnimationManager				animMgr;
	set<Pointer<MouseResponder>, ResponseOrderLess>
									responders;
	set<MouseResponder*, DrawingOrderLess>
									responderDrawPtrs;
	vecI             	 			mouseVec
									, oldMouse
	;
};

#endif




