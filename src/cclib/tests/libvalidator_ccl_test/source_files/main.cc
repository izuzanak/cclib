
#include "main.h"

// === constants and definitions ===============================================

const char *test_name = "libvalidator_ccl_test";

std::array<const char *,test_cnt> test_names =
{/*{{{*/
  "prop_type",
  "prop_equal",
  "prop_not_equal",
  "prop_lesser",
  "prop_greater",
  "prop_lesser_equal",
  "prop_greater_equal",
  "prop_length_equal",
  "prop_length_not_equal",
  "prop_length_lesser",
  "prop_length_greater",
  "prop_length_lesser_equal",
  "prop_length_greater_equal",
  "prop_reference",
  "prop_regex",
  "prop_items",
  "prop_opt_items",
  "prop_all_items",
  "prop_all_keys",
};/*}}}*/

std::array<ccl::test_function_t,test_cnt> test_functions =
{/*{{{*/
  test_prop_type,
  test_prop_equal,
  test_prop_not_equal,
  test_prop_lesser,
  test_prop_greater,
  test_prop_lesser_equal,
  test_prop_greater_equal,
  test_prop_length_equal,
  test_prop_length_not_equal,
  test_prop_length_lesser,
  test_prop_length_greater,
  test_prop_length_lesser_equal,
  test_prop_length_greater_equal,
  test_prop_reference,
  test_prop_regex,
  test_prop_items,
  test_prop_opt_items,
  test_prop_all_items,
  test_prop_all_keys,
};/*}}}*/

// === test execution functions ================================================

void test_prop_type()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{"type",ccl::bi_type_blank}},
    {"type_int"   ,{"type",ccl::bi_type_int}},
    {"type_float" ,{"type",ccl::bi_type_float}},
    {"type_string",{"type",ccl::bi_type_string}},
    {"type_array" ,{"type",ccl::bi_type_array}},
    {"type_dict"  ,{"type",ccl::bi_type_dict}},
  });

  cassert(validator.validate("type_blank" ,{}));
  cassert(validator.validate("type_int"   ,120));
  cassert(validator.validate("type_float" ,120.123));
  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  // - invalid reference name -
  try {
    (void)validator.validate("invalid",{});
    cassert(false);
  }
  catch (...) {}
}/*}}}*/

void test_prop_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{"==",{}} },
    {"type_int"   ,{"==",120}},
    {"type_float" ,{"==",120.123}},
    {"type_string",{"==","Hello world!"}},
    {"type_array" ,{"==",{1,2,3}} },
    {"type_dict"  ,{"==",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_blank" ,{}));
  cassert(validator.validate("type_int"   ,120));
  cassert(validator.validate("type_float" ,120.123));
  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_blank" ,1));
  cassert(!validator.validate("type_int"   ,1));
  cassert(!validator.validate("type_float" ,1));
  cassert(!validator.validate("type_string",1));
  cassert(!validator.validate("type_array" ,1));
  cassert(!validator.validate("type_dict"  ,1));
}/*}}}*/

void test_prop_not_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{"!=",{}} },
    {"type_int"   ,{"!=",120}},
    {"type_float" ,{"!=",120.123}},
    {"type_string",{"!=","Hello world!"}},
    {"type_array" ,{"!=",{1,2,3}} },
    {"type_dict"  ,{"!=",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_blank" ,1));
  cassert(validator.validate("type_int"   ,1));
  cassert(validator.validate("type_float" ,1));
  cassert(validator.validate("type_string",1));
  cassert(validator.validate("type_array" ,1));
  cassert(validator.validate("type_dict"  ,1));

  cassert(!validator.validate("type_blank" ,{}));
  cassert(!validator.validate("type_int"   ,120));
  cassert(!validator.validate("type_float" ,120.123));
  cassert(!validator.validate("type_string","Hello world!"));
  cassert(!validator.validate("type_array" ,{1,2,3}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));
}/*}}}*/

void test_prop_lesser()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{"<",{}} },
    {"type_int"   ,{"<",120}},
    {"type_float" ,{"<",120.123}},
    {"type_string",{"<","Hello world!"}},
    {"type_array" ,{"<",{1,2,3}} },
    {"type_dict"  ,{"<",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_int"   ,119));
  cassert(validator.validate("type_float" ,119.123));
  cassert(validator.validate("type_string","Hello world"));
  cassert(validator.validate("type_array" ,{1,2,2}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{3,5}}));

  cassert(!validator.validate("type_blank" ,{}));
  cassert(!validator.validate("type_int"   ,120));
  cassert(!validator.validate("type_float" ,120.123));
  cassert(!validator.validate("type_string","Hello world!"));
  cassert(!validator.validate("type_array" ,{1,2,3}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));
}/*}}}*/

