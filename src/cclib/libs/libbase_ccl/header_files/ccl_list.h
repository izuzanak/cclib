
#ifndef LIBS_LIBBASE_CCL_HEADER_FILES_CCL_LIST_H
#define LIBS_LIBBASE_CCL_HEADER_FILES_CCL_LIST_H

namespace ccl {

template <typename VALUE,typename INDEX = uint32_t>
struct list
{
  static constexpr INDEX idx_not_exist = std::numeric_limits<INDEX>::max();

  struct element_s
  {
    friend list;

  private:
    INDEX m_next_idx;
    INDEX m_prev_idx;
    bool m_valid;
    VALUE m_value;

  public:
    [[nodiscard]] auto is_valid() const noexcept -> bool { return m_valid; }
    [[nodiscard]] auto object() const noexcept -> const VALUE & { return m_value; }
  };

private:
  INDEX m_size = 0;
  INDEX m_used = 0;
  INDEX m_count = 0;
  element_s *m_data = nullptr;
  INDEX m_free_idx = idx_not_exist;
  INDEX m_first_idx = idx_not_exist;
  INDEX m_last_idx = idx_not_exist;

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

      element_s *ptr = m_data;
      element_s *ptr_end = ptr + m_size;
      do {
        ptr->m_value.~VALUE();
      } while(++ptr < ptr_end);

      cfree(m_data);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const element_s *a_data) noexcept -> typename std::enable_if<std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    memcpy(m_data,a_data,a_size*sizeof(element_s));
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const element_s *a_data) noexcept -> typename std::enable_if<!std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/

    // - ignore std::bad_alloc -
    //static_assert(std::is_nothrow_copy_assignable<VALUE>::value,"");

    element_s *ptr = m_data;
    element_s *s_ptr = a_data;
    element_s *s_ptr_end = s_ptr + a_size;

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
      element_s *ptr = m_data + a_size;
      element_s *ptr_end = m_data + m_size;

      do {
        ptr->m_value.~VALUE();
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
      element_s *ptr = m_data + m_size;
      element_s *ptr_end = m_data + a_size;

      do {
        new (&ptr->m_value) VALUE;
      } while(++ptr < ptr_end);
    }
  }/*}}}*/

  auto __prepend() noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx;

    if (m_free_idx != idx_not_exist)
    {
      new_idx = m_free_idx;
      m_free_idx = m_data[new_idx].m_next_idx;
    }
    else
    {
      if (m_used >= m_size)
      {
        INDEX new_size = (m_size << 1) + c_array_add;
        debug_assert(new_size > 0);

        copy_resize(new_size);
      }

      new_idx = m_used++;
    }

    element_s &new_element = m_data[new_idx];

    new_element.m_valid = true;

    new_element.m_next_idx = m_first_idx;
    new_element.m_prev_idx = idx_not_exist;

    if (m_first_idx != idx_not_exist)
    {
      m_data[m_first_idx].m_prev_idx = new_idx;
    }
    else
    {
      m_last_idx = new_idx;
    }

    m_first_idx = new_idx;

    m_count++;

    return new_idx;
  }/*}}}*/

  auto __append() noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx;

    if (m_free_idx != idx_not_exist)
    {
      new_idx = m_free_idx;
      m_free_idx = m_data[new_idx].m_next_idx;
    }
    else
    {
      if (m_used >= m_size)
      {
        INDEX new_size = (m_size << 1) + c_array_add;
        debug_assert(new_size > 0);

        copy_resize(new_size);
      }

      new_idx = m_used++;
    }

    element_s &new_element = m_data[new_idx];

    new_element.m_valid = true;

    new_element.m_next_idx = idx_not_exist;
    new_element.m_prev_idx = m_last_idx;

    if (m_last_idx != idx_not_exist)
    {
      m_data[m_last_idx].m_next_idx = new_idx;
    }
    else
    {
      m_first_idx = new_idx;
    }

    m_last_idx = new_idx;

    m_count++;

    return new_idx;
  }/*}}}*/

  auto __insert_before(INDEX a_idx) noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    INDEX new_idx;

    if (m_free_idx != idx_not_exist)
    {
      new_idx = m_free_idx;
      m_free_idx = m_data[new_idx].m_next_idx;
    }
    else
    {
      if (m_used >= m_size)
      {
        INDEX new_size = (m_size << 1) + c_array_add;
        debug_assert(new_size > 0);

        copy_resize(new_size);
      }

      new_idx = m_used++;
    }

    element_s &idx_element = m_data[a_idx];
    element_s &new_element = m_data[new_idx];

    new_element.m_valid = true;

    new_element.m_next_idx = a_idx;
    new_element.m_prev_idx = idx_element.m_prev_idx;

    if (idx_element.m_prev_idx != idx_not_exist)
    {
      m_data[idx_element.m_prev_idx].m_next_idx = new_idx;
    }
    else
    {
      m_first_idx = new_idx;
    }

    idx_element.m_prev_idx = new_idx;

    m_count++;

    return new_idx;
  }/*}}}*/

  auto __insert_after(INDEX a_idx) noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    INDEX new_idx;

    if (m_free_idx != idx_not_exist)
    {
      new_idx = m_free_idx;
      m_free_idx = m_data[new_idx].m_next_idx;
    }
    else
    {
      if (m_used >= m_size)
      {
        INDEX new_size = (m_size << 1) + c_array_add;
        debug_assert(new_size > 0);

        copy_resize(new_size);
      }

      new_idx = m_used++;
    }

    element_s &idx_element = m_data[a_idx];
    element_s &new_element = m_data[new_idx];

    new_element.m_valid = true;

    new_element.m_next_idx = idx_element.m_next_idx;
    new_element.m_prev_idx = a_idx;

    if (idx_element.m_next_idx != idx_not_exist)
    {
      m_data[idx_element.m_next_idx].m_prev_idx = new_idx;
    }
    else
    {
      m_last_idx = new_idx;
    }

    idx_element.m_next_idx = new_idx;

    m_count++;

    return new_idx;
  }/*}}}*/

