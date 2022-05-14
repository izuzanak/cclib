
#ifndef LIBS_LIBJSON_CCL_HEADER_FILES_CCL_JSON_H
#define LIBS_LIBJSON_CCL_HEADER_FILES_CCL_JSON_H

#include "ccl_var.h"

namespace ccl {

constexpr int JSON_PARSE_UNRECOGNIZED_TERMINAL = 1;
constexpr int JSON_PARSE_INVALID_SYNTAX = 2;

// === constants and definitions ===============================================

constexpr uint32_t c_max_number_string_length = 128;

#define json_blank c_idx_not_exist
#define JSON_SHIFT(VALUE) VALUE
#define JSON_REDUCE(VALUE) c_json_lalr_table_reduce_base + VALUE
#define JSON_GOTO(VALUE) VALUE

constexpr uint32_t c_json_lalr_table_reduce_base = 0x80000000;
constexpr uint32_t c_json_terminal_plus_nonterminal_cnt = 30;
constexpr uint32_t json_lalr_state_cnt = 32;

// === definition of structure lalr_stack_element_s ============================

struct lalr_stack_element_s
{/*{{{*/
private:
  uint32_t m_lalr_state = 0;
  uint32_t m_terminal_start = 0;
  uint32_t m_terminal_end = 0;

public:
  [[nodiscard]] auto lalr_state() const noexcept -> uint32_t { return m_lalr_state; }
  [[nodiscard]] auto terminal_start() const noexcept -> uint32_t { return m_terminal_start; }
  [[nodiscard]] auto terminal_end() const noexcept -> uint32_t { return m_terminal_end; }

  lalr_stack_element_s() = default;
  explicit lalr_stack_element_s(uint32_t a_lalr_state) : m_lalr_state(a_lalr_state) {}
  lalr_stack_element_s(uint32_t a_lalr_state,uint32_t a_terminal_start,uint32_t a_terminal_end) :
    m_lalr_state(a_lalr_state), m_terminal_start(a_terminal_start), m_terminal_end(a_terminal_end) {}
};/*}}}*/

// === definition of structure json_parser_s ===================================

struct json_parser_s
{/*{{{*/
private:
  const char *m_data = nullptr;
  uint32_t m_size = 0;

  uint32_t m_input_idx = 0;
  uint32_t m_old_input_idx = 0;

  tree<int64_t> m_const_integers;
  tree<double> m_const_floats;

  array<char> m_buffer;
  tree<array<char>> m_const_buffers;

  var_s m_null_var;
  var_s m_true_var;
  var_s m_false_var;

  array<var_s> m_integer_vars;
  array<var_s> m_float_vars;
  array<var_s> m_string_vars;

  array<uint32_t> m_string_idxs;

  array<var_s> m_values;
  array<var_s> m_arrays;
  array<var_s> m_objects;

  array<lalr_stack_element_s> m_lalr_stack;

  static auto pa_null(json_parser_s *parser) noexcept -> void;
  static auto pa_object_begin(json_parser_s *parser) noexcept -> void;
  static auto pa_object_pair(json_parser_s *parser) noexcept -> void;
  static auto pa_array_begin(json_parser_s *parser) noexcept -> void;
  static auto pa_array_value(json_parser_s *parser) noexcept -> void;
  static auto pa_val_string(json_parser_s *parser) noexcept -> void;
  static auto pa_val_integer(json_parser_s *parser) noexcept -> void;
  static auto pa_val_float(json_parser_s *parser) noexcept -> void;
  static auto pa_val_object(json_parser_s *parser) noexcept -> void;
  static auto pa_val_array(json_parser_s *parser) noexcept -> void;
  static auto pa_val_true(json_parser_s *parser) noexcept -> void;
  static auto pa_val_false(json_parser_s *parser) noexcept -> void;
  static auto pa_val_null(json_parser_s *parser) noexcept -> void;
  static auto pa_string(json_parser_s *parser) noexcept -> void;

  constexpr static uint32_t c_parse_action_cnt = 24;
  static std::array<void (*)(json_parser_s *parser),c_parse_action_cnt> pa_callers;

  static auto process_json_string(const char *a_ptr,const char *a_ptr_end,array<char> *a_trg) -> void;

public:
  json_parser_s() = default;
  json_parser_s(const json_parser_s &a_src) = delete;
  json_parser_s(json_parser_s &&a_src) = default;

  auto operator=(const json_parser_s &a_src) -> json_parser_s & = delete;
  auto operator=(json_parser_s &&a_src) -> json_parser_s & = default;

  [[nodiscard]] auto recognize_terminal() noexcept -> uint32_t;
  auto parse(uint32_t a_size,const char *a_data) -> var_s;
};/*}}}*/

} // namespace ccl

#endif

