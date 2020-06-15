
#ifndef LIBS_LIBBASE_CCL_HEADER_FILES_CCL_ARRAY_H
#define LIBS_LIBBASE_CCL_HEADER_FILES_CCL_ARRAY_H

namespace ccl {

template <typename VALUE,typename INDEX = uint32_t>
struct array
{
  static constexpr INDEX idx_not_exist = std::numeric_limits<INDEX>::max();

private:
  INDEX m_size = 0;
  INDEX m_used = 0;
  VALUE *m_data = nullptr;

  template<class TYPE = VALUE>
  auto __destruct() -> typename std::enable_if<std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
    if (m_data != nullptr)
    {
      cfree(m_data);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __destruct() -> typename std::enable_if<!std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
    if (m_data != nullptr)
    {
      static_assert(std::is_nothrow_destructible<VALUE>::value,"");

      VALUE *ptr = m_data;
      VALUE *ptr_end = ptr + m_size;
      do {
        ptr->~VALUE();
      } while(++ptr < ptr_end);

      cfree(m_data);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const VALUE *a_data) noexcept -> typename std::enable_if<std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    memcpy(m_data,a_data,a_size*sizeof(VALUE));
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const VALUE *a_data) noexcept -> typename std::enable_if<!std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/

    // - ignore std::bad_alloc -
    //static_assert(std::is_nothrow_copy_assignable<VALUE>::value,"");

    VALUE *ptr = m_data;
    const VALUE *s_ptr = a_data;
    const VALUE *s_ptr_end = s_ptr + a_size;

    do {
      *ptr = *s_ptr;
    } while(++ptr,++s_ptr < s_ptr_end);
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_resize_clear(INDEX a_size) -> typename std::enable_if<std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_resize_clear(INDEX a_size) -> typename std::enable_if<!std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
    static_assert(std::is_nothrow_destructible<VALUE>::value,"");

