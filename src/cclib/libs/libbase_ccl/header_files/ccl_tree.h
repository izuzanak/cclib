
#ifndef LIBS_LIBBASE_CCL_HEADER_FILES_CCL_TREE_H
#define LIBS_LIBBASE_CCL_HEADER_FILES_CCL_TREE_H

namespace ccl {

template <typename VALUE,typename INDEX = uint32_t>
struct tree
{
  static constexpr INDEX idx_not_exist = std::numeric_limits<INDEX>::max();

  struct stack_s
  {
    friend tree;

  private:
    static constexpr size_t max_depth = 32;
    std::array<INDEX,max_depth> m_data;
    INDEX m_top = 0;

    auto push(INDEX a_value) noexcept -> void
    {/*{{{*/
      m_data[m_top++] = a_value;
    }/*}}}*/

    [[nodiscard]] auto pop() noexcept -> INDEX
    {/*{{{*/
      return m_data[--m_top];
    }/*}}}*/

    [[nodiscard]] auto not_empty() const noexcept -> bool
    {/*{{{*/
      return m_top > 0;
    }/*}}}*/
  };

  struct node_s
  {
    friend tree;

  private:
    INDEX m_parent_idx;
    INDEX m_left_idx;
    INDEX m_right_idx;
    char m_color;
    char m_valid;
    VALUE m_value;

  public:
    [[nodiscard]] auto is_valid() const noexcept -> bool { return m_valid != 0; }
    [[nodiscard]] auto object() const noexcept -> const VALUE & { return m_value; }
  };

private:
  INDEX m_size = 0;
  INDEX m_used = 0;
  INDEX m_count = 0;
  node_s *m_data = nullptr;
  INDEX m_free_idx = idx_not_exist;
  INDEX m_root_idx = idx_not_exist;
  INDEX m_leaf_idx = idx_not_exist;

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

      node_s *ptr = m_data;
      node_s *ptr_end = ptr + m_size;

      do {
        ptr->m_value.~VALUE();
      } while(++ptr < ptr_end);

