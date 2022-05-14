
#include "ccl_validator.h"

namespace ccl {

// === methods of structure validator_s ========================================

auto validator_s::recognize_property(const char *a_string) -> uint32_t
{/*{{{*/
#define VALIDATOR_GET_NEXT_CHAR() \
{\
   in_char = (unsigned char)a_string[input_idx];\
}

#define VALIDATOR_CLOSE_CHAR(RET_TERM_IDX) \
{\
   if (in_char == '\0') {\
      return RET_TERM_IDX;\
   }\
\
   input_idx++;\
}

   uint32_t input_idx = 0;
   unsigned char in_char;

// - STATE 0 -
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 33) {
      goto state_1_label;
   }
   if (in_char == 60) {
      goto state_2_label;
   }
   if (in_char == 61) {
      goto state_3_label;
   }
   if (in_char == 62) {
      goto state_4_label;
   }
   if (in_char == 97) {
      goto state_5_label;
   }
   if (in_char == 105) {
      goto state_6_label;
   }
   if (in_char == 108) {
      goto state_7_label;
   }
   if (in_char == 111) {
      goto state_8_label;
   }
   if (in_char == 114) {
      goto state_9_label;
   }
   if (in_char == 116) {
      goto state_10_label;
   }
   return c_idx_not_exist;

// - STATE 1 -
state_1_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_11_label;
   }
   return c_idx_not_exist;

// - STATE 2 -
state_2_label:
   VALIDATOR_CLOSE_CHAR(3);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_12_label;
   }
   return 3;

// - STATE 3 -
state_3_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_13_label;
   }
   return c_idx_not_exist;

// - STATE 4 -
state_4_label:
   VALIDATOR_CLOSE_CHAR(4);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_14_label;
   }
   return 4;

// - STATE 5 -
state_5_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_15_label;
   }
   return c_idx_not_exist;

// - STATE 6 -
state_6_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 110) {
      goto state_16_label;
   }
   if (in_char == 116) {
      goto state_17_label;
   }
   return c_idx_not_exist;

// - STATE 7 -
state_7_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_18_label;
   }
   return c_idx_not_exist;

// - STATE 8 -
state_8_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 112) {
      goto state_19_label;
   }
   return c_idx_not_exist;

// - STATE 9 -
state_9_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_20_label;
   }
   return c_idx_not_exist;

// - STATE 10 -
state_10_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_21_label;
   }
   if (in_char == 121) {
      goto state_22_label;
   }
   return c_idx_not_exist;

// - STATE 11 -
state_11_label:
   VALIDATOR_CLOSE_CHAR(2);
   return 2;

// - STATE 12 -
state_12_label:
   VALIDATOR_CLOSE_CHAR(5);
   return 5;

// - STATE 13 -
state_13_label:
   VALIDATOR_CLOSE_CHAR(1);
   return 1;

// - STATE 14 -
state_14_label:
   VALIDATOR_CLOSE_CHAR(6);
   return 6;

// - STATE 15 -
state_15_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 108) {
      goto state_23_label;
   }
   return c_idx_not_exist;

// - STATE 16 -
state_16_label:
   VALIDATOR_CLOSE_CHAR(20);
   return 20;

// - STATE 17 -
state_17_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_24_label;
   }
   return c_idx_not_exist;

// - STATE 18 -
state_18_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 110) {
      goto state_25_label;
   }
   return c_idx_not_exist;

// - STATE 19 -
state_19_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_26_label;
   }
   return c_idx_not_exist;

// - STATE 20 -
state_20_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 102) {
      goto state_27_label;
   }
   if (in_char == 103) {
      goto state_28_label;
   }
   return c_idx_not_exist;

// - STATE 21 -
state_21_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_29_label;
   }
   return c_idx_not_exist;

// - STATE 22 -
state_22_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 112) {
      goto state_30_label;
   }
   return c_idx_not_exist;

// - STATE 23 -
state_23_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 45) {
      goto state_31_label;
   }
   return c_idx_not_exist;

// - STATE 24 -
state_24_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_32_label;
   }
   return c_idx_not_exist;

// - STATE 25 -
state_25_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 103) {
      goto state_33_label;
   }
   return c_idx_not_exist;

