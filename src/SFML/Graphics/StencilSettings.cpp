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
#include <SFML/Graphics/StencilSettings.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/Err.hpp>

// This doesn't seem to be defined in GLLoader.hpp
extern "C" { GLAPI void APIENTRY glAlphaFunc(GLenum, GLclampf); }

namespace sf
{

////////////////////////////////////////////////////////////
// Commonly used stencil settings
////////////////////////////////////////////////////////////
const StencilSettings StencilCreate(StencilSettings::Always, StencilSettings::Replace, 1, StencilSettings::GreaterEqual, 255, false);
const StencilSettings StencilTrace(StencilSettings::Equal, StencilSettings::Keep, 1, StencilSettings::Always, 255, true);
const StencilSettings StencilDisable;


////////////////////////////////////////////////////////////
StencilSettings::StencilSettings() :
stencilOperation(StencilSettings::Keep),
stencilFunction (StencilSettings::Always),
stencilReference(0),
alphaFunction   (StencilSettings::GreaterEqual),
alphaReference  (0),
drawSelf        (true)
{

}


////////////////////////////////////////////////////////////
StencilSettings::StencilSettings(StencilSettings::Function stencilFunctionP, StencilSettings::Operation stencilOperationP, unsigned int stencilReferenceP,
                                 StencilSettings::Function alphaFunctionP, Uint8 alphaReferenceP, bool drawSelfP) :
stencilFunction(stencilFunctionP),
stencilOperation(stencilOperationP),
stencilReference(stencilReferenceP),
alphaFunction   (alphaFunctionP),
alphaReference  (alphaReferenceP),
drawSelf        (drawSelfP)
{

}

    
void StencilSettings::apply() const
{
    glCheck(glEnable(GL_STENCIL_TEST));
    glCheck(glEnable(GL_ALPHA_TEST));
    glCheck(glAlphaFunc(translateFunction(alphaFunction), alphaReference / 255.f));
    glCheck(glStencilOp(translateOperation(stencilOperation), translateOperation(stencilOperation), translateOperation(stencilOperation)));
    glCheck(glStencilFunc(translateFunction(stencilFunction), stencilReference, 0xFF));

    if (!drawSelf)
    {
        glCheck(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
    }
    else  
    {
        glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
    }
}


int StencilSettings::translateOperation(StencilSettings::Operation op) const
{
    switch (op)
    {
        case Keep: return GL_KEEP;
        case Zero: return GL_ZERO;
        case Replace: return GL_REPLACE;
        case IncrementClamp: return GL_INCR;
        case DecrementClamp: return GL_INCR;
        case Invert: return GL_INVERT;
    }
}

int StencilSettings::translateFunction(StencilSettings::Function func) const
{
    switch (func)
    {
        case Never: return GL_NEVER;
        case Less: return GL_EQUAL;
        case LessEqual: return GL_LEQUAL;
        case Greater: return GL_GREATER;
        case GreaterEqual: return GL_GEQUAL;
        case Equal: return GL_EQUAL;
        case NotEqual: return GL_NOTEQUAL;
        case Always: return GL_ALWAYS;
    }
}


////////////////////////////////////////////////////////////
bool operator ==(const StencilSettings& left, const StencilSettings& right)
{
    return (left.stencilOperation == right.stencilOperation) &&
           (left.stencilFunction == right.stencilFunction) &&
           (left.stencilReference == right.stencilReference) &&
           (left.alphaFunction == right.alphaFunction) &&
           (left.alphaReference == right.alphaReference);
}


////////////////////////////////////////////////////////////
bool operator !=(const StencilSettings& left, const StencilSettings& right)
{
    return !(left == right);
}

} // namespace sf