      cfree(m_data);
    }
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const node_s *a_data) noexcept -> typename std::enable_if<std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/
    memcpy(m_data,a_data,a_size*sizeof(node_s));
  }/*}}}*/

  template<class TYPE = VALUE>
  auto __copy_from(INDEX a_size,const node_s *a_data) noexcept -> typename std::enable_if<!std::is_trivially_copy_assignable<TYPE>::value,void>::type
  {/*{{{*/

    // - ignore std::bad_alloc -
    //static_assert(std::is_nothrow_copy_assignable<VALUE>::value,"");

    node_s *ptr = m_data;
    const node_s *s_ptr = a_data;
    const node_s *s_ptr_end = s_ptr + a_size;

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
      node_s *ptr = m_data + a_size;
      node_s *ptr_end = m_data + m_size;

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
      node_s *ptr = m_data + m_size;
      node_s *ptr_end = m_data + a_size;

      do {
        new (&ptr->m_value) VALUE;
      } while(++ptr < ptr_end);
    }
  }/*}}}*/

  [[nodiscard]] auto __get_grandparent_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    node_s &nd = m_data[a_idx];

    if (nd.m_parent_idx != idx_not_exist)
    {
      return m_data[nd.m_parent_idx].m_parent_idx;
    }

    return idx_not_exist;
  }/*}}}*/

  [[nodiscard]] auto __get_uncle_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    INDEX gp_idx = __get_grandparent_idx(a_idx);

    if (gp_idx != idx_not_exist)
    {
      node_s &gp = m_data[gp_idx];
      return gp.m_left_idx == m_data[a_idx].m_parent_idx ? gp.m_right_idx : gp.m_left_idx;
    }

    return idx_not_exist;
  }/*}}}*/

  [[nodiscard]] auto __get_sibling_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    node_s &p = m_data[m_data[a_idx].m_parent_idx];
    return p.m_left_idx == a_idx ? p.m_right_idx : p.m_left_idx;
  }/*}}}*/

  auto __rotate_left(INDEX a_idx) noexcept -> void
  {/*{{{*/
    node_s &root = m_data[a_idx];
    node_s &pivot = m_data[root.m_right_idx];

    if (a_idx == m_root_idx)
    {
      m_root_idx = root.m_right_idx;
      pivot.m_parent_idx = idx_not_exist;
    }
    else
    {
      node_s &rp = m_data[root.m_parent_idx];

      if (rp.m_right_idx == a_idx)
      {
        rp.m_right_idx = root.m_right_idx;
      }
      else
      {
        rp.m_left_idx = root.m_right_idx;
      }

      pivot.m_parent_idx = root.m_parent_idx;
    }

    root.m_parent_idx = root.m_right_idx;

    root.m_right_idx = pivot.m_left_idx;
    m_data[root.m_right_idx].m_parent_idx = a_idx;

    pivot.m_left_idx = a_idx;
  }/*}}}*/

  auto __rotate_right(INDEX a_idx) noexcept -> void
  {/*{{{*/
    node_s &root = m_data[a_idx];
    node_s &pivot = m_data[root.m_left_idx];

    if (a_idx == m_root_idx)
    {
      m_root_idx = root.m_left_idx;
      pivot.m_parent_idx = idx_not_exist;
    }
    else
    {
      node_s &rp = m_data[root.m_parent_idx];

      if (rp.m_right_idx == a_idx)
      {
        rp.m_right_idx = root.m_left_idx;
      }
      else
      {
        rp.m_left_idx = root.m_left_idx;
      }

      pivot.m_parent_idx = root.m_parent_idx;
    }

    root.m_parent_idx = root.m_left_idx;

    root.m_left_idx = pivot.m_right_idx;
    m_data[root.m_left_idx].m_parent_idx = a_idx;

    pivot.m_right_idx = a_idx;
  }/*}}}*/

  auto __get_new_index() noexcept -> INDEX
  {/*{{{*/
    INDEX new_idx;

    if (m_free_idx != idx_not_exist)
    {
      new_idx = m_free_idx;
      m_free_idx = m_data[new_idx].m_parent_idx; // NOLINT
    }
    else
    {
      if (m_used >= m_size)
      {
        INDEX new_size = (m_size << 1) + c_array_add;
        debug_assert(new_size > 0);

        copy_resize(new_size);
      }

      if (m_leaf_idx == idx_not_exist)
      {
        m_leaf_idx = m_used++;
        node_s &leaf = m_data[m_leaf_idx];

        leaf.m_valid = 0; // NOLINT
        leaf.m_color = 1; // NOLINT
      }

      new_idx = m_used++;
    }

    m_data[new_idx].m_valid = 1; // NOLINT
    m_count++;

    return new_idx;
  }/*}}}*/

  auto __binary_tree_insert(INDEX a_new_idx,const VALUE &a_value,bool a_unique) noexcept -> INDEX
  {/*{{{*/
    if (m_root_idx == idx_not_exist)
    {
      m_data[a_new_idx].m_parent_idx = idx_not_exist;
      m_root_idx = a_new_idx;
    }
    else
    {
      INDEX node_idx = m_root_idx;
      do {
        node_s &nd = m_data[node_idx];

        int comp_result = ::compare(a_value,nd.m_value);
        if (comp_result < 0)
        {
          if (nd.m_left_idx == m_leaf_idx)
          {
            nd.m_left_idx = a_new_idx;
            break;
          }
          node_idx = nd.m_left_idx;
        }
        else
        {
          if (a_unique && comp_result == 0)
          {
            return node_idx;
          }

          if (nd.m_right_idx == m_leaf_idx)
          {
            nd.m_right_idx = a_new_idx;
            break;
          }
          node_idx = nd.m_right_idx;
        }
      } while(true);

      m_data[a_new_idx].m_parent_idx = node_idx;
    }

    node_s &new_nd = m_data[a_new_idx];
    new_nd.m_left_idx = m_leaf_idx;
    new_nd.m_right_idx = m_leaf_idx;
    new_nd.m_color = 0;

    return idx_not_exist;
  }/*}}}*/

  auto __replace_delete_node_by_child(INDEX a_idx,INDEX a_ch_idx) noexcept -> void
  {/*{{{*/
    node_s &nd = m_data[a_idx];

    if (nd.m_parent_idx != idx_not_exist)
    {
      node_s &parent = m_data[nd.m_parent_idx];

      if (parent.m_left_idx == a_idx)
      {
        parent.m_left_idx = a_ch_idx;
      }
      else
      {
        parent.m_right_idx = a_ch_idx;
      }

      m_data[a_ch_idx].m_parent_idx = nd.m_parent_idx;
    }
    else
    {
      m_root_idx = a_ch_idx == m_leaf_idx ? idx_not_exist : a_ch_idx;
      m_data[a_ch_idx].m_parent_idx = idx_not_exist;
    }
  }/*}}}*/

  auto __remove_black_black(INDEX a_idx) noexcept -> void
  {/*{{{*/
    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_parent_idx == idx_not_exist)
      {
        return;
      }

      INDEX parent_idx = nd.m_parent_idx;
      node_s &parent = m_data[parent_idx];

      {
        INDEX sibling_idx = parent.m_left_idx == node_idx ? parent.m_right_idx : parent.m_left_idx;
        node_s &sibling = m_data[sibling_idx];

        if (!sibling.m_color)
        {
          parent.m_color = 0;
          sibling.m_color = 1;

          if (node_idx == parent.m_left_idx)
          {
            __rotate_left(parent_idx);
          }
          else
          {
            __rotate_right(parent_idx);
          }
        }
      }

      {
        INDEX sibling_idx = parent.m_left_idx == node_idx?parent.m_right_idx:parent.m_left_idx;
        node_s &sibling = m_data[sibling_idx];

        if (parent.m_color && sibling.m_color && m_data[sibling.m_left_idx].m_color && m_data[sibling.m_right_idx].m_color)
        {
          sibling.m_color = 0;
          node_idx = parent_idx;
          continue;
        }

        if (!parent.m_color && sibling.m_color && m_data[sibling.m_left_idx].m_color && m_data[sibling.m_right_idx].m_color)
        {
          sibling.m_color = 0;
          parent.m_color = 1;
          return;
        }

        if (sibling.m_color)
        {
          if (node_idx == parent.m_left_idx && m_data[sibling.m_right_idx].m_color && !m_data[sibling.m_left_idx].m_color)
          {
            sibling.m_color = 0;
            m_data[sibling.m_left_idx].m_color = 1;
            __rotate_right(sibling_idx);
          }
          else if (node_idx == parent.m_right_idx && m_data[sibling.m_left_idx].m_color && !m_data[sibling.m_right_idx].m_color)
          {
            sibling.m_color = 0;
            m_data[sibling.m_right_idx].m_color = 1;
            __rotate_left(sibling_idx);
          }
        }

        {
          INDEX sibling_idx = parent.m_left_idx == node_idx ? parent.m_right_idx : parent.m_left_idx;
          node_s &sibling = m_data[sibling_idx];

          sibling.m_color = parent.m_color;
          parent.m_color = 1;

          if (node_idx == parent.m_left_idx)
          {
            m_data[sibling.m_right_idx].m_color = 1;
            __rotate_left(parent_idx);
          }
          else
          {
            m_data[sibling.m_left_idx].m_color = 1;
            __rotate_right(parent_idx);
          }
        }

        return;
      }

    } while(true);
  }/*}}}*/

  auto __remove_one_child(INDEX a_idx,INDEX a_ch_idx) noexcept -> void
  {/*{{{*/
    node_s &nd = m_data[a_idx];
    __replace_delete_node_by_child(a_idx,a_ch_idx);

    nd.m_parent_idx = m_free_idx;
    m_free_idx = a_idx;

    nd.m_valid = 0;
    m_count--;

    if (nd.m_color)
    {
      node_s &child_nd = m_data[a_ch_idx];

      if (!child_nd.m_color)
      {
        child_nd.m_color = 1;
      }
      else
      {
        __remove_black_black(a_ch_idx);
      }
    }
  }/*}}}*/

  auto __insert_operation(INDEX a_idx) noexcept -> void
  {/*{{{*/
    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_parent_idx == idx_not_exist)
      {
        nd.m_color = 1;
        return;
      }

      if (m_data[nd.m_parent_idx].m_color)
      {
        return;
      }

      INDEX uncle_idx = __get_uncle_idx(node_idx);
      if (uncle_idx != idx_not_exist && !m_data[uncle_idx].m_color)
      {
        m_data[nd.m_parent_idx].m_color = 1;
        m_data[uncle_idx].m_color = 1;

        node_idx = __get_grandparent_idx(node_idx);
        m_data[node_idx].m_color = 0;

        continue;
      }

      INDEX grandparent_idx = __get_grandparent_idx(node_idx);

      if (node_idx == m_data[nd.m_parent_idx].m_right_idx && nd.m_parent_idx == m_data[grandparent_idx].m_left_idx)
      {
        __rotate_left(nd.m_parent_idx);
        node_idx = nd.m_left_idx;
      }
      else if (node_idx == m_data[nd.m_parent_idx].m_left_idx && nd.m_parent_idx == m_data[grandparent_idx].m_right_idx)
      {
        __rotate_right(nd.m_parent_idx);
        node_idx = nd.m_right_idx;
      }

      {
        INDEX grandparent_idx = __get_grandparent_idx(node_idx);
        node_s &nd = m_data[node_idx];

        m_data[nd.m_parent_idx].m_color = 1;
        m_data[grandparent_idx].m_color = 0;

        if (node_idx == m_data[nd.m_parent_idx].m_left_idx && nd.m_parent_idx == m_data[grandparent_idx].m_left_idx)
        {
          __rotate_right(grandparent_idx);
        }
        else
        {
          __rotate_left(grandparent_idx);
        }

        return;
      }
    } while(true);
  }/*}}}*/

