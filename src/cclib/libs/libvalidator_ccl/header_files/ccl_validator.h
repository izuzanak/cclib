
#ifndef LIBS_LIBVALIDATOR_CCL_HEADER_FILES_CCL_VALIDATOR_H
#define LIBS_LIBVALIDATOR_CCL_HEADER_FILES_CCL_VALIDATOR_H

#include "ccl_regex.h"
#include "ccl_var.h"

namespace ccl {

constexpr int VALIDATOR_INVALID_SCHEMA_TYPE = 1;
constexpr int VALIDATOR_INVALID_PROPERTY_TYPE = 2;
constexpr int VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE = 3;
constexpr int VALIDATOR_INVALID_VALUE_TYPE = 4;
constexpr int VALIDATOR_INVALID_PROPERTY_ID = 5;
constexpr int VALIDATOR_INVALID_REFERENCE_NAME_TYPE = 6;
constexpr int VALIDATOR_INVALID_VALIDATION_ENTRY = 7;
constexpr int VALIDATOR_INVALID_VALUE = 8;
constexpr int VALIDATOR_INVALID_PROPERTY_REGULAR_EXPRESSION = 9;

// === constants and definitions ===============================================

enum {
  c_validator_prop_type = 0,
  c_validator_prop_equal,
  c_validator_prop_not_equal,
  c_validator_prop_lesser,
  c_validator_prop_greater,
  c_validator_prop_lesser_equal,
  c_validator_prop_greater_equal,
  c_validator_prop_length_equal,
  c_validator_prop_length_not_equal,
  c_validator_prop_length_lesser,
  c_validator_prop_length_greater,
  c_validator_prop_length_lesser_equal,
  c_validator_prop_length_greater_equal,
  c_validator_prop_reference,
  c_validator_prop_regex,
  c_validator_prop_test,
  c_validator_prop_items,
  c_validator_prop_opt_items,
  c_validator_prop_all_items,
  c_validator_prop_all_keys,
  c_validator_prop_in,
};

// === definition of structure validator_s =====================================

struct validator_s
{
private:
  var_s m_schema;
  array<var_s> m_value_stack;
  array<var_s> m_props_stack;

  tree<var_s> m_regex_map;
  array<regex_s> m_regex_list;

  static auto recognize_property(const char *a_string) -> uint32_t;
  [[nodiscard]] auto validate_pair(const var_s &a_value,const var_s &a_props) noexcept -> int;

public:
  ~validator_s() = default;
  validator_s() = default;

  explicit validator_s(const var_s &a_schema)
  {/*{{{*/
    if (a_schema.type() != bi_type_dict)
    {
      throw_error(VALIDATOR_INVALID_SCHEMA_TYPE);
    }

    m_schema = a_schema;
  }/*}}}*/

  [[nodiscard]] auto validate(const var_s &a_ref_name,const var_s &a_value) -> bool;
};

} // namespace ccl

#endif

