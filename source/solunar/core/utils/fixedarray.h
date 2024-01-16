#ifndef FIXEDARRAY_H
#define FIXEDARRAY_H

#include <array>

namespace engine
{
    //! Fixed array with std::vector like interface
    template <typename T, const size_t N>
    class fixed_array
    {
    public:
        typedef size_t size_type;
        typedef T value_type;
    public:
        //fixed_array(const T& initialize_value) { for (int i = 0; i < N; i++) m_data[i] = initialize_value; }
        fixed_array();
        ~fixed_array();

        T* begin() { return m_data; }
        T* end() { return m_data + m_capacity; }

        const size_t capacity() const { return m_capacity; }
        size_t size() { return m_pos; }

        void push_back(T value);
        
        T& operator[](size_t index);

    private:
        T m_data[N];

        size_t m_capacity;
        int m_pos;
    };

    template <typename T, const size_t N>
    fixed_array<T, N>::fixed_array()
    {
        m_capacity = N;
        m_pos = 0;
    }

    template <typename T, const size_t N>
    fixed_array<T, N>::~fixed_array()
    {
    }

    template <typename T, const size_t N>
    void fixed_array<T, N>::push_back(T value)
    {
        m_data[m_pos++] = value;
    }

    template <typename T, const size_t N>
    T& fixed_array<T, N>::operator[](size_t index)
    {
        return m_data[index];
    }
}

#endif