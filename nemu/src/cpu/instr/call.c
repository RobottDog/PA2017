#include "cpu/instr.h"

make_instr_func(call_rel_v)
{
  OPERAND temp_eip, rel;
  int data_byte = data_size / 8;
  int len = 1 + data_byte;
  cpu.esp -= data_byte;

  temp_eip.type = OPR_MEM;
  temp_eip.data_size = data_size;
  temp_eip.val = eip+len;
  temp_eip.addr = cpu.esp;
  temp_eip.sreg = SREG_SS;
  operand_write(&temp_eip);

  rel.type = OPR_IMM;
  rel.data_size = data_size;
  rel.addr = eip + 1;
  operand_read(&rel);
  print_asm_1("call", "", len, &rel);

  if (data_size == 16){
    rel.val = (int32_t)(rel.val << 16) >> 16;
  } 
  len += (int32_t)rel.val;

  return len;
}

make_instr_func(call_rm_v)
{
  OPERAND temp_eip, rm;
  int data_byte = data_size / 8;
  int len = 1;
  cpu.esp -= data_byte;

  rm.data_size = data_size;
  len += modrm_opcode_rm(eip + len, &opcode, &rm);
  operand_read(&rm);
  
  temp_eip.type = OPR_MEM;
  temp_eip.data_size = data_size;
  temp_eip.val = eip + len;
  temp_eip.addr = cpu.esp;
  temp_eip.sreg = SREG_SS;
  operand_write(&temp_eip);

  print_asm_1("call", "", len, &rm);

  if (data_size == 16)
    cpu.eip = rm.val & 0xffff;
  else
    cpu.eip = rm.val;

  return 0;
}