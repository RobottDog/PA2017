#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src){
  cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, uint32_t dest){
  if(cpu.eflags.CF && result <= src)
    cpu.eflags.CF = 1;
  else
    set_CF_add(result, src);
}

void set_PF(uint32_t result){
  int zero_num = 0;
  for(int i=0;i<8;i++){
    if(result%2 == 1)
      zero_num++;
    result >>= 1;
  }
  cpu.eflags.PF = zero_num%2 == 0;
}

void set_ZF(uint32_t result){
  cpu.eflags.ZF = result == 0;
}

void set_SF(uint32_t result){
  cpu.eflags.SF = sign(result);
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest){
  if(sign(src) == sign(dest)){
    cpu.eflags.OF = sign(result)!=sign(src);
  } else {
    cpu.eflags.OF = 0;
  }
}

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t result = src + dest;
  set_CF_add(result, src);
  set_PF(result);
  set_ZF(result);
  set_SF(result);
  set_OF_add(result, src, dest);
	return result;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
  uint32_t result = src + dest + cpu.eflags.CF;
  set_CF_adc(result, src, dest);
  set_PF(result);
  set_ZF(result);
  set_SF(result);
  set_OF_add(result, src, dest);
	return result;
}

void set_CF_sub(uint32_t src, uint32_t dest){
  cpu.eflags.CF = dest < src;
}

void set_OF_sub(uint32_t src, uint32_t dest){
  uint32_t src_neg = ~src + 1;
  set_OF_add(src_neg + dest, src_neg, dest);
}

uint32_t alu_sub(uint32_t src, uint32_t dest) {
	uint32_t result = dest - src;
  set_CF_sub(src, dest);
  set_PF(result);
  set_ZF(result);
  set_SF(result);
  set_OF_sub(src, dest);
	return result;
}

void set_CF_sbb(uint32_t src, uint32_t dest){
  if(!cpu.eflags.CF){
    set_CF_sub(src, dest);
  } else {
    cpu.eflags.CF = src >= dest;
  }
}

void set_OF_sbb(uint32_t src, uint32_t dest){
  if(!cpu.eflags.CF){
    set_OF_sub(src, dest);
  } else {
    uint32_t src_neg = ~(src+1) + 1;
    set_OF_add(src_neg + dest, src_neg, dest);
  }
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
  uint32_t result = dest - (src + cpu.eflags.CF);
  set_PF(result);
  set_ZF(result);
  set_SF(result);
  set_OF_sbb(src, dest);
  set_CF_sbb(src, dest);
	return result;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t result = (uint64_t)src * (uint64_t)dest;
  bool res=0;
  uint8_t high_8_bits;
  uint16_t high_16_bits;
  uint32_t high_32_bits;
  switch(data_size){
    case 8:
      high_8_bits = (result & 0xff00) >> 8;
      res = high_8_bits ? 1:0;
      break;
    case 16:
      high_16_bits = (result & 0xffff0000) >> 16;
      res = high_16_bits ? 1:0;
      break;
    case 32:
      high_32_bits = (result & 0xffffffff00000000) >> 32;
      res = high_32_bits ? 1:0;
      break;
    default:
      break;
  }
  cpu.eflags.OF = res ? 1:0;
  cpu.eflags.CF = res ? 1:0;
	return result;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
  uint32_t result = src & dest;
  set_PF(result);
  set_SF(result);
  set_ZF(result);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;
	return result;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
  uint32_t result = src ^ dest;
  set_PF(result);
  set_SF(result);
  set_ZF(result);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;
	return result;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
  uint32_t result = src | dest;
  set_PF(result);
  set_SF(result);
  set_ZF(result);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;
	return result;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t result = dest;
  if(src<0) return result;
  switch(data_size){
    case 8:
    result = dest & 0xff << 24;
    dest = (dest & 0xffffff00) | (dest & 0xff << src);
    break;
    case 16: 
    result = dest & 0xffff << 16;
    dest = (dest & 0xffff0000) | (dest & 0xffff << src);
    break;
    default:
    dest = dest << src;
    break;
  }
  if(src == 0){
    set_PF(result);
    set_SF(result);
    set_ZF(result);
  } else {
    if(src==1){
      cpu.eflags.OF = ((result & 0x80000000) >> 31) == ((result & 0x40000000) >> 30) ? 0:1;
    }
    cpu.eflags.CF = (result << (src-1)) & 0x80000000;
    set_PF(result);
    set_SF(result);
    set_ZF(result);
  }
	return dest;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	return alu_shl(src, dest, data_size);
	return 0;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

