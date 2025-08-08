#ifndef ARKSIGNING_UTILITY_H
#define ARKSIGNING_UTILITY_H

#include <utility>
#include <memory>
#include <functional>

namespace ArkSigning {
namespace Utility {

/**
 * @brief Perfect forwarding wrapper for function calls
 * 
 * This utility provides perfect forwarding for callback functions,
 * preserving value categories and enabling efficient parameter passing.
 */
template<typename Func, typename... Args>
auto invoke_with_perfect_forwarding(Func&& func, Args&&... args) 
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...))
{
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

/**
 * @brief Factory function with perfect forwarding
 * 
 * Creates objects with perfect forwarding of constructor arguments.
 * This is particularly useful for creating objects in containers
 * without unnecessary copies or moves.
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique_with_forwarding(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * @brief Conditional move utility
 * 
 * Moves the value if the condition is true, otherwise copies it.
 * Useful for optimizing performance in conditional scenarios.
 */
template<typename T>
T conditional_move(T& value, bool should_move)
{
    return should_move ? std::move(value) : value;
}

/**
 * @brief Safe move utility
 * 
 * Moves the value and leaves the source in a safe, default state.
 * This is useful when you need to ensure the moved-from object
 * is in a predictable state.
 */
template<typename T>
T safe_move(T& source)
{
    T result = std::move(source);
    source = T{};  // Reset to default state
    return result;
}

/**
 * @brief Forwarding reference wrapper
 * 
 * Wraps a value for perfect forwarding in template contexts.
 * This is useful when you need to store a forwarding reference
 * for later use.
 */
template<typename T>
class ForwardingWrapper {
private:
    T&& value_;

public:
    explicit ForwardingWrapper(T&& value) : value_(std::forward<T>(value)) {}
    
    T&& get() && { return std::forward<T>(value_); }
    
    template<typename Func>
    auto apply(Func&& func) && -> decltype(func(std::forward<T>(value_)))
    {
        return func(std::forward<T>(value_));
    }
};

/**
 * @brief Create a forwarding wrapper
 */
template<typename T>
ForwardingWrapper<T> make_forwarding_wrapper(T&& value)
{
    return ForwardingWrapper<T>(std::forward<T>(value));
}

/**
 * @brief Move-only function wrapper
 * 
 * A simple function wrapper that can only be moved, not copied.
 * This is useful for storing move-only callables.
 */
template<typename Signature>
class MoveOnlyFunction;

template<typename R, typename... Args>
class MoveOnlyFunction<R(Args...)> {
private:
    std::unique_ptr<void, void(*)(void*)> storage_;
    R(*invoke_)(void*, Args&&...);

    template<typename F>
    static R invoke_impl(void* storage, Args&&... args)
    {
        return (*static_cast<F*>(storage))(std::forward<Args>(args)...);
    }

    template<typename F>
    static void destroy_impl(void* storage)
    {
        static_cast<F*>(storage)->~F();
        delete static_cast<F*>(storage);
    }

public:
    MoveOnlyFunction() = default;

    template<typename F>
    MoveOnlyFunction(F&& func)
        : storage_(new F(std::forward<F>(func)), &destroy_impl<F>)
        , invoke_(&invoke_impl<F>)
    {}

    MoveOnlyFunction(const MoveOnlyFunction&) = delete;
    MoveOnlyFunction& operator=(const MoveOnlyFunction&) = delete;

    MoveOnlyFunction(MoveOnlyFunction&&) = default;
    MoveOnlyFunction& operator=(MoveOnlyFunction&&) = default;

    R operator()(Args... args)
    {
        return invoke_(storage_.get(), std::forward<Args>(args)...);
    }

    explicit operator bool() const
    {
        return static_cast<bool>(storage_);
    }
};

/**
 * @brief Create a move-only function
 */
template<typename F>
auto make_move_only_function(F&& func) -> MoveOnlyFunction<typename std::decay<F>::type>
{
    return MoveOnlyFunction<typename std::decay<F>::type>(std::forward<F>(func));
}

/**
 * @brief RAII scope guard with perfect forwarding
 * 
 * Executes a function when the scope guard is destroyed.
 * The function is stored with perfect forwarding.
 */
template<typename Func>
class ScopeGuard {
private:
    Func func_;
    bool active_;

public:
    template<typename F>
    explicit ScopeGuard(F&& func) 
        : func_(std::forward<F>(func)), active_(true) {}

    ~ScopeGuard()
    {
        if (active_)
        {
            func_();
        }
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& other) noexcept
        : func_(std::move(other.func_)), active_(other.active_)
    {
        other.active_ = false;
    }

    ScopeGuard& operator=(ScopeGuard&& other) noexcept
    {
        if (this != &other)
        {
            if (active_)
            {
                func_();
            }
            func_ = std::move(other.func_);
            active_ = other.active_;
            other.active_ = false;
        }
        return *this;
    }

    void dismiss()
    {
        active_ = false;
    }
};

/**
 * @brief Create a scope guard
 */
template<typename F>
ScopeGuard<typename std::decay<F>::type> make_scope_guard(F&& func)
{
    return ScopeGuard<typename std::decay<F>::type>(std::forward<F>(func));
}

} // namespace Utility
} // namespace ArkSigning

#endif // ARKSIGNING_UTILITY_H
