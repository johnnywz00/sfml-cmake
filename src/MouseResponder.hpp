#ifndef MOUSERESPONDER_HPP
#define MOUSERESPONDER_HPP

#include "zsprite.hpp"

class BaseState;
class MouseResponder;

struct MouseResponderConfig
{
	function<void(const vecF&)> clickFunc;
	function<void(const vecF&)>	rightClickFunc;
	function<void()>			mouseEnterFunc;
	function<void()>			mouseExitFunc;
	int							drawingOrder = 100;
	int							responseOrder = 100;
	bool						needsExplicitDrawCall = false;
	bool						propagateClick = false;
	bool						draggable = false;
	bool						rightDraggable = false;
	bool						clickable = false;
	bool						rightClickable = false;
};

/* No inheritance so that we can init with {.position=... syntax */
struct MouseSpriteConfig
{
	MouseSpriteConfig (const SpriteConfig& sfc, const MouseResponderConfig& mc)
		: sfCfg(sfc), mrCfg(mc) { }
	SpriteConfig			sfCfg;
	MouseResponderConfig	mrCfg;
};

struct MouseTextConfig
{
	MouseTextConfig (const TextConfig& sfc, const MouseResponderConfig& mc)
		: sfCfg(sfc), mrCfg(mc) { }
	TextConfig				sfCfg;
	MouseResponderConfig	mrCfg;
};

struct MouseRectConfig: public MouseResponderConfig, public RectConfig
{
	MouseRectConfig (const RectConfig& sfc, const MouseResponderConfig& mc)
		: sfCfg(sfc), mrCfg(mc) { }
	RectConfig				sfCfg;
	MouseResponderConfig	mrCfg;
};

struct MouseCircleConfig: public MouseResponderConfig, public CircleConfig
{
	MouseCircleConfig (const CircleConfig& sfc, const MouseResponderConfig& mc)
		: sfCfg(sfc), mrCfg(mc) { }
	CircleConfig				sfCfg;
	MouseResponderConfig	mrCfg;
};


inline void initMouseCircle (MouseResponder* resp, const MouseCircleConfig& cfg);
inline void initMouseRect   (MouseResponder* resp, const MouseRectConfig&   cfg);
inline void initMouseText   (MouseResponder* resp, const MouseTextConfig&   cfg);
inline void initMouseSprite (MouseResponder* resp, const MouseSpriteConfig& cfg);


class MouseResponder: public Drawable
{
	friend class BaseState;
public:
	MouseResponder () = default;
	
	virtual ~MouseResponder () = default;
	
	void onMouseMove_ (const vecF& pos)
	{
		if (beingDragged)
			dragTo(pos);
		else if (containsCoords(pos)) {
			if (!containsCursor)
				onMouseEnter_();
		}
		else { /* Coords are outside of us */
			if (containsCursor)
				onMouseExit_();
		}
		onMouseMove(pos);
	}
	
	void onMouseEnter_ ()
	{
		containsCursor = true;
		if (mouseEnterFunc)
			mouseEnterFunc();
		onMouseEnter();
	}
	
	void onMouseExit_ ()
	{
		containsCursor = false;
		if (mouseExitFunc)
			mouseExitFunc();
		onMouseExit();
	}
	
	bool onMouseDown_ (const vecF& pos)
	{
		if (respondsToClick(pos)) {
			storeClickOffset(pos);
			if (isDraggable) {
				startDrag_(pos);
			}
			if (isClickable)
				clickedWithin = true;
			onMouseDown(pos);
			return propagateClick(pos);
		}
		return true;
	}
	
	void onMouseUp_ (const vecF& pos)
	{
		if (beingDragged)
			endDrag_(pos);
		if (clickedWithin && clickFunc && containsCursor)
			clickFunc(pos);
		onMouseUp(pos);
		clickedWithin = false;
	}
	
