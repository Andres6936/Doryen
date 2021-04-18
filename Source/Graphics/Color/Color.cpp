/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmath>
#include <algorithm>

#include <Doryen/Graphics/Color/ColorTranslator.hpp>
#include "Doryen/Graphics/Color/Color.hpp"

using namespace Doryen;

// Getters

std::uint8_t Color::getRed() const
{
	return r;
}

std::uint8_t Color::getGreen() const
{
	return g;
}

std::uint8_t Color::getBlue() const
{
	return b;
}

std::uint8_t Color::getAlpha() const
{
	return a;
}

// Setters

void Color::setRed(const std::uint8_t _r)
{
	r = _r;
}

void Color::setGreen(const std::uint8_t _g)
{
	g = _g;
}

void Color::setBlue(const std::uint8_t _b)
{
	b = _b;
}

void Color::setAlpha(const std::uint8_t _a)
{
	a = _a;
}

Color::Color(std::int32_t r, std::int32_t g, std::int32_t b) noexcept: Color(r, g, b, 255)
{
	// Delegate the construction of Color to another construct
}

Color::Color() noexcept
{
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}

Color::Color(std::int32_t r, std::int32_t g, std::int32_t b, std::int32_t a) noexcept
{
	// Invariants (Values lesser that zero)
	if (r < 0) this->r = 0;
	if (g < 0) this->g = 0;
	if (b < 0) this->b = 0;
	if (a < 0) this->a = 0;

	// Invariant (Values greater that 255)
	this->r = std::min(255, r);
	this->g = std::min(255, g);
	this->b = std::min(255, b);
	this->a = std::min(255, a);
}

void Color::copy(const Color& c) noexcept
{
	this->setRed(c.getRed());
	this->setGreen(c.getGreen());
	this->setBlue(c.getBlue());
	this->setAlpha(c.getAlpha());
}

bool Color::equals(const Color& c) const noexcept
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

void Color::trasformColor(const Color& another, Doryen::BlendModes flag) noexcept
{
	std::int32_t nr = 0;
	std::int32_t ng = 0;
	std::int32_t nb = 0;

	switch (flag)
	{

		// Noping
	case BlendModes::NONE:
	case BlendModes::DEFAULT:
		break;

	case BlendModes::SET:

		// Copy information color
		this->r = another.r;
		this->g = another.g;
		this->b = another.b;
		break;

	case BlendModes::MULTIPLY:

		this->multiply(another);
		break;

	case BlendModes::LIGHTEN:

		this->r = std::max(this->r, another.r);
		this->g = std::max(this->g, another.g);
		this->b = std::max(this->b, another.b);
		break;

	case BlendModes::DARKEN:

		this->r = std::min(this->r, another.r);
		this->g = std::min(this->g, another.g);
		this->b = std::min(this->b, another.b);
		break;

	case BlendModes::SCREEN:

		this->r = (std::uint8_t)(255 - (255 - this->r) * (255 - another.r) / 255);
		this->g = (std::uint8_t)(255 - (255 - this->g) * (255 - another.g) / 255);
		this->b = (std::uint8_t)(255 - (255 - this->b) * (255 - another.b) / 255);
		break;

	case BlendModes::COLOR_DODGE:

		// The component in R, G or B should be different of 255 for avoid division by zero
		if (this->r not_eq 255) this->r = std::min(255, (255 * another.r) / (255 - this->r));
		if (this->g not_eq 255) this->g = std::min(255, (255 * another.g) / (255 - this->g));
		if (this->b not_eq 255) this->b = std::min(255, (255 * another.b) / (255 - this->b));

		break;

	case BlendModes::COLOR_BURN:

		// The component in R, G or B should be greater that 0 for avoid division by zero
		if (another.r > 0) this->r = std::clamp((255 - (255 * (255 - this->r)) / another.r), 0, 255);
		if (another.g > 0) this->g = std::clamp((255 - (255 * (255 - this->g)) / another.g), 0, 255);
		if (another.b > 0) this->b = std::clamp((255 - (255 * (255 - this->b)) / another.b), 0, 255);

		break;

	case BlendModes::ADD:

		this->add(another);
		break;

	case BlendModes::ADDA:

		// TODO: Implemented Alpha Sum
		this->add(another);
		break;

	case BlendModes::BURN:

		this->r = std::clamp(this->r + another.r - 255, 0, 255);
		this->g = std::clamp(this->g + another.g - 255, 0, 255);
		this->b = std::clamp(this->b + another.b - 255, 0, 255);
		break;

	case BlendModes::OVERLAY:

		if (another.r <= 128)
		{
			nr = 2 * (another.r) * this->r / 255;
		}
		else
		{
			nr = 255 - 2 * (255 - another.r) * (255 - this->r) / 255;
		}

		if (another.g <= 128)
		{
			ng = 2 * (another.g) * this->g / 255;
		}
		else
		{
			ng = 255 - 2 * (255 - another.g) * (255 - this->g) / 255;
		}

		if (another.b <= 128)
		{
			nb = 2 * (another.b) * this->b / 255;
		}
		else
		{
			nb = 255 - 2 * (255 - another.b) * (255 - this->b) / 255;
		}

		this->r = std::clamp(nr, 0, 255);
		this->g = std::clamp(ng, 0, 255);
		this->b = std::clamp(nb, 0, 255);
		break;

	case BlendModes::ALPH:

		// TODO: Implemented Alpha
		break;
	}
}

void Color::multiply(const Color& other) noexcept
{
	this->r = std::min(255, this->r * other.r / 255);
	this->g = std::min(255, this->g * other.g / 255);
	this->b = std::min(255, this->b * other.b / 255);
}

void Color::multiply(float value) noexcept
{
	// Resolver invariant
	if (value < 0.0f) value = 0.0f;

	// Needed the static cast for avoid overflow problem
	// Needed that value will be greater that zero (value > 0.0f),
	// because the component r, g and b are positives.
	this->r = std::min(255, static_cast<std::int32_t>(this->r * value));
	this->g = std::min(255, static_cast<std::int32_t>(this->g * value));
	this->b = std::min(255, static_cast<std::int32_t>(this->b * value));
}

void Color::add(const Color& other) noexcept
{
	this->r = std::min(255, this->r + other.r);
	this->g = std::min(255, this->g + other.g);
	this->b = std::min(255, this->b + other.b);
}

void Color::addRed(const std::int32_t red) noexcept
{
	// In this point, the componentRed can be greater than 255 or lesser
	// than 0 (A value negative).
	const std::int32_t componentRed = this->getRed() + red;
	// Apply clamp operation above the componentRed for limit the range
	// of it to [0, 255] (Inclusive).
	this->setRed(std::clamp(componentRed, 0, 255));
}

Color::Color(const Color& _rhs) noexcept
{
	this->copy(_rhs);
}

void Color::operator=(const Color& _rhs) noexcept
{
	this->copy(_rhs);
}

std::string Color::toString() const
{
	return { '{' + std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b) + '}' };
}

Color Color::fromString(std::string_view string)
{
	return ColorTranslator::fromHTML(string);
}

Color Color::lerp(const Color& a, const Color& b, float coefficient)
{
	const std::int32_t cR = (a.r + (b.r - a.r) * coefficient);
	const std::int32_t cG = (a.g + (b.g - a.g) * coefficient);
	const std::int32_t cB = (a.b + (b.b - a.b) * coefficient);

	return { cR, cG, cB };
}