void test_prop_greater()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{">",{}} },
    {"type_int"   ,{">",120}},
    {"type_float" ,{">",120.123}},
    {"type_string",{">","Hello world!"}},
    {"type_array" ,{">",{1,2,3}} },
    {"type_dict"  ,{">",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_blank" ,1));
  cassert(validator.validate("type_int"   ,121));
  cassert(validator.validate("type_float" ,121.123));
  cassert(validator.validate("type_string","Hello world!!"));
  cassert(validator.validate("type_array" ,{1,2,4}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{5,5}}));

  cassert(!validator.validate("type_blank" ,{}));
  cassert(!validator.validate("type_int"   ,120));
  cassert(!validator.validate("type_float" ,120.123));
  cassert(!validator.validate("type_string","Hello world!"));
  cassert(!validator.validate("type_array" ,{1,2,3}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));
}/*}}}*/

void test_prop_lesser_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{"<=",{}} },
    {"type_int"   ,{"<=",120}},
    {"type_float" ,{"<=",120.123}},
    {"type_string",{"<=","Hello world!"}},
    {"type_array" ,{"<=",{1,2,3}} },
    {"type_dict"  ,{"<=",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_blank" ,{}));
  cassert(validator.validate("type_int"   ,120));
  cassert(validator.validate("type_float" ,120.123));
  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_blank" ,1));
  cassert(!validator.validate("type_int"   ,121));
  cassert(!validator.validate("type_float" ,121.123));
  cassert(!validator.validate("type_string","Hello world!!"));
  cassert(!validator.validate("type_array" ,{1,2,4}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{5,5}}));
}/*}}}*/

void test_prop_greater_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_blank" ,{">=",{}} },
    {"type_int"   ,{">=",120}},
    {"type_float" ,{">=",120.123}},
    {"type_string",{">=","Hello world!"}},
    {"type_array" ,{">=",{1,2,3}} },
    {"type_dict"  ,{">=",{ccl::map_s{1,2},{2,3},{4,5}} }},
  });

  cassert(validator.validate("type_blank" ,{}));
  cassert(validator.validate("type_int"   ,120));
  cassert(validator.validate("type_float" ,120.123));
  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_int"   ,119));
  cassert(!validator.validate("type_float" ,119.123));
  cassert(!validator.validate("type_string","Hello world"));
  cassert(!validator.validate("type_array" ,{1,2,2}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{3,5}}));
}/*}}}*/

void test_prop_length_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length ==",12}},
    {"type_array" ,{"length ==",3}},
    {"type_dict"  ,{"length ==",3}},
  });

  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_string","Hello world!!"));
  cassert(!validator.validate("type_array" ,{1,2,3,4}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5},{6,7}}));
}/*}}}*/

void test_prop_length_not_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length !=",12}},
    {"type_array" ,{"length !=",3}},
    {"type_dict"  ,{"length !=",3}},
  });

  cassert(validator.validate("type_string","Hello world!!"));
  cassert(validator.validate("type_array" ,{1,2,3,4}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5},{6,7}}));

  cassert(!validator.validate("type_string","Hello world!"));
  cassert(!validator.validate("type_array" ,{1,2,3}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));
}/*}}}*/

void test_prop_length_lesser()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length <",13}},
    {"type_array" ,{"length <",4}},
    {"type_dict"  ,{"length <",4}},
  });

  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_string","Hello world!!"));
  cassert(!validator.validate("type_array" ,{1,2,3,4}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5},{6,7}}));
}/*}}}*/

void test_prop_length_greater()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length >",11}},
    {"type_array" ,{"length >",2}},
    {"type_dict"  ,{"length >",2}},
  });

  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_string","Hello world"));
  cassert(!validator.validate("type_array" ,{1,2}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3}}));
}/*}}}*/

void test_prop_length_lesser_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length <=",12}},
    {"type_array" ,{"length <=",3}},
    {"type_dict"  ,{"length <=",3}},
  });

  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_string","Hello world!!"));
  cassert(!validator.validate("type_array" ,{1,2,3,4}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5},{6,7}}));
}/*}}}*/

