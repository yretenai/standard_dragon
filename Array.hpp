//
// Created by Lilith on 2020-06-05.
//

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <sstream>
#include <type_traits>
#include <vector>

namespace dragon {
    class out_of_bounds_exception : public std::exception {};

    template <typename T> class Array {
      public:
        std::shared_ptr<T[]> Pointer = nullptr;
        size_t Length = 0;
        uintptr_t Offset = 0;

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            explicit Iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }
            Iterator& operator++() {
                m_ptr++;
                return *this;
            }
            Iterator operator++(int) {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }
            friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

          private:
            pointer m_ptr;
        };

        Array() = default;

        explicit Array(std::vector<T> vector) : Array(vector.data(), vector.size(), true) {}

        Array(T* buffer, size_t size, bool copy) {
            if (copy) {
                Pointer = std::shared_ptr<T[]>(new T[size]);
                std::copy_n(buffer, size, Pointer.get());
            } else {
                Pointer = std::shared_ptr<T[]>(buffer);
            }
            Length = size;
        }

        Array(std::shared_ptr<T[]> pointer, size_t size) {
            Pointer = pointer;
            Length = size;
        }

        Array(size_t size, const T* default_value) {
            Pointer = std::shared_ptr<T[]>(new T[size]);
            if (default_value != nullptr) {
                for (size_t i = 0; i < size; ++i)
                    Pointer[i] = *default_value;
            }
            Length = size;
        }

        Array(std::shared_ptr<Array<T>> parent, uintptr_t offset) {
            Pointer = parent->Pointer;
            Offset = offset + parent->Offset;
            Length = parent->Length;
        }

        Array(std::shared_ptr<Array<T>> parent, uintptr_t offset, size_t length) {
            Pointer = parent->Pointer;
            Offset = offset + parent->Offset;
            if (length > parent->Length - Offset)
                Length = parent->Length;
            else
                Length = length + offset;
        }

        template <typename U> static Array<T> ptr_cast(U* buffer, size_t size) {
            return Array<T>(reinterpret_cast<T*>(buffer), size * sizeof(U) / sizeof(T), false);
        }

        T& operator[](uintptr_t index) const { return get(index); }

        T& get(uintptr_t index) const {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return data()[index];
        }

        void set(uintptr_t index, T value) const {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            data()[index] = value;
        }

        template <typename U> U cast(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return reinterpret_cast<U*>(data() + index)[0];
        }

        template <typename U> U lpcast(uintptr_t* index) {
            U tmp = cast<U>(*index);
            *index += sizeof(U) / sizeof(T);
            return tmp;
        }

        template <typename U> Array<U> cast(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<U>(reinterpret_cast<U*>(data() + index), size, false);
        }

        template <typename U> Array<U> lpcast(uintptr_t* index, size_t size) {
            Array<U> tmp = cast<U>(*index, size);
            (*index) += size * sizeof(U) / sizeof(T);
            return tmp;
        }

        Array<T> slice(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>((data() + index), size, false);
        }

        Array<T> shift(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>(this, index);
        }

        Array<T> lpslice(uintptr_t* index, size_t size) {
            Array<T> tmp = slice(*index, size);
            (*index) += size;
            return tmp;
        }

        void copy(uintptr_t ptr, uintptr_t index, size_t size) {
            if (size < 0 || size >= this->size()) {
                throw out_of_bounds_exception();
            }
            std::copy_n((data() + index), size, reinterpret_cast<T*>(ptr));
        }

        void lpcopy(uintptr_t* ptr, uintptr_t* index, size_t size) {
            copy(*ptr, *index, size);
            (*ptr) += size * sizeof(T);
            (*index) += size;
        }

        std::string to_string() { return std::string(reinterpret_cast<char*>(data()), size()); }

        std::wstring to_wstring() {
            if (sizeof(T) == sizeof(char)) {
                return std::wstring(this->to_string());
            }

            return std::wstring(reinterpret_cast<wchar_t*>(data()), size());
        }

        std::istringstream to_string_stream() { return std::istringstream(reinterpret_cast<char*>(data()), size()); }

        std::iostream to_stream() { return std::iostream(reinterpret_cast<char*>(data()), byte_size()); }

        std::vector<T> to_vector() { return std::vector<T>(data(), data() + size()); }

        T* data() const { return Pointer.get() + Offset; }

        std::shared_ptr<T[]> pointer() const { return Pointer; }

        [[nodiscard]] size_t size() const { return Length - Offset; }

        [[nodiscard]] size_t byte_size() const { return size() * sizeof(T); }

        [[nodiscard]] bool empty() const { return this->size() <= 0 || this->Pointer == nullptr; }

        Iterator begin() const { return Iterator(data()); }

        Iterator end() const { return Iterator(data() + size()); }

#if USE_NOESIS
        T* to_noesis(noeRAPI_t* rapi) {
            T* buffer = (T*)rapi->Noesis_UnpooledAlloc(byte_size());
            std::copy_n(begin(), size(), buffer);
            return buffer;
        }
#endif
    };
} // namespace dragon
