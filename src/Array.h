#ifndef Array_h
#define Array_h

// Local include.
#include "Helper.h"

// Library includes.
#include <assert.h>


/// @brief Replacement data container for boards that do not support the C++ STL and therefore do not have the std::array class.
/// @tparam T Type of the underlying data the list should point too.
/// @tparam Capacity Amount of elements that can be saved into the underlying data structure allows to wrap a simple c-array and allocate it on the stack.
template <typename T, size_t Capacity>
class Array {
  public:
    using value_type = T;

    /// @brief Constructor
    Array(void) = default;

    /// @brief Constructor that allows compatibility with std::vector, simply forwards call to internal insert method and copies all data between the first and last iterator
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element we want to copy into our underlying data container
    /// @param last Iterator pointing to one past the end of the elements we want to copy into our underlying data container
    template<typename InputIterator>
    Array(InputIterator const & first, InputIterator const & last)
      : m_elements()
      , m_size(0U)
    {
        insert(nullptr, first, last);
    }

    /// @brief Constructor that allows compatibility with std::vector, simply forwards call to internal insert method and copies all data from the given container
    /// @tparam Container Class that contains the actual data we want to copy into our internal data container,
    /// requires access to a begin() and end() method, that point to the first element and one past the last element we want to copy respectively.
    /// Both methods need to return an InputIterator, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param container Data container with begin() and end() method that we want to copy fully into our underlying data container
    template<typename Container>
    Array(Container const & container)
      : m_elements()
      , m_size(0U)
    {
        insert(nullptr, container.begin(), container.end());
    }

    /// @brief Method that allows compatibility with std::vector, simply forwards call to internal insert method and copies all data between the first and last iterator
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param first Iterator pointing to the first element we want to copy into our underlying data container
    /// @param last Iterator pointing to one past the end of the elements we want to copy into our underlying data container
    template<typename InputIterator>
    void assign(InputIterator const & first, InputIterator const & last) {
        insert(nullptr, first, last);
    }

    /// @brief Method that allows compatibility with std::vector, simply forwards call to internal insert method and copies all data from the given container
    /// @tparam Container Class that contains the actual data we want to copy into our internal data container,
    /// requires access to a begin() and end() method, that point to the first element and one past the last element we want to copy respectively.
    /// Both methods need to return an InputIterator, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param container Data container with begin() and end() method that we want to copy fully into our underlying data container
    template<typename Container>
    void assign(Container const & container) {
        insert(nullptr, container.begin(), container.end());
    }

    /// @brief Returns whether there are still any element in the underlying data container
    /// @return Whether the underlying data container is empty or not
    bool empty() const {
        return m_size == 0U;
    }

    /// @brief Gets the current amount of elements in the underlying data container
    /// @return The amount of items currently in the underlying data container
    size_t size() const {
        return m_size;
    }

    /// @brief Gets the maximum amount of elements that can currently be stored in the underlying data container
    /// @return The maximum amount of items that can currently be stored in the underlying data container
    size_t constexpr capacity() const {
        return Capacity;
    }

    /// @brief Returns a iterator to the first element of the underlying data container
    /// @return Iterator pointing to the first element of the underlying data container
    T * begin() {
        return m_elements;
    }

    /// @brief Returns a constant iterator to the first element of the underlying data container
    /// @return Constant iterator pointing to the first element of the underlying data container
    T const * begin() const {
        return m_elements;
    }

    /// @brief Returns a constant iterator to the first element of the underlying data container
    /// @return Constant iterator pointing to the first element of the underlying data container
    T const * cbegin() const {
        return m_elements;
    }

    /// @brief Returns the last element of the vector
    /// @return Reference to the last element of the vector
    T & back() {
        assert(m_size != 0U);
        return m_elements[m_size - 1U];
    }

    /// @brief Returns a iterator to one-past-the-end element of the underlying data container
    /// @return Iterator pointing to one-past-the-end element of the underlying data container
    T * end() {
        return m_elements + m_size;
    }

    /// @brief Returns a constantiterator to one-past-the-end element of the underlying data container
    /// @return Constant iterator pointing to one-past-the-end element of the underlying data container
    T const * end() const {
        return m_elements + m_size;
    }

    /// @brief Returns a constant iterator to one-past-the-end element of the underlying data container
    /// @return Constant iterator pointing to one-past-the-end element of the underlying data container
    T const * cend() const {
        return m_elements + m_size;
    }

    /// @brief Inserts the given element at the end of the underlying data container,
    /// if the interal data structure is full already then this method will assert and stop the application.
    /// Because if we do not we could cause an out of bounds write, which could possibly overwrite other memory.
    /// Causing hard to debug issues, therefore this behaviour is not allowed in the first place
    /// @param element Element that should be inserted at the end
    void push_back(T const & element) {
        assert(m_size < Capacity);
        m_elements[m_size++] = element;
    }

    /// @brief Inserts all element from the given start to the given end iterator into the underlying data container.
    /// Simply calls push_back on each element, meaning if the initally allocated size if not big enough to hold all elements,
    /// then this method will simply not insert those elements instead
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param position Attribute is not used and can be left as nullptr, simply there to keep compatibility with std::vector insert method
    /// @param first Iterator pointing to the first element we want to copy into our underlying data container
    /// @param last Iterator pointing to one past the end of the elements we want to copy into our underlying data container
    template<typename InputIterator>
    void insert(T const * position, InputIterator const & first, InputIterator const & last) {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    /// @brief Removes the element at the given position, has to move all element one to the left if the index is not at the end of the array
    /// @tparam InputIterator Class that points to the begin and end iterator
    /// of the given data container, allows for using / passing either std::vector or std::array.
    /// See https://en.cppreference.com/w/cpp/iterator/input_iterator for more information on the requirements of the iterator
    /// @param position Iterator pointing to the element, that should be removed from the underlying data container
    template <typename InputIterator>
    void erase(InputIterator const& position)
    {
        size_t const index = Helper::distance(begin(), position);
    
        // Check if the given index is within the valid range
        if (index < m_size)
        {
            // Only shift elements if the erased element is not the last one AND Capacity > 1
            if (1 < Capacity)
            {
                // Move elements after the index one position to the left
                for (size_t i = index; i < m_size - 1; ++i)
                {
                    m_elements[i] = m_elements[i + 1];
                }
            }
            // Decrease the size to remove the element
            m_size--;
        }
    }

    /// @brief Method to access an element at a given index,
    /// ensures the device crashes if we attempted to access in an invalid location
    /// @param index Index we want to get the corresponding element for
    T & at(size_t const & index) {
        assert(index < m_size);
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    T & operator[](size_t const & index) {
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    T const & operator[](size_t const & index) const {
        return m_elements[index];
    }

    /// @brief Clears the given underlying data container.
    /// Simply sets the underlying size to 0, data will only be cleared in the destructor
    void clear() {
        m_size = 0;
    }

  private:
    T      m_elements[Capacity] = {}; // Underlying c-array holding our data
    size_t m_size = {};               // Used size that shows how many elements we entered
};

#endif // Array_h
