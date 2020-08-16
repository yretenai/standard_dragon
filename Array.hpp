//
// Created by yretenai on 6/5/2020.
//

#ifndef DRAGON_ARRAY_H
#define DRAGON_ARRAY_H

#include <algorithm>
#include <memory>
#include <vector>

namespace dragon {
    class out_of_bounds_exception : public std::exception {};

    template <typename T> class Array {
      public:
        std::shared_ptr<T[]> Pointer = nullptr;
        size_t Length = 0;
        uintptr_t Offset = 0;

        class Iterator {
          private:
            const Array<T>* Parent;
            size_t Index;

          public:
            Iterator() {
                Parent = nullptr;
                Index = 0;
            }

            explicit Iterator(const Array<T>* parent, size_t index = 0) {
                Parent = parent;
                Index = index;
            }

            bool operator==(const Iterator& rhs) const {
                return Parent->Pointer == rhs.Parent->Pointer && Index == rhs.Index && Parent->size() == rhs.Parent->size() &&
                       Parent->Offset == rhs.Parent->Offset;
            }

            bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

            T operator*() const { return Parent->get(Index); }

            Iterator& operator++() {
                if (Index >= Parent->size()) {
                    throw out_of_bounds_exception();
                }
                Index++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp(*this);
                ++*this;
                return temp;
            }
        };

        Array() {}

        Array(T* buffer, size_t size, const T* default_value) : Array(size, default_value) {
            std::copy_n(buffer, size, Pointer.get());
            Length = size;
        }

        Array(size_t size, const T* default_value) {
            Pointer = std::shared_ptr<T[]>(new T[size]);
            for (size_t i = 0; i < size; ++i)
                Pointer[i] = default_value == nullptr ? T() : *default_value;
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

        ~Array() = default;

        template <typename U> [[maybe_unused]] static Array<T> ptr_cast(U* buffer, size_t size) {
            return Array<T>(reinterpret_cast<T*>(buffer), size * sizeof(U) / sizeof(T), nullptr);
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

        template <typename U> [[maybe_unused]] U cast(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return reinterpret_cast<U*>(data() + index)[0];
        }

        template <typename U> [[maybe_unused]] U lpcast(uintptr_t* index) {
            U tmp = cast<U>(*index);
            *index += sizeof(U) / sizeof(T);
            return tmp;
        }

        template <typename U> [[maybe_unused]] Array<U> cast(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<U>(reinterpret_cast<U*>(data() + index), size, nullptr);
        }

        template <typename U> [[maybe_unused]] Array<U> lpcast(uintptr_t* index, size_t size) {
            Array<U> tmp = cast<U>(*index, size);
            (*index) += size * sizeof(U) / sizeof(T);
            return tmp;
        }

        [[maybe_unused]] Array<T> slice(uintptr_t index, size_t size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>((data() + index), size, nullptr);
        }

        [[maybe_unused]] Array<T> shift(uintptr_t index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>(this, index);
        }

        [[maybe_unused]] Array<T> lpslice(uintptr_t* index, size_t size) {
            Array<T> tmp = slice(*index, size);
            (*index) += size;
            return tmp;
        }

        [[maybe_unused]] std::vector<T> to_vector() { return std::vector<T>(data(), data() + size()); }

        T* data() const { return Pointer.get() + Offset; }

        [[maybe_unused]] std::shared_ptr<T[]> pointer() const { return Pointer; }

        [[nodiscard]] size_t size() const { return Length - Offset; }

        [[nodiscard]] size_t byte_size() const { return size() * sizeof(T); }

        [[nodiscard]] bool empty() const { return this->size() <= 0; }

        Iterator begin() const { return Iterator(this, 0); }

        Iterator end() const { return Iterator(this, size()); }

#if USE_NOESIS
        T* to_noesis(noeRAPI_t* rapi) {
            T* buffer = (T*)rapi->Noesis_UnpooledAlloc(byte_size());
            std::copy_n(begin(), size(), buffer);
            return buffer;
        }
#endif
    };
} // namespace dragon

#endif // DRAGON_ARRAY_H
