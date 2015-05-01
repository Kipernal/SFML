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

#ifndef SFML_STENCILSETTINGS_HPP
#define SFML_STENCILSETTINGS_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>


namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Stencil settings for drawing
///
////////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API StencilSettings
{
    ////////////////////////////////////////////////////////
    /// \brief Enumeration of the stencil operations
    ///
    /// The operations are mapped directly to their OpenGL equivalents,
    /// specified by glStencilOp() or glStencilOpSeparate().
    ////////////////////////////////////////////////////////
    enum Operation
    {
        Keep,             ///< Doesn't change the current stencil bits
        Zero,             ///< Sets the stencil bits to zero
        Replace,          ///< Replaces the stencil bits with a custom value
        IncrementClamp,   ///< Increments the stencil bits without wrapping
        DecrementClamp,   ///< Decrements the stencil bits without wrapping
        Invert,           ///< Bitwise inverts the stencil bits
    };

    ////////////////////////////////////////////////////////
    /// \brief Enumeration of the stencil functions
    ///
    /// The equations are mapped directly to their OpenGL equivalents,
    /// specified by glStencilFunc() or glStencilFuncSeparate().
    ////////////////////////////////////////////////////////
    enum Function
    {
        Never,        ///< The stencil test never succeeds
        Less,         ///< Passes if the value is less than the stencil bits
        LessEqual,    ///< Passes if the value is less than or equal to the stencil bits
        Greater,      ///< Passes if the value is greater than the stencil bits
        GreaterEqual, ///< Passes if the value is greater than or equal to the stencil bits
        Equal,        ///< Passes if the value is strictly equal to the stencil bits
        NotEqual,     ///< Passes if the value is strictly inequal to the stencil bits
        Always        ///< Always passes
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs stencil settings that allows for stencil operations
    ///
    ////////////////////////////////////////////////////////////
    StencilSettings();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the stencil settings given the factors and equation.
    ///
    /// \param stencilFunction        Specifies the test that determines if we write to the stencil buffer
    /// \param stencilOperation       Specifies what we do to the buffer if we write to it
    /// \param stencilReference       Specifies what value in the stencil buffer we write and compare against for stencilFunction
    /// \param alphaFunction          Specifies how we determine which alpha values contribute to the stencil buffer
    /// \param alphaReference         Specifies what alpha value we compare against for alphaFunction
    /// \param drawSelf               Specifies whether or not we draw to the color buffer in addition to the stencil buffer
    ///
    ////////////////////////////////////////////////////////////
    StencilSettings(Function stencilFunction, Operation stencilOperation, unsigned int stencilReference,
                    Function alphaFunction, Uint8 alphaReference, bool drawSelf);

    ////////////////////////////////////////////////////////////
    /// \brief Apply the stencil settings.
    ///
    /// Thus function applies the relevant OpenGL operations based on the
    /// StencilSetting's member variables.
    ///
    ////////////////////////////////////////////////////////////
    void apply() const;

    ////////////////////////////////////////////////////////////
    // Member Data
    ////////////////////////////////////////////////////////////
    Operation stencilOperation;    ///< The operation we're using when writing to the stencil buffer 
    Function stencilFunction;      ///< The function we're using when doing comparisons on the stencil buffer
    unsigned int stencilReference; ///< The stencil value we're comparing against when doing comparisons on the stencil buffer
    Function alphaFunction;        ///< The function we're using when doing alpha comparisons
    Uint8 alphaReference;          ///< The alpha value we're comparing against when doing alpha comparisons
    bool drawSelf;                 ///< Whether we should draw onto the screen in addition to the stencil buffer



private:

    ////////////////////////////////////////////////////////////
    /// \brief Convert an Operation constant to the corresponding OpenGL constant.
    ///
    ////////////////////////////////////////////////////////////
    int translateOperation(Operation op) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a Function constant to the corresponding OpenGL constant.
    ///
    ////////////////////////////////////////////////////////////
    int translateFunction(Function func) const;
};

////////////////////////////////////////////////////////////
/// \relates StencilSettings
/// \brief Overload of the == operator
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if stencil settings are equal, false if they are different
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator ==(const StencilSettings& left, const StencilSettings& right);

////////////////////////////////////////////////////////////
/// \relates StencilSettings
/// \brief Overload of the != operator
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if stencil settings are different, false if they are equal
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator !=(const StencilSettings& left, const StencilSettings& right);



////////////////////////////////////////////////////////////
// Commonly used stencils
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API extern const StencilSettings StencilCreate;    ///< Draw onto a stencil instead of the screen
SFML_GRAPHICS_API extern const StencilSettings StencilTrace;     ///< Draw only where there's no stencil
SFML_GRAPHICS_API extern const StencilSettings StencilDisable;   ///< "Turns off" the stencil and draws normally


} // namespace sf


#endif // SFML_STENCILSETTINGS_HPP


  ////////////////////////////////////////////////////////////
  /// TODO: Add description here
  ////////////////////////////////////////////////////////////