    if (m_size > a_size)
    {
      VALUE *ptr = m_data + a_size;
      VALUE *ptr_end = m_data + m_size;

      do {
        ptr->~VALUE();
      } while(++ptr < ptr_end);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_resize_init(INDEX a_size) -> typename std::enable_if<std::is_trivially_constructible<TYPE>::value,void>::type
  {/*{{{*/
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_resize_init(INDEX a_size) -> typename std::enable_if<!std::is_trivially_constructible<TYPE>::value,void>::type
  {/*{{{*/
    static_assert(std::is_nothrow_constructible<VALUE>::value,"");

    if (a_size > m_size)
    {
      VALUE *ptr = m_data + m_size;
      VALUE *ptr_end = m_data + a_size;

      do {
        new (ptr) VALUE;
      } while(++ptr < ptr_end);
    }
  }/*}}}*/

public:
  auto size() noexcept -> INDEX & { return m_size; }
  auto used() noexcept -> INDEX & { return m_used; }
  auto data() noexcept -> VALUE * { return m_data; }

  [[nodiscard]] constexpr auto size() const noexcept -> const INDEX & { return m_size; }
  [[nodiscard]] constexpr auto used() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto count() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto data() const noexcept -> const VALUE * { return m_data; }

  ~array()
  {/*{{{*/
    __destruct();
  }/*}}}*/

  array() = default;

  array(const array &a_src) noexcept
  {/*{{{*/
    if (a_src.m_used <= 0)
    {
      return;
    }

    copy_resize(a_src.m_used);
    __copy_from(a_src.m_used,a_src.m_data);

    m_used = a_src.m_used;
  }/*}}}*/

  array(array &&a_src) noexcept :
    m_size(a_src.m_size),
    m_used(a_src.m_used),
    m_data(a_src.m_data)
  {/*{{{*/
    new (&a_src) array;
  }/*}}}*/

  explicit array(INDEX a_size) noexcept
  {/*{{{*/
    copy_resize(a_size);
  }/*}}}*/

  array(std::initializer_list<VALUE> a_init)
  {/*{{{*/
    if (a_init.size() <= 0)
    {
      return;
    }

    copy_resize(a_init.size());

    debug_assert(m_data != nullptr);
    __copy_from(a_init.size(),a_init.begin());

    m_used = a_init.size();
  }/*}}}*/

  auto operator=(const array &a_src) noexcept -> array &
  {/*{{{*/
    if (this == &a_src)
    {
      return *this;
    }

    m_used = 0;

    if (m_size < a_src.m_used || m_size > (a_src.m_used << 1))
    {
      copy_resize(a_src.m_used);
    }

    if (a_src.m_used > 0)
    {
      __copy_from(a_src.m_used,a_src.m_data);
    }

    m_used = a_src.m_used;

    return *this;
  }/*}}}*/

  auto operator=(array &&a_src) noexcept -> array &
  {/*{{{*/
    this->~array();
    new (this) array(std::move(a_src));

    return *this;
  }/*}}}*/

  auto clear() noexcept -> array &
  {/*{{{*/
    this->~array();
    new (this) array;

    return *this;
  }/*}}}*/

  auto reset() noexcept -> array &
  {/*{{{*/
    m_used = 0;

    return *this;
  }/*}}}*/

  auto set(INDEX a_size,const VALUE *a_data) noexcept -> array &
  {/*{{{*/
    debug_assert(a_data != nullptr);

    m_used = 0;

    if (m_size < a_size || m_size > (a_size << 1))
    {
      copy_resize(a_size);
    }

    if (a_size > 0)
    {
      __copy_from(a_size,a_data);
    }

    m_used = a_size;

    return *this;
  }/*}}}*/

  auto reserve(INDEX a_cnt) noexcept -> array &
  {/*{{{*/
    INDEX required_cnt = m_used + a_cnt;

    if (required_cnt > m_size)
    {
      INDEX r_size = m_size;
      do {
        r_size = (r_size << 1) + c_array_add;
      } while(r_size < required_cnt);

      debug_assert(r_size > 0);
      copy_resize(r_size);
    }

    return *this;
  }/*}}}*/

  auto flush() noexcept -> array &
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<std::is_trivial<TYPE>::value,array &>::type
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<!std::is_trivial<TYPE>::value,array &>::type
  {/*{{{*/
    copy_resize(m_used);

    if (m_used <= 0)
    {
      return *this;
    }

    VALUE *ptr = m_data;
    VALUE *ptr_end = ptr + m_used;

    do {
      ptr->flush_all();
    } while(++ptr < ptr_end);

    return *this;
  }/*}}}*/

  auto swap(array &a_second) noexcept -> array &
  {/*{{{*/
    if (this == &a_second)
    {
      return *this;
    }

    array tmp(std::move(*this));
    *this = std::move(a_second);
    a_second = std::move(tmp);

    return *this;
  }/*}}}*/

  auto operator[](INDEX a_idx) noexcept -> VALUE &
  {/*{{{*/
    debug_assert(a_idx < m_used);
    return m_data[a_idx];
  }/*}}}*/

  auto operator[](INDEX a_idx) const noexcept -> const VALUE &
  {/*{{{*/
    debug_assert(a_idx < m_used);
    return m_data[a_idx];
  }/*}}}*/

  auto push(VALUE &&a_value) noexcept -> array &
  {/*{{{*/
    if (m_used >= m_size)
    {
      INDEX new_size = (m_size << 1) + c_array_add;
      debug_assert(new_size > 0);

      copy_resize(new_size);
    }

    m_data[m_used++] = std::move(a_value);

    return *this;
  }/*}}}*/

  auto push(const VALUE &a_value) noexcept -> array &
  {/*{{{*/
    if (m_used >= m_size)
    {
      INDEX new_size = (m_size << 1) + c_array_add;
      debug_assert(new_size > 0);

      copy_resize(new_size);
    }

    m_data[m_used++] = a_value;

    return *this;
  }/*}}}*/

  auto push_blank() noexcept -> array &
  {/*{{{*/
    if (m_used >= m_size)
    {
      INDEX new_size = (m_size << 1) + c_array_add;
      debug_assert(new_size > 0);

      copy_resize(new_size);
    }

    ++m_used;

    return *this;
  }/*}}}*/

  auto push_blanks(INDEX a_cnt) noexcept -> array &
  {/*{{{*/
    reserve(a_cnt);

    m_used += a_cnt;

    return *this;
  }/*}}}*/

  auto pop() noexcept -> VALUE &
  {/*{{{*/
    debug_assert(m_used > 0);
    return m_data[--m_used];
  }/*}}}*/

  auto last() noexcept -> VALUE &
  {/*{{{*/
    debug_assert(m_used > 0);
    return m_data[m_used - 1];
  }/*}}}*/

  template<class TYPE = VALUE>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<std::is_trivially_move_constructible<TYPE>::value,array &>::type
  {/*{{{*/
    debug_assert(a_size >= m_used);

    __copy_resize_clear(a_size);

    if (a_size <= 0)
    {
      if (m_data != nullptr)
      {
        cfree(m_data);
      }

      m_data = nullptr;
    }
    else
    {
      m_data = static_cast<VALUE *>(crealloc(m_data,a_size*sizeof(VALUE)));
    }

    __copy_resize_init(a_size);

    m_size = a_size;

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<!std::is_trivially_move_constructible<TYPE>::value,array &>::type
  {/*{{{*/
    debug_assert(a_size >= m_used);

    static_assert(std::is_nothrow_move_constructible<VALUE>::value,"");

    VALUE *n_data;

    if (a_size <= 0)
    {
      n_data = nullptr;
    }
    else
    {
      n_data = static_cast<VALUE *>(cmalloc(a_size*sizeof(VALUE)));
#ifdef __clang_analyzer__
      memset(n_data,0,a_size*sizeof(VALUE));
#endif

      if (m_used < a_size)
      {
        VALUE *ptr = n_data + m_used;
        VALUE *ptr_end = n_data + a_size;

        do {
          new (ptr) VALUE;
        } while(++ptr < ptr_end);
      }
    }

    if (m_used > 0)
    {
      VALUE *ptr = m_data;
      VALUE *ptr_end = ptr + m_used;
      VALUE *n_ptr = n_data;

      do {
        new (n_ptr) VALUE(std::move(*ptr));
      } while(++n_ptr,++ptr < ptr_end);
    }

    if (m_size > m_used)
    {
      VALUE *ptr = m_data + m_used;
      VALUE *ptr_end = m_data + m_size;

      do {
        ptr->~VALUE();
      } while(++ptr < ptr_end);
    }

    if (m_data != nullptr)
    {
      cfree(m_data);
    }

    m_data = n_data;
    m_size = a_size;

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto fill(const VALUE &a_value) noexcept -> typename std::enable_if<std::is_trivial<TYPE>::value && sizeof(TYPE) == 1,array &>::type
  {/*{{{*/
    if (m_size <= 0)
    {
      return *this;
    }

    memset(m_data,a_value,m_size);

    m_used = m_size;

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto fill(const VALUE &a_value) noexcept -> typename std::enable_if<!(std::is_trivial<TYPE>::value && sizeof(TYPE) == 1),array &>::type
  {/*{{{*/
    if (m_size <= 0)
    {
      return *this;
    }

    VALUE *ptr = m_data;
    VALUE *ptr_end = m_data + m_size;

    do {
      *ptr = a_value;
    } while(++ptr < ptr_end);

    m_used = m_size;

    return *this;
  }/*}}}*/

  [[nodiscard]] auto get_idx(const VALUE &a_value) const noexcept -> INDEX
  {/*{{{*/
    if (m_used <= 0)
    {
      return idx_not_exist;
    }

    VALUE *ptr = m_data;
    VALUE *ptr_end = m_data + m_used;

    do {
      if (*ptr == a_value)
      {
        return ptr - m_data;
      }
    } while(++ptr < ptr_end);

    return idx_not_exist;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto operator==(const array &a_second) const noexcept -> typename std::enable_if<std::is_compound<TYPE>::value,bool>::type
  {/*{{{*/
    if (m_used != a_second.m_used)
    {
      return false;
    }

    if (m_used <= 0)
    {
      return true;
    }

    VALUE *ptr = m_data;
    VALUE *ptr_end = ptr + m_used;
    VALUE *s_ptr = a_second.m_data;

    do {
      if (!(*ptr == *s_ptr))
      {
        return false;
      }
    } while(++s_ptr,++ptr < ptr_end);

    return true;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto operator==(const array &a_second) const noexcept -> typename std::enable_if<!std::is_compound<TYPE>::value,bool>::type
  {/*{{{*/
    if (m_used != a_second.m_used)
    {
      return false;
    }

    if (m_used <= 0)
    {
      return true;
    }

    return (memcmp(m_data,a_second.m_data,m_used*sizeof(VALUE)) == 0);
  }/*}}}*/

  [[nodiscard]] auto compare(const array &a_second) const noexcept -> int
  {/*{{{*/
    if (m_used != a_second.m_used)
    {
      return m_used < a_second.m_used ? -1 : 1;
    }

    if (m_used > 0)
    {
      VALUE *f_ptr = m_data;
      VALUE *f_ptr_end = f_ptr + m_used;
      VALUE *s_ptr = a_second.m_data;

      do {
        int res = ::compare(*f_ptr,*s_ptr);
        if (res != 0)
        {
          return res;
        }
      } while(++s_ptr,++f_ptr < f_ptr_end);
    }

    return 0;
  }/*}}}*/

  auto append(INDEX a_count,const char *a_data) noexcept -> array<char> &;
  auto append(const std::string &a_data) noexcept -> array<char> &;
  auto append_format(const char *a_format,va_list a_ap) noexcept -> array<char> &;
  auto append_format(const char *a_format,...) noexcept -> array<char> &;

  explicit array(const char *a_format,...);

  template <typename PARAM>
  auto string(const PARAM &a_value) noexcept -> array<char> &;

  template <typename PARAM>
  auto json(const PARAM &a_value) noexcept -> array<char> &;

  template <typename PARAM>
  auto json_nice(const PARAM &a_value,ccl::json_nice_s *a_json_nice) noexcept -> array<char> &;

  template <typename PARAM>
  auto debug(const PARAM &a_value) noexcept -> const char *;
};

template <>
inline auto array<char>::append(uint32_t a_count,const char *a_data) noexcept -> array<char> &
{/*{{{*/
  if (a_count > 0)
  {
    uint32_t old_used = m_used;
    push_blanks(a_count);

    debug_assert(m_data != nullptr);
    memcpy(m_data + old_used,a_data,a_count);
  }

  return *this;
}/*}}}*/

template <>
inline auto array<char>::append(const std::string &a_data) noexcept -> array<char> &
{/*{{{*/
  return append(a_data.length(),a_data.data());
}/*}}}*/

template <>
auto array<char>::append_format(const char *a_format,va_list a_ap) noexcept -> array<char> &;

template <>
inline auto array<char>::append_format(const char *a_format,...) noexcept -> array<char> & // NOLINT(cert-dcl50-cpp)
{/*{{{*/
  va_list ap;
  va_start(ap,a_format);
  append_format(a_format,ap);
  va_end(ap);

  return *this;
}/*}}}*/

template <>
inline array<char>::array(const char *a_format,...) // NOLINT(cert-dcl50-cpp)
{/*{{{*/
  va_list ap;
  va_start(ap,a_format);
  append_format(a_format,ap);
  va_end(ap);
}/*}}}*/

template <>
template <typename VALUE>
auto ccl::array<char>::debug(const VALUE &a_value) noexcept -> const char *
{/*{{{*/
  return this->reset().string(a_value).push('\0').data();
}/*}}}*/

} // namespace ccl

#endif