// - STATE 26 -
state_26_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 45) {
      goto state_34_label;
   }
   return c_idx_not_exist;

// - STATE 27 -
state_27_label:
   VALIDATOR_CLOSE_CHAR(13);
   return 13;

// - STATE 28 -
state_28_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_35_label;
   }
   return c_idx_not_exist;

// - STATE 29 -
state_29_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_36_label;
   }
   return c_idx_not_exist;

// - STATE 30 -
state_30_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_37_label;
   }
   return c_idx_not_exist;

// - STATE 31 -
state_31_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 105) {
      goto state_38_label;
   }
   if (in_char == 107) {
      goto state_39_label;
   }
   return c_idx_not_exist;

// - STATE 32 -
state_32_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_40_label;
   }
   return c_idx_not_exist;

// - STATE 33 -
state_33_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_41_label;
   }
   return c_idx_not_exist;

// - STATE 34 -
state_34_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 105) {
      goto state_42_label;
   }
   return c_idx_not_exist;

// - STATE 35 -
state_35_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 120) {
      goto state_43_label;
   }
   return c_idx_not_exist;

// - STATE 36 -
state_36_label:
   VALIDATOR_CLOSE_CHAR(15);
   return 15;

// - STATE 37 -
state_37_label:
   VALIDATOR_CLOSE_CHAR(0);
   return 0;

// - STATE 38 -
state_38_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_44_label;
   }
   return c_idx_not_exist;

// - STATE 39 -
state_39_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_45_label;
   }
   return c_idx_not_exist;

// - STATE 40 -
state_40_label:
   VALIDATOR_CLOSE_CHAR(16);
   return 16;

// - STATE 41 -
state_41_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 104) {
      goto state_46_label;
   }
   return c_idx_not_exist;

// - STATE 42 -
state_42_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 116) {
      goto state_47_label;
   }
   return c_idx_not_exist;

// - STATE 43 -
state_43_label:
   VALIDATOR_CLOSE_CHAR(14);
   return 14;

// - STATE 44 -
state_44_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_48_label;
   }
   return c_idx_not_exist;

// - STATE 45 -
state_45_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 121) {
      goto state_49_label;
   }
   return c_idx_not_exist;

// - STATE 46 -
state_46_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 32) {
      goto state_50_label;
   }
   return c_idx_not_exist;

// - STATE 47 -
state_47_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 101) {
      goto state_51_label;
   }
   return c_idx_not_exist;

// - STATE 48 -
state_48_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_52_label;
   }
   return c_idx_not_exist;

// - STATE 49 -
state_49_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_53_label;
   }
   return c_idx_not_exist;

// - STATE 50 -
state_50_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 33) {
      goto state_54_label;
   }
   if (in_char == 60) {
      goto state_55_label;
   }
   if (in_char == 61) {
      goto state_56_label;
   }
   if (in_char == 62) {
      goto state_57_label;
   }
   return c_idx_not_exist;

// - STATE 51 -
state_51_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 109) {
      goto state_58_label;
   }
   return c_idx_not_exist;

// - STATE 52 -
state_52_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_59_label;
   }
   return c_idx_not_exist;

// - STATE 53 -
state_53_label:
   VALIDATOR_CLOSE_CHAR(19);
   return 19;

// - STATE 54 -
state_54_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_60_label;
   }
   return c_idx_not_exist;

// - STATE 55 -
state_55_label:
   VALIDATOR_CLOSE_CHAR(9);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_61_label;
   }
   return 9;

// - STATE 56 -
state_56_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_62_label;
   }
   return c_idx_not_exist;

// - STATE 57 -
state_57_label:
   VALIDATOR_CLOSE_CHAR(10);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 61) {
      goto state_63_label;
   }
   return 10;

// - STATE 58 -
state_58_label:
   VALIDATOR_CLOSE_CHAR(c_idx_not_exist);
   VALIDATOR_GET_NEXT_CHAR();

   if (in_char == 115) {
      goto state_64_label;
   }
   return c_idx_not_exist;

// - STATE 59 -
state_59_label:
   VALIDATOR_CLOSE_CHAR(18);
   return 18;

// - STATE 60 -
state_60_label:
   VALIDATOR_CLOSE_CHAR(8);
   return 8;

