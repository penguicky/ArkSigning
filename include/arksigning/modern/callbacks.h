#ifndef ARKSIGNING_CALLBACKS_H
#define ARKSIGNING_CALLBACKS_H

#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include "types.h"

namespace ArkSigning {
namespace Callbacks {

/**
 * @brief Modern callback system using lambda expressions
 * 
 * This provides a type-safe, efficient callback system that replaces
 * traditional function pointers with modern lambda expressions.
 */

// ============================================================================
// CALLBACK TYPE DEFINITIONS
// ============================================================================

using ProgressCallback = std::function<void(double progress, const std::string& message)>;
using ErrorCallback = std::function<void(Types::ErrorCode code, const std::string& message)>;
using CompletionCallback = std::function<void(bool success, const std::string& result)>;
using FileProcessCallback = std::function<bool(const std::string& filePath)>;
using ValidationCallback = std::function<bool(const std::string& data, std::string& errorMessage)>;

// Specialized callbacks for signing operations
using SigningProgressCallback = std::function<void(const std::string& fileName, int current, int total)>;
using SigningErrorCallback = std::function<void(const std::string& fileName, const std::string& error)>;
using SigningCompletionCallback = std::function<void(int successful, int total, double elapsedSeconds)>;

// ============================================================================
// CALLBACK MANAGER CLASS
// ============================================================================

/**
 * @brief Thread-safe callback manager for handling multiple callback types
 */
class CallbackManager {
private:
    mutable std::mutex mutex_;
    ProgressCallback progressCallback_;
    ErrorCallback errorCallback_;
    CompletionCallback completionCallback_;
    SigningProgressCallback signingProgressCallback_;
    SigningErrorCallback signingErrorCallback_;
    SigningCompletionCallback signingCompletionCallback_;

public:
    CallbackManager() = default;
    ~CallbackManager() = default;

    // Disable copy and move semantics for thread safety (mutex can't be moved)
    CallbackManager(const CallbackManager&) = delete;
    CallbackManager& operator=(const CallbackManager&) = delete;
    CallbackManager(CallbackManager&&) = delete;
    CallbackManager& operator=(CallbackManager&&) = delete;

    // ========================================================================
    // CALLBACK REGISTRATION
    // ========================================================================

    void setProgressCallback(ProgressCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        progressCallback_ = std::move(callback);
    }

    void setErrorCallback(ErrorCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        errorCallback_ = std::move(callback);
    }

    void setCompletionCallback(CompletionCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        completionCallback_ = std::move(callback);
    }

    void setSigningProgressCallback(SigningProgressCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        signingProgressCallback_ = std::move(callback);
    }

    void setSigningErrorCallback(SigningErrorCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        signingErrorCallback_ = std::move(callback);
    }

    void setSigningCompletionCallback(SigningCompletionCallback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        signingCompletionCallback_ = std::move(callback);
    }

    // ========================================================================
    // CALLBACK INVOCATION
    // ========================================================================

    void reportProgress(double progress, const std::string& message) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (progressCallback_) {
            progressCallback_(progress, message);
        }
    }

    void reportError(Types::ErrorCode code, const std::string& message) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (errorCallback_) {
            errorCallback_(code, message);
        }
    }

    void reportCompletion(bool success, const std::string& result) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (completionCallback_) {
            completionCallback_(success, result);
        }
    }

    void reportSigningProgress(const std::string& fileName, int current, int total) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (signingProgressCallback_) {
            signingProgressCallback_(fileName, current, total);
        }
    }

    void reportSigningError(const std::string& fileName, const std::string& error) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (signingErrorCallback_) {
            signingErrorCallback_(fileName, error);
        }
    }

    void reportSigningCompletion(int successful, int total, double elapsedSeconds) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (signingCompletionCallback_) {
            signingCompletionCallback_(successful, total, elapsedSeconds);
        }
    }

    // ========================================================================
    // UTILITY METHODS
    // ========================================================================

    bool hasProgressCallback() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<bool>(progressCallback_);
    }

    bool hasErrorCallback() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<bool>(errorCallback_);
    }

    bool hasCompletionCallback() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<bool>(completionCallback_);
    }

    void clearAllCallbacks() {
        std::lock_guard<std::mutex> lock(mutex_);
        progressCallback_ = nullptr;
        errorCallback_ = nullptr;
        completionCallback_ = nullptr;
        signingProgressCallback_ = nullptr;
        signingErrorCallback_ = nullptr;
        signingCompletionCallback_ = nullptr;
    }
};

// ============================================================================
// LAMBDA UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Create a progress callback that prints to console
 */
inline ProgressCallback createConsoleProgressCallback() {
    return [](double progress, const std::string& message) {
        printf("Progress: %.1f%% - %s\n", progress * 100.0, message.c_str());
        fflush(stdout);
    };
}

/**
 * @brief Create an error callback that prints to stderr
 */
inline ErrorCallback createConsoleErrorCallback() {
    return [](Types::ErrorCode code, const std::string& message) {
        fprintf(stderr, "Error %d: %s\n", code, message.c_str());
        fflush(stderr);
    };
}

/**
 * @brief Create a completion callback that prints results
 */
inline CompletionCallback createConsoleCompletionCallback() {
    return [](bool success, const std::string& result) {
        if (success) {
            printf("✅ Success: %s\n", result.c_str());
        } else {
            printf("❌ Failed: %s\n", result.c_str());
        }
        fflush(stdout);
    };
}

/**
 * @brief Create a signing progress callback with modern formatting
 */
inline SigningProgressCallback createModernSigningProgressCallback() {
    return [](const std::string& fileName, int current, int total) {
        double progress = static_cast<double>(current) / total;
        printf("\r🔄 [%3d/%3d] (%.1f%%) %s", 
               current, total, progress * 100.0, fileName.c_str());
        fflush(stdout);
        if (current == total) {
            printf("\n");
        }
    };
}

/**
 * @brief Create a signing error callback with modern formatting
 */
inline SigningErrorCallback createModernSigningErrorCallback() {
    return [](const std::string& fileName, const std::string& error) {
        printf("\n❌ Failed: %s - %s\n", fileName.c_str(), error.c_str());
        fflush(stdout);
    };
}

/**
 * @brief Create a signing completion callback with modern formatting
 */
inline SigningCompletionCallback createModernSigningCompletionCallback() {
    return [](int successful, int total, double elapsedSeconds) {
        printf("\n🎉 Completed: %d/%d successful (%.2f seconds)\n", 
               successful, total, elapsedSeconds);
        if (successful == total) {
            printf("✅ All files signed successfully!\n");
        } else {
            printf("⚠️  %d files failed to sign\n", total - successful);
        }
        fflush(stdout);
    };
}

/**
 * @brief Create a file validation callback using lambda (C++11 compatible)
 */
template<typename Predicate>
inline ValidationCallback createValidationCallback(Predicate predicate, const std::string& errorMsg) {
    return [predicate, errorMsg](const std::string& data, std::string& errorMessage) -> bool {
        if (!predicate(data)) {
            errorMessage = errorMsg;
            return false;
        }
        return true;
    };
}

/**
 * @brief Create a batch processing callback with lambda (C++11 compatible)
 */
template<typename Processor>
inline FileProcessCallback createBatchProcessor(Processor processor) {
    return [processor](const std::string& filePath) -> bool {
        return processor(filePath);
    };
}

} // namespace Callbacks
} // namespace ArkSigning

#endif // ARKSIGNING_CALLBACKS_H
