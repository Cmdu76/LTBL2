#include "LightShape.hpp"
#include "LightSystem.hpp"

namespace ltbl
{

LightShape::LightShape(LightSystem& system)
	: mSystem(system)
	, mRenderLightOver(true)
{
}

void LightShape::setRenderLightOver(bool renderLightOver)
{
	mRenderLightOver = renderLightOver;
}

bool LightShape::renderLightOver() const
{
	return mRenderLightOver;
}

sf::FloatRect LightShape::getAABB() const
{
	return getGlobalBounds();
}

void LightShape::remove()
{
	mSystem.removeShape(this);
}

void LightShape::setTurnedOn(bool turnedOn)
{
	mTurnedOn = turnedOn;
}

bool LightShape::isTurnedOn() const
{
	return mTurnedOn;
}

void LightShape::toggleTurnedOn()
{
	mTurnedOn = !mTurnedOn;
}

} // namespace lum