void test_prop_length_greater_equal()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"type_string",{"length >=",12}},
    {"type_array" ,{"length >=",3}},
    {"type_dict"  ,{"length >=",3}},
  });

  cassert(validator.validate("type_string","Hello world!"));
  cassert(validator.validate("type_array" ,{1,2,3}));
  cassert(validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3},{4,5}}));

  cassert(!validator.validate("type_string","Hello world"));
  cassert(!validator.validate("type_array" ,{1,2}));
  cassert(!validator.validate("type_dict"  ,{ccl::map_s{1,2},{2,3}}));
}/*}}}*/

void test_prop_reference()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"reference",{
      "==",100
    }},
    {"main",{
      "ref","reference",
    }},
  });

  cassert(validator.validate("main",100));
  cassert(!validator.validate("main",101));
}/*}}}*/

void test_prop_regex()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"id_0",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_1",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_2",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_3",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_4",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_5",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"id_6",{"regex","^[a-zA-Z][_a-zA-Z0-9]*$"}},
    {"number",{"regex","^[1-9][0-9]*$"}},
  });

  cassert(validator.validate("id_0","identifier_0"));
  cassert(validator.validate("id_1","identifier_0"));
  cassert(validator.validate("id_2","identifier_0"));
  cassert(validator.validate("id_3","identifier_0"));
  cassert(validator.validate("id_4","identifier_0"));
  cassert(validator.validate("id_5","identifier_0"));
  cassert(validator.validate("id_6","identifier_0"));
  cassert(validator.validate("number","101"));

  cassert(!validator.validate("id_0","0_invalid"));
  cassert(!validator.validate("number","101a"));
}/*}}}*/

void test_prop_items()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"array",{
      "items",{
        0,{"==",1},
        1,{"==",2},
        2,{"==",3},
        3,{"==",4},
      }
    }},
    {"dict",{
      "items",{
        "one"  ,{"==",1},
        "two"  ,{"==",2},
        "three",{"==",3},
        "four" ,{"==",4},
      }
    }},
  });

  cassert(validator.validate("array",{1,2,3,4}));
  cassert(!validator.validate("array",{1,2,3,5}));
  cassert(!validator.validate("array",{1,2,3}));

  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",4},}));
  cassert(!validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",5},}));
  cassert(!validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"five",5},}));

  cassert(!validator.validate("array",120));
  cassert(!validator.validate("dict",120));
}/*}}}*/

void test_prop_opt_items()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"array",{
      "opt-items",{
        0,{"==",1},
        1,{"==",2},
        2,{"==",3},
        3,{"==",4},
      }
    }},
    {"dict",{
      "opt-items",{
        "one"  ,{"==",1},
        "two"  ,{"==",2},
        "three",{"==",3},
        "four" ,{"==",4},
      }
    }},
  });

  cassert(validator.validate("array",{1,2,3,4}));
  cassert(!validator.validate("array",{1,2,3,5}));
  cassert(validator.validate("array",{1,2,3}));

  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",4},}));
  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"five",5},}));
  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"three",3},}));
  cassert(!validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",5},}));

  cassert(!validator.validate("array",120));
  cassert(!validator.validate("dict",120));
}/*}}}*/

void test_prop_all_items()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"array",{
      "all-items",{
        "type",ccl::bi_type_int
      }
    }},
    {"dict",{
      "all-items",{
        "type",ccl::bi_type_int
      }
    }},
  });

  cassert(validator.validate("array",{1,2,3,4}));
  cassert(!validator.validate("array",{1,2,3,"Hello"}));

  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",4},}));
  cassert(!validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four","four"},}));
}/*}}}*/

void test_prop_all_keys()
{/*{{{*/
  ccl::validator_s validator({ccl::map_s
    {"dict",{
      "all-keys",{
        "type",ccl::bi_type_string
      }
    }},
  });

  cassert(validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{"four",4},}));
  cassert(!validator.validate("dict",{ccl::map_s{"one",1},{"two",2},{"three",3},{4,4},}));
}/*}}}*/

// === program entry function ==================================================

auto main(int argc,char **argv) -> int
{/*{{{*/
  (void)argc;
  (void)argv;

  ccl::execute_tests(argv,test_name,test_names.data(),test_functions.data(),test_names.size());

  return 0;
}/*}}}*/

