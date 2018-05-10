#include "cpu/instr.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val = sign_ext(opr_src.val, data_size);
	opr_dest.val = alu_sub(opr_src.val, opr_dest.val);
	print_asm_2("cmp", "", 2, &opr_src, &opr_dest);
}

make_instr_impl_2op(cmp, r, rm, v);
make_instr_impl_2op(cmp, i, rm, v);
make_instr_impl_2op(cmp, rm, r, v);
make_instr_impl_2op(cmp, rm, r, b);

make_instr_func(cmps_v) {
	OPERAND msi, mdi;
	msi.data_size = mdi.data_size = data_size;
	msi.type = mdi.type = OPR_MEM;
	msi.sreg = mdi.sreg = SREG_DS;
	msi.addr = cpu.gpr[REG_ESI]._32;
	mdi.addr = cpu.gpr[REG_EDI]._32;
	operand_read(&msi);
	operand_read(&mdi);
	alu_sub(mdi.val, msi.val);
	int incDec = 0;
	incDec = cpu.eflags.DF == 0 ? data_size / 8 : -data_size / 8;
	cpu.gpr[REG_ESI]._32 += incDec;
	cpu.gpr[REG_EDI]._32 += incDec;

	print_asm_0("cmps (%%edi), (%%esi)", "", 1);
	return 1;
}

make_instr_func(cmps_b) {
        OPERAND msi, mdi;
        msi.data_size = mdi.data_size = 8;
        msi.type = mdi.type = OPR_MEM;
	msi.sreg = mdi.sreg = SREG_DS;
        msi.addr = cpu.gpr[REG_ESI]._32;
        mdi.addr = cpu.gpr[REG_EDI]._32;
        operand_read(&msi);
        operand_read(&mdi);
	alu_sub(mdi.val, msi.val);
        int incDec = 0;
        incDec = cpu.eflags.DF == 0 ? 1 : -1;
        cpu.gpr[REG_ESI]._32 += incDec;
        cpu.gpr[REG_EDI]._32 += incDec;
	print_asm_0("cmps (%%edi), (%%esi)", "", 1);
	return 1;
}

make_instr_func(cmp_i2rm_bv)
{
	OPERAND imm, rm;

	int len = 1;
	rm.data_size = data_size;
	len += modrm_opcode_rm(eip + 1, &opcode, &rm);
	operand_read(&rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.data_size = 8;
	len += 1;
	operand_read(&imm);

	print_asm_2("cmp", "", len, &imm, &rm);
	imm.data_size = 32;
	imm.val = sign_ext(imm.val, 8); // sign extension
	rm.val = alu_sub(imm.val, rm.val);
	// operand_write(&rm);;
	return len;
}