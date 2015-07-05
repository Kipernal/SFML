#include <vector>
#include <assert.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sf
{
    class BatchTarget : public RenderTarget, public Drawable
    {
    public:

        BatchTarget()
        {
            clear();
            initialize();
            clearView();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Clears the view on this BatchTarget
        ///
        /// Because batch targets have no size, they also have no
        /// default view.  Thus, setView(getDefaultView()) will
        /// not work as expected.  clearView() will remove any
        /// view that has been set to this batch target.  Note that
        /// like setView, this function cannot be called if draw has
        /// already been called between now and the last call to clear.
        ///
        /// \see setView, getView
        ///
        ////////////////////////////////////////////////////////////
        void clearView()
        {
            m_viewWasUsed = false;
            m_cache.viewChanged = false;
        }


        ////////////////////////////////////////////////////////////
        /// \brief Draw primitives defined by an array of vertices
        ///
        /// Note that between calling this function and calling clear,
        /// no settings, such as view or texture, can be changed.
        /// This means, for example, that if you draw a bunch of sprites
        /// to this object, they must all share the same texture.
        /// In addition, certain different types of objects may be
        /// incompatible with each other--just like all objects must
        /// share the same texture they must all also fall under the
        /// same braod category of triangle, line, or point.  All
        /// built-in SFML types use triangles, however, so when
        /// using sprites, text, and shapes (including shapes with
        /// an outline), this issue will never come up.
        ///
        /// \param vertices    Pointer to the vertices
        /// \param vertexCount Number of vertices in the array
        /// \param type        Type of primitives to draw
        /// \param states      Render states to use for drawing
        ///
        ////////////////////////////////////////////////////////////
        virtual void draw(const Vertex* vertices, std::size_t vertexCount,
                          PrimitiveType type, const RenderStates& states = RenderStates::Default)
        {
            // First figure out if we need to modify our primitive type.
            // If we were passed TrianglesStrip (sf::Sprite does this), for example, then drawing separate entities will cause problems.
            // Drawing two of these sf::Sprites would cause them to be "glued" together.
            // Thus, we need to detatch "composite" primitive types:
            // sf::LinesStrip -> sf::Lines
            // sf::TrianglesStrip -> sf::Triangles
            // sf::TrianglesFan -> sf::Triangles
            // A bit more work, but on the plus side, this means that more types can be drawn at once without needing to clear and start over.

            // Keep the original type--we'll need it later on to determine which vertices, if any, need duplication.
            PrimitiveType originalType = type;
            if (originalType == sf::LinesStrip)
                type = sf::Lines;
            else if (originalType == sf::TrianglesStrip)
                type = sf::Triangles;
            else if (originalType == sf::TrianglesFan)
                type = sf::Triangles;

            // For BatchTargets, most settings can only be changed before any drawing has begun,
            // or on the very first draw call for RenderStates settings.
            // If we haven't drawn anything already, then store the settings in our cache.
            // Otherwise check to make sure we haven't changed anything.
            if (m_hasDrawnSomething)
            {
                assert(!m_cache.viewChanged && "The view has changed--only one view can be used for all objects drawn to this target.");
                assert(m_cachedPrimitiveType == type && "The vertex primitive type has changed--only one type can be used for all objects drawn to this target.");
                assert((states.texture? states.texture->getNativeHandle() : 0) == m_cache.lastTextureId && "The texture has changed--only one texture can be used for all objects drawn to this target");
                assert(m_cache.lastBlendMode == states.blendMode && "The blend mode has changed--only one blend mode can be used for all objects drawn to this target");

                // We cannot compare shaders, so we just have to trust the user here.

                // Transforms are not checked, because they're almost always guaranteed to change between calls so we handle them on the CPU.
            }
            else
            {
                // Record now that we've drawn something--from this point out no further changes are allowed until clear() is called.
                m_hasDrawnSomething = true;

                // Record if we've used a view.
                // Views are not actually used until the BatchTarget itself is drawn.
                if (m_cache.viewChanged)
                    m_viewWasUsed = true;
                m_cache.viewChanged = false;

                // Set which texture we'll be using.
                // Note that we can't simply use m_cache.texture because that only records the ID. We need the whole texture.
                m_cachedTexture = states.texture;
                m_cache.lastTextureId = states.texture? states.texture->getNativeHandle() : 0;

                // Finally, which primitive type we're using (this is the decomposite type).
                m_cachedPrimitiveType = type;
            }

            // Keep track of the first vertex we are going to insert--used for TrianglesFan.
            std::size_t firstVectorInsertedIndex = -1;

            for (std::size_t i = 0; i < vertexCount; i++)
            {
                // If we're drawing one of the "composite" types, then add in the extra vertices.
                if (originalType == sf::PrimitiveType::LinesStrip && i > 0)
                {
                    // Copy the previous vertex and use it again here to start a new line.
                    m_vertexArray.push_back(m_vertexArray.back());
                }
                else if (originalType == sf::PrimitiveType::TrianglesStrip && i > 2)
                {
                    // Copy the last two vertices and use them again here to start a new triangle
                    std::size_t lastIndex = m_vertexArray.size() - 1;
                    std::size_t secondToLastIndex = m_vertexArray.size() - 2;
                    m_vertexArray.push_back(m_vertexArray[secondToLastIndex]);
                    m_vertexArray.push_back(m_vertexArray[lastIndex]);
                }
                else if (originalType == sf::PrimitiveType::TrianglesFan && i > 2)
                {
                    // Copy the first and last vertices and use them again here to start a new triangle
                    std::size_t lastIndex = m_vertexArray.size() - 1;
                    m_vertexArray.push_back(m_vertexArray[firstVectorInsertedIndex]);
                    m_vertexArray.push_back(m_vertexArray[lastIndex]);
                }

                // Now transform and append our new vertex.
                Vertex transformedVertex = vertices[i];
                transformedVertex.position = states.transform * transformedVertex.position;
                m_vertexArray.push_back(transformedVertex);

                // Record the index of this vertex
                // (Used for TrianglesFan, which needs to know this information)
                if (firstVectorInsertedIndex == -1)
                    firstVectorInsertedIndex = m_vertexArray.size() - 1;
            }
        }

        ////////////////////////////////////////////////////////////
        /// \brief Return the size of the rendering region of the target
        ///
        /// As a BatchTarget does not represent an actual target,
        /// it does not actually have a size and will always return
        /// the same value of (0, 0).
        ///
        /// \return Size in pixels
        ///
        ////////////////////////////////////////////////////////////
        virtual Vector2u getSize() const
        {
            return Vector2u(0, 0);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Activate the target for rendering
        ///
        /// \param active True to make the target active, false to deactivate it
        ///
        /// \return True if the function succeeded
        ///
        ////////////////////////////////////////////////////////////
        virtual bool activate(bool active)
        {
            return true;
        }



        ////////////////////////////////////////////////////////////
        /// \brief Draw a drawable object to the render target
        ///
        /// Note that between calling this function and calling clear,
        /// no settings, such as view or texture, can be changed.
        /// This means, for example, that if you draw a bunch of sprites
        /// to this object, they must all share the same texture.
        /// In addition, certain different types of objects may be
        /// incompatible with each other--just like all objects must
        /// share the same texture they must all also fall under the
        /// same braod category of triangle, line, or point.  All
        /// built-in SFML types use triangles, however, so when
        /// using sprites, text, and shapes (including shapes with
        /// an outline), this issue will never come up.
        ///
        /// \param drawable Object to draw
        /// \param states   Render states to use for drawing
        ///
        //////////////////////////////////////////////////////////// 
        void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default)
        {
                RenderTarget::draw(drawable, states);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Draw the BatchTarget to a render target
        ///
        /// This effectively takes all cumulative draws to this object
        /// and draws them all at once to another render target.
        ///
        /// \param target Render target to draw to
        /// \param states Current render states
        ///
        ////////////////////////////////////////////////////////////
        virtual void draw(RenderTarget& target, RenderStates states) const
        {
            auto backupView = target.getView();
            if (m_viewWasUsed)
                target.setView(getView());
            else
                target.setView(target.getDefaultView());

            states.texture = m_cachedTexture;
            states.blendMode = m_cache.lastBlendMode;

            target.draw(m_vertexArray.data(), m_vertexArray.size(), m_cachedPrimitiveType, states);

            if (m_viewWasUsed)
                target.setView(backupView);
        }


        ////////////////////////////////////////////////////////////
        /// \brief Clear the BatchTarget
        ///
        /// This function clears all batched information, including
        /// settings. This means that after calling this you are
        /// free to use a different texture with your sprites (though
        /// they must still all share the same texture--that part of
        /// course doesn't change).
        ///
        /// \param color Ignored--BatchTargets have no notion of a background color to clear.
        ///
        ////////////////////////////////////////////////////////////
        virtual void clear(const Color& color = Color(0, 0, 0, 255))
        {
            m_hasDrawnSomething = false;
            m_cachedTexture = NULL;
            m_vertexArray.clear();
        }

        ////////////////////////////////////////////////////////////
        /// \brief Reserves memory for batched vertex information
        ///
        /// Use this function if you think you can predict in advance
        /// how many (or roughly how any) vertices all your objects
        /// that you draw to this object will use.  Note that even
        /// after calling clear the allocated memory will not change,
        /// so you only need to call this function once.
        ///
        ////////////////////////////////////////////////////////////
        void reserve(std::size_t vertexCount)
        {
            m_vertexArray.reserve(vertexCount);
        }

    private:

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        bool                    m_hasDrawnSomething;   ///< Have we drawn anything since clear()?
        bool                    m_viewWasUsed;         ///< Are we using a view?
        const sf::Texture*      m_cachedTexture;       ///< What texture are we using?
        sf::PrimitiveType       m_cachedPrimitiveType; ///< What decomposed primitive type are we using?
        std::vector<sf::Vertex> m_vertexArray;         ///< The vertices we use to draw.

    };
}