
#include "ccl_json.h"

namespace ccl {

// === constants and definitions ===============================================

constexpr uint32_t c_json_rule_cnt = 24;
constexpr std::array<uint32_t,c_json_rule_cnt> json_rule_head_idxs = {17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, };
constexpr std::array<uint32_t,c_json_rule_cnt> json_rule_body_lengths = {1, 2, 2, 1, 2, 1, 3, 1, 3, 2, 1, 2, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

constexpr std::array<uint32_t,json_lalr_state_cnt*c_json_terminal_plus_nonterminal_cnt> json_lalr_table =
{/*{{{*/
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(1),     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,     JSON_GOTO(2),     JSON_GOTO(3),
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(15),       json_blank,  JSON_REDUCE(15),  JSON_REDUCE(15),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(15),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(16),       json_blank,  JSON_REDUCE(16),  JSON_REDUCE(16),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(16),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(17),       json_blank,  JSON_REDUCE(17),  JSON_REDUCE(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(18),       json_blank,  JSON_REDUCE(18),  JSON_REDUCE(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(18),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(19),       json_blank,  JSON_REDUCE(19),  JSON_REDUCE(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(20),       json_blank,  JSON_REDUCE(20),  JSON_REDUCE(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(21),       json_blank,  JSON_REDUCE(21),  JSON_REDUCE(21),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(21),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(22),       json_blank,  JSON_REDUCE(22),  JSON_REDUCE(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(23),       json_blank,  JSON_REDUCE(23),  JSON_REDUCE(23),  JSON_REDUCE(23),       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(23),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(19),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(20),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
   JSON_SHIFT(11),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(3),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(21),    JSON_GOTO(22),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(23),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),  JSON_REDUCE(10),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),    JSON_GOTO(24),    JSON_GOTO(25),     JSON_GOTO(3),
   JSON_REDUCE(5),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(5),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),  JSON_REDUCE(12),       json_blank,  JSON_REDUCE(12),  JSON_REDUCE(12),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(2),       json_blank,   JSON_REDUCE(2),   JSON_REDUCE(2),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(2),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(9),       json_blank,   JSON_REDUCE(9),   JSON_REDUCE(9),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(9),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(4),       json_blank,       json_blank,   JSON_SHIFT(26),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(7),       json_blank,       json_blank,   JSON_REDUCE(7),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_SHIFT(27),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(11),   JSON_SHIFT(28),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(14),  JSON_REDUCE(14),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
   JSON_SHIFT(11),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(29),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,    JSON_GOTO(23),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,    JSON_GOTO(30),     JSON_GOTO(3),
   JSON_SHIFT(11),    JSON_SHIFT(4),    JSON_SHIFT(5),    JSON_SHIFT(8),    JSON_SHIFT(9),   JSON_SHIFT(10),   JSON_SHIFT(16),       json_blank,   JSON_SHIFT(17),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,     JSON_GOTO(6),    JSON_GOTO(12),    JSON_GOTO(14),       json_blank,       json_blank,     JSON_GOTO(7),    JSON_GOTO(13),    JSON_GOTO(15),       json_blank,    JSON_GOTO(31),     JSON_GOTO(3),
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(6),       json_blank,       json_blank,   JSON_REDUCE(6),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,   JSON_REDUCE(8),       json_blank,       json_blank,   JSON_REDUCE(8),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,  JSON_REDUCE(13),  JSON_REDUCE(13),       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,       json_blank,
};/*}}}*/

std::array<void (*)(json_parser_s *parser),json_parser_s::c_parse_action_cnt> json_parser_s::pa_callers =
{/*{{{*/
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_object_begin,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_object_pair,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_null,
  json_parser_s::pa_array_begin,
  json_parser_s::pa_array_value,
  json_parser_s::pa_array_value,
  json_parser_s::pa_val_string,
  json_parser_s::pa_val_integer,
  json_parser_s::pa_val_float,
  json_parser_s::pa_val_object,
  json_parser_s::pa_val_array,
  json_parser_s::pa_val_true,
  json_parser_s::pa_val_false,
  json_parser_s::pa_val_null,
  json_parser_s::pa_string,
};/*}}}*/

// === methods of structure json_parser_s ======================================

auto json_parser_s::pa_null(json_parser_s *parser) noexcept -> void
{/*{{{*/
  (void)parser;

  debug_assert(false);
}/*}}}*/

auto json_parser_s::pa_object_begin(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_objects.push(var_s::new_dict());
}/*}}}*/

auto json_parser_s::pa_object_pair(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_objects.last()[parser->m_string_vars[parser->m_string_idxs.pop()]] = std::move(parser->m_values.pop());
}/*}}}*/

auto json_parser_s::pa_array_begin(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_arrays.push(var_s::new_array());
}/*}}}*/

auto json_parser_s::pa_array_value(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_arrays.last().push(std::move(parser->m_values.pop()));
}/*}}}*/

auto json_parser_s::pa_val_string(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_values.push(parser->m_string_vars[parser->m_string_idxs.pop()]);
}/*}}}*/

auto json_parser_s::pa_val_integer(json_parser_s *parser) noexcept -> void
{/*{{{*/
  lalr_stack_element_s &lse = parser->m_lalr_stack.last();

  // - retrieve number from source -
  int64_t const_int = strtoll(parser->m_data + lse.terminal_start(),nullptr,10);

  // - get constant position in array -
  uint32_t cd_idx = parser->m_const_integers.unique_insert(const_int);

  if (cd_idx >= parser->m_integer_vars.used())
  {
    // - skip tree zero index -
    if (parser->m_integer_vars.used() == 0)
    {
      parser->m_integer_vars.push_blank();
    }

    // - create new integer variable -
    parser->m_integer_vars.push(var_s(const_int));
  }

  parser->m_values.push(parser->m_integer_vars[cd_idx]);
}/*}}}*/

auto json_parser_s::pa_val_float(json_parser_s *parser) noexcept -> void
{/*{{{*/
  lalr_stack_element_s &lse = parser->m_lalr_stack.last();

  // - retrieve number from source -
  double const_float = strtod(parser->m_data + lse.terminal_start(),nullptr);

  // - get constant position in array -
  uint32_t cd_idx = parser->m_const_floats.unique_insert(const_float);

  if (cd_idx >= parser->m_float_vars.used())
  {
    // - skip tree zero index -
    if (parser->m_float_vars.used() == 0)
    {
      parser->m_float_vars.push_blank();
    }

    // - create new float variable -
    parser->m_float_vars.push(var_s(const_float));
  }

  // - insert float variable to m_values -
  parser->m_values.push(parser->m_float_vars[cd_idx]);
}/*}}}*/

auto json_parser_s::pa_val_object(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_values.push(std::move(parser->m_objects.pop()));
}/*}}}*/

auto json_parser_s::pa_val_array(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_values.push(std::move(parser->m_arrays.pop()));
}/*}}}*/

auto json_parser_s::pa_val_true(json_parser_s *parser) noexcept -> void
{/*{{{*/
  if (parser->m_true_var.type() == bi_type_blank)
  {
    parser->m_true_var = var_s(true);
  }

  parser->m_values.push(parser->m_true_var);
}/*}}}*/

auto json_parser_s::pa_val_false(json_parser_s *parser) noexcept -> void
{/*{{{*/
  if (parser->m_false_var.type() == bi_type_blank)
  {
    parser->m_false_var = var_s(false);
  }

  parser->m_values.push(parser->m_false_var);
}/*}}}*/

auto json_parser_s::pa_val_null(json_parser_s *parser) noexcept -> void
{/*{{{*/
  parser->m_values.push(parser->m_null_var);
}/*}}}*/

auto json_parser_s::pa_string(json_parser_s *parser) noexcept -> void
{/*{{{*/
  lalr_stack_element_s &lse = parser->m_lalr_stack.last();

  const char *ptr = parser->m_data + lse.terminal_start() + 1;
  const char *ptr_end = parser->m_data + lse.terminal_end() - 1;

  process_json_string(ptr,ptr_end,&parser->m_buffer);

  // - get constant position in array -
  uint32_t cs_idx = parser->m_const_buffers.unique_insert(parser->m_buffer);

  if (cs_idx >= parser->m_string_vars.used())
  {
    // - skip tree zero index -
    if (parser->m_string_vars.used() == 0)
    {
      parser->m_string_vars.push_blank();
    }

    array<char> &buffer = parser->m_const_buffers[cs_idx];
    parser->m_string_vars.push(var_s(std::string(buffer.data(),buffer.used())));
  }

  // - store string index -
  parser->m_string_idxs.push(cs_idx);
}/*}}}*/

auto json_parser_s::process_json_string(const char *a_ptr,const char *a_ptr_end,array<char> *a_trg) -> void
{/*{{{*/
  a_trg->used() = 0;

  if (a_ptr < a_ptr_end)
  {
    do
    {
      if (*a_ptr == '\\')
      {
        a_ptr++;

        // - process character represented by unicode number -
        if (*a_ptr == 'u')
        {
          a_ptr++;

          unsigned value = 0;

          // - retrieve character value -
          const char *ptr_end = a_ptr + 4;
          do {
            value <<= 4;

            if (*a_ptr >= '0' && *a_ptr <= '9')
            {
              value += *a_ptr - '0';
            }
            else if (*a_ptr >= 'a' && *a_ptr <= 'f')
            {
              value += 10 + (*a_ptr - 'a');
            }
            else if (*a_ptr >= 'A' && *a_ptr <= 'F')
            {
              value += 10 + (*a_ptr - 'A');
            }
            else
            {
              debug_assert(false);
            }

          } while(++a_ptr < ptr_end);

          // - convert utf16/32 value to utf8 character string -
          if (value <= 0x7f)
          {
            a_trg->push(value);
          }
          else if (value <= 0x7ff)
          {
            (*a_trg)
              .push(0xc0 | value >> 6)
              .push(0x80 | (value & 0x3f));
          }
          else if (value <= 0xffff)
          {
            (*a_trg)
              .push(0xe0 |   value >> 12)
              .push(0x80 | ((value >>  6) & 0x3f))
              .push(0x80 |  (value        & 0x3f));
          }
          else
          {
            debug_assert(false);
          }
        }
        else
        {
          switch (*a_ptr++)
          {
          case 'b':
            a_trg->push('\b');
            break;
          case 'f':
            a_trg->push('\f');
            break;
          case 'n':
            a_trg->push('\n');
            break;
          case 'r':
            a_trg->push('\r');
            break;
          case 't':
            a_trg->push('\t');
            break;
          case '\\':
            a_trg->push('\\');
            break;
          case '"':
            a_trg->push('"');
            break;
          default:
            debug_assert(false);
          }
        }
      }
      else
      {
        a_trg->push(*a_ptr++);
      }
    }
    while(a_ptr < a_ptr_end);
  }
}/*}}}*/

auto json_parser_s::recognize_terminal() noexcept -> uint32_t // lgtm [cpp/use-of-goto]
{/*{{{*/
#define JSON_GET_NEXT_CHAR() \
  {\
    if (m_input_idx < m_size) {\
      in_char = (uint8_t)m_data[m_input_idx];\
    }\
    else {\
      in_char = '\0';\
    }\
  }

#define JSON_CLOSE_CHAR(RET_TERM_IDX) \
  {\
    if (in_char == '\0') {\
      return RET_TERM_IDX;\
    }\
    \
    ++m_input_idx;\
  }

   uint8_t in_char;

// - STATE 0 -
   JSON_GET_NEXT_CHAR();

   if (in_char == 0) {
      goto state_1_label;
   }
   if (in_char >= 8 && in_char < 14) {
      goto state_17_label;
   }
   if (in_char == 32) {
      goto state_17_label;
   }
   if (in_char == 34) {
      goto state_2_label;
   }
   if (in_char == 35) {
      goto state_3_label;
   }
   if (in_char == 44) {
      goto state_4_label;
   }
   if (in_char == 45) {
      goto state_5_label;
   }
   if (in_char == 47) {
      goto state_6_label;
   }
   if (in_char == 48) {
      goto state_7_label;
   }
   if (in_char >= 49 && in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 58) {
      goto state_9_label;
   }
   if (in_char == 91) {
      goto state_10_label;
   }
   if (in_char == 93) {
      goto state_11_label;
   }
   if (in_char == 102) {
      goto state_12_label;
   }
   if (in_char == 110) {
      goto state_13_label;
   }
   if (in_char == 116) {
      goto state_14_label;
   }
   if (in_char == 123) {
      goto state_15_label;
   }
   if (in_char == 125) {
      goto state_16_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   JSON_CLOSE_CHAR(16);
   return 16;

// - STATE 2 -
state_2_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 34) {
      goto state_2_label;
   }
   if (in_char == 34) {
      goto state_18_label;
   }
   if (in_char < 92) {
      goto state_2_label;
   }
   if (in_char == 92) {
      goto state_19_label;
   }
   goto state_2_label;

// - STATE 3 -
state_3_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_3_label;
   }
   if (in_char == 10) {
      goto state_20_label;
   }
   goto state_3_label;

// - STATE 4 -
state_4_label:
   JSON_CLOSE_CHAR(10);
   return 10;

// - STATE 5 -
state_5_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 48) {
      goto state_7_label;
   }
   if (in_char >= 49 && in_char < 58) {
      goto state_8_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 42) {
      goto state_21_label;
   }
   if (in_char == 47) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   JSON_CLOSE_CHAR(1);
   JSON_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_23_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 1;

// - STATE 8 -
state_8_label:
   JSON_CLOSE_CHAR(1);
   JSON_GET_NEXT_CHAR();

   if (in_char == 46) {
      goto state_23_label;
   }
   if (in_char >= 48 && in_char < 58) {
      goto state_8_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 1;

// - STATE 9 -
state_9_label:
   JSON_CLOSE_CHAR(11);
   return 11;

// - STATE 10 -
state_10_label:
   JSON_CLOSE_CHAR(8);
   return 8;

// - STATE 11 -
state_11_label:
   JSON_CLOSE_CHAR(9);
   return 9;

// - STATE 12 -
state_12_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 97) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 13 -
state_13_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 117) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 14 -
state_14_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 114) {
      goto state_27_label;
   }
   return c_idx_not_exist;

// - STATE 15 -
state_15_label:
   JSON_CLOSE_CHAR(6);
   return 6;

// - STATE 16 -
state_16_label:
   JSON_CLOSE_CHAR(7);
   return 7;

// - STATE 17 -
state_17_label:
   JSON_CLOSE_CHAR(12);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 8 && in_char < 14) {
      goto state_17_label;
   }
   if (in_char == 32) {
      goto state_17_label;
   }
   return 12;

