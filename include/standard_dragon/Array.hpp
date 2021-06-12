//
// Created by Lilith on 2020-06-05.
//

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <new>
#include <sstream>
#include <type_traits>
#include <vector>

#include <standard_dragon/exception/out_of_bounds.hpp>

namespace dragon {
    template<typename T>
    class Array {
    public:
        std::shared_ptr<T[]> Pointer = nullptr;
        size_t Length                = 0;
        uintptr_t Offset             = 0;
        size_t Alignment             = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T *;
            using reference         = T &;

            explicit Iterator(pointer ptr) : m_ptr(ptr) { }

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }
            Iterator &operator++() {
                m_ptr++;
                return *this;
            }
            Iterator operator++(int) { // NOLINT(cert-dcl21-cpp)
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }
            friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
        };

        Array() = default;

        explicit Array(std::vector<T> vector) : Array(vector.data(), vector.size()) { }

        Array(T *buffer, size_t size, size_t alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            if (alignment < 1) {
                alignment = 1;
            }

            if (size < 1) {
                size = 1;
            }

            Pointer    = std::shared_ptr<T[]>(new T[size + alignment - 1]);
            auto delta = alignment_delta(alignment);
            if (delta != 0) {
                Offset = alignment - delta;
            }
            std::copy_n(buffer, size, data());
            Alignment = alignment;
            Length    = size;
        }

        Array(std::shared_ptr<T[]> pointer, size_t size) {
            Pointer = pointer;

            if (size < 1) {
                size = 1;
            }

            Length = size;
        }

        Array(size_t size, const T *default_value, size_t alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
            if (alignment < 1) {
                alignment = 1;
            }

            if (size < 1) {
                size = 1;
            }

            Pointer = std::shared_ptr<T[]>(new T[size + alignment - 1]);
            if (default_value != nullptr) {
                for (size_t i = 0; i < size + alignment; ++i)
                    Pointer[i] = *default_value;
            }
            Length = size;

            auto delta = alignment_delta(alignment);
            if (delta != 0) {
                Offset = alignment - delta;
            }

            Alignment = alignment;
        }

        Array(std::shared_ptr<Array<T>> parent, uintptr_t offset) {
            Pointer = parent->Pointer;
            Offset  = offset + parent->Offset;
            Length  = parent->Length;
        }

        Array(std::shared_ptr<Array<T>> parent, uintptr_t offset, size_t length) {
            Pointer = parent->Pointer;
            Offset  = offset + parent->Offset;
            if (length > parent->Length - Offset || length <= 0)
                Length = parent->Length;
            else
                Length = length + offset;
        }

        template<typename U>
        static Array<T> ptr_cast(U *buffer, size_t size) {
            return Array<T>(reinterpret_cast<T *>(buffer), size * sizeof(U) / sizeof(T));
        }

        T &operator[](uintptr_t index) const { return get(index); }

        T &get(uintptr_t index) const {
            if (index < 0 || index >= this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            return data()[index];
        }

        [[nodiscard]] size_t alignment_delta(size_t alignment) const {
            return reinterpret_cast<uintptr_t>(data()) % alignment;
        }

        [[nodiscard]] bool is_aligned(size_t alignment) const {
            return alignment_delta(alignment) == 0;
        }

        Array<T> align_as(size_t alignment) const {
            return Array<T>(data(), size(), alignment);
        }

        void set(uintptr_t index, T value) const {
            if (index < 0 || index >= this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            data()[index] = value;
        }

        template<typename U>
        U cast(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            return reinterpret_cast<U *>(data() + index)[0];
        }

        template<typename U>
        U lpcast(uintptr_t &index) {
            U tmp = cast<U>(index);
            index += sizeof(U) / sizeof(T);
            return tmp;
        }

        template<typename U>
        Array<U> cast(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            return Array<U>(reinterpret_cast<U *>(data() + index), size);
        }

        template<typename U>
        Array<U> lpcast(uintptr_t &index, size_t size) {
            Array<U> tmp = cast<U>(index, size);
            index += size * sizeof(U) / sizeof(T);
            return tmp;
        }

        Array<T> slice(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            return Array<T>((data() + index), size);
        }

        Array<T> shift(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            return Array<T>(this, index);
        }

        Array<T> lpslice(uintptr_t &index, size_t size) {
            Array<T> tmp = slice(index, size);
            index += size;
            return tmp;
        }

        void copy(uintptr_t ptr, uintptr_t index, size_t size) {
            if (size < 0 || size > this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            std::copy_n((data() + index), size, reinterpret_cast<T *>(ptr));
        }

        void lpcopy(uintptr_t &ptr, uintptr_t &index, size_t size) {
            copy(ptr, index, size);
            ptr += size * sizeof(T);
            index += size;
        }

        void paste(uintptr_t ptr, uintptr_t index, size_t size) {
            if (size < 0 || size > this->size()) {
                throw dragon::exception::out_of_bounds();
            }
            std::copy_n(reinterpret_cast<T *>(ptr), size, (data() + index));
        }

        void lppaste(uintptr_t &ptr, uintptr_t &index, size_t size) {
            paste(ptr, index, size);
            ptr += size * sizeof(T);
            index += size;
        }

        template<typename U = T>
        typename std::enable_if<sizeof(U) <= 2 && std::is_same<U, T>::value && std::is_integral<U>::value, void>::type ensure_null_terminated() {
            auto buffer = Pointer;
            if (buffer[size() - 1] != 0) {
                Length += 1;
                Pointer    = std::shared_ptr<T[]>(new T[size() + Alignment - 1]);
                auto delta = alignment_delta(Alignment);
                if (delta != 0) {
                    Offset = Alignment - delta;
                }
                std::copy_n(buffer.get(), size() - 1, data());
                data()[size() - 1] = static_cast<T>(0);
            }
        }

        template<typename U = T>
        typename std::enable_if<sizeof(U) == 1 && std::is_same<U, T>::value && std::is_integral<U>::value, std::string>::type to_string() {
            ensure_null_terminated<U>();
            return std::string(reinterpret_cast<char *>(data()), size());
        }

        template<typename U = T>
        typename std::enable_if<sizeof(U) <= 2 && std::is_same<U, T>::value && std::is_integral<U>::value, std::string>::type to_wstring() {
            ensure_null_terminated<U>();
            if (sizeof(U) == 1) {
                return std::wstring(to_string());
            }

            return std::wstring(reinterpret_cast<wchar_t *>(data()), size());
        }

        template<typename U = T>
        typename std::enable_if<sizeof(U) == 1 && std::is_same<U, T>::value && std::is_integral<U>::value, std::stringstream>::type to_string_stream() {
            ensure_null_terminated<U>();
            return std::stringstream(reinterpret_cast<char *>(data()), std::ios::in | std::ios::out);
        }

        template<typename U = T>
        typename std::enable_if<sizeof(U) == 2 && std::is_same<U, T>::value && std::is_integral<U>::value, std::wstringstream>::type to_wstring_stream() {
            ensure_null_terminated<U>();
            return std::wstringstream(reinterpret_cast<wchar_t *>(data()), std::ios::in | std::ios::out);
        }

        template<typename U = T>
        typename std::enable_if<sizeof(T) == 1 && std::is_same<U, T>::value && std::is_integral<U>::value, std::iostream>::type to_stream() { return std::iostream(reinterpret_cast<char *>(data()), std::ios::in | std::ios::out, byte_size()); }

        std::vector<T> to_vector() { return std::vector<T>(data(), data() + size()); }

        T *data() const { return Pointer.get() + Offset; }

        std::shared_ptr<T[]> pointer() const { return Pointer; }

        [[nodiscard]] size_t size() const { return Length - Offset; }

        [[nodiscard]] size_t byte_size() const { return size() * sizeof(T); }

        [[nodiscard]] bool empty() const { return this->size() <= 0 || this->Pointer == nullptr; }

        Iterator begin() const { return Iterator(data()); }

        Iterator end() const { return Iterator(data() + size()); }
    };
} // namespace dragon
