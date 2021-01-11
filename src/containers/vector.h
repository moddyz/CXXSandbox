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

    /// \typedef size_type
    ///
    /// The value type of the container size.
    using size_type = size_t;

    // -----------------------------------------------------------------------
    /// \name Construction
    // -----------------------------------------------------------------------

    /// Constructs an empty vector.
    Vector() {}

    /// Constructs a vector with \p count number of elements.
    ///
    /// \param count The number of elements.
    explicit Vector(size_type count) { resize(count); }

    /// Constructs a vector with \p count number of elements initialized to \p
    /// value.
    ///
    /// \param count The number of elements.
    /// \param value The default value initialized for each element.
    explicit Vector(size_type count, const value_type& value)
    {
        resize(count, value);
    }

    /// Destroys the vector.
    ~Vector() { _Reset(); }

    /// Copy constructor.
    ///
    /// \param src The source vector to copy contents from.
    Vector(const Vector& src) { _CopyFrom(src); }

    /// Move constructor.
    ///
    /// \param src The source vector to move resource ownership from.
    Vector(Vector&& src) noexcept { src.swap(*this); }

    /// Initializer-list constructor.
    ///
    /// \param src The source initializer list.
    Vector(std::initializer_list<value_type> src) { _CopyFromInitList(src); }

    /// Copy assignment operator.
    ///
    /// \param src The source vector to copy contents from.
    Vector& operator=(const Vector& src)
    {
        _CopyFrom(src);
        return *this;
    }

    /// Move assignment operator.
    ///
    /// \param src The source vector to move resource ownership from.
    Vector& operator=(Vector&& src) noexcept
    {
        src.swap(*this);
        return *this;
    }

    /// Initializer list assignment operator.
    ///
    /// \param src The source initializer list to copy contents from.
    Vector& operator=(std::initializer_list<value_type> src) noexcept
    {
        _CopyFromInitList(src);
        return *this;
    }

    /// Replaces element values in this container.
    ///
    /// \param src The source vector to copy contents from.
    void assign(size_type count, const value_type& value)
    {
        _ResizeOps(count, _NoOp, [&](void) {
            for (size_type index = 0; index < count; ++index) {
                m_buffer[index] = value;
            }
        });
    }

    /// Replaces elements in this container with an initializer list.
    ///
    /// \param src The source initializer list to copy contents from.
    void assign(std::initializer_list<value_type> src)
    {
        _CopyFromInitList(src);
    }

    // -----------------------------------------------------------------------
    /// \name Element access
    // -----------------------------------------------------------------------

    /// Constant indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    const value_type& operator[](size_type index) const
    {
        return m_buffer[index];
    }

    /// Mutable indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    value_type& operator[](size_type index) { return m_buffer[index]; }

    /// Constant indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    const value_type& at(size_type index) const { return m_buffer[index]; }

    /// Mutable indexed element accessor.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    value_type& at(size_type index) { return m_buffer[index]; }

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
    size_type size() const { return m_size; }

    /// Update the capacity of this vector.
    ///
    /// \param count Number of elements to update the capacity to.
    void reserve(size_type count)
    {
        if (count > m_capacity) {
            _Realloc(count);
        }
    };

    /// Get the number of elements which can be contained in the current
    /// allocation.
    ///
    /// \return Number of elements allocated.
    size_type capacity() const { return m_capacity; }

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
        for (size_type index = 0; index < m_size; ++index) {
            m_buffer[index].~value_type();
        }

        m_size = 0;
    }

    /// Appends an element to the end of the container.
    ///
    /// \param value The element value.
    void push_back(const value_type& value)
    {
        // Allocate more memory if required.
        if (m_size == m_capacity) {
            _Realloc(_NextCapacity(1));
        }

        // Initialize the element at the end.
        new (m_buffer + m_size) value_type();

        // Copy value into element at end.
        m_buffer[m_size] = value;

        // Increase size by 1.
        m_size++;
    }

    /// Appends an element to the end of the container by moving the element.
    ///
    /// \param value The element value.
    void push_back(value_type&& value)
    {
        // Allocate more memory if required.
        if (m_size == m_capacity) {
            _Realloc(_NextCapacity(1));
        }

        // Initialize the element at the end.
        new (m_buffer + m_size) value_type();

        // Move value into buffer.
        m_buffer[m_size] = std::move(value);

        // Increase size by 1.
        m_size++;
    }

    /// Resize the vector to contain \p count number of elements.
    ///
    /// \param count The number of elements.
    void resize(size_type count) { _ResizeOps(count, _NoOp, _NoOp); }

    /// Resize the vector to contain \p count number of elements, appending
    /// default-initialized \p value when the vector increases in size.
    ///
    /// \param count The number of elements.
    /// \param value The default initialized value.
    void resize(size_type count, const value_type& value)
    {
        _ResizeOps(
            count,
            [&](void) {
                for (size_type index = m_size; index < count; ++index) {
                    m_buffer[index] = value;
                }
            },
            _NoOp);
    }

    /// Swaps the contents with the \p other vector.
    ///
    /// \param other The other vector.
    void swap(Vector& other) noexcept
    {
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
        std::swap(m_buffer, other.m_buffer);
    }