// - STATE 18 -
state_18_label:
   JSON_CLOSE_CHAR(0);
   return 0;

// - STATE 19 -
state_19_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 34) {
      goto state_2_label;
   }
   if (in_char == 47) {
      goto state_2_label;
   }
   if (in_char == 92) {
      goto state_2_label;
   }
   if (in_char == 98) {
      goto state_2_label;
   }
   if (in_char == 102) {
      goto state_2_label;
   }
   if (in_char == 110) {
      goto state_2_label;
   }
   if (in_char == 114) {
      goto state_2_label;
   }
   if (in_char == 116) {
      goto state_2_label;
   }
   if (in_char == 117) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   JSON_CLOSE_CHAR(13);
   return 13;

// - STATE 21 -
state_21_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 42) {
      goto state_21_label;
   }
   if (in_char == 42) {
      goto state_29_label;
   }
   goto state_21_label;

// - STATE 22 -
state_22_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 10) {
      goto state_22_label;
   }
   if (in_char == 10) {
      goto state_30_label;
   }
   goto state_22_label;

// - STATE 23 -
state_23_label:
   JSON_CLOSE_CHAR(2);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_23_label;
   }
   if (in_char == 69) {
      goto state_24_label;
   }
   if (in_char == 101) {
      goto state_24_label;
   }
   return 2;

