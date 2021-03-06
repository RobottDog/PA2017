#include "cpu/instr.h"

static void instr_execute_1op(){
  operand_read(&opr_src);
  opr_src.val = alu_sub(1, opr_src.val);
  operand_write(&opr_src);
}

make_instr_impl_1op(dec, rm, v);
make_instr_impl_1op_reg(dec, REG_EAX, v)
make_instr_impl_1op_reg(dec, REG_ECX, v)
make_instr_impl_1op_reg(dec, REG_EDX, v)
make_instr_impl_1op_reg(dec, REG_EBX, v)
make_instr_impl_1op_reg(dec, REG_EBP, v)
make_instr_impl_1op_reg(dec, REG_ESP, v)
make_instr_impl_1op_reg(dec, REG_ESI, v)
make_instr_impl_1op_reg(dec, REG_EDI, v)