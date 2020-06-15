
#include "ccl_crypto.h"

namespace ccl {

crypto_init_s g_crypto;

// === members of structure crypto_s ===========================================

const std::array<char,16> crypto_s::c_base16_map = {
  '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

// === methods of structure crypto_s ===========================================

auto crypto_s::encode_base16(unsigned a_size,const char *a_data,array<char> *a_trg) -> void
{/*{{{*/
  if (a_size != 0)
  {
    a_trg->reserve(a_size << 1);

    const auto *s_ptr = reinterpret_cast<const unsigned char *>(a_data);
    const auto *s_ptr_end = s_ptr + a_size;
    auto *t_ptr = reinterpret_cast<unsigned char *>(a_trg->data() + a_trg->used());

    do {
      *t_ptr++ = c_base16_map[*s_ptr >> 4];
      *t_ptr++ = c_base16_map[*s_ptr & 0x0f];
    } while(++s_ptr < s_ptr_end);

    a_trg->used() += a_size << 1;
  }
}/*}}}*/

auto crypto_s::decode_base16(unsigned a_size,const char *a_data,array<char> *a_trg) -> void
{/*{{{*/
  if ((a_size & 0x01) != 0)
  {
    throw_error(CRYPTO_INVALID_BASE_DATA_SIZE);
  }

  if (a_size != 0)
  {
    a_trg->reserve(a_size >> 1);

    const auto *s_ptr = reinterpret_cast<const unsigned char *>(a_data);
    const auto *s_ptr_end = s_ptr + a_size;
    auto *t_ptr = reinterpret_cast<unsigned char *>(a_trg->data() + a_trg->used());

#define BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(OPERATOR) \
{/*{{{*/\
  unsigned char ch = *s_ptr++;\
\
  if      (ch >= '0' && ch <= '9') { *t_ptr OPERATOR ch - '0'; }\
  else if (ch >= 'a' && ch <= 'f') { *t_ptr OPERATOR 10 + (ch - 'a'); }\
  else if (ch >= 'A' && ch <= 'F') { *t_ptr OPERATOR 10 + (ch - 'A'); }\
  else\
  {\
    throw_error(CRYPTO_INVALID_BASE_DATA);\
  }\
}/*}}}*/

    do {
      BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(=);
      *t_ptr <<= 4;
      BIC_CRYPTO_METHOD_DECODE_BASE16_CHARACTER(+=);
    } while(++t_ptr,s_ptr < s_ptr_end);

    a_trg->used() += a_size >> 1;
  }
}/*}}}*/

auto crypto_s::encode_base64(unsigned a_size,const char *a_data,array<char> *a_trg) -> void
{/*{{{*/
  if (a_size != 0)
  {
    unsigned encode_size = ((a_size/3 + 1) << 2) + 1;
    a_trg->reserve(encode_size);

    a_trg->used() += EVP_EncodeBlock(
        reinterpret_cast<unsigned char *>(a_trg->data() + a_trg->used()),
        reinterpret_cast<const unsigned char *>(a_data),
        a_size);
  }
}/*}}}*/

auto crypto_s::decode_base64(unsigned a_size,const char *a_data,array<char> *a_trg) -> void
{/*{{{*/
  if ((a_size & 0x03) != 0)
  {
    throw_error(CRYPTO_INVALID_BASE_DATA_SIZE);
  }

  if (a_size != 0)
  {
    unsigned decode_size = ((a_size >> 2) * 3) + 1;
    a_trg->reserve(decode_size);

    int length = EVP_DecodeBlock(
        reinterpret_cast<unsigned char *>(a_trg->data() + a_trg->used()),
        reinterpret_cast<const unsigned char *>(a_data),
        a_size);

    // - ERROR -
    if (length == -1)
    {
      throw_error(CRYPTO_INVALID_BASE_DATA);
    }

    // - adjust target length according to input padding characters -
    if (a_data[a_size - 2] == '=')
    {
      length -= 2;
    }
    else if (a_data[a_size - 1] == '=')
    {
      length -= 1;
    }

    a_trg->used() += length;
  }
}/*}}}*/

} // namespace ccl