// - STATE 24 -
state_24_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 43) {
      goto state_31_label;
   }
   if (in_char == 45) {
      goto state_31_label;
   }
   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_34_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 117) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 28 -
state_28_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_36_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_36_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_36_label;
   }
   return c_idx_not_exist;

// - STATE 29 -
state_29_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char < 47) {
      goto state_21_label;
   }
   if (in_char == 47) {
      goto state_37_label;
   }
   goto state_21_label;

// - STATE 30 -
state_30_label:
   JSON_CLOSE_CHAR(14);
   return 14;

// - STATE 31 -
state_31_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   JSON_CLOSE_CHAR(2);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_32_label;
   }
   return 2;

// - STATE 33 -
state_33_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_38_label;
   }
   return c_idx_not_exist;

// - STATE 34 -
state_34_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_39_label;
   }
   return c_idx_not_exist;

// - STATE 35 -
state_35_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_40_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_41_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_41_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_41_label;
   }
   return c_idx_not_exist;

// - STATE 37 -
state_37_label:
   JSON_CLOSE_CHAR(15);
   return 15;

// - STATE 38 -
state_38_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_42_label;
   }
   return c_idx_not_exist;

// - STATE 39 -
state_39_label:
   JSON_CLOSE_CHAR(5);
   return 5;