private:
    static void _NoOp() {}

    // Computes a new capacity to contain an additional \p count number of
    // elements being inserted into this container.
    size_type _NextCapacity(size_type count)
    {
        // Compute base capacity value.
        size_type nextCapacity = m_capacity;
        if (nextCapacity == 0) {
            nextCapacity = 1;
        }

        // Increase by multipliers of 2 until it can accomodate the insertion.
        while (count + m_size > nextCapacity) {
            nextCapacity *= 2;
        }

        return nextCapacity;
    }

    // Procedure for performing a resize, then an operation against newly
    // initialized elements, as well as another operation against all elements.
    // This reduces the logic duplication of callers.
    template<typename NewElementsOp, typename AllElementsOp>
    void _ResizeOps(size_type count,
                    NewElementsOp newElementsOp,
                    AllElementsOp allElementsOp)
    {
        // Perform buffer re-allocation if required.
        if (count > m_capacity) {
            _Realloc(count);
        }

        if (count > m_size) {
            // Call constructor on new, un-initialized elements.
            for (size_type index = m_size; index < count; ++index) {
                new (m_buffer + index) value_type();
            }

            // Run caller-specified operation on new elements.
            newElementsOp();
        } else if (count < m_size) {
            // Run de-constructor on elements removed due to down-sizing.
            for (size_type index = count; index < m_size; ++index) {
                m_buffer[index].~value_type();
            }
        }

        // Perform an operation on all the elements.
        allElementsOp();

        m_size = count;
    }

    // Shared functionality for copying a source Vector to this one.
    void _CopyFrom(const Vector& src)
    {
        _ResizeOps(src.m_size, _NoOp, [&](void) {
            _CopyBuffer(src.m_buffer, src.m_size, m_buffer, src.m_size);
        });
    }

    // Shared functionality for copying a source Vector to this one.
    void _CopyFromInitList(const std::initializer_list<value_type>& src)
    {
        _ResizeOps(src.size(), _NoOp, [&](void) {
            size_type index = 0;
            for (auto it = src.begin(); it != src.end(); ++it, ++index) {
                m_buffer[index] = *it;
            }
        });
    }

    // Create a new allocation to contain \p count elements.
    // If this vector had existing elements, they will be migrated
    // into the newly allocated buffer.
    // The old elements are deconstructed and their buffer destroyed.
    void _Realloc(size_type count)
    {
        // Create a new allocation.
        value_type* newBuffer =
            static_cast<value_type*>(malloc(sizeof(value_type) * count));

        if (m_buffer != nullptr) {
            // Perform initialization on existing elements (in new buffer).
            for (size_type index = 0; index < m_size; ++index) {
                new (newBuffer + index) value_type();
            }

            // Copy existing elements into new buffer.
            _CopyBuffer(m_buffer, m_size, newBuffer, count);

            // De-construct elements in old buffer.
            for (size_type index = 0; index < m_size; ++index) {
                m_buffer[index].~value_type();
            }

            // Free old allocation.
            free(m_buffer);
        }

        // Assign new buffer ptr.
        m_buffer = newBuffer;

        // Update allocation size.
        m_capacity = count;
    }

    // Copy elements from one buffer to another.
    static void _CopyBuffer(value_type* srcBuffer,
                            size_type srcSize,
                            value_type* dstBuffer,
                            size_type dstSize)
    {
        size_type copyCount = std::min(srcSize, dstSize);
        for (size_type i = 0; i < copyCount; ++i) {
            dstBuffer[i] = srcBuffer[i];
        }
    }

    // Reset internal members to default state.  Memory will be freed.
    void _Reset()
    {
        if (m_buffer != nullptr) {
            // Call deconstructor on elements.
            for (size_type index = 0; index < m_size; ++index) {
                m_buffer[index].~value_type();
            }

            // Free buffer.
            free(m_buffer);

            m_buffer = nullptr;
            m_size = 0;
            m_capacity = 0;
        }
    }

    // Number of elements that this vector currently contains.
    size_type m_size = 0;

    // Number of elements that this vector can contain
    // without needing to resize.
    size_type m_capacity = 0;

    // Pointer to the allocated buffer.
    value_type* m_buffer = nullptr;
};