// - STATE 61 -
state_61_label:
   VALIDATOR_CLOSE_CHAR(11);
   return 11;

// - STATE 62 -
state_62_label:
   VALIDATOR_CLOSE_CHAR(7);
   return 7;

// - STATE 63 -
state_63_label:
   VALIDATOR_CLOSE_CHAR(12);
   return 12;

// - STATE 64 -
state_64_label:
   VALIDATOR_CLOSE_CHAR(17);
   return 17;

}/*}}}*/

#define VALIDATE_PAIR_REFERENCE(VALUE,PROPS_REF,ERR_CODE) \
{/*{{{*/\
  var_s *props = m_schema.get(PROPS_REF);\
\
  if (props == nullptr)\
  {\
    ERR_CODE;\
\
    return_error(VALIDATOR_INVALID_VALIDATION_ENTRY);\
  }\
\
  if (validate_pair(VALUE,*props) != 0)\
  {\
    m_props_stack.push(PROPS_REF);\
\
    ERR_CODE;\
\
    return VALIDATOR_INVALID_VALUE;\
  }\
}/*}}}*/

#define VALIDATE_PAIR_CALL(VALUE,PROPS,ERR_CODE) \
{/*{{{*/\
  if (PROPS.type() == bi_type_string)\
  {\
    VALIDATE_PAIR_REFERENCE(VALUE,PROPS,ERR_CODE);\
  }\
  else\
  {\
    if (validate_pair(VALUE,PROPS))\
    {\
      ERR_CODE;\
\
      return VALIDATOR_INVALID_VALUE;\
    }\
  }\
}/*}}}*/

