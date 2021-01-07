#pragma once

#include <cstring>

#include "utils.h"

/// \class Vector
///
/// A dynamically re-sizable, homogenous array.
///
/// \tparam ValueT The type of each element.
template<typename ValueT>
class Vector
{
public:
    /// \typedef value_type
    ///
    /// The value type of each element.
    using value_type = ValueT;

    // -----------------------------------------------------------------------
    /// \name Construction
    // -----------------------------------------------------------------------

    /// Constructs an empty vector.
    Vector() {}

    /// Destroys the vector.
    ~Vector() { _Reset(); }

    /// Copy constructor.
    Vector(const Vector& src) { _DeepCopyFrom(src); }

    /// Copy assignment operator.
    Vector& operator=(const Vector& src) { _DeepCopyFrom(src); }

    // -----------------------------------------------------------------------
    /// \name Capacity
    // -----------------------------------------------------------------------

    /// Check if the vector contains no elements.
    ///
    /// \retval true If the vector is empty.
    /// \retval false Otherwise.
    bool empty() const noexcept { return m_size == 0; }

    /// Get the number of elements contained in this vector.
    ///
    /// \return Number of elements.
    size_t size() const { return m_size; }

    /// Update the capacity of this vector.
    ///
    /// \param count Number of elements to update the capacity to.
    void reserve(size_t count)
    {
        if (count > m_capacity) {
            _Realloc(count);
        }
    };

    /// Get the number of elements which can be contained in the current
    /// allocation.
    ///
    /// \return Number of elements allocated.
    size_t capacity() const { return m_capacity; }

    /// Clear un-used capacity allocated for this vector.
    void shrink_to_fit()
    {
        if (m_size == 0) {
            _Reset();
        } else if (m_size < m_capacity) {
            _Realloc(m_size);
        }
    }

    // -----------------------------------------------------------------------
    /// \name Modifiers
    // -----------------------------------------------------------------------

    /// Clear all elements from the vector.
    void clear()
    {
        for (size_t index = 0; index < m_size; ++index) {
            m_buffer[index].~value_type();
        }

        m_size = 0;
    }

    /// Resize the vector to contain \p count number of elements.
    ///
    /// \param count The number of elements.
    void resize(size_t count)
    {
        if (count > m_capacity) {
            _Realloc(count);

            // Call constructor on new elements.
            if (count > m_size) {
                for (size_t index = m_size; index < count; ++index) {
                    new (m_buffer + index) value_type();
                }
            }
        } else if (count < m_size) {
            for (size_t index = count; index < m_size; ++index) {
                m_buffer[index].~value_type();
            }
        }

        m_size = count;
    }

    /// Resize the vector to contain \p count number of elements, appending
    /// default-initialized \p value when the vector increases in size.
    ///
    /// \param count The number of elements.
    /// \param value The default initialized value.
    void resize(size_t count, const value_type& value)
    {
        // XXX: How to fix this code dupe??
        if (count > m_capacity) {
            _Realloc(count);

            // Initialize default values.
            if (count > m_size) {
                for (size_t index = m_size; index < count; ++index) {
                    m_buffer[index] = value;
                }
            }
        } else if (count < m_size) {
            for (size_t index = count; index < m_size; ++index) {
                m_buffer[index].~value_type();
            }
        }

        m_size = count;
    }

    // -----------------------------------------------------------------------
    /// \name Access
    // -----------------------------------------------------------------------

    /// Constant, indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    const value_type& operator[](size_t index) const { return m_buffer[index]; }

    /// Mutable, indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    value_type& operator[](size_t index) { return m_buffer[index]; }

private:
    // Shared functionality for copying a source Vector to this one.
    void _DeepCopyFrom(const Vector& src)
    {
        // Increase capacity if required.
        if (src.m_size > m_capacity) {
            _Realloc(src.m_size);
        }

        // Update members.
        m_size = src.m_size;

        // Copy over elements.
        if (m_size > 0) {
            _CopyBuffer(src.m_buffer, src.m_size, m_buffer, m_size);
        }
    }

    // Reallocate a new buffer, replacing the old count.
    void _Realloc(size_t count)
    {
        // Allocate new buffer.
        value_type* newBuffer =
            static_cast<value_type*>(malloc(sizeof(value_type) * count));
        if (m_buffer != nullptr) {
            _CopyBuffer(m_buffer, m_size, newBuffer, count);
            delete m_buffer;
        }
        m_buffer = newBuffer;
        m_capacity = count;
    }

    // Copy memory from srcBuffer to dstBuffer.
    // Src and dst sizes are also provided as
    static void _CopyBuffer(value_type* srcBuffer,
                            size_t srcSize,
                            value_type* dstBuffer,
                            size_t dstSize)
    {
        size_t copyCount = std::min(srcSize, dstSize);
        memcpy(dstBuffer, srcBuffer, copyCount * sizeof(value_type));
    }

    // Reset internal members to default state.  Memory will be freed.
    void _Reset()
    {
        // Call deconstructor on elements.
        for (size_t index = 0; index < m_size; ++index) {
            m_buffer[index].~value_type();
        }

        if (m_buffer != nullptr) {
            delete m_buffer;
            m_buffer = nullptr;
        }

        m_size = 0;
        m_capacity = 0;
    }

    // Number of elements that this vector currently contains.
    size_t m_size = 0;

    // Number of elements that this vector can contain
    // without needing to resize.
    size_t m_capacity = 0;

    // Pointer to the block of memory.
    value_type* m_buffer = nullptr;
};
