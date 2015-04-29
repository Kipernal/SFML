////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderStates.hpp>
#include <cstddef>


namespace sf
{
////////////////////////////////////////////////////////////
// We cannot use the default constructor here, because it accesses BlendAlpha, which is also global (and dynamically
// initialized). Initialization order of global objects in different translation units is not defined.
// For similar reasons we need to provide the default stencil settings, and thus all other settings along with it
const RenderStates RenderStates::Default(
    BlendMode(BlendMode::SrcAlpha, BlendMode::OneMinusSrcAlpha, BlendMode::Add, BlendMode::One, BlendMode::OneMinusSrcAlpha, BlendMode::Add),
    StencilSettings(StencilSettings::Always, StencilSettings::Keep, 0, StencilSettings::GreaterEqual, 0, true),
    Transform(),
    NULL,
    NULL);


////////////////////////////////////////////////////////////
RenderStates::RenderStates() :
blendMode(BlendAlpha),
stencilSettings(),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Transform& theTransform) :
blendMode(BlendAlpha),
stencilSettings(),
transform(theTransform),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode) :
blendMode(theBlendMode),
stencilSettings(),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const StencilSettings& theStencilSettings) :
blendMode(BlendAlpha),
stencilSettings(theStencilSettings),
transform(),
texture(NULL),
shader(NULL)
{}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Texture* theTexture) :
blendMode(BlendAlpha),
stencilSettings(),
transform(),
texture  (theTexture),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Shader* theShader) :
blendMode(BlendAlpha),
stencilSettings(),
transform(),
texture  (NULL),
shader   (theShader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode, const Transform& theTransform,
                           const Texture* theTexture, const Shader* theShader) :
blendMode(theBlendMode),
stencilSettings(),
transform(theTransform),
texture  (theTexture),
shader   (theShader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode, const StencilSettings& theStencilSettings,
                           const Transform& theTransform, const Texture* theTexture, const Shader* theShader) :
blendMode(theBlendMode),
stencilSettings(theStencilSettings),
transform(theTransform),
texture(theTexture),
shader(theShader)
{}

} // namespace sf
