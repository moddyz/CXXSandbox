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

    /// Get the number of elements in this vector.
    ///
    /// \return Number of elements.
    size_t size() const { return m_size; }

    /// Get the number of elements allocated by this vector,
    /// which is also the maximum number of elements this vector
    /// can hold without needing to resize.
    ///
    /// \return Number of elements allocated.
    size_t capacity() const { return m_capacity; }

    /// Resize the vector to contain \p count number of elements.
    ///
    /// \param count The number of elements.
    void resize(size_t count)
    {
        // No-op if specified count is less-than-or-equal current capacity.
        if (count > m_capacity) {
            value_type* newBuffer = AllocAndMigrate(count);
            free(m_buffer);

            // Update internal members.
            m_buffer = newBuffer;
            m_capacity = count;
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
        // XXX: How to elegantly clean up this code dupe?

        // No-op if specified count is less-than-or-equal current capacity.
        if (count > m_capacity) {
            value_type* newBuffer = AllocAndMigrate(count);
            free(m_buffer);
            _InitializeDefaultValues(count, value, newBuffer);

            // Update internal members.
            m_buffer = newBuffer;
            m_capacity = count;
        }

        m_size = count;
    }

    /// Update the capacity of this vector.
    ///
    /// \param count Number of elements to update the capacity to.
    void reserve(size_t count)
    {
        // XXX: How to elegantly clean up this code dupe?
        if (count > m_capacity) {
            value_type* newBuffer = AllocAndMigrate(count);

            // Free old buffer.
            free(m_buffer);

            // Update internal members.
            m_buffer = newBuffer;
            m_capacity = count;
        }
    };

private:
    value_type* AllocAndMigrate(size_t count)
    {
        // Allocate a new buffer.
        value_type* newBuffer =
            static_cast<value_type*>(malloc(count * sizeof(value_type)));

        // Perform data migration.
        if (m_buffer != nullptr) {
            size_t elementsToCopy;
            if (elementsToCopy >= m_size) {
                elementsToCopy = m_size;
            } else {
                elementsToCopy = count;
            }

            memcpy(newBuffer, m_buffer, elementsToCopy * sizeof(value_type));
        }

        return newBuffer;
    }

    void _InitializeDefaultValues(size_t count,
                                  const value_type& value,
                                  value_type* newBuffer)
    {
        if (count > m_size) {
            size_t elementsToInit = count - m_size;
            for (size_t index = m_size; index < count; ++index) {
                newBuffer[index] = value;
            }
        }
    }

    // Number of elements that this vector currently contains.
    size_t m_size = 0;

    // Number of elements that this vector can contain
    // without needing to resize.
    size_t m_capacity = 0;

    // Pointer to the block of memory.
    value_type* m_buffer = nullptr;
};
