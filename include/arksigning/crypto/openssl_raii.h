#ifndef OPENSSL_RAII_H
#define OPENSSL_RAII_H

#include <memory>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/cms.h>
#include <openssl/conf.h>
#include <openssl/asn1.h>
#include <openssl/pkcs12.h>
#include <openssl/bn.h>

namespace ArkSigning {
namespace OpenSSL {

/**
 * @brief RAII wrapper for OpenSSL BIO objects
 * 
 * Automatically manages BIO lifecycle with proper cleanup.
 * Provides safe access to BIO operations with automatic resource management.
 */
class BIOWrapper {
private:
    std::unique_ptr<BIO, decltype(&BIO_free)> bio_;

public:
    explicit BIOWrapper(BIO* bio) : bio_(bio, BIO_free) {}
    
    // Factory methods for common BIO types
    static BIOWrapper createMemBuf(const void* data, int len) {
        return BIOWrapper(BIO_new_mem_buf(data, len));
    }
    
    static BIOWrapper createMem() {
        return BIOWrapper(BIO_new(BIO_s_mem()));
    }
    
    static BIOWrapper createFile(const char* filename, const char* mode) {
        return BIOWrapper(BIO_new_file(filename, mode));
    }
    
    // Access methods
    BIO* get() const { return bio_.get(); }
    BIO* operator->() const { return bio_.get(); }
    operator bool() const { return bio_ != nullptr; }
    
    // Release ownership (for cases where OpenSSL takes ownership)
    BIO* release() { return bio_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL X509 certificate objects
 */
class X509Wrapper {
private:
    std::unique_ptr<X509, decltype(&X509_free)> cert_;

public:
    explicit X509Wrapper(X509* cert) : cert_(cert, X509_free) {}
    
    X509* get() const { return cert_.get(); }
    X509* operator->() const { return cert_.get(); }
    operator bool() const { return cert_ != nullptr; }
    X509* release() { return cert_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL EVP_PKEY objects
 */
class EVPKeyWrapper {
private:
    std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> key_;

public:
    explicit EVPKeyWrapper(EVP_PKEY* key) : key_(key, EVP_PKEY_free) {}
    
    EVP_PKEY* get() const { return key_.get(); }
    EVP_PKEY* operator->() const { return key_.get(); }
    operator bool() const { return key_ != nullptr; }
    EVP_PKEY* release() { return key_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL CMS_ContentInfo objects
 */
class CMSWrapper {
private:
    std::unique_ptr<CMS_ContentInfo, decltype(&CMS_ContentInfo_free)> cms_;

public:
    explicit CMSWrapper(CMS_ContentInfo* cms) : cms_(cms, CMS_ContentInfo_free) {}
    
    CMS_ContentInfo* get() const { return cms_.get(); }
    CMS_ContentInfo* operator->() const { return cms_.get(); }
    operator bool() const { return cms_ != nullptr; }
    CMS_ContentInfo* release() { return cms_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL CONF objects
 */
class CONFWrapper {
private:
    std::unique_ptr<CONF, decltype(&NCONF_free)> conf_;

public:
    explicit CONFWrapper(CONF* conf) : conf_(conf, NCONF_free) {}
    
    static CONFWrapper create() {
        return CONFWrapper(NCONF_new(nullptr));
    }
    
    CONF* get() const { return conf_.get(); }
    CONF* operator->() const { return conf_.get(); }
    operator bool() const { return conf_ != nullptr; }
    CONF* release() { return conf_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL ASN1_TYPE objects
 */
class ASN1TypeWrapper {
private:
    std::unique_ptr<ASN1_TYPE, decltype(&ASN1_TYPE_free)> asn1_;

public:
    explicit ASN1TypeWrapper(ASN1_TYPE* asn1) : asn1_(asn1, ASN1_TYPE_free) {}
    
    ASN1_TYPE* get() const { return asn1_.get(); }
    ASN1_TYPE* operator->() const { return asn1_.get(); }
    operator bool() const { return asn1_ != nullptr; }
    ASN1_TYPE* release() { return asn1_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL PKCS12 objects
 */
class PKCS12Wrapper {
private:
    std::unique_ptr<PKCS12, decltype(&PKCS12_free)> p12_;

public:
    explicit PKCS12Wrapper(PKCS12* p12) : p12_(p12, PKCS12_free) {}
    
    PKCS12* get() const { return p12_.get(); }
    PKCS12* operator->() const { return p12_.get(); }
    operator bool() const { return p12_ != nullptr; }
    PKCS12* release() { return p12_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL BIGNUM objects
 */
class BIGNUMWrapper {
private:
    std::unique_ptr<BIGNUM, decltype(&BN_free)> bn_;

public:
    explicit BIGNUMWrapper(BIGNUM* bn) : bn_(bn, BN_free) {}
    
    BIGNUM* get() const { return bn_.get(); }
    BIGNUM* operator->() const { return bn_.get(); }
    operator bool() const { return bn_ != nullptr; }
    BIGNUM* release() { return bn_.release(); }
};

/**
 * @brief RAII wrapper for OpenSSL X509 certificate stacks
 */
class X509StackWrapper {
private:
    std::unique_ptr<STACK_OF(X509), void(*)(STACK_OF(X509)*)> stack_;

    static void stack_deleter(STACK_OF(X509)* stack) {
        sk_X509_pop_free(stack, X509_free);
    }

public:
    explicit X509StackWrapper(STACK_OF(X509)* stack) : stack_(stack, stack_deleter) {}

    static X509StackWrapper create() {
        return X509StackWrapper(sk_X509_new_null());
    }

    STACK_OF(X509)* get() const { return stack_.get(); }
    operator bool() const { return stack_ != nullptr; }
    STACK_OF(X509)* release() { return stack_.release(); }

    // Convenience methods
    bool push(X509* cert) {
        return sk_X509_push(stack_.get(), cert) > 0;
    }

    int size() const {
        return sk_X509_num(stack_.get());
    }
};

/**
 * @brief RAII wrapper for OpenSSL X509_ATTRIBUTE objects
 */
class X509AttributeWrapper {
private:
    std::unique_ptr<X509_ATTRIBUTE, decltype(&X509_ATTRIBUTE_free)> attr_;

public:
    explicit X509AttributeWrapper(X509_ATTRIBUTE* attr) : attr_(attr, X509_ATTRIBUTE_free) {}
    
    static X509AttributeWrapper create() {
        return X509AttributeWrapper(X509_ATTRIBUTE_new());
    }
    
    X509_ATTRIBUTE* get() const { return attr_.get(); }
    X509_ATTRIBUTE* operator->() const { return attr_.get(); }
    operator bool() const { return attr_ != nullptr; }
    X509_ATTRIBUTE* release() { return attr_.release(); }
};

} // namespace OpenSSL
} // namespace ArkSigning

#endif // OPENSSL_RAII_H
