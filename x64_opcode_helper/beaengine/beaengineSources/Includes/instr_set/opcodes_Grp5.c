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
 *      0ffh
 * ==================================================================== */
void __bea_callspec__ G5_Ev(PDISASM pMyDisasm)
{
    UInt32 regopcode = ((*((UInt8*)(UIntPtr) (GV.EIP_))) >> 3) & 0x7;
    if (regopcode == 0) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+ARITHMETIC_INSTRUCTION;
        (*pMyDisasm).Instruction.Mnemonic = I_INC;
        Ev(pMyDisasm);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
        FillFlags(pMyDisasm, EFLAGS_INC);
        if ((*pMyDisasm).Prefix.LockState == InvalidPrefix && (*pMyDisasm).Argument1.ArgType & MEMORY_TYPE) {
            (*pMyDisasm).Prefix.LockState = InUsePrefix;
        }
    }
    else if (regopcode == 1) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+ARITHMETIC_INSTRUCTION;
        (*pMyDisasm).Instruction.Mnemonic = I_DEC;
        Ev(pMyDisasm);
        (*pMyDisasm).Argument1.AccessMode = READ+WRITE;
        FillFlags(pMyDisasm, EFLAGS_DEC);
        if ((*pMyDisasm).Prefix.LockState == InvalidPrefix && (*pMyDisasm).Argument1.ArgType & MEMORY_TYPE) {
            (*pMyDisasm).Prefix.LockState = InUsePrefix;
        }
    }
    else if (regopcode == 2) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+CONTROL_TRANSFER;
        (*pMyDisasm).Instruction.BranchType = CallType;
        (*pMyDisasm).Instruction.Mnemonic = I_CALL;
        if ((*pMyDisasm).Archi == 64) {
            (*pMyDisasm).Instruction.OperandSize = 64;
        }
        Ev(pMyDisasm);
        (*pMyDisasm).Argument1.AccessMode = READ;
        (*pMyDisasm).Instruction.ImplicitModifiedRegs = GENERAL_REG+REG4;
    }
    else if (regopcode == 3) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+CONTROL_TRANSFER;
        (*pMyDisasm).Instruction.BranchType = CallType;
        (*pMyDisasm).Instruction.Mnemonic = I_CALL_FAR;
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_ == 3) {
            FailDecode(pMyDisasm);
        }
        (*pMyDisasm).Argument1.ArgSize = (*pMyDisasm).Instruction.OperandSize + 16;
        (*pMyDisasm).Argument1.ArgType = MEMORY_TYPE+FARPTR_MEM;
        (*pMyDisasm).Argument1.AccessMode = READ;
        GV.EIP_ += GV.DECALAGE_EIP;
        (*pMyDisasm).Instruction.ImplicitModifiedRegs = GENERAL_REG+REG4;
    }
    else if (regopcode == 4) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+CONTROL_TRANSFER;
        (*pMyDisasm).Instruction.BranchType = JmpType;
        (*pMyDisasm).Instruction.Mnemonic = I_JMP;
        if ((*pMyDisasm).Archi == 64) {
            (*pMyDisasm).Instruction.OperandSize = 64;
        }
        Ev(pMyDisasm);
        (*pMyDisasm).Argument1.AccessMode = READ;
    }
    else if (regopcode == 5) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+CONTROL_TRANSFER;
        (*pMyDisasm).Instruction.BranchType = JmpType;
        (*pMyDisasm).Instruction.Mnemonic = I_JMP_FAR;
        MOD_RM(&(*pMyDisasm).Argument1, pMyDisasm);
        if (GV.MOD_ == 3) {
            FailDecode(pMyDisasm);
        }
        (*pMyDisasm).Argument1.ArgSize = (*pMyDisasm).Instruction.OperandSize + 16;
        (*pMyDisasm).Argument1.ArgType = MEMORY_TYPE+FARPTR_MEM;
        (*pMyDisasm).Argument1.AccessMode = READ;
        GV.EIP_ += GV.DECALAGE_EIP;
    }
    else if (regopcode == 6) {
        (*pMyDisasm).Instruction.Category = GENERAL_PURPOSE_INSTRUCTION+DATA_TRANSFER;
        (*pMyDisasm).Instruction.Mnemonic = I_PUSH;
        if ((*pMyDisasm).Archi == 64 && (*pMyDisasm).Instruction.OperandSize != 16) {
            (*pMyDisasm).Instruction.OperandSize = 64;
        }
        Ev(pMyDisasm);
        (*pMyDisasm).Instruction.ImplicitModifiedRegs = GENERAL_REG+REG4;
    }
    else {
        FailDecode(pMyDisasm);
    }
}

