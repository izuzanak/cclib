
#ifndef LIBS_LIBVAR_CCL_HEADER_FILES_CCL_VAR_H
#define LIBS_LIBVAR_CCL_HEADER_FILES_CCL_VAR_H

#include "ccl_base.h"

namespace ccl {

constexpr int VAR_INVALID_TYPE = 1;
constexpr int VAR_INVALID_INDEX = 2;
constexpr int VAR_DICT_INVALID_KEY = 3;

enum
{
  bi_type_blank = 0,
  bi_type_bool,
  bi_type_int,
  bi_type_float,
  bi_type_string,
  bi_type_array,
  bi_type_dict,
  bi_type_count,
};

// === definition of structure loc_s ===========================================

struct var_s;
struct map_s;
struct iter_s;

struct loc_s
{/*{{{*/
public:
  virtual ~loc_s() = default;

  [[nodiscard]] virtual auto to_bool() const -> bool { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto to_int() const -> int64_t { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto to_float() const -> double { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto to_str() const -> const std::string & { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto to_array() -> array<var_s> & { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto to_dict() -> tree<map_s> & { throw_error(VAR_INVALID_TYPE); }

  [[nodiscard]] virtual auto length() const -> int64_t { throw_error(VAR_INVALID_TYPE); }

  virtual auto push(const var_s &a_value) -> void { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto push(var_s &&a_value) -> void { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto pop() -> var_s & { throw_error(VAR_INVALID_TYPE); }

  virtual auto has_key(const var_s &a_value) -> bool { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto remove_key(const var_s &a_value) -> void { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto remove_if_key(const var_s &a_value) -> void { (void)a_value; throw_error(VAR_INVALID_TYPE); }

  virtual auto operator[](const var_s &a_value) -> var_s & { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto operator[](var_s &&a_value) -> var_s & { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto operator[](int64_t a_idx) -> var_s & { (void)a_idx; throw_error(VAR_INVALID_TYPE); }

  virtual auto get(const var_s &a_value) -> var_s * { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto get(var_s &&a_value) -> var_s * { (void)a_value; throw_error(VAR_INVALID_TYPE); }
  virtual auto get(int64_t a_idx) -> var_s * { (void)a_idx; throw_error(VAR_INVALID_TYPE); }

  [[nodiscard]] virtual auto first_idx() const -> uint32_t { throw_error(VAR_INVALID_TYPE); }
  [[nodiscard]] virtual auto next_idx(uint32_t a_idx) const -> uint32_t { (void)a_idx; throw_error(VAR_INVALID_TYPE); }
  virtual auto at_idx(uint32_t a_idx) -> var_s & { (void)a_idx; throw_error(VAR_INVALID_TYPE); }

  [[nodiscard]] virtual auto type() const noexcept -> int = 0;
  virtual auto operator==(const loc_s &a_second) const noexcept -> bool = 0;
  [[nodiscard]] virtual auto compare(const loc_s &a_second) const noexcept -> int = 0;
  virtual auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & = 0;
  virtual auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & = 0;
  virtual auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & = 0;
};/*}}}*/

// === definition of structure loc_blank_s =====================================

struct loc_blank_s : public loc_s
{/*{{{*/
public:
  ~loc_blank_s() override = default;

  loc_blank_s() = default;

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_blank; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override { (void)a_second; return true; }
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override { (void)a_second; return 0; }
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->append(7,"<blank>");
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->append(4,"null");
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    (void)a_json_nice;

    return a_trg->append(4,"null");
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_bool_s ======================================

struct loc_bool_s : public loc_s
{/*{{{*/
private:
  bool m_value;

public:
  ~loc_bool_s() override = default;

  explicit loc_bool_s(bool a_value) : m_value(a_value) {}

  [[nodiscard]] auto to_bool() const -> bool override { return m_value; }

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_bool; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_bool_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_bool_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json(m_value);
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json_nice(m_value,a_json_nice);
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_int_s =======================================

struct loc_int_s : public loc_s
{/*{{{*/
private:
  int64_t m_value;

public:
  ~loc_int_s() override = default;

  explicit loc_int_s(int64_t a_value) : m_value(a_value) {}

  [[nodiscard]] auto to_int() const -> int64_t override { return m_value; }

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_int; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_int_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_int_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json(m_value);
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json_nice(m_value,a_json_nice);
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_float_s =====================================

struct loc_float_s : public loc_s
{/*{{{*/
private:
  double m_value;

public:
  ~loc_float_s() override = default;

  explicit loc_float_s(double a_value) : m_value(a_value) {}

  [[nodiscard]] auto to_float() const -> double override { return m_value; }

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_float; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_float_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_float_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json(m_value);
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json_nice(m_value,a_json_nice);
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_string_s ====================================

struct loc_string_s : public loc_s
{/*{{{*/
private:
  std::string m_value;

public:
  ~loc_string_s() override = default;

  explicit loc_string_s(std::string &&a_value) : m_value(std::move(a_value)) {}

  [[nodiscard]] auto to_str() const -> const std::string & override { return m_value; }

  [[nodiscard]] auto length() const -> int64_t override { return m_value.length(); }

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_string; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_string_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_string_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json(m_value);
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json_nice(m_value,a_json_nice);
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_array_s =====================================

struct loc_array_s : public loc_s
{/*{{{*/
private:
  array<var_s> m_value;

public:
  ~loc_array_s() override = default;

  explicit loc_array_s(array<var_s> &&a_value) : m_value(std::move(a_value)) {}

  [[nodiscard]] auto to_array() -> array<var_s> & override { return m_value; }

  [[nodiscard]] auto length() const -> int64_t override { return m_value.used(); }

  auto push(const var_s &a_value) -> void override { m_value.push(a_value); }
  auto push(var_s &&a_value) -> void override { m_value.push(std::move(a_value)); }
  auto pop() -> var_s & override { return m_value.pop(); }

  inline auto operator[](const var_s &a_value) -> var_s & override;
  inline auto operator[](var_s &&a_value) -> var_s & override;
  auto operator[](int64_t a_idx) -> var_s & override { return m_value[a_idx]; }

  inline auto get(const var_s &a_value) -> var_s * override;
  inline auto get(var_s &&a_value) -> var_s * override;
  auto get(int64_t a_idx) -> var_s * override
  {/*{{{*/
    return (a_idx >= 0 && a_idx < m_value.used()) ? &m_value[a_idx] : nullptr;
  }/*}}}*/

  [[nodiscard]] auto first_idx() const -> uint32_t override
  {/*{{{*/
    return m_value.used() > 0 ? 0 : c_idx_not_exist;
  }/*}}}*/
  [[nodiscard]] auto next_idx(uint32_t a_idx) const -> uint32_t override
  {/*{{{*/
    if (a_idx < 0 || a_idx >= m_value.used())
    {
      throw_error(VAR_INVALID_INDEX);
    }

    return ++a_idx < m_value.used() ? a_idx : c_idx_not_exist;
  }/*}}}*/
  auto at_idx(uint32_t a_idx) -> var_s & override
  {/*{{{*/
    if (a_idx < 0 || a_idx >= m_value.used())
    {
      throw_error(VAR_INVALID_INDEX);
    }

    return m_value[a_idx];
  }/*}}}*/

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_array; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_array_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_array_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json(m_value);
  }/*}}}*/
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->json_nice(m_value,a_json_nice);
  }/*}}}*/
};/*}}}*/

// === definition of structure loc_dict_s ======================================

struct loc_dict_s : public loc_s
{/*{{{*/
private:
  tree<map_s> m_value;

public:
  ~loc_dict_s() override = default;

  explicit loc_dict_s(tree<map_s> &&a_value) : m_value(std::move(a_value)) {}

  [[nodiscard]] auto to_dict() -> tree<map_s> & override { return m_value; }

  [[nodiscard]] auto length() const -> int64_t override { return m_value.count(); }

  inline auto has_key(const var_s &a_value) -> bool override;
  inline auto remove_key(const var_s &a_value) -> void override;
  inline auto remove_if_key(const var_s &a_value) -> void override;

  inline auto operator[](const var_s &a_value) -> var_s & override;
  inline auto operator[](var_s &&a_value) -> var_s & override;
  inline auto operator[](int64_t a_idx) -> var_s & override;

  inline auto get(const var_s &a_value) -> var_s * override;
  inline auto get(var_s &&a_value) -> var_s * override;
  inline auto get(int64_t a_idx) -> var_s * override;

  [[nodiscard]] auto first_idx() const -> uint32_t override
  {/*{{{*/
    return m_value.count() > 0 ? m_value.min_idx() : c_idx_not_exist;
  }/*}}}*/
  [[nodiscard]] inline auto next_idx(uint32_t a_idx) const -> uint32_t override;
  inline auto at_idx(uint32_t a_idx) -> var_s & override;

  [[nodiscard]] auto type() const noexcept -> int override { return bi_type_dict; }
  auto operator==(const loc_s &a_second) const noexcept -> bool override
  {/*{{{*/
    return m_value == static_cast<const loc_dict_s &>(a_second).m_value;
  }/*}}}*/
  [[nodiscard]] auto compare(const loc_s &a_second) const noexcept -> int override
  {/*{{{*/
    return ::compare(m_value,static_cast<const loc_dict_s &>(a_second).m_value);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & override
  {/*{{{*/
    return a_trg->string(m_value);
  }/*}}}*/
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & override;
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> & override;
};/*}}}*/

// === definition of structure var_s ===========================================

struct var_s
{/*{{{*/
private:
  static std::shared_ptr<loc_s> m_loc_blank;

  std::shared_ptr<loc_s> m_loc = nullptr;

public:
  ~var_s() = default;

  var_s() : m_loc(m_loc_blank) {}

  var_s(bool a_value) : m_loc(std::make_shared<loc_bool_s>(a_value)) {} // NOLINT(google-explicit-constructor)

  var_s(int8_t  a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(int16_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(int32_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(int64_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)

  var_s(uint8_t  a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(uint16_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(uint32_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(uint64_t a_value) : m_loc(std::make_shared<loc_int_s>(a_value)) {} // NOLINT(google-explicit-constructor)

  var_s(float  a_value) : m_loc(std::make_shared<loc_float_s>(a_value)) {} // NOLINT(google-explicit-constructor)
  var_s(double a_value) : m_loc(std::make_shared<loc_float_s>(a_value)) {} // NOLINT(google-explicit-constructor)

  var_s(std::string &&a_value) : m_loc(std::make_shared<loc_string_s>(std::move(a_value))) {} // NOLINT(google-explicit-constructor)
  var_s(const char *a_value) : m_loc(std::make_shared<loc_string_s>(std::string(a_value))) {} // NOLINT(google-explicit-constructor)

  var_s(array<var_s> &&a_value) : m_loc(std::make_shared<loc_array_s>(std::move(a_value))) {}  // NOLINT(google-explicit-constructor)
  var_s(std::initializer_list<var_s> a_init) : m_loc(std::make_shared<loc_array_s>(a_init)) {} // NOLINT(google-explicit-constructor)

  var_s(tree<map_s> &&a_value) : m_loc(std::make_shared<loc_dict_s>(std::move(a_value))) {}   // NOLINT(google-explicit-constructor)
  var_s(std::initializer_list<map_s> a_init) : m_loc(std::make_shared<loc_dict_s>(a_init)) {} // NOLINT(google-explicit-constructor)

  var_s(const var_s &a_src) = default;
  var_s(var_s &&a_src) = default;

  auto operator=(const var_s &a_src) -> var_s & = default;
  auto operator=(var_s &&a_src) -> var_s & = default;

  auto static new_array() -> var_s { return var_s(array<var_s>()); }
  auto static new_dict() -> var_s { return var_s(tree<map_s>()); }

  [[nodiscard]] auto to_bool() const -> bool { return m_loc->to_bool(); }
  [[nodiscard]] auto to_int() const -> int64_t { return m_loc->to_int(); }
  [[nodiscard]] auto to_float() const -> double { return m_loc->to_float(); }
  [[nodiscard]] auto to_str() const -> const std::string & { return m_loc->to_str(); }
  [[nodiscard]] auto to_array() const -> const array<var_s> & { return m_loc->to_array(); }
  [[nodiscard]] auto to_array() -> array<var_s> & { return m_loc->to_array(); }
  [[nodiscard]] auto to_dict() const -> const tree<map_s> & { return m_loc->to_dict(); }
  [[nodiscard]] auto to_dict() -> tree<map_s> & { return m_loc->to_dict(); }

  [[nodiscard]] auto length() const -> int64_t { return m_loc->length(); }

  auto push(const var_s &a_value) -> void { m_loc->push(a_value); }
  auto push(var_s &&a_value) -> void { m_loc->push(std::move(a_value)); }
  auto pop() -> var_s & { return m_loc->pop(); }

  auto has_key(const var_s &a_value) -> bool { return m_loc->has_key(a_value); }
  auto remove_key(const var_s &a_value) -> void { m_loc->remove_key(a_value); }
  auto remove_if_key(const var_s &a_value) -> void { m_loc->remove_if_key(a_value); }

  auto operator[](const var_s &a_value) -> var_s & { return (*m_loc)[a_value]; }
  auto operator[](var_s &&a_value) -> var_s & { return (*m_loc)[std::move(a_value)]; }
  auto operator[](int64_t a_idx) -> var_s & { return (*m_loc)[a_idx]; }

  auto get(const var_s &a_value) -> var_s * { return m_loc->get(a_value); }
  auto get(var_s &&a_value) -> var_s * { return m_loc->get(std::move(a_value)); }
  auto get(int64_t a_idx) -> var_s * { return m_loc->get(a_idx); }

  [[nodiscard]] auto first_idx() const -> uint32_t { return m_loc->first_idx(); }
  [[nodiscard]] auto next_idx(uint32_t a_idx) const -> uint32_t { return m_loc->next_idx(a_idx); }
  auto at_idx(uint32_t a_idx) -> var_s & { return m_loc->at_idx(a_idx); }

  inline auto begin() -> iter_s;
  inline auto end() -> iter_s;

  [[nodiscard]] auto type() const noexcept -> int { return m_loc->type(); }
  auto operator==(const var_s &a_second) const noexcept -> bool
  {/*{{{*/
    return (m_loc->type() == a_second.m_loc->type() && *m_loc == *a_second.m_loc);
  }/*}}}*/
  [[nodiscard]] auto compare(const var_s &a_second) const noexcept -> int
  {/*{{{*/
    auto f_type = m_loc->type();
    auto s_type = a_second.m_loc->type();

    return f_type < s_type ? -1 : f_type > s_type ? 1 : m_loc->compare(*a_second.m_loc);
  }/*}}}*/
  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> & { return m_loc->to_string(a_trg); }
  auto to_json(array<char> *a_trg) const noexcept -> ccl::array<char> & { return a_trg->json(*m_loc); }
  auto to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return a_trg->json_nice(*m_loc,a_json_nice);
  }/*}}}*/
};/*}}}*/

struct iter_s
{/*{{{*/
  friend var_s;

private:
  var_s m_cont;
  uint32_t m_index;

public:
  iter_s(var_s a_cont,uint32_t a_index) : m_cont(std::move(a_cont)), m_index(a_index) {}

  auto operator++() -> iter_s & { m_index = m_cont.next_idx(m_index); return *this; }
  auto operator!=(const iter_s &a_second) const noexcept -> bool { return m_index != a_second.m_index; }
  auto operator*() noexcept -> var_s & { return m_cont.at_idx(m_index); }
};/*}}}*/

// === definition of structure map_s ===========================================

struct map_s
{/*{{{*/
  friend loc_dict_s;

private:
  var_s m_key;
  var_s m_value;

public:
  [[nodiscard]] auto key() const -> const var_s & { return m_key; }
  [[nodiscard]] auto value() const -> const var_s & { return m_value; }

  ~map_s() = default;
  map_s() = default;

  explicit map_s(var_s &&a_key) : m_key(std::move(a_key)) {}
  map_s(var_s &&a_key,var_s &&a_value) : m_key(std::move(a_key)), m_value(std::move(a_value)) {}

  map_s(const map_s &a_second) = default;
  map_s(map_s &&a_second) = default;

  auto operator=(const map_s &a_src) -> map_s & = default;
  auto operator=(map_s &&a_src) -> map_s & = default;

  auto value() -> var_s & { return m_value; }

  auto operator==(const map_s &a_second) const -> bool
  {/*{{{*/
    return m_key == a_second.m_key && m_value == a_second.m_value;
  }/*}}}*/

  [[nodiscard]] auto compare(const map_s &a_second) const -> int
  {/*{{{*/
    return ::compare(m_key,a_second.m_key);
  }/*}}}*/

  auto to_string(array<char> *a_trg) const noexcept -> ccl::array<char> &
  {/*{{{*/
    return (*a_trg)
      .string(m_key)
      .push(':')
      .string(m_value);
  }/*}}}*/
};/*}}}*/

// === inline methods of structure loc_array_s =================================

inline auto loc_array_s::operator[](const var_s &a_value) -> var_s &
{/*{{{*/
  return m_value[a_value.to_int()];
}/*}}}*/

inline auto loc_array_s::operator[](var_s &&a_value) -> var_s &
{/*{{{*/
  return m_value[a_value.to_int()];
}/*}}}*/

inline auto loc_array_s::get(const var_s &a_value) -> var_s *
{/*{{{*/
  return a_value.type() == bi_type_int ? get(a_value.to_int()) : nullptr;
}/*}}}*/

inline auto loc_array_s::get(var_s &&a_value) -> var_s *
{/*{{{*/
  return a_value.type() == bi_type_int ? get(a_value.to_int()) : nullptr;
}/*}}}*/

// === inline methods of structure loc_dict_s ==================================

inline auto loc_dict_s::has_key(const var_s &a_value) -> bool
{/*{{{*/
  return m_value.get_idx(map_s(var_s(a_value))) != c_idx_not_exist;
}/*}}}*/

inline auto loc_dict_s::remove_key(const var_s &a_value) -> void
{/*{{{*/
  uint32_t idx = m_value.get_idx(map_s(var_s(a_value)));

  if (idx == c_idx_not_exist)
  {
    throw_error(VAR_DICT_INVALID_KEY);
  }

  m_value.remove(idx);
}/*}}}*/

inline auto loc_dict_s::remove_if_key(const var_s &a_value) -> void
{/*{{{*/
  uint32_t idx = m_value.get_idx(map_s(var_s(a_value)));

  if (idx == c_idx_not_exist)
  {
    return;
  }

  m_value.remove(idx);
}/*}}}*/

inline auto loc_dict_s::operator[](const var_s &a_value) -> var_s &
{/*{{{*/
  uint32_t idx = m_value.unique_insert(map_s(var_s(a_value)));
  return m_value[idx].value();
}/*}}}*/

inline auto loc_dict_s::operator[](var_s &&a_value) -> var_s &
{/*{{{*/
  uint32_t idx = m_value.unique_insert(map_s(std::move(a_value)));
  return m_value[idx].value();
}/*}}}*/

inline auto loc_dict_s::operator[](int64_t a_idx) -> var_s &
{/*{{{*/
  return (*this)[var_s(a_idx)];
}/*}}}*/

inline auto loc_dict_s::get(const var_s &a_value) -> var_s *
{/*{{{*/
  uint32_t idx = m_value.get_idx(map_s(var_s(a_value)));
  return idx != c_idx_not_exist ? &m_value[idx].value() : nullptr;
}/*}}}*/

inline auto loc_dict_s::get(var_s &&a_value) -> var_s *
{/*{{{*/
  uint32_t idx = m_value.get_idx(map_s(std::move(a_value)));
  return idx != c_idx_not_exist ? &m_value[idx].value() : nullptr;
}/*}}}*/

inline auto loc_dict_s::get(int64_t a_idx) -> var_s *
{/*{{{*/
  return get(var_s(a_idx));
}/*}}}*/

inline auto loc_dict_s::next_idx(uint32_t a_idx) const -> uint32_t
{/*{{{*/
  if (a_idx < 0 || a_idx >= m_value.used() || !m_value.data()[a_idx].is_valid())
  {
    throw_error(VAR_INVALID_INDEX);
  }

  return m_value.next_idx(a_idx);
}/*}}}*/

inline auto loc_dict_s::at_idx(uint32_t a_idx) -> var_s &
{/*{{{*/
  if (a_idx < 0 || a_idx >= m_value.used() || !m_value.data()[a_idx].is_valid())
  {
    throw_error(VAR_INVALID_INDEX);
  }

  return m_value[a_idx].m_key;
}/*}}}*/

// === inline methods of structure var_s =======================================

inline auto var_s::begin() -> iter_s
{/*{{{*/
  return iter_s(*this,first_idx());
}/*}}}*/

inline auto var_s::end() -> iter_s
{/*{{{*/
  return iter_s(*this,c_idx_not_exist);
}/*}}}*/

} // namespace ccl

#endif

