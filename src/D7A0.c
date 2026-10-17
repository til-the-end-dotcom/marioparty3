#include "common.h"

extern s16 func_8000CC98_D898(s32 arg0, s32 arg1);
extern void func_80013390_13F90(
    s16 arg0, s32 arg1, s32 arg2, void (*arg3)(void));
extern void func_8000D268_DE68(void);

s16 func_8000CBA0_D7A0(s32 arg0, s32 arg1) {
    s16 modelId;
    s16 i;
    f32 zero;
    f32 negTen;
    u8 *modelSetup;
    u8 *hmfData;
    u8 *entry;

    modelId = func_8000CC98_D898(arg0, arg1 & ~0x60);
    func_80013390_13F90(modelId, -1, 0, func_8000D268_DE68);

    hmfData = (u8 *)HmfModelData[modelId].hmf;
    modelSetup = (u8 *)HmfModelData[modelId].unkBC;
    entry = *(u8 **)(modelSetup + 0x58);

    *(f32 *)(modelSetup + 0x08) = 5.0f;
    *(f32 *)(modelSetup + 0x0C) = 20.0f;
    i = 0;
    if (*(s16 *)(hmfData + 0x1A) > 0) {
        zero = 0.0f;
        negTen = -10.0f;
        do {
            *(f32 *)(entry + 0x14) = zero;
            *(f32 *)(entry + 0x10) = zero;
            *(f32 *)(entry + 0x0C) = zero;
            *(f32 *)(entry + 0x20) = zero;
            *(f32 *)(entry + 0x18) = zero;
            *(f32 *)(entry + 0x1C) = negTen;
            *(f32 *)(entry + 0x08) = zero;
            *(f32 *)(entry + 0x04) = zero;
            i++;
            entry += 0x30;
        } while (i < *(s16 *)(hmfData + 0x1A));
    }

    return modelId;
}

s16 func_8000CC98_D898(s32 arg0, s32 arg1) {
    s16 modelId;

    modelId = func_8000B108_BD08(arg0, arg1);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    func_8001C2FC_1CEFC(modelId, 0x20000, 0x20000);
    func_8001C448_1D048(modelId);
    func_8001C954_1D554(modelId);
    func_8001C514_1D114(modelId);

    return modelId;
}

void func_8000CD00_D900(s16 modelId, f32 x, f32 y, f32 z) {
    s16 i;
    u8 *hmfData;
    u8 *modelSetup;
    u8 *entry;

    hmfData = (u8 *)HmfModelData[modelId].hmf;
    modelSetup = (u8 *)HmfModelData[modelId].unkBC;
    entry = *(u8 **)(modelSetup + 0x58);

    *(f32 *)(modelSetup + 0x08) = x;
    *(f32 *)(modelSetup + 0x0C) = y;

    i = 0;
    if (*(s16 *)(hmfData + 0x1A) > 0) {
        do {
            *(f32 *)(entry + 0x1C) = -z;
            i++;
            entry += 0x30;
        } while (i < *(s16 *)(hmfData + 0x1A));
    }
}

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000CD74_D974);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000CED8_DAD8);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000CFA4_DBA4);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000CFD8_DBD8);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D018_DC18);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D044_DC44);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D06C_DC6C);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D268_DE68);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D3AC_DFAC);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D3E0_DFE0);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D550_E150);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D658_E258);

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D694_E294);
