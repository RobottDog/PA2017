#include "cpu/instr.h"

make_instr_func(ret_v)
{
  OPERAND rel;

  rel.type = OPR_MEM;
  rel.addr = cpu.esp;
  rel.data_size = data_size;
  operand_read(&rel);
  cpu.esp += data_size / 8;
  if(data_size == 16){
    cpu.eip = rel.val & 0x0000ffff;
  } else {
    cpu.eip = rel.val;
  }
  
  return 0;
}