#ifndef EQUATION_PROCESSOR
#define EQUATION_PROCESSOR

#ifndef OPER
    #define OPER inline constexpr const //OPERator return type modifier
#endif // OPER

#define PERM static constexpr const

#include "add_impl.hpp"
#include "cmd_list.hpp"
#include "CT_counter.hpp"
#include "divide_impl.hpp"
#include "EIF_filler.hpp"
#include "equals_impl.hpp"
#include "exp_impl.hpp"
#include "innertype_calc.hpp"
#include "mult_impl.hpp"
#include "ntuple.hpp"
#include "OP_numeric.hpp"
#include "op_tree.hpp"
#include "pack.hpp"
#include "restriction_calc.hpp"
#include "self_ref.hpp"
#include "subtract_impl.hpp"
#include "trashcan.hpp"
#include "unique_type.hpp"
#include "var.hpp"
#include "void_t.hpp"

#define known   static constexpr const var<ticket(),1>
#define unknown static constexpr const var<ticket(),0>

#include "equation_sheet.hpp"

#undef known
#undef unknown

#undef PERM
#undef OPER

#endif // EQUATION_PROCESSOR