// - STATE 40 -
state_40_label:
   JSON_CLOSE_CHAR(3);
   return 3;

// - STATE 41 -
state_41_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_43_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_43_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_43_label;
   }
   return c_idx_not_exist;

// - STATE 42 -
state_42_label:
   JSON_CLOSE_CHAR(4);
   return 4;

// - STATE 43 -
state_43_label:
   JSON_CLOSE_CHAR(c_idx_not_exist);
   JSON_GET_NEXT_CHAR();

   if (in_char >= 48 && in_char < 58) {
      goto state_2_label;
   }
   if (in_char >= 65 && in_char < 71) {
      goto state_2_label;
   }
   if (in_char >= 97 && in_char < 103) {
      goto state_2_label;
   }
   return c_idx_not_exist;

}/*}}}*/

auto json_parser_s::parse(uint32_t a_size,const char *a_data) -> var_s
{/*{{{*/
  m_data = a_data;
  m_size = a_size;

  m_input_idx = 0;
  m_old_input_idx = 0;

  m_lalr_stack.used() = 0;
  m_lalr_stack.push(lalr_stack_element_s());

  uint32_t ret_term = c_idx_not_exist;

  do
  {
    // - retrieve next terminal symbol -
    while (ret_term == c_idx_not_exist)
    {
      m_old_input_idx = m_input_idx;

      ret_term = recognize_terminal();

      // - PARSE ERROR unrecognized terminal -
      if (ret_term == c_idx_not_exist)
      {
        throw_error(JSON_PARSE_UNRECOGNIZED_TERMINAL);
      }

      // - skipping of _SKIP_ terminals -
      if (ret_term >= 12 && ret_term <= 15)
      {
        ret_term = c_idx_not_exist;
      }
    }

    // - retrieve action from table of actions -
    uint32_t parse_action = json_lalr_table[m_lalr_stack.last().lalr_state()*c_json_terminal_plus_nonterminal_cnt + ret_term];

    // - PARSE ERROR wrong syntax -
    if (parse_action == c_idx_not_exist)
    {
      throw_error(JSON_PARSE_INVALID_SYNTAX);
    }

    // - action JSON_SHIFT -
    if (parse_action < c_json_lalr_table_reduce_base)
    {
      if (ret_term == 16)
      {
        break;
      }

      // - insertion of state, and terminal position in source string -
      m_lalr_stack.push(lalr_stack_element_s(parse_action,m_old_input_idx,m_input_idx));
      ret_term = c_idx_not_exist;
    }

    // - action JSON_REDUCE-
    else
    {
      parse_action -= c_json_lalr_table_reduce_base;

      // - call function assigned to reduction -
      if (pa_callers[parse_action] != json_parser_s::pa_null)
      {
        pa_callers[parse_action](this);
      }

      // - remove rule body from lalr stack -
      m_lalr_stack.used() -= json_rule_body_lengths[parse_action];

      // - insert new automata state to lalr stack -
      uint32_t goto_val = json_lalr_table[m_lalr_stack.last().lalr_state()*c_json_terminal_plus_nonterminal_cnt + json_rule_head_idxs[parse_action]];
      m_lalr_stack.push(lalr_stack_element_s(goto_val));
    }
  }
  while(true);

  return std::move(m_values.pop());
}/*}}}*/

} // namespace ccl

