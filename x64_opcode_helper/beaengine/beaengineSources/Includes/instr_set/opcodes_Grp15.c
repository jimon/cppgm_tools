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
 *      0faeh
 * ==================================================================== */
void __bea_callspec__ G15_(PDISASM pMyDisasm)
{
    GV.DECALAGE_EIP = 1;
    if (!Security(1, pMyDisasm)) return;
    GV.REGOPCODE = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    if (GV.REGOPCODE == 0) {
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_!= 0x3) {
            (*pMyDisasm).Instruction.Category = FPU_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_FXSAVE;
            (*pMyDisasm).Argument1.ArgSize = 512;
        }
        else {
            FailDecode(pMyDisasm);
        }
    }
    else if (GV.REGOPCODE == 1) {
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_!= 0x3) {
            (*pMyDisasm).Instruction.Category = FPU_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_FXRSTOR;
            (*pMyDisasm).Argument1.ArgSize = 512;
            (*pMyDisasm).Argument1.AccessMode = READ;
        }
        else {
            FailDecode(pMyDisasm);
        }

    }
    else if (GV.REGOPCODE == 2) {
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_!= 0x3) {
            (*pMyDisasm).Argument1.ArgSize = 32;
            (*pMyDisasm).Instruction.Category = SSE_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_LDMXCSR;
            (*pMyDisasm).Argument1.AccessMode = READ;
        }
        else {
            FailDecode(pMyDisasm);
        }

    }
    else if (GV.REGOPCODE == 3) {
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_!= 0x3) {
            (*pMyDisasm).Argument1.ArgSize = 32;
            (*pMyDisasm).Instruction.Category = SSE_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_STMXCSR;
        }
        else {
            FailDecode(pMyDisasm);
        }
    }

    else if (GV.REGOPCODE == 4) {
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_!= 0x3) {
            (*pMyDisasm).Instruction.Category = FPU_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_XSAVE;
            (*pMyDisasm).Argument1.ArgSize = 512;
            (*pMyDisasm).Instruction.ImplicitUsedRegs = GENERAL_REG+REG0+REG2;
        }
        else {
            FailDecode(pMyDisasm);
        }
    }

    else if (GV.REGOPCODE == 5) {
        GV.MOD_= ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 6) & 0x3;
        if (GV.MOD_== 0x3) {
            (*pMyDisasm).Instruction.Category = SSE2_INSTRUCTION+CACHEABILITY_CONTROL;
            (*pMyDisasm).Instruction.Mnemonic = I_LFENCE;
        }
        else {
            MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
            (*pMyDisasm).Instruction.Category = FPU_INSTRUCTION+STATE_MANAGEMENT;
            (*pMyDisasm).Instruction.Mnemonic = I_XRSTOR;
            (*pMyDisasm).Argument1.ArgSize = 512;
            (*pMyDisasm).Argument1.AccessMode = READ;
            (*pMyDisasm).Instruction.ImplicitUsedRegs = GENERAL_REG+REG0+REG2;
        }

    }
    else if (GV.REGOPCODE == 6) {
        GV.MOD_= ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 6) & 0x3;
        if (GV.MOD_== 0x3) {
            (*pMyDisasm).Instruction.Category = SSE2_INSTRUCTION+CACHEABILITY_CONTROL;
            (*pMyDisasm).Instruction.Mnemonic = I_MFENCE;
        }
        else {
            FailDecode(pMyDisasm);
        }
    }
    else if (GV.REGOPCODE == 7) {
        GV.MOD_= ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 6) & 0x3;
        if (GV.MOD_== 0x3) {
            (*pMyDisasm).Instruction.Category = SSE2_INSTRUCTION+CACHEABILITY_CONTROL;
            (*pMyDisasm).Instruction.Mnemonic = I_SFENCE;
        }
        else {
            (*pMyDisasm).Instruction.OperandSize = 8;
            MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
            (*pMyDisasm).Argument1.ArgSize = 8;
            (*pMyDisasm).Instruction.Category = SSE2_INSTRUCTION+CACHEABILITY_CONTROL;
            (*pMyDisasm).Instruction.Mnemonic = I_CLFLUSH;
            (*pMyDisasm).Argument1.AccessMode = READ;
        }
    }

    else {
        FailDecode(pMyDisasm);
    }
    GV.EIP_+= GV.DECALAGE_EIP;
}
