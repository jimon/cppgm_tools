/* Copyright 2006-2009, BeatriX
 * File coded by BeatriX
 *
 * This file is part of BeaEngine.
 *
 *    BeaEngine is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    BeaEngine is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with BeaEngine.  If not, see <http://www.gnu.org/licenses/>. */

/* ====================================================================
 *      0c0h
 * ==================================================================== */
void __bea_callspec__ G2_EbIb(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    EbIb(pMyDisasm);
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
    }
}


/* ====================================================================
 *      0c1h
 * ==================================================================== */
void __bea_callspec__ G2_EvIb(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    EvIb(pMyDisasm);
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
}

/* ====================================================================
 *      0d0h
 * ==================================================================== */
void __bea_callspec__ G2_Eb1(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    Eb(pMyDisasm);
    (*pMyDisasm).Argument2.ArgType = CONSTANT_TYPE+ABSOLUTE_;
    (*pMyDisasm).Argument2.ArgSize = 8;
    (*pMyDisasm).Instruction.Immediat = 1;
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT1);
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT1);
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC1);
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC1);
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
}


/* ====================================================================
 *      0d1h
 * ==================================================================== */
void __bea_callspec__ G2_Ev1(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    Ev(pMyDisasm);
    (*pMyDisasm).Argument2.ArgType = CONSTANT_TYPE+ABSOLUTE_;
    (*pMyDisasm).Argument2.ArgSize = 8;
    (*pMyDisasm).Instruction.Immediat = 1;
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT1);
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT1);
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC1);
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC1);
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT1);
    }
}


/* ====================================================================
 *      0d2h
 * ==================================================================== */
void __bea_callspec__ G2_EbCL(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    Eb(pMyDisasm);
    (*pMyDisasm).Argument2.ArgType = REGISTER_TYPE+GENERAL_REG+REG1;
    (*pMyDisasm).Argument2.ArgSize = 8;
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
}


/* ====================================================================
 *      0d3h
 * ==================================================================== */
void __bea_callspec__ G2_EvCL(PDISASM pMyDisasm)
{
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    Ev(pMyDisasm);
    (*pMyDisasm).Argument2.ArgType = REGISTER_TYPE+GENERAL_REG+REG1;
    (*pMyDisasm).Argument2.ArgSize = 8;
    if (GV.REGOPCODE == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROL;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_ROR;
        FillFlags(pMyDisasm, EFLAGS_ROT);
    }
    else if (GV.REGOPCODE == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCL;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_RCR;
        FillFlags(pMyDisasm, EFLAGS_ROTC);
    }
    else if (GV.REGOPCODE == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SHR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAL;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
    else if (GV.REGOPCODE == 7) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+SHIFT_ROTATE;
        (*pMyDisasm).Instruction.Mnemonic = I_SAR;
        FillFlags(pMyDisasm, EFLAGS_SHIFT);
    }
}
