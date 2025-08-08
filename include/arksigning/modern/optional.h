#ifndef ARKSIGNING_OPTIONAL_H
#define ARKSIGNING_OPTIONAL_H

#include <stdexcept>
#include <utility>

namespace ArkSigning {

/**
 * @brief A simple optional implementation for C++11 compatibility
 * 
 * This provides std::optional-like functionality for C++11 codebases.
 * It safely handles nullable values and eliminates error-prone null checks.
 * 
 * @tparam T The type to wrap in optional
 */
template<typename T>
class Optional {
private:
    alignas(T) char storage_[sizeof(T)];
    bool has_value_;

    T* ptr() { return reinterpret_cast<T*>(storage_); }
    const T* ptr() const { return reinterpret_cast<const T*>(storage_); }

public:
    /**
     * @brief Default constructor - creates empty optional
     */
    Optional() : has_value_(false) {}

    /**
     * @brief Construct from value
     */
    Optional(const T& value) : has_value_(true) {
        new (storage_) T(value);
    }

    /**
     * @brief Move constructor
     */
    Optional(T&& value) : has_value_(true) {
        new (storage_) T(std::move(value));
    }

    /**
     * @brief Copy constructor
     */
    Optional(const Optional& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(*other.ptr());
        }
    }

    /**
     * @brief Move constructor
     */
    Optional(Optional&& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(std::move(*other.ptr()));
            other.reset();
        }
    }

    /**
     * @brief Destructor
     */
    ~Optional() {
        reset();
    }

    /**
     * @brief Copy assignment
     */
    Optional& operator=(const Optional& other) {
        if (this != &other) {
            reset();
            has_value_ = other.has_value_;
            if (has_value_) {
                new (storage_) T(*other.ptr());
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment
     */
    Optional& operator=(Optional&& other) {
        if (this != &other) {
            reset();
            has_value_ = other.has_value_;
            if (has_value_) {
                new (storage_) T(std::move(*other.ptr()));
                other.reset();
            }
        }
        return *this;
    }

    /**
     * @brief Value assignment
     */
    Optional& operator=(const T& value) {
        reset();
        has_value_ = true;
        new (storage_) T(value);
        return *this;
    }

    /**
     * @brief Move value assignment
     */
    Optional& operator=(T&& value) {
        reset();
        has_value_ = true;
        new (storage_) T(std::move(value));
        return *this;
    }

    /**
     * @brief Check if optional has a value
     */
    bool has_value() const { return has_value_; }

    /**
     * @brief Boolean conversion operator
     */
    explicit operator bool() const { return has_value_; }

    /**
     * @brief Get the value (throws if empty)
     */
    T& value() {
        if (!has_value_) {
            throw std::runtime_error("Optional is empty");
        }
        return *ptr();
    }

    /**
     * @brief Get the value (throws if empty) - const version
     */
    const T& value() const {
        if (!has_value_) {
            throw std::runtime_error("Optional is empty");
        }
        return *ptr();
    }

    /**
     * @brief Get value or default
     */
    T value_or(const T& default_value) const {
        return has_value_ ? *ptr() : default_value;
    }

    /**
     * @brief Dereference operator
     */
    T& operator*() { return value(); }

    /**
     * @brief Dereference operator - const version
     */
    const T& operator*() const { return value(); }

    /**
     * @brief Arrow operator
     */
    T* operator->() { return &value(); }

    /**
     * @brief Arrow operator - const version
     */
    const T* operator->() const { return &value(); }

    /**
     * @brief Reset to empty state
     */
    void reset() {
        if (has_value_) {
            ptr()->~T();
            has_value_ = false;
        }
    }

    /**
     * @brief Emplace construct value in-place
     */
    template<typename... Args>
    T& emplace(Args&&... args) {
        reset();
        has_value_ = true;
        new (storage_) T(std::forward<Args>(args)...);
        return *ptr();
    }
};

/**
 * @brief Create an optional with a value
 */
template<typename T>
Optional<T> make_optional(const T& value) {
    return Optional<T>(value);
}

/**
 * @brief Create an optional with a moved value
 */
template<typename T>
Optional<T> make_optional(T&& value) {
    return Optional<T>(std::move(value));
}

/**
 * @brief Equality comparison
 */
template<typename T>
bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
    if (lhs.has_value() != rhs.has_value()) {
        return false;
    }
    return !lhs.has_value() || (*lhs == *rhs);
}

/**
 * @brief Inequality comparison
 */
template<typename T>
bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) {
    return !(lhs == rhs);
}

/**
 * @brief Compare optional with value
 */
template<typename T>
bool operator==(const Optional<T>& opt, const T& value) {
    return opt.has_value() && (*opt == value);
}

/**
 * @brief Compare value with optional
 */
template<typename T>
bool operator==(const T& value, const Optional<T>& opt) {
    return opt == value;
}

} // namespace ArkSigning

// Convenience alias for global scope
template<typename T>
using Optional = ArkSigning::Optional<T>;

#endif // ARKSIGNING_OPTIONAL_H