public:
  [[nodiscard]] constexpr auto size() const noexcept -> const INDEX & { return m_size; }
  [[nodiscard]] constexpr auto used() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto count() const noexcept -> const INDEX & { return m_count; }
  [[nodiscard]] constexpr auto data() const noexcept -> const element_s * { return m_data; }
  [[nodiscard]] constexpr auto first_idx() const noexcept -> const INDEX & { return m_first_idx; }
  [[nodiscard]] constexpr auto last_idx() const noexcept -> const INDEX & { return m_last_idx; }
  [[nodiscard]] constexpr auto is_valid(INDEX a_index) const noexcept -> bool
  {/*{{{*/
    return a_index < m_used && m_data[a_index].m_valid;
  }/*}}}*/

  ~list()
  {/*{{{*/
    __destruct();
  }/*}}}*/

  list() = default;

  list(const list &a_src) noexcept
  {/*{{{*/
    if (a_src.m_first_idx == idx_not_exist)
    {
      return;
    }

    copy_resize(a_src.m_used);
    __copy_from(a_src.m_used,a_src.m_data);

    m_used = a_src.m_used;
    m_count = a_src.m_count;
    m_free_idx = a_src.m_free_idx;
    m_first_idx = a_src.m_first_idx;
    m_last_idx = a_src.m_last_idx;
  }/*}}}*/

  list(list &&a_src) noexcept :
    m_size(a_src.m_size),
    m_used(a_src.m_used),
    m_count(a_src.m_count),
    m_data(a_src.m_data),
    m_free_idx(a_src.m_free_idx),
    m_first_idx(a_src.m_first_idx),
    m_last_idx(a_src.m_last_idx)
  {/*{{{*/
    new (&a_src) list;
  }/*}}}*/

  explicit list(INDEX a_size) noexcept
  {/*{{{*/
    copy_resize(a_size);
  }/*}}}*/

  list(std::initializer_list<VALUE> a_init)
  {/*{{{*/
    if (a_init.size() <= 0)
    {
      return;
    }

    copy_resize(a_init.size());

    const VALUE *i_ptr = a_init.begin();
    const VALUE *i_ptr_end = a_init.end();
    do {
      append(*i_ptr);
    } while(++i_ptr < i_ptr_end);
  }/*}}}*/

  auto operator=(const list &a_src) noexcept -> list &
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
    m_count = a_src.m_count;
    m_free_idx = a_src.m_free_idx;
    m_first_idx = a_src.m_first_idx;
    m_last_idx = a_src.m_last_idx;

    return *this;
  }/*}}}*/

  auto operator=(list &&a_src) noexcept -> list &
  {/*{{{*/
    this->~list();
    new (this) list(std::move(a_src));

    return *this;
  }/*}}}*/

  auto clear() noexcept -> list &
  {/*{{{*/
    this->~list();
    new (this) list;

    return *this;
  }/*}}}*/

  auto reset() noexcept -> list &
  {/*{{{*/
    m_used = 0;
    m_count = 0;
    m_free_idx = idx_not_exist;
    m_first_idx = idx_not_exist;
    m_last_idx = idx_not_exist;

    return *this;
  }/*}}}*/

  auto reserve(INDEX a_cnt) noexcept -> list &
  {/*{{{*/
    INDEX required_cnt = m_count + a_cnt;

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

  auto flush() noexcept -> list &
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<std::is_trivial<TYPE>::value,list &>::type
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<!std::is_trivial<TYPE>::value,list &>::type
  {/*{{{*/
    copy_resize(m_used);

    if (m_used <= 0)
    {
      return *this;
    }

    element_s *ptr = m_data;
    element_s *ptr_end = ptr + m_used;

    do {
      ptr->m_value.flush_all();
    } while(++ptr < ptr_end);

    return *this;
  }/*}}}*/

  auto swap(list &a_second) noexcept -> list &
  {/*{{{*/
    if (this == &a_second)
    {
      return *this;
    }

    list tmp(std::move(*this));
    *this = std::move(a_second);
    a_second = std::move(tmp);

    return *this;
  }/*}}}*/

  auto operator[](INDEX a_idx) noexcept -> VALUE &
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);
    return m_data[a_idx].m_value;
  }/*}}}*/

  auto operator[](INDEX a_idx) const noexcept -> const VALUE &
  {/*{{{*/
    debug_assert(a_idx < m_used);
    return m_data[a_idx].m_value;
  }/*}}}*/

  auto prepend(VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __prepend();

    m_data[new_idx].m_value = std::move(a_value);

    return new_idx;
  }/*}}}*/

  auto prepend(const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __prepend();

    m_data[new_idx].m_value = a_value;

    return new_idx;
  }/*}}}*/

  auto append(VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __append();

    m_data[new_idx].m_value = std::move(a_value);

    return new_idx;
  }/*}}}*/

  auto append(const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __append();

    m_data[new_idx].m_value = a_value;

    return new_idx;
  }/*}}}*/

  auto insert_before(INDEX a_idx,VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __insert_before(a_idx);

    m_data[new_idx].m_value = std::move(a_value);

    return new_idx;
  }/*}}}*/

  auto insert_before(INDEX a_idx,const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __insert_before(a_idx);

    m_data[new_idx].m_value = a_value;

    return new_idx;
  }/*}}}*/

  auto insert_after(INDEX a_idx,VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __insert_after(a_idx);

    m_data[new_idx].m_value = std::move(a_value);

    return new_idx;
  }/*}}}*/

  auto insert_after(INDEX a_idx,const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx = __insert_after(a_idx);

    m_data[new_idx].m_value = a_value;

    return new_idx;
  }/*}}}*/

  auto remove(INDEX a_idx) noexcept -> list &
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    element_s &rm_element = m_data[a_idx];

    if (rm_element.m_next_idx != idx_not_exist)
    {
      m_data[rm_element.m_next_idx].m_prev_idx = rm_element.m_prev_idx;
    }
    else
    {
      m_last_idx = rm_element.m_prev_idx;
    }

    if (rm_element.m_prev_idx != idx_not_exist)
    {
      m_data[rm_element.m_prev_idx].m_next_idx = rm_element.m_next_idx;
    }
    else
    {
      m_first_idx = rm_element.m_next_idx;
    }

    rm_element.m_valid = 0;
    rm_element.m_next_idx = m_free_idx;

    m_count--;

    m_free_idx = a_idx;

    return *this;
  }/*}}}*/

  [[nodiscard]] auto next_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(m_data[a_idx].m_valid);
    return m_data[a_idx].m_next_idx;
  }/*}}}*/

  [[nodiscard]] auto prev_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(m_data[a_idx].m_valid);
    return m_data[a_idx].m_prev_idx;
  }/*}}}*/

  template<class TYPE = element_s>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<std::is_trivially_move_constructible<TYPE>::value,list &>::type
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
      m_data = static_cast<element_s *>(crealloc(m_data,a_size*sizeof(element_s)));
    }

    __copy_resize_init(a_size);

    m_size = a_size;

    return *this;
  }/*}}}*/

  template<class TYPE = element_s>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<!std::is_trivially_move_constructible<TYPE>::value,list &>::type
  {/*{{{*/
    debug_assert(a_size >= m_used);

    static_assert(std::is_nothrow_move_constructible<element_s>::value,"");

    element_s *n_data;

    if (a_size <= 0)
    {
      n_data = nullptr;
    }
    else
    {
      n_data = static_cast<element_s *>(cmalloc(a_size*sizeof(element_s)));
#ifdef __clang_analyzer__
      memset(n_data,0,a_size*sizeof(element_s));
#endif

      if (m_used < a_size)
      {
        element_s *ptr = n_data + m_used;
        element_s *ptr_end = n_data + a_size;

        do {
          new (&ptr->m_value) VALUE;
        } while(++ptr < ptr_end);
      }
    }

    if (m_used > 0)
    {
      element_s *ptr = m_data;
      element_s *ptr_end = ptr + m_used;
      element_s *n_ptr = n_data;

      do {
        new (n_ptr) element_s(std::move(*ptr));
      } while(++n_ptr,++ptr < ptr_end);
    }

    if (m_size > m_used)
    {
      element_s *ptr = m_data + m_used;
      element_s *ptr_end = m_data + m_size;

      do {
        ptr->m_value.~VALUE();
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

  [[nodiscard]] auto get_idx(const VALUE &a_value) const noexcept -> INDEX
  {/*{{{*/
    if (m_first_idx == idx_not_exist)
    {
      return idx_not_exist;
    }

    INDEX idx = m_first_idx;
    do {
      element_s &elem = m_data[idx];

      if (elem.m_value == a_value)
      {
        return idx;
      }

      idx = elem.m_next_idx;
    } while(idx != idx_not_exist);

    return idx_not_exist;
  }/*}}}*/

  auto operator==(const list &a_second) const noexcept -> bool
  {/*{{{*/
    if (m_count != a_second.m_count)
    {
      return false;
    }

    if (m_count > 0)
    {
      INDEX idx = m_first_idx;
      INDEX s_idx = a_second.m_first_idx;

      do {
        element_s &f_element = m_data[idx];
        element_s &s_element = a_second.m_data[s_idx];

        if (!(f_element.m_value == s_element.m_value))
        {
          return false;
        }

        idx = f_element.m_next_idx;
        s_idx = s_element.m_next_idx;

      } while(idx != idx_not_exist);
    }

    return true;
  }/*}}}*/

  [[nodiscard]] auto compare(const list &a_second) const noexcept -> int
  {/*{{{*/
    if (m_count < a_second.m_count) { return -1; }
    if (m_count > a_second.m_count) { return 1; }

    if (m_count > 0)
    {
      INDEX idx = m_first_idx;
      INDEX s_idx = a_second.m_first_idx;

      do {
        element_s &f_element = m_data[idx];
        element_s &s_element = a_second.m_data[s_idx];

        int res = ::compare(f_element.m_value,s_element.m_value);

        if (res != 0)
        {
          return res;
        }

        idx = f_element.m_next_idx;
        s_idx = s_element.m_next_idx;
      } while(idx != idx_not_exist);
    }

    return 0;
  }/*}}}*/

  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    a_trg->push('[');

    if (m_first_idx != idx_not_exist)
    {
      INDEX idx = m_first_idx;

      do {
        element_s &elem = m_data[idx];
        a_trg->string(elem.m_value);

        if ((idx = elem.m_next_idx) == idx_not_exist)
        {
          break;
        }

        a_trg->push(',');
      } while(true);
    }

    return a_trg->push(']');
  }/*}}}*/

  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    a_trg->push('[');

    if (m_first_idx != idx_not_exist)
    {
      INDEX idx = m_first_idx;

      do {
        element_s &elem = m_data[idx];
        a_trg->json(elem.m_value);

        if ((idx = elem.m_next_idx) == idx_not_exist)
        {
          break;
        }

        a_trg->push(',');
      } while(true);
    }

    return a_trg->push(']');
  }/*}}}*/

  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    if (m_first_idx != idx_not_exist)
    {
      (*a_trg)
        .push('[')
        .json(a_json_nice->push());

      if (m_first_idx != idx_not_exist)
      {
        INDEX idx = m_first_idx;

        do {
          element_s &elem = m_data[idx];
          a_trg->json_nice(elem.m_value,a_json_nice);

          if ((idx = elem.m_next_idx) == idx_not_exist)
          {
            break;
          }

          (*a_trg)
            .push(',')
            .json(*a_json_nice);
        } while(true);
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