	bool onRightMouseDown_ (const vecF& pos)
	{
		// If need arises, right button could have its own
		//   response condition, own propagateClick decision, click offset
		if (respondsToClick(pos)) {
			storeClickOffset(pos);
			if (isRightDraggable) {
				startDrag_(pos, true);
			}
			if (isRightClickable)
				rightClickedWithin = true;
			onRightMouseDown(pos);
			return propagateClick(pos);
		}
		return true;
	}
	
	void onRightMouseUp_ (const vecF& pos)
	{
		if (beingRightDragged)
			endDrag_(pos, true);
		if (rightClickedWithin && rightClickFunc && containsCursor)
			rightClickFunc(pos);
		onRightMouseUp(pos);
		rightClickedWithin = false;
	}
	
	void startDrag_ (const vecF& pos, bool isRight=false)
	{
		if (isRight)
			beingRightDragged = true;
		else beingDragged = true;
		startDrag(pos);
	}
	
	void endDrag_ (const vecF& pos, bool isRight=false)
	{
		if (isRight)
			beingRightDragged = false;
		else beingDragged = false;
		endDrag(pos);
	}
	
	
	function<void(const vecF&)>	clickFunc;
	function<void(const vecF&)>	rightClickFunc;
	function<void()>			mouseEnterFunc;
	function<void()>			mouseExitFunc;
	int							drawingOrder = 100;
	int							responseOrder = 100;
	bool						needsExplicitDrawCall = false;
	bool						propagateClick_ = false;
	bool						isDraggable = false;
	bool						isRightDraggable = false;
	bool						isClickable = true;
	bool						isRightClickable = false;
	
	vecF						clickOffset;
	bool						beingDragged = false;
	bool						beingRightDragged = false;
	bool						containsCursor = false;
	bool						clickedWithin = false;
	bool						rightClickedWithin = false;
	bool						isSelected = false;
	bool						pendingDeletion = false;
	
	void draw (RenderTarget& target, RenderStates st) const override { }
	
	virtual bool respondsToClick (const vecF& pos)
	{
		return containsCursor;
	}
	
	virtual bool propagateClick (const vecF& pos) { return propagateClick_; }
	
	virtual bool containsCoords (const vecF& pos) { return false; }
	
	virtual void storeClickOffset (const vecF& pos) { }

	virtual void dragTo (const vecF& pos) { }
	

/* Optional hooks for subclasses to override */
	virtual void onMouseMove (const vecF& pos) { }

	virtual void onMouseExit () { }
	
	virtual void onMouseEnter () { }
	
	virtual void onMouseDown (const vecF& pos) { }

	virtual void startDrag (const vecF& pos) { }
	
	/* (clickFunc is already called) */
	virtual void onMouseUp (const vecF& pos) { }

	virtual void endDrag (const vecF& pos) { }

	virtual void onRightMouseDown (const vecF& pos) { }
	
	/* (rightClickFunc is already called) */
	virtual void onRightMouseUp (const vecF& pos) { }

};



class MouseRespondingSprite: public ZSprite, public MouseResponder
{
public:
	MouseRespondingSprite (const Texture& tex) : ZSprite(tex) { }
	
	MouseRespondingSprite (const MouseSpriteConfig& cfg)
		: ZSprite(*cfg.sfCfg.texture)
	{
		initMouseSprite(this, cfg);
	}

	void draw (RenderTarget& target, RenderStates st) const override
	{
		ZSprite::draw(target, st);
	}
	
	bool containsCoords (const vecF& pos) override
	{
		return gGB().contains(pos);
	}
	
	void storeClickOffset (const vecF& pos) override
	{
		clickOffset = getPosition() - pos;
	}

	void dragTo (const vecF& pos) override
	{
		setPosition(pos + clickOffset);
	}
};



template<typename T>
class MouseRespondingShape: public MouseResponder
{
public:
	MouseRespondingShape<T> () = default;