auto validator_s::validate_pair(const var_s &a_value,const var_s &a_props) noexcept -> int // NOLINT(misc-no-recursion)
{/*{{{*/
  if (a_props.type() != bi_type_array)
  {
    return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
  }

  const array<var_s> &props_array = a_props.to_array();

  if ((props_array.used() & 0x01) != 0)
  {
    return_error(VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE);
  }

  if (props_array.used() > 0)
  {
    const var_s *prop_ptr = props_array.data();
    const var_s *prop_ptr_end = prop_ptr + props_array.used();

    do {

      // - retrieve property key and value -
      const var_s &prop_key = prop_ptr[0];
      const var_s &prop_value = prop_ptr[1];

#define VALIDATE_STACKS_PUSH_PROP_KEY() \
/*{{{*/\
  m_props_stack.push(prop_key);
/*}}}*/

#define VALIDATE_STACKS_PUSH_ITEMS() \
/*{{{*/\
  m_value_stack.push(item_key);\
  m_props_stack.push(item_key);\
\
  VALIDATE_STACKS_PUSH_PROP_KEY();\
/*}}}*/

      if (prop_key.type() != bi_type_string)
      {
        return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
      }

      unsigned prop_id = recognize_property(prop_key.to_str().data());

      switch (prop_id)
      {
      case c_validator_prop_type:
      {/*{{{*/
        if (prop_value.type() != bi_type_int)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        if (a_value.type() != prop_value.to_int())
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE_TYPE);
        }
      }/*}}}*/
      break;
      case c_validator_prop_equal:
      case c_validator_prop_not_equal:
      case c_validator_prop_lesser:
      case c_validator_prop_greater:
      case c_validator_prop_lesser_equal:
      case c_validator_prop_greater_equal:
      {/*{{{*/
        int result = ::compare(a_value,prop_value);
        bool ok;

        switch (prop_id)
        {
          case c_validator_prop_equal:
            ok = result == 0;
            break;
          case c_validator_prop_not_equal:
            ok = result != 0;
            break;
          case c_validator_prop_lesser:
            ok = result < 0;
            break;
          case c_validator_prop_greater:
            ok = result > 0;
            break;
          case c_validator_prop_lesser_equal:
            ok = result <= 0;
            break;
          case c_validator_prop_greater_equal:
            ok = result >= 0;
            break;
        }

        if (!ok)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE);
        }
      }/*}}}*/
      break;
      case c_validator_prop_length_equal:
      case c_validator_prop_length_not_equal:
      case c_validator_prop_length_lesser:
      case c_validator_prop_length_greater:
      case c_validator_prop_length_lesser_equal:
      case c_validator_prop_length_greater_equal:
      {/*{{{*/
        if (prop_value.type() != bi_type_int)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        int64_t prop_length = prop_value.to_int();
        int64_t value_length;

        try
        {
          value_length = a_value.length();
        }
        catch (...)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE_TYPE);
        }

        bool ok;

        switch (prop_id)
        {
          case c_validator_prop_length_equal:
            ok = value_length == prop_length;
            break;
          case c_validator_prop_length_not_equal:
            ok = value_length != prop_length;
            break;
          case c_validator_prop_length_lesser:
            ok = value_length < prop_length;
            break;
          case c_validator_prop_length_greater:
            ok = value_length > prop_length;
            break;
          case c_validator_prop_length_lesser_equal:
            ok = value_length <= prop_length;
            break;
          case c_validator_prop_length_greater_equal:
            ok = value_length >= prop_length;
            break;
        }

        if (!ok)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE);
        }
      }/*}}}*/
      break;
      case c_validator_prop_reference:
      {/*{{{*/
        if (prop_value.type() != bi_type_string)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        VALIDATE_PAIR_REFERENCE(a_value,prop_value,
          VALIDATE_STACKS_PUSH_PROP_KEY();
        );
      }/*}}}*/
      break;
      case c_validator_prop_regex:
      {/*{{{*/
        if (prop_value.type() != bi_type_string)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        if (a_value.type() != bi_type_string)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE_TYPE);
        }

        // - retrieve regular expression index -
        uint32_t regex_idx = m_regex_map.unique_insert(prop_value);

        regex_s *regp;
        if (regex_idx >= m_regex_list.used())
        {
          while (m_regex_list.used() < regex_idx)
          {
            m_regex_list.push_blank();
          }

          try
          {
            m_regex_list.push(regex_s(prop_value.to_str()));
          }
          catch (...)
          {
            VALIDATE_STACKS_PUSH_PROP_KEY();

            return_error(VALIDATOR_INVALID_PROPERTY_REGULAR_EXPRESSION);
          }

          regp = &m_regex_list.last();
        }
        else
        {
          regp = &m_regex_list[regex_idx];
        }

        // - ERROR -
        regmatch_s match;
        if (!regp->match(a_value.to_str().data(),&match))
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE);
        }
      }/*}}}*/
      break;
      case c_validator_prop_items:
      {/*{{{*/

        // - ERROR -
        if (prop_value.type() != bi_type_array)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        const array<var_s> &item_array = prop_value.to_array();

        // - ERROR -
        if ((item_array.used() & 0x01) != 0)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE);
        }

        if (item_array.used() > 0)
        {
          const var_s *i_ptr = item_array.data();
          const var_s *i_ptr_end = i_ptr + item_array.used();
          do {
            const var_s &item_key = i_ptr[0];
            const var_s &item_props = i_ptr[1];

            try
            {
              const var_s *item = const_cast<var_s &>(a_value).get(item_key);

              if (item == nullptr)
              {
                VALIDATE_STACKS_PUSH_ITEMS();

                return_error(VALIDATOR_INVALID_VALUE);
              }

              VALIDATE_PAIR_CALL(*item,item_props,
                VALIDATE_STACKS_PUSH_ITEMS();
              );
            }
            catch (...)
            {
              VALIDATE_STACKS_PUSH_ITEMS();

              return_error(VALIDATOR_INVALID_VALUE);
            }
          } while((i_ptr += 2) < i_ptr_end);
        }
      }/*}}}*/
      break;
      case c_validator_prop_opt_items:
      {/*{{{*/
        if (prop_value.type() != bi_type_array)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTY_TYPE);
        }

        const array<var_s> &item_array = prop_value.to_array();

        if ((item_array.used() & 0x01) != 0)
        {
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_PROPERTIES_ARRAY_SIZE);
        }

        if (item_array.used() != 0)
        {
          const var_s *i_ptr = item_array.data();
          const var_s *i_ptr_end = i_ptr + item_array.used();
          do {
            const var_s &item_key = i_ptr[0];
            const var_s &item_props = i_ptr[1];

            try
            {
              const var_s *item = const_cast<var_s &>(a_value).get(item_key);

              if (item != nullptr)
              {
                VALIDATE_PAIR_CALL(*item,item_props,
                  VALIDATE_STACKS_PUSH_ITEMS();
                );
              }
            }
            catch (...)
            {
              VALIDATE_STACKS_PUSH_ITEMS();

              return_error(VALIDATOR_INVALID_VALUE);
            }
          } while((i_ptr += 2) < i_ptr_end);
        }
      }/*}}}*/
      break;
      case c_validator_prop_all_items:
      {/*{{{*/
        switch (a_value.type())
        {
        case bi_type_array:
        {/*{{{*/
          const array<var_s> &value_array = a_value.to_array();

          if (value_array.used() != 0)
          {
            const var_s *ptr = value_array.data();
            const var_s *ptr_end = ptr + value_array.used();
            do {
              VALIDATE_PAIR_CALL(*ptr,prop_value,
                var_s index(static_cast<int64_t>(ptr - value_array.data()));

                m_value_stack.push(index);
                m_props_stack.push(std::move(index));

                VALIDATE_STACKS_PUSH_PROP_KEY();
              );
            } while(++ptr < ptr_end);
          }
        }/*}}}*/
        break;
        case bi_type_dict:
        {/*{{{*/
          const tree<map_s> &tree = a_value.to_dict();

          if (tree.used() > 0)
          {
            const auto *ptr = tree.data();
            const auto *ptr_end = ptr + tree.used();
            do {
              if (ptr->is_valid())
              {
                VALIDATE_PAIR_CALL(ptr->object().value(),prop_value,
                  var_s key(ptr->object().key());

                  m_value_stack.push(key);
                  m_props_stack.push(std::move(key));

                  VALIDATE_STACKS_PUSH_PROP_KEY();
                );
              }
            } while(++ptr < ptr_end);
          }
        }/*}}}*/
        break;

        // - ERROR -
        default:
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE);
        }
      }/*}}}*/
      break;
      case c_validator_prop_all_keys:
      {/*{{{*/
        switch (a_value.type())
        {
        case bi_type_dict:
        {/*{{{*/
          const tree<map_s> &tree = a_value.to_dict();

          if (tree.used() > 0)
          {
            const auto *ptr = tree.data();
            const auto *ptr_end = ptr + tree.used();
            do {
              if (ptr->is_valid())
              {
                VALIDATE_PAIR_CALL(ptr->object().key(),prop_value,
                  var_s key(ptr->object().key());

                  m_value_stack.push(key);
                  m_props_stack.push(std::move(key));

                  VALIDATE_STACKS_PUSH_PROP_KEY();
                );
              }
            } while(++ptr < ptr_end);
          }
        }/*}}}*/
        break;

        // - ERROR -
        default:
          VALIDATE_STACKS_PUSH_PROP_KEY();

          return_error(VALIDATOR_INVALID_VALUE);
        }
      }/*}}}*/
      break;

      // - ERROR -
      default:
        VALIDATE_STACKS_PUSH_PROP_KEY();

        return_error(VALIDATOR_INVALID_PROPERTY_ID);
      }
    } while((prop_ptr += 2) < prop_ptr_end);
  }

  return 0;
}/*}}}*/

auto validator_s::validate(const var_s &a_ref_name,const var_s &a_value) -> bool
{/*{{{*/
  if (a_ref_name.type() != bi_type_string)
  {
    throw_error(VALIDATOR_INVALID_REFERENCE_NAME_TYPE);
  }

  const var_s *props = m_schema.get(a_ref_name);

  if (props == nullptr)
  {
    throw_error(VALIDATOR_INVALID_VALIDATION_ENTRY);
  }

  m_value_stack.used() = 0;
  m_props_stack.used() = 0;

  // - ERROR -
  if (validate_pair(a_value,*props) != 0)
  {
    m_props_stack.push(a_ref_name);

    std::reverse(m_value_stack.data(),m_value_stack.data() + m_value_stack.used());
    std::reverse(m_props_stack.data(),m_props_stack.data() + m_props_stack.used());

    return false;
  }

  return true;
}/*}}}*/

} // namespace ccl

