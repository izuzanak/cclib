
#include "ccl_var.h"

namespace ccl {

std::shared_ptr<loc_s> var_s::m_loc_blank = std::make_shared<loc_blank_s>(); // NOLINT(cert-err58-cpp)

auto loc_dict_s::to_json(array<char> *a_trg) const noexcept -> ccl::array<char> &
{/*{{{*/
  a_trg->push('{');

  if (m_value.count() > 0)
  {
    const auto *ptr = m_value.data();
    const auto *ptr_end = ptr + m_value.used();
    bool comma = false;
    do
    {
      if (ptr->is_valid())
      {
        const auto &object = ptr->object();

        if (comma)
        {
          a_trg->push(',');
        }
        else
        {
          comma = true;
        }

        (*a_trg)
          .json(object.key())
          .push(':')
          .json(object.value());
      }
    } while(++ptr < ptr_end);
  }

  a_trg->push('}');

  return *a_trg;
}/*}}}*/

auto loc_dict_s::to_json_nice(json_nice_s *a_json_nice,array<char> *a_trg) const noexcept -> ccl::array<char> &
{/*{{{*/
  if (m_value.count() > 0)
  {
    (*a_trg)
      .push('{')
      .json(a_json_nice->push());

    const auto *ptr = m_value.data();
    const auto *ptr_end = ptr + m_value.used();
    bool comma = false;
    do
    {
      if (ptr->is_valid())
      {
        const auto &object = ptr->object();

        if (comma)
        {
          (*a_trg)
            .push(',')
            .json(*a_json_nice);
        }
        else
        {
          comma = true;
        }

        (*a_trg)
          .json_nice(object.key(),a_json_nice)
          .push(':')
          .push(' ')
          .json_nice(object.value(),a_json_nice);
      }
    } while(++ptr < ptr_end);

    (*a_trg)
      .json(a_json_nice->pop())
      .push('}');
  }
  else
  {
    (*a_trg)
      .push('{')
      .push('}');
  }

  return *a_trg;
}/*}}}*/

} // namespace ccl

