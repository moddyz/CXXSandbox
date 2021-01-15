#pragma once

#include <cstring>

#include "utils.h"

/// \class Vector
///
/// A dynamically re-sizable, type-homogenous array.
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
    using size_type = std::size_t;

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

    /// Access a read-only element.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    const value_type& operator[](size_type index) const
    {
        return m_buffer[index];
    }

    /// Access a mutable element.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    value_type& operator[](size_type index) { return m_buffer[index]; }

    /// Access a read-only element with bounds checking.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    const value_type& at(size_type index) const
    {
        if (index >= m_size) {
            throw std::out_of_range("Index is out of range.");
        }

        return m_buffer[index];
    }

    /// Access a mutable element with bounds checking.
    ///
    /// \param index The index of the element.
    ///
    /// \return The element.
    value_type& at(size_type index)
    {
        if (index >= m_size) {
            throw std::out_of_range("Index is out of range.");
        }

        return m_buffer[index];
    }

    /// Access the first element, in a read-only fashion.
    ///
    /// This results in undefined behavior if this vector is empty.
    ///
    /// \return The first element.
    const value_type& front() const { return m_buffer[0]; }

    /// Access the first element, in a mutable fashion.
    ///
    /// \pre This results in undefined behavior if this vector is empty.
    ///
    /// \return The first element.
    value_type& front() { return m_buffer[0]; }

    /// Access the last element, in a read-only fashion.
    ///
    /// This results in undefined behavior if this vector is empty.
    ///
    /// \return The last element.
    const value_type& back() const { return m_buffer[m_size - 1]; }

    /// Access the last element, in a mutable fashion.
    ///
    /// \pre This results in undefined behavior if this vector is empty.
    ///
    /// \return The last element.
    value_type& back() { return m_buffer[m_size - 1]; }

    // -----------------------------------------------------------------------
    /// \name Iterators
    // -----------------------------------------------------------------------

    /// \class iterator
    ///
    /// iterator for the \ref Vector class.
    class iterator final
    {
        /// \typedef value_type
        ///
        /// The value type of element being iterated.
        using value_type = ValueT;

        /// \typedef size_type
        ///
        /// The value type of the container size.
        using size_type = std::size_t;

    public:
        /// Default constructor.
        iterator() = default;

        /// iterator construction, with the current position.
        ///
        /// \param current The position to initialize this iterator to.
        iterator(value_type* ptr)
          : m_ptr(ptr)
        {}

        /// Check if the current position of the iterator matches another.
        ///
        /// \retval true if the positions match.
        bool operator!=(const iterator& i_other) const
        {
            return m_ptr != i_other.m_ptr;
        }

        /// De-reference this iterator, returning value in a readable manner.
        const value_type& operator*() const { return *m_ptr; }

        /// De-reference this iterator, returning value in a writable manner.
        value_type& operator*() { return *m_ptr; }

        /// Increment this iterator forwards.
        const iterator& operator++()
        {
            m_ptr++;
            return (*this);
        }

        /// Increment this iterator backwards.
        const iterator& operator--()
        {
            m_ptr--;
            return (*this);
        }

        /// Increment this iterator forwards.
        const iterator& operator++(int)
        {
            m_ptr++;
            return (*this);
        }

        /// Increment this iterator backwards.
        const iterator& operator--(int)
        {
            m_ptr--;
            return (*this);
        }

        /// Create a new iterator which is \p count positions forward.
        ///
        /// \param count The number of positions forward.
        ///
        /// \return New iterator.
        iterator operator+(size_type count)
        {
            return iterator(m_ptr + count);
        }

        /// Create a new iterator which is \p count positions backwards.
        ///
        /// \param count The number of positions backwards.
        ///
        /// \return New iterator.
        iterator operator-(size_type count)
        {
            return iterator(m_ptr - count);
        }

        /// Increment this iterator backwards.
        size_type operator-(const iterator& it) const
        {
            return m_ptr - it.m_ptr;
        }

    private:
        value_type* m_ptr = nullptr;
    };

    /// Creates an iterator which points to the position of the first element.
    ///
    /// The iterator will be invalid if the container is empty.
    ///
    /// \return An iterator.
    iterator begin() const { return iterator(m_buffer); }

    /// Creates an iterator which points to the position right \em after the
    /// last element.
    ///
    /// \return An iterator.
    iterator end() const { return iterator(m_buffer + m_size); }

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

    /// Constructs a new element at the end of the container.
    ///
    /// \param value The element value.
    template<class... Args>
    value_type& emplace_back(Args&&... args)
    {
        // Allocate more memory if required.
        if (m_size == m_capacity) {
            _Realloc(_NextCapacity(1));
        }

        // Initialize the element at the end.
        value_type* element =
            new (m_buffer + m_size) value_type(std::forward<Args>(args)...);

        // Increase size by 1.
        m_size++;

        // Return the newly allocated element.
        return *element;
    }

    /// Removes the last element of this vector.
    ///
    /// If the vector is empty, this results in undefined behavior!
    void pop_back()
    {
        m_buffer[m_size - 1].~value_type();
        m_size--;
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

    /// Insert elements at the specified location in the container.
    ///
    /// \param position The position to insert elements before.
    iterator insert(iterator position, const value_type& value)
    {
        size_type posIndex = position - begin();
        size_type insertCount = 1;

        // Allocate more memory if required.
        value_type* targetBuffer = nullptr;
        if (m_size == m_capacity) {
            size_type count = _NextCapacity(insertCount);

            // Create a new allocation.
            targetBuffer = _Alloc(count);

            // Initialize target range.
            for (size_type index = 0; index < m_size + insertCount; ++index) {
                new (targetBuffer + index) value_type();
            }
        } else {
            targetBuffer = m_buffer;
            for (size_type index = m_size; index < m_size + insertCount;
                 ++index) {
                new (targetBuffer + index) value_type();
            }
        }

        if (m_buffer != nullptr) {
            // Migrate left range.
            if (posIndex != 0) {
                _CopyBuffer(m_buffer, posIndex, targetBuffer, posIndex);
            }

            // Migrate right range.
            if (m_size - posIndex != 0) {
                _CopyBufferReversed(m_buffer + posIndex,
                                    m_size - posIndex,
                                    targetBuffer + insertCount + posIndex,
                                    m_size - posIndex);
            }

            if (targetBuffer != m_buffer) {
                // De-construct elements in old buffer.
                for (size_type index = 0; index < m_size; ++index) {
                    m_buffer[index].~value_type();
                }

                // Free old allocation.
                free(m_buffer);

                m_buffer = targetBuffer;
            }
        }

        // Set inserted value.
        m_buffer[posIndex] = value;

        // Increase size by 1.
        m_size += insertCount;

        return iterator(m_buffer + posIndex);
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

    // Allocate a block of memory containing \p count elements.
    static value_type* _Alloc(size_type count)
    {
        return static_cast<value_type*>(malloc(sizeof(value_type) * count));
    }

    // Create a new allocation to contain \p count elements.
    // If this vector had existing elements, they will be migrated
    // into the newly allocated buffer.
    // The old elements are deconstructed and their buffer destroyed.
    void _Realloc(size_type count)
    {
        // Create a new allocation.
        value_type* newBuffer = _Alloc(count);

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

    // Copy elements from one buffer to another in reversed order.
    static void _CopyBufferReversed(value_type* srcBuffer,
                                    size_type srcSize,
                                    value_type* dstBuffer,
                                    size_type dstSize)
    {
        size_type copyCount = std::min(srcSize, dstSize);
        for (size_type i = copyCount; i > 0; --i) {
            dstBuffer[i - 1] = srcBuffer[i - 1];
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