	MouseRespondingShape<T> (const MouseRectConfig& cfg)
	{
		initMouseRect(this, cfg);
	}
	
	MouseRespondingShape<T> (const MouseCircleConfig& cfg)
	{
		initMouseCircle(this, cfg);
	}

	void draw (RenderTarget& target, RenderStates st) const override
	{
		target.draw(shape, st);
	}
	
	bool containsCoords (const vecF& pos) override
	{
		return shape.gGB().contains(pos);
	}
	
	void storeClickOffset (const vecF& pos) override
	{
		clickOffset = shape.getPosition() - pos;
	}
	
	void dragTo (const vecF& pos) override
	{
		this->shape.setPosition(pos + this->clickOffset);
	}
	
	T			shape;
};



class MouseRespondingText: public MouseResponder
{
public:
	MouseRespondingText (const Font& font) : txt(font) { }
	
	MouseRespondingText (const MouseTextConfig& cfg)
		: txt(*cfg.sfCfg.font)
	{
		initMouseText(this, cfg);
	}
	
	void draw (RenderTarget& target, RenderStates st) const override
	{
		target.draw(txt, st);
	}
	
	bool containsCoords (const vecF& pos) override
	{
		return txt.gGB().contains(pos);
	}
	
	void storeClickOffset (const vecF& pos) override
	{
		clickOffset = txt.getPosition() - pos;
	}
	
	void dragTo (const vecF& pos) override
	{
		txt.setPosition(pos + clickOffset);
	}
	
	Text			txt;
};

using MouseRespondingRect = MouseRespondingShape<RectangleShape>;
using MouseRespondingCircle = MouseRespondingShape<CircleShape>;

//===============================================================

inline void initMouseResponder (MouseResponder* resp, const MouseResponderConfig& cfg)
{
	resp->clickFunc = cfg.clickFunc;
	resp->rightClickFunc = cfg.rightClickFunc;
	resp->isClickable = (cfg.clickFunc ? true : cfg.clickable);
	resp->isRightClickable = (cfg.rightClickFunc ? true : cfg.clickable);
	resp->mouseEnterFunc = cfg.mouseEnterFunc;
	resp->mouseExitFunc = cfg.mouseExitFunc;
	resp->drawingOrder = cfg.drawingOrder;
	resp->responseOrder = cfg.responseOrder;
	resp->needsExplicitDrawCall = cfg.needsExplicitDrawCall;
	resp->propagateClick_ = cfg.propagateClick;
	resp->isDraggable = cfg.draggable;
	resp->isRightDraggable = cfg.rightDraggable;
}

inline void initMouseSprite (MouseResponder* resp, const MouseSpriteConfig& cfg)
{
	if (auto spr = dynamic_cast<MouseRespondingSprite*>(resp)) {
		initSprite(spr->s, cfg.sfCfg);
		initMouseResponder(resp, cfg.mrCfg);
	}
}

inline void initMouseText (MouseResponder* resp, const MouseTextConfig& cfg)
{
	if (auto text = dynamic_cast<MouseRespondingText*>(resp)) {
		initText(text->txt, cfg.sfCfg);
		initMouseResponder(resp, cfg.mrCfg);
	}
}

inline void initMouseRect (MouseResponder* resp, const MouseRectConfig& cfg)
{
	if (auto rect = dynamic_cast<MouseRespondingShape<RectangleShape>*>(resp)) {
		initRect(rect->shape, cfg.sfCfg);
		initMouseResponder(resp, cfg.mrCfg);
	}
}

inline void initMouseCircle (MouseResponder* resp, const MouseCircleConfig& cfg)
{
	if (auto circ = dynamic_cast<MouseRespondingShape<CircleShape>*>(resp)) {
		initCircle(circ->shape, cfg.sfCfg);
		initMouseResponder(resp, cfg.mrCfg);
	}
}

#endif /* MOUSERESPONDER_HPP */
