
#ifndef LIBS_LIBBASE_CCL_HEADER_FILES_CCL_QUEUE_H
#define LIBS_LIBBASE_CCL_HEADER_FILES_CCL_QUEUE_H

namespace ccl {

template <typename VALUE,typename INDEX = uint32_t>
struct queue
{
private:
  INDEX m_size = 0;
  INDEX m_used = 0;
  INDEX m_begin = 0;
  VALUE *m_data = nullptr;

  template<class TYPE = VALUE>
  auto _destruct() -> typename std::enable_if<std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
    if (m_data != nullptr)
    {
      cfree(m_data);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _destruct() -> typename std::enable_if<!std::is_trivially_destructible<TYPE>::value,void>::type
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
  auto _copy_from(const queue &a_src) noexcept -> typename std::enable_if<std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    INDEX fir_cnt;
    INDEX sec_cnt;

    if (a_src.m_begin + a_src.m_used > a_src.m_size)
    {
      sec_cnt = a_src.m_begin + a_src.m_used - a_src.m_size;
      fir_cnt = a_src.m_used - sec_cnt;
    }
    else
    {
      fir_cnt = a_src.m_used;
      sec_cnt = 0;
    }

    memcpy(m_data,a_src.m_data + a_src.m_begin,fir_cnt*sizeof(VALUE));

    if (sec_cnt > 0)
    {
      memcpy(m_data + fir_cnt,a_src.m_data,sec_cnt*sizeof(VALUE));
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_from(const queue &a_src) noexcept -> typename std::enable_if<!std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    INDEX sec_cnt;
    VALUE *ptr = m_data;
    VALUE *s_ptr = a_src.m_data + a_src.m_begin;
    VALUE *s_ptr_end;

    if (a_src.m_begin + a_src.m_used > a_src.m_size)
    {
      s_ptr_end = a_src.m_data + a_src.m_size;
      sec_cnt = a_src.m_begin + a_src.m_used - a_src.m_size;
    }
    else
    {
      s_ptr_end = s_ptr + a_src.m_used;
      sec_cnt = 0;
    }

    do {
      *ptr = *s_ptr;
    } while(++ptr,++s_ptr < s_ptr_end);

    if (sec_cnt > 0)
    {
      s_ptr = a_src.m_data;
      s_ptr_end = s_ptr + sec_cnt;

      do {
        *ptr = *s_ptr;
      } while(++ptr,++s_ptr < s_ptr_end);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _queue_initlist_copy(const VALUE *a_data,size_t a_size) -> typename std::enable_if<std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    memcpy(m_data,a_data,a_size*sizeof(VALUE));
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _queue_initlist_copy(const VALUE *a_data,size_t a_size) -> typename std::enable_if<!std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    INDEX sec_cnt;
    VALUE *ptr = m_data;
    VALUE *s_ptr = a_data;
    VALUE *s_ptr_end = s_ptr = a_size;

    do {
      *ptr = *s_ptr;
    } while(++ptr,++s_ptr < s_ptr_end);
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_init(VALUE *n_data,INDEX a_size) -> typename std::enable_if<std::is_trivially_constructible<TYPE>::value,void>::type
  {/*{{{*/
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_init(VALUE *n_data,INDEX a_size) -> typename std::enable_if<!std::is_trivially_constructible<TYPE>::value,void>::type
  {/*{{{*/
    static_assert(std::is_nothrow_constructible<VALUE>::value,"");

    if (a_size > m_used)
    {
      VALUE *ptr = n_data + m_used;
      VALUE *ptr_end = n_data + a_size;

      do {
        new (ptr) VALUE;
      } while(++ptr < ptr_end);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_move(VALUE *n_data,INDEX fir_cnt,INDEX sec_cnt) -> typename std::enable_if<std::is_trivially_move_constructible<TYPE>::value,void>::type
  {/*{{{*/
    memcpy(n_data,m_data + m_begin,fir_cnt*sizeof(VALUE));

    if (sec_cnt > 0)
    {
      memcpy(n_data + fir_cnt,m_data,sec_cnt*sizeof(VALUE));
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_move(VALUE *n_data,INDEX fir_cnt,INDEX sec_cnt) -> typename std::enable_if<!std::is_trivially_move_constructible<TYPE>::value,void>::type
  {/*{{{*/
    static_assert(std::is_nothrow_move_constructible<VALUE>::value,"");

    VALUE *ptr = n_data;
    VALUE *ptr_end = ptr + fir_cnt;
    VALUE *s_ptr = m_data + m_begin;

    do {
      new (ptr) VALUE(std::move(*s_ptr));
    } while(++s_ptr,++ptr < ptr_end);

    if (sec_cnt > 0)
    {
      ptr_end = ptr + sec_cnt;
      s_ptr = m_data;

      do {
        new (ptr) VALUE(std::move(*s_ptr));
      } while(++s_ptr,++ptr < ptr_end);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_clear() -> typename std::enable_if<std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
  }/*}}}*/

  template<class TYPE = VALUE>
  auto _copy_resize_clear() -> typename std::enable_if<!std::is_trivially_destructible<TYPE>::value,void>::type
  {/*{{{*/
    static_assert(std::is_nothrow_destructible<VALUE>::value,"");

    if (m_size > m_used)
    {
      VALUE *ptr;
      VALUE *ptr_end;
      VALUE *s_ptr;

      if (m_begin + m_used >= m_size)
      {
        ptr = m_data + (m_begin + m_used - m_size);
        ptr_end = m_data + m_begin;
        s_ptr = nullptr;
      }
      else
      {
        ptr = m_data;
        ptr_end = m_data + m_begin;
        s_ptr = ptr_end + m_used;
      }

      if (ptr < ptr_end)
      {
        do {
          ptr->~VALUE();
        } while(++ptr < ptr_end);
      }

      if (s_ptr != nullptr)
      {
        VALUE *s_ptr_end = m_data + m_size;
        do {
          s_ptr->~VALUE();
        } while(++s_ptr < s_ptr_end);
      }
    }
  }/*}}}*/

  auto _insert() noexcept -> INDEX
  {/*{{{*/
    if (m_used >= m_size)
    {
      INDEX new_size = (m_size << 1) + c_array_add;
      debug_assert(new_size > 0);

      copy_resize(new_size);
    }

    INDEX inserted_idx = m_begin + m_used++;

    if (inserted_idx >= m_size)
    {
      inserted_idx -= m_size;
    }

    return inserted_idx;
  }/*}}}*/

public:
  [[nodiscard]] constexpr auto size() const noexcept -> const INDEX & { return m_size; }
  [[nodiscard]] constexpr auto used() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto count() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto begin() const noexcept -> const INDEX & { return m_begin; }
  [[nodiscard]] constexpr auto data() const noexcept -> const VALUE * { return m_data; }

  ~queue()
  {/*{{{*/
    _destruct();
  }/*}}}*/

  queue() = default;

  queue(const queue &a_src) noexcept
  {/*{{{*/
    if (a_src.m_used <= 0)
    {
      return;
    }

    copy_resize(a_src.m_used);
    _copy_from(a_src);

    m_used = a_src.m_used;
  }/*}}}*/

  queue(queue &&a_src) noexcept :
    m_size(a_src.m_size),
    m_used(a_src.m_used),
    m_begin(a_src.m_begin),
    m_data(a_src.m_data)
  {/*{{{*/
    new (&a_src) queue;
  }/*}}}*/

  explicit queue(INDEX a_size) noexcept
  {/*{{{*/
    copy_resize(a_size);
  }/*}}}*/

  queue(std::initializer_list<VALUE> a_init)
  {/*{{{*/
    if (a_init.size() <= 0)
    {
      return;
    }

    copy_resize(a_init.size());
    _queue_initlist_copy(a_init.begin(),a_init.size());

    m_used = a_init.size();
  }/*}}}*/

  auto operator=(const queue &a_src) noexcept -> queue &
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
      _copy_from(a_src);
    }

    m_used = a_src.m_used;
    m_begin = 0;

    return *this;
  }/*}}}*/

  auto operator=(queue &&a_src) noexcept -> queue &
  {/*{{{*/
    this->~queue();
    new (this) queue(std::move(a_src));

    return *this;
  }/*}}}*/

  auto clear() noexcept -> queue &
  {/*{{{*/
    this->~queue();
    new (this) queue;

    return *this;
  }/*}}}*/

  auto reset() noexcept -> queue &
  {/*{{{*/
    m_used = 0;
    m_begin = 0;

    return *this;
  }/*}}}*/

  auto reserve(INDEX a_cnt) noexcept -> queue &
  {/*{{{*/
    INDEX required_cnt = m_used + a_cnt;

    if (required_cnt > m_size)
    {
      INDEX r_size = m_size;
      do {
        r_size = (r_size << 1) + c_array_add;
      } while(r_size < required_cnt);

      copy_resize(r_size);
    }

    return *this;
  }/*}}}*/

  auto flush() noexcept -> queue &
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<std::is_trivial<TYPE>::value,queue &>::type
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<!std::is_trivial<TYPE>::value,queue &>::type
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

  auto swap(queue &a_second) noexcept -> queue &
  {/*{{{*/
    if (this == &a_second)
    {
      return *this;
    }

    queue tmp(std::move(*this));
    *this = std::move(a_second);
    a_second = std::move(tmp);

    return *this;
  }/*}}}*/

  auto insert(VALUE &&a_value) noexcept -> queue &
  {/*{{{*/
    INDEX inserted_idx = _insert();

    m_data[inserted_idx] = std::move(a_value);

    return *this;
  }/*}}}*/

  auto insert(const VALUE &a_value) noexcept -> queue &
  {/*{{{*/
    INDEX inserted_idx = _insert();

    m_data[inserted_idx] = a_value;

    return *this;
  }/*}}}*/

  auto next() noexcept -> VALUE &
  {/*{{{*/
    debug_assert(m_used > 0);

    INDEX ret_idx = m_begin;

    if (++m_begin >= m_size)
    {
      m_begin = 0;
    }

    m_used--;

    return m_data[ret_idx];
  }/*}}}*/

  auto first() noexcept -> VALUE &
  {/*{{{*/
    debug_assert(m_used > 0);

    return m_data[m_begin];
  }/*}}}*/

  auto last() noexcept -> VALUE &
  {/*{{{*/
    debug_assert(m_used > 0);

    INDEX last_idx = m_begin + (m_used - 1);
    if (last_idx >= m_size)
    {
      return m_data[last_idx - m_size];
    }

    return m_data[last_idx];
  }/*}}}*/

  auto copy_resize(INDEX a_size) noexcept -> queue &
  {/*{{{*/
    debug_assert(a_size >= m_used);

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

      _copy_resize_init(n_data,a_size);
    }

    if (m_used > 0)
    {
      INDEX fir_cnt;
      INDEX sec_cnt;

      if (m_begin + m_used > m_size)
      {
        sec_cnt = m_begin + m_used - m_size;
        fir_cnt = m_used - sec_cnt;
      }
      else
      {
        fir_cnt = m_used;
        sec_cnt = 0;
      }

      _copy_resize_move(n_data,fir_cnt,sec_cnt);
    }

    _copy_resize_clear();

    if (m_data != nullptr)
    {
      cfree(m_data);
    }

    m_data = n_data;
    m_size = a_size;
    m_begin = 0;

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto operator==(const queue &a_second) const noexcept -> typename std::enable_if<std::is_compound<TYPE>::value,bool>::type
  {/*{{{*/
    if (m_used != a_second.m_used)
    {
      return false;
    }

    if (m_used <= 0)
    {
      return true;
    }

    VALUE *ptr = m_data + m_begin;
    VALUE *ptr_break = m_data + m_size;
    VALUE *ptr_end;
    VALUE *s_ptr = a_second.m_data + a_second.m_begin;
    VALUE *s_ptr_break = a_second.m_data + a_second.m_size;

    if (m_begin + m_used > m_size)
    {
      ptr_end = m_data + (m_begin + m_used - m_size);
    }
    else
    {
      ptr_end = ptr + m_used;
    }

    do {
      if (!(*ptr == *s_ptr))
      {
        return false;
      }

      if (++ptr >= ptr_break)
      {
        ptr = m_data;
      }

      if (++s_ptr >= s_ptr_break)
      {
        s_ptr = a_second.m_data;
      }

    } while(ptr != ptr_end);

    return true;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto operator==(const queue &a_second) const noexcept -> typename std::enable_if<!std::is_compound<TYPE>::value,bool>::type
  {/*{{{*/
    if (m_used != a_second.m_used)
    {
      return false;
    }

    if (m_used <= 0)
    {
      return true;
    }

    bool f_break;
    bool s_break;
    INDEX pos;
    INDEX pos_end;
    INDEX s_pos;

    f_break = (m_begin + m_used > m_size);
    s_break = (a_second.m_begin + a_second.m_used > a_second.m_size);
    pos = m_begin;
    s_pos = a_second.m_begin;

    if (f_break)
    {
      pos_end = m_begin + m_used - m_size;
    }
    else
    {
      pos_end = m_begin + m_used;
    }

    do {
      if (f_break)
      {
        INDEX offset = m_size - pos;

        if (s_break)
        {
          INDEX s_offset = a_second.m_size - s_pos;

          if (offset < s_offset)
          {
            if (memcmp(m_data + pos,a_second.m_data + s_pos,offset*sizeof(VALUE)) != 0)
            {
              return false;
            }

            s_pos += offset;
            pos = 0;
            f_break = false;
          }
          else
          {
            if (memcmp(m_data + pos,a_second.m_data + s_pos,s_offset*sizeof(VALUE)) != 0)
            {
              return false;
            }

            if ((pos += s_offset) >= m_size)
            {
              pos = 0;
              f_break = false;
            }

            s_pos = 0;
            s_break = false;
          }
        }
        else
        {
          if (memcmp(m_data + pos,a_second.m_data + s_pos,offset*sizeof(VALUE)) != 0)
          {
            return false;
          }

          s_pos += offset;
          pos = 0;
          f_break = false;
        }
      }
      else
      {
        if (s_break)
        {
          INDEX s_offset = a_second.m_size - s_pos;

          if (memcmp(m_data + pos,a_second.m_data + s_pos,s_offset*sizeof(VALUE)) != 0)
          {
            return false;
          }

          pos += s_offset;
          s_pos = 0;
          s_break = false;
        }
        else
        {
          return memcmp(m_data + pos,a_second.m_data + s_pos,(pos_end - pos)*sizeof(VALUE)) == 0;
        }
      }
    } while(true);
  }/*}}}*/

  [[nodiscard]] auto compare(const queue &a_second) const noexcept -> int
  {/*{{{*/
    if (m_used < a_second.m_used) { return -1; }
    if (m_used > a_second.m_used) { return 1; }

    if (m_used > 0)
    {
      VALUE *ptr = m_data + m_begin;
      VALUE *ptr_break = m_data + m_size;
      VALUE *ptr_end;
      VALUE *s_ptr = a_second.m_data + a_second.m_begin;
      VALUE *s_ptr_break = a_second.m_data + a_second.m_size;

      if (m_begin + m_used > m_size)
      {
        ptr_end = m_data + (m_begin + m_used - m_size);
      }
      else
      {
        ptr_end = ptr + m_used;
      }

      do
      {
        int res = ::compare(*ptr,*s_ptr);

        if (res != 0)
        {
          return res;
        }

        if (++ptr >= ptr_break)
        {
          ptr = m_data;
        }

        if (++s_ptr >= s_ptr_break)
        {
          s_ptr = a_second.m_data;
        }

      } while(ptr != ptr_end);
    }

    return 0;
  }/*}}}*/

  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    a_trg->push('[');

    if (m_used > 0)
    {
      INDEX sec_cnt;
      VALUE *ptr = m_data + m_begin;
      VALUE *ptr_end;

      if (m_begin + m_used > m_size)
      {
        ptr_end = m_data + m_size;
        sec_cnt = m_begin + m_used - m_size;
      }
      else
      {
        ptr_end = ptr + m_used;
        sec_cnt = 0;
      }

      do {
        a_trg->string(*ptr);

        if (++ptr >= ptr_end)
        {
          break;
        }

        a_trg->push(',');
      } while(true);

      if (sec_cnt > 0)
      {
        ptr = m_data;
        ptr_end = ptr + sec_cnt;

        do {
          (*a_trg)
            .push(',')
            .string(*ptr);
        } while(++ptr < ptr_end);
      }
    }

    return a_trg->push(']');
  }/*}}}*/

  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    a_trg->push('[');

    if (m_used > 0)
    {
      INDEX sec_cnt;
      VALUE *ptr = m_data + m_begin;
      VALUE *ptr_end;

      if (m_begin + m_used > m_size)
      {
        ptr_end = m_data + m_size;
        sec_cnt = m_begin + m_used - m_size;
      }
      else
      {
        ptr_end = ptr + m_used;
        sec_cnt = 0;
      }

      do {
        a_trg->json(*ptr);

        if (++ptr >= ptr_end)
        {
          break;
        }

        a_trg->push(',');
      } while(true);

      if (sec_cnt > 0)
      {
        ptr = m_data;
        ptr_end = ptr + sec_cnt;

        do {
          (*a_trg)
            .push(',')
            .json(*ptr);
        } while(++ptr < ptr_end);
      }
    }

    return a_trg->push(']');
  }/*}}}*/

  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    if (m_used > 0)
    {
      (*a_trg)
        .push('[')
        .json(a_json_nice->push());

      INDEX sec_cnt;
      VALUE *ptr = m_data + m_begin;
      VALUE *ptr_end;

      if (m_begin + m_used > m_size)
      {
        ptr_end = m_data + m_size;
        sec_cnt = m_begin + m_used - m_size;
      }
      else
      {
        ptr_end = ptr + m_used;
        sec_cnt = 0;
      }

      do {
        a_trg->json_nice(*ptr,a_json_nice);

        if (++ptr >= ptr_end)
        {
          break;
        }

        (*a_trg)
          .push(',')
          .json(*a_json_nice);
      } while(true);

      if (sec_cnt > 0)
      {
        ptr = m_data;
        ptr_end = ptr + sec_cnt;

        do {
          (*a_trg)
            .push(',')
            .json(*a_json_nice);

          a_trg->json_nice(*ptr,a_json_nice);
        } while(++ptr < ptr_end);
      }

      (*a_trg)
        .json(a_json_nice->pop())
        .push(']');
    }
    else
    {
      (*a_trg)
        .push('[')
        .push(']');
    }

    return *a_trg;
  }/*}}}*/
};

} // namespace ccl

#endif