public:
  [[nodiscard]] constexpr auto size() const noexcept -> const INDEX & { return m_size; }
  [[nodiscard]] constexpr auto used() const noexcept -> const INDEX & { return m_used; }
  [[nodiscard]] constexpr auto count() const noexcept -> const INDEX & { return m_count; }
  [[nodiscard]] constexpr auto data() const noexcept -> const node_s * { return m_data; }
  [[nodiscard]] constexpr auto root_idx() const noexcept -> const INDEX & { return m_root_idx; }
  [[nodiscard]] constexpr auto is_valid(INDEX a_index) const noexcept -> bool
  {/*{{{*/
    return a_index < m_used && m_data[a_index].m_valid != 0;
  }/*}}}*/

  ~tree()
  {/*{{{*/
    __destruct();
  }/*}}}*/

  tree() = default;

  tree(const tree &a_src) noexcept
  {/*{{{*/
    if (a_src.m_root_idx == idx_not_exist)
    {
      return;
    }

    copy_resize(a_src.m_used);
    __copy_from(a_src.m_used,a_src.m_data);

    m_used = a_src.m_used;
    m_count = a_src.m_count;
    m_free_idx = a_src.m_free_idx;
    m_root_idx = a_src.m_root_idx;
    m_leaf_idx = a_src.m_leaf_idx;
  }/*}}}*/

  tree(tree &&a_src) noexcept :
    m_size(a_src.m_size),
    m_used(a_src.m_used),
    m_count(a_src.m_count),
    m_data(a_src.m_data),
    m_free_idx(a_src.m_free_idx),
    m_root_idx(a_src.m_root_idx),
    m_leaf_idx(a_src.m_leaf_idx)
  {/*{{{*/
    new (&a_src) tree;
  }/*}}}*/

  explicit tree(INDEX a_size) noexcept
  {/*{{{*/
    copy_resize(a_size + 1);
  }/*}}}*/

  tree(std::initializer_list<VALUE> a_init)
  {/*{{{*/
    if (a_init.size() <= 0)
    {
      return;
    }

    copy_resize(a_init.size() + 1);

    const VALUE *i_ptr = a_init.begin();
    const VALUE *i_ptr_end = a_init.end();
    do {
      insert(*i_ptr);
    } while(++i_ptr < i_ptr_end);
  }/*}}}*/

  auto operator=(const tree &a_src) noexcept -> tree &
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
    m_root_idx = a_src.m_root_idx;
    m_leaf_idx = a_src.m_leaf_idx;

    return *this;
  }/*}}}*/

  auto operator=(tree &&a_src) noexcept -> tree &
  {/*{{{*/
    this->~tree();
    new (this) tree(std::move(a_src));

    return *this;
  }/*}}}*/

  auto clear() noexcept -> tree &
  {/*{{{*/
    this->~tree();
    new (this) tree;

    return *this;
  }/*}}}*/

  auto reset() noexcept -> tree &
  {/*{{{*/
    m_used = 0;
    m_count = 0;
    m_free_idx = idx_not_exist;
    m_root_idx = idx_not_exist;
    m_leaf_idx = idx_not_exist;

    return *this;
  }/*}}}*/

  auto reserve(INDEX a_cnt) noexcept -> tree &
  {/*{{{*/
    INDEX required_cnt = m_count + a_cnt + 1;

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

  auto flush() noexcept -> tree &
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<std::is_trivial<TYPE>::value,tree &>::type
  {/*{{{*/
    copy_resize(m_used);

    return *this;
  }/*}}}*/

  template<class TYPE = VALUE>
  auto flush_all() noexcept -> typename std::enable_if<!std::is_trivial<TYPE>::value,tree &>::type
  {/*{{{*/
    copy_resize(m_used);

    if (m_used <= 0)
    {
      return *this;
    }

    node_s *ptr = m_data;
    node_s *ptr_end = ptr + m_used;

    do {
      ptr->m_value.flush_all();
    } while(++ptr < ptr_end);

    return *this;
  }/*}}}*/

  auto swap(tree &a_second) noexcept -> tree &
  {/*{{{*/
    if (this == &a_second)
    {
      return *this;
    }

    tree tmp(std::move(*this));
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
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);
    return m_data[a_idx].m_value;
  }/*}}}*/

  [[nodiscard]] constexpr auto descent_stack() const noexcept -> stack_s
  {/*{{{*/
    return stack_s();
  }/*}}}*/

  [[nodiscard]] auto stack_min_idx(stack_s *a_stack,INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_left_idx == m_leaf_idx)
      {
        return node_idx;
      }

      a_stack->push(node_idx);
      node_idx = nd.m_left_idx;
    } while(true);
  }/*}}}*/

  [[nodiscard]] auto stack_min_idx(stack_s *a_stack) const noexcept -> INDEX
  {/*{{{*/
    return stack_min_idx(a_stack,m_root_idx);
  }/*}}}*/

  [[nodiscard]] auto stack_next_idx(stack_s *a_stack,INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    node_s &nd = m_data[a_idx];

    if (nd.m_right_idx != m_leaf_idx)
    {
      return stack_min_idx(a_stack,nd.m_right_idx);
    }

    if (a_stack->not_empty())
    {
      return a_stack->pop();
    }

    return idx_not_exist;
  }/*}}}*/

  [[nodiscard]] auto min_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_left_idx == m_leaf_idx)
      {
        return node_idx;
      }

      node_idx = nd.m_left_idx;
    } while(true);
  }/*}}}*/

  [[nodiscard]] auto min_idx() const noexcept -> INDEX
  {/*{{{*/
    return min_idx(m_root_idx);
  }/*}}}*/

  [[nodiscard]] auto max_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_right_idx == m_leaf_idx)
      {
        return node_idx;
      }

      node_idx = nd.m_right_idx;
    } while(true);
  }/*}}}*/

  [[nodiscard]] auto next_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    node_s &nd = m_data[a_idx];

    if (nd.m_right_idx != m_leaf_idx)
    {
      return min_idx(nd.m_right_idx);
    }

    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_parent_idx == idx_not_exist)
      {
        return idx_not_exist;
      }

      if (m_data[nd.m_parent_idx].m_right_idx != node_idx)
      {
        return nd.m_parent_idx;
      }

      node_idx = nd.m_parent_idx;
    } while(true);
  }/*}}}*/

  [[nodiscard]] auto prev_idx(INDEX a_idx) const noexcept -> INDEX
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    node_s &nd = m_data[a_idx];

    if (nd.m_left_idx != m_leaf_idx)
    {
      return max_idx(nd.m_left_idx);
    }

    INDEX node_idx = a_idx;
    do {
      node_s &nd = m_data[node_idx];

      if (nd.m_parent_idx == idx_not_exist)
      {
        return idx_not_exist;
      }

      if (m_data[nd.m_parent_idx].m_left_idx != node_idx)
      {
        return nd.m_parent_idx;
      }

      node_idx = nd.m_parent_idx;
    } while(true);
  }/*}}}*/

  auto insert(VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_node_idx = __get_new_index();
    __binary_tree_insert(new_node_idx,a_value,false);
    __insert_operation(new_node_idx);

    m_data[new_node_idx].m_value = std::move(a_value);

    return new_node_idx;
  }/*}}}*/

  auto insert(const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_node_idx = __get_new_index();
    __binary_tree_insert(new_node_idx,a_value,false);
    __insert_operation(new_node_idx);

    m_data[new_node_idx].m_value = a_value;

    return new_node_idx;
  }/*}}}*/

  auto unique_insert(VALUE &&a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_node_idx = __get_new_index();
    INDEX old_node_idx = __binary_tree_insert(new_node_idx,a_value,true);

    if (old_node_idx != idx_not_exist)
    {
      node_s &new_nd = m_data[new_node_idx];

      new_nd.m_parent_idx = m_free_idx;
      m_free_idx = new_node_idx;

      new_nd.m_valid = 0;
      m_count--;

      return old_node_idx;
    }

    __insert_operation(new_node_idx);

    m_data[new_node_idx].m_value = std::move(a_value);

    return new_node_idx;
  }/*}}}*/

  auto unique_insert(const VALUE &a_value) noexcept -> INDEX
  {/*{{{*/
    INDEX new_node_idx = __get_new_index();
    INDEX old_node_idx = __binary_tree_insert(new_node_idx,a_value,true);

    if (old_node_idx != idx_not_exist)
    {
      node_s &new_nd = m_data[new_node_idx];

      new_nd.m_parent_idx = m_free_idx;
      m_free_idx = new_node_idx;

      new_nd.m_valid = 0;
      m_count--;

      return old_node_idx;
    }

    __insert_operation(new_node_idx);

    m_data[new_node_idx].m_value = a_value;

    return new_node_idx;
  }/*}}}*/

  auto remove(INDEX a_idx) noexcept -> tree &
  {/*{{{*/
    debug_assert(a_idx < m_used && m_data[a_idx].m_valid);

    node_s &del_nd = m_data[a_idx];

    if (del_nd.m_left_idx != m_leaf_idx)
    {
      if (del_nd.m_right_idx != m_leaf_idx)
      {
        INDEX found_idx = del_nd.m_right_idx;
        do {
          node_s &nd = m_data[found_idx];

          if (nd.m_left_idx == m_leaf_idx)
          {
            break;
          }

          found_idx = nd.m_left_idx;
        } while(true);

        node_s &found_nd = m_data[found_idx];

        // - process del_node parent_idx -
        if (del_nd.m_parent_idx != idx_not_exist)
        {
          node_s &del_nd_parent = m_data[del_nd.m_parent_idx];

          if (del_nd_parent.m_left_idx == a_idx)
          {
            del_nd_parent.m_left_idx = found_idx;
          }
          else
          {
            del_nd_parent.m_right_idx = found_idx;
          }
        }
        else
        {
          m_root_idx = found_idx;
        }

        // - process del_node left_idx -
        m_data[del_nd.m_left_idx].m_parent_idx = found_idx;

        // - process found_node right_idx -
        if (found_nd.m_right_idx != m_leaf_idx)
        {
          m_data[found_nd.m_right_idx].m_parent_idx = a_idx;
        }

        if (del_nd.m_right_idx == found_idx)
        {
          // - found node is right child of deleted node -
          del_nd.m_right_idx = found_nd.m_right_idx;
          found_nd.m_right_idx = a_idx;

          found_nd.m_parent_idx = del_nd.m_parent_idx;
          del_nd.m_parent_idx = found_idx;

          found_nd.m_left_idx = del_nd.m_left_idx;
          del_nd.m_left_idx = m_leaf_idx;

          char tmp_char = found_nd.m_color;
          found_nd.m_color = del_nd.m_color;
          del_nd.m_color = tmp_char;
        }
        else
        {
          // - process found_node parent -
          node_s &found_nd_parent = m_data[found_nd.m_parent_idx];

          if (found_nd_parent.m_left_idx == found_idx)
          {
            found_nd_parent.m_left_idx = a_idx;
          }
          else
          {
            found_nd_parent.m_right_idx = a_idx;
          }

          // - process del_node right_idx -
          m_data[del_nd.m_right_idx].m_parent_idx = found_idx;

          // - swap index pointers between nodes -
          INDEX tmp_index = found_nd.m_parent_idx;
          found_nd.m_parent_idx = del_nd.m_parent_idx;
          del_nd.m_parent_idx = tmp_index;

          found_nd.m_left_idx = del_nd.m_left_idx;
          del_nd.m_left_idx = m_leaf_idx;

          tmp_index = found_nd.m_right_idx;
          found_nd.m_right_idx = del_nd.m_right_idx;
          del_nd.m_right_idx = tmp_index;

          char tmp_char = found_nd.m_color;
          found_nd.m_color = del_nd.m_color;
          del_nd.m_color = tmp_char;
        }

        __remove_one_child(a_idx,del_nd.m_right_idx);
      }
      else
      {
        __remove_one_child(a_idx,del_nd.m_left_idx);
      }
    }
    else
    {
      __remove_one_child(a_idx,del_nd.m_right_idx);
    }

    return *this;
  }/*}}}*/

  template<class TYPE = node_s>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<std::is_trivially_move_constructible<TYPE>::value,tree &>::type
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
      m_data = static_cast<node_s *>(crealloc(m_data,a_size*sizeof(node_s)));
    }

    __copy_resize_init(a_size);

    m_size = a_size;

    return *this;
  }/*}}}*/

  template<class TYPE = node_s>
  auto copy_resize(INDEX a_size) noexcept -> typename std::enable_if<!std::is_trivially_move_constructible<TYPE>::value,tree &>::type
  {/*{{{*/
    debug_assert(a_size >= m_used);

    static_assert(std::is_nothrow_move_constructible<node_s>::value,"");

    node_s *n_data;

    if (a_size <= 0)
    {
      n_data = nullptr;
    }
    else
    {
      n_data = static_cast<node_s *>(cmalloc(a_size*sizeof(node_s)));
#ifdef __clang_analyzer__
      memset(n_data,0,a_size*sizeof(node_s));
#endif

      if (m_used < a_size)
      {
        node_s *ptr = n_data + m_used;
        node_s *ptr_end = n_data + a_size;

        do {
          new (&ptr->m_value) VALUE;
        } while(++ptr < ptr_end);
      }
    }

    if (m_used > 0)
    {
      node_s *ptr = m_data;
      node_s *ptr_end = ptr + m_used;
      node_s *n_ptr = n_data;

      do {
        new (n_ptr) node_s(std::move(*ptr));
      } while(++n_ptr,++ptr < ptr_end);
    }

    if (m_size > m_used)
    {
      node_s *ptr = m_data + m_used;
      node_s *ptr_end = m_data + m_size;

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
    if (m_root_idx == idx_not_exist)
    {
      return idx_not_exist;
    }

    INDEX node_idx = m_root_idx;
    do {
      node_s &nd = m_data[node_idx];

      int comp_result = ::compare(a_value,nd.m_value);

      if (comp_result < 0)
      {
        node_idx = nd.m_left_idx;
      }
      else
      {
        if (comp_result == 0)
        {
          return node_idx;
        }

        node_idx = nd.m_right_idx;
      }
    } while(node_idx != m_leaf_idx);

    return idx_not_exist;
  }/*}}}*/

  [[nodiscard]] auto get_idx_left(const VALUE &a_value) const noexcept -> INDEX
  {/*{{{*/
    if (m_root_idx == idx_not_exist)
    {
      return idx_not_exist;
    }

    INDEX good_idx = idx_not_exist;
    INDEX node_idx = m_root_idx;
    do {
      node_s &nd = m_data[node_idx];

      int comp_result = ::compare(a_value,nd.m_value);

      if (comp_result < 0)
      {
        node_idx = nd.m_left_idx;
      }
      else
      {
        if (comp_result == 0)
        {
          good_idx = node_idx;
          node_idx = nd.m_left_idx;
        }
        else
        {
          node_idx = nd.m_right_idx;
        }
      }
    } while(node_idx != m_leaf_idx);

    return good_idx;
  }/*}}}*/

  [[nodiscard]] auto get_gre_idx(const VALUE &a_value) const noexcept -> INDEX
  {/*{{{*/
    if (m_root_idx == idx_not_exist)
    {
      return idx_not_exist;
    }

    INDEX good_idx = idx_not_exist;
    INDEX node_idx = m_root_idx;
    do {
      node_s &nd = m_data[node_idx];

      int comp_result = ::compare(a_value,nd.m_value);

      if (comp_result < 0)
      {
        good_idx = node_idx;
        node_idx = nd.m_left_idx;
      }
      else
      {
        if (comp_result == 0)
        {
          return node_idx;
        }

        node_idx = nd.m_right_idx;
      }
    } while(node_idx != m_leaf_idx);

    return good_idx;
  }/*}}}*/

  [[nodiscard]] auto get_lee_idx(const VALUE &a_value) const noexcept -> INDEX
  {/*{{{*/
    if (m_root_idx == idx_not_exist)
    {
      return idx_not_exist;
    }

    INDEX good_idx = idx_not_exist;
    INDEX node_idx = m_root_idx;
    do {
      node_s &nd = m_data[node_idx];

      int comp_result = ::compare(a_value,nd.m_value);

      if (comp_result < 0)
      {
        node_idx = nd.m_left_idx;
      }
      else
      {
        if (comp_result == 0)
        {
          return node_idx;
        }

        good_idx = node_idx;
        node_idx = nd.m_right_idx;
      }
    } while(node_idx != m_leaf_idx);

    return good_idx;
  }/*}}}*/

  auto get_idxs(const VALUE &a_value,array<INDEX> *a_idxs) const noexcept -> const tree &
  {/*{{{*/
    a_idxs->reset();

    if (m_root_idx == idx_not_exist)
    {
      return *this;
    }

    auto stack = descent_stack();
    stack.push(m_root_idx);

    do {
      INDEX node_idx = stack.pop();
      node_s &nd = m_data[node_idx];

      int comp_result = ::compare(a_value,nd.m_value);
      if (comp_result < 0)
      {
        if (nd.m_left_idx != m_leaf_idx)
        {
          stack.push(nd.m_left_idx);
        }
      }
      else
      {
        if (comp_result == 0)
        {
          a_idxs->push(node_idx);

          if (nd.m_left_idx != m_leaf_idx)
          {
            stack.push(nd.m_left_idx);
          }
        }

        if (nd.m_right_idx != m_leaf_idx)
        {
          stack.push(nd.m_right_idx);
        }
      }
    } while(stack.not_empty() > 0);

    return *this;
  }/*}}}*/

  auto operator==(const tree &a_second) const noexcept -> bool
  {/*{{{*/
    if (m_count != a_second.m_count)
    {
      return false;
    }

    if (m_count > 0)
    {
      auto stack = descent_stack();
      auto s_stack = a_second.descent_stack();

      INDEX node_idx = stack_min_idx(&stack);
      INDEX s_node_idx = a_second.stack_min_idx(&s_stack);
      do {
        if (!(m_data[node_idx].m_value == a_second.m_data[s_node_idx].m_value))
        {
          return false;
        }

        node_idx = stack_next_idx(&stack,node_idx);
        s_node_idx = a_second.stack_next_idx(&s_stack,s_node_idx);
      } while(node_idx != idx_not_exist);
    }

    return true;
  }/*}}}*/

  [[nodiscard]] auto compare(const tree &a_second) const noexcept -> int
  {/*{{{*/
    if (m_count < a_second.m_count) { return -1; }
    if (m_count > a_second.m_count) { return 1; }

    if (m_count > 0)
    {
      auto stack = descent_stack();
      auto s_stack = a_second.descent_stack();

      INDEX node_idx = stack_min_idx(&stack);
      INDEX s_node_idx = a_second.stack_min_idx(&s_stack);
      do {
        int res = ::compare(m_data[node_idx].m_value,a_second.m_data[s_node_idx].m_value);

        if (res != 0)
        {
          return res;
        }

        node_idx = stack_next_idx(&stack,node_idx);
        s_node_idx = a_second.stack_next_idx(&s_stack,s_node_idx);
      } while(node_idx != idx_not_exist);
    }

    return 0;
  }/*}}}*/

  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    a_trg->push('[');

    if (m_root_idx != idx_not_exist)
    {
      auto stack = descent_stack();
      INDEX idx = stack_min_idx(&stack);

      do {
        a_trg->string(m_data[idx].m_value);

        idx = stack_next_idx(&stack,idx);
        if (idx == idx_not_exist)
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

    if (m_root_idx != idx_not_exist)
    {
      auto stack = descent_stack();
      INDEX idx = stack_min_idx(&stack);

      do {
        a_trg->json(m_data[idx].m_value);

        idx = stack_next_idx(&stack,idx);
        if (idx == idx_not_exist)
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
    if (m_root_idx != idx_not_exist)
    {
      (*a_trg)
        .push('[')
        .json(a_json_nice->push());

      auto stack = descent_stack();
      INDEX idx = stack_min_idx(&stack);

      do {
        a_trg->json_nice(m_data[idx].m_value,a_json_nice);

        idx = stack_next_idx(&stack,idx);
        if (idx == idx_not_exist)
        {
          break;
        }

        (*a_trg)
          .push(',')
          .json(*a_json_nice);
      } while(true);

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

