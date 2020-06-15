
#ifndef LIBS_LIBCRYPTO_CCL_HEADER_FILES_CCL_CRYPTO_H
#define LIBS_LIBCRYPTO_CCL_HEADER_FILES_CCL_CRYPTO_H

#include "ccl_base.h"

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

namespace ccl {

constexpr int CRYPTO_INVALID_BASE_DATA_SIZE = 1;
constexpr int CRYPTO_INVALID_BASE_DATA = 2;

// === definition of structure crypto_init_s ===================================

struct crypto_init_s
{/*{{{*/
public:
  crypto_init_s() noexcept
  {/*{{{*/
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
  }/*}}}*/

  ~crypto_init_s() noexcept
  {/*{{{*/
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
  }/*}}}*/
};/*}}}*/

// === definition of structure crypto_s ========================================

struct crypto_s
{/*{{{*/
private:
  static const std::array<char,16> c_base16_map;

public:
  static auto random(int a_count,array<char> *a_trg) -> void
  {/*{{{*/
    a_trg->reserve(a_count);

    RAND_bytes(reinterpret_cast<unsigned char *>(a_trg->data() + a_trg->used()),a_count);

    a_trg->used() += a_count;
  }/*}}}*/

  static auto encode_base16(unsigned a_size,const char *a_data,array<char> *a_trg) -> void;
  static auto decode_base16(unsigned a_size,const char *a_data,array<char> *a_trg) -> void;
  static auto encode_base64(unsigned a_size,const char *a_data,array<char> *a_trg) -> void;
  static auto decode_base64(unsigned a_size,const char *a_data,array<char> *a_trg) -> void;
};/*}}}*/

} // namespace ccl

#endif

