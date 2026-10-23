#include "common.h"
#include "game/hmfman.h"

extern void func_8000D06C_DC6C(omObjData *);

extern s16 func_8000CC98_D898(s32 arg0, s32 arg1);
extern void func_80013390_13F90(
    s16 arg0, s32 arg1, s32 arg2, void (*arg3)(void));
extern void func_8000D268_DE68(s16);

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

s32 func_8000CD74_D974(s16 modelId, f32 x, f32 y, f32 z) {
    s16 i;
    u8 *hmfData;
    u8 *modelSetup;
    u8 *entry;
    f32 base;
    f32 value;
    u8 random;

    hmfData = (u8 *)HmfModelData[modelId].hmf;
    modelSetup = (u8 *)HmfModelData[modelId].unkBC;
    entry = *(u8 **)(modelSetup + 0x58);

    i = 0;
    if (*(s16 *)(hmfData + 0x1A) > 0) {
        do {
            if (*(f32 *)(entry + 0x04) <= 0.0f) {
                *(f32 *)(entry + 0x0C) = x;
                *(f32 *)(entry + 0x10) = y;
                *(f32 *)(entry + 0x14) = z;

                random = rand8();
                base = *(f32 *)(modelSetup + 0x08);

                value = (base * 0.7f) +
                        ((base * 0.3f) *
                         ((f32)((random % 10) + 1) * 0.1f));

                *(f32 *)(entry + 0x08) = value;
                *(f32 *)(entry + 0x04) = value;
                return 1;
            }

            i++;
            entry += 0x30;
        } while (i < *(s16 *)(hmfData + 0x1A));
    }

    return 0;
}

s16 func_8000CED8_DAD8(s32 arg0, s32 arg1) {
    omObjData *obj;
    s16 modelId;
    HmfModelData_UnkBC_Struct *setup;
    f32 zero;

    obj = omAddObj(1000, 0, 0, -1, func_8000D06C_DC6C);

    modelId = func_8000CBA0_D7A0(arg0, arg1);
    obj->work[0] = modelId;

    setup = HmfModelData[modelId].unkBC;

    zero = 0.0f;
    *(f32 *)((u8 *)setup + 0x24) = zero;
    *(f32 *)((u8 *)setup + 0x20) = zero;
    *(f32 *)((u8 *)setup + 0x1C) = zero;

    setup->unk04 |= 1;

    *(f32 *)((u8 *)setup + 0x10) = 100.0f;
    *(f32 *)((u8 *)setup + 0x14) = -1.0f;

    return modelId;
}

void func_8000CFA4_DBA4(s16 modelId, f32 x, f32 y, f32 z) {
    HmfModelData_UnkBC_Struct *setup;

    setup = HmfModelData[modelId].unkBC;

    *(f32 *)((u8 *)setup + 0x1C) = x;
    *(f32 *)((u8 *)setup + 0x20) = y;
    *(f32 *)((u8 *)setup + 0x24) = z;
}

void func_8000CFD8_DBD8(s16 modelId, s16 timer) {
    HmfModelData_UnkBC_Struct *setup;

    setup = HmfModelData[modelId].unkBC;
    *(f32 *)((u8 *)setup + 0x14) = (f32)timer;
}

void func_8000D018_DC18(s16 modelId, f32 value) {
    HmfModelData_UnkBC_Struct *setup;

    setup = HmfModelData[modelId].unkBC;
    *(f32 *)((u8 *)setup + 0x10) = value;
}

extern void func_800138EC_144EC(s16 arg0, s32 arg1, s32 arg2);

void func_8000D044_DC44(s16 modelId) {
    func_800138EC_144EC(modelId, 5, 4);
}

INCLUDE_ASM("asm/nonmatchings/D7A0", func_8000D06C_DC6C);

extern u8 D_80097720_98320[];

void func_8000D268_DE68(s16 modelId) {
    s16 i;
    s16 count;
    u8 *hmfData;
    u8 *modelSetup;
    u8 *entry;
    f32 zero;
    f32 factor;
    f32 one;
    f32 life;

    hmfData = (u8 *)HmfModelData[modelId].hmf;
    modelSetup = (u8 *)HmfModelData[modelId].unkBC;
    entry = *(u8 **)(modelSetup + 0x58);

    i = 0;
    count = *(s16 *)(hmfData + 0x1A);

    if (count > 0) {
        zero = 0.0f;
        factor = 0.15f;
        one = 1.0f;

        do {
            if (zero < *(f32 *)(entry + 0x04)) {
                *(f32 *)(entry + 0x0C) +=
                    *(f32 *)(entry + 0x18);

                *(f32 *)(entry + 0x10) +=
                    *(f32 *)(entry + 0x1C);

                *(f32 *)(entry + 0x14) +=
                    *(f32 *)(entry + 0x20);

                life = *(f32 *)(entry + 0x04) -
                       (*(f32 *)(modelSetup + 0x08) /
                        *(f32 *)(modelSetup + 0x0C));

                *(f32 *)(entry + 0x04) = life;

                if (life <= zero) {
                    *(f32 *)(entry + 0x04) = zero;
                }

                *(f32 *)(entry + 0x08) =
                    ((f32)D_80097720_98320[(*(s16 *)(modelSetup + 0x02) + i) & 3] * factor + one) *
                    *(f32 *)(entry + 0x04);
            }

            i++;
            entry += 0x30;
        } while (i < count);
    }

    (*(u16 *)(modelSetup + 0x02))++;
}

extern void *D_800D6A00_D7600[8];

void func_8000D3AC_DFAC(void) {
    s16 i;
    void **entry;

    entry = D_800D6A00_D7600;
    i = 0;

    do {
        *entry = 0;
        i++;
        entry++;
    } while (i < 8);
}

extern void *HuMemAllocTag(s32 size, s16 tag);

s16 func_8000D3E0_DFE0(
    s32 allocSize,
    s32 modelArg,
    s16 count,
    void (*callback)(void)) {
    s16 slot;
    s16 i;
    s16 modelId;
    void **poolSlot;
    u8 *pool;
    u8 *entries;
    f32 zero;

    poolSlot = D_800D6A00_D7600;

    for (slot = 0; slot < 8; slot++, poolSlot++) {
        if (*poolSlot == 0) {
            break;
        }
    }

    if (slot == 8) {
        return -1;
    }

    pool = HuMemAllocTag(8, 0x7918);
    *poolSlot = pool;

    *(s16 *)(pool + 0x00) = count;

    entries = HuMemAllocTag((s16)count << 4, 0x7918);
    *(u8 **)(pool + 0x04) = entries;

    i = 0;

    if (count > 0) {
        zero = 0.0f;

        do {
            modelId = func_8000CC98_D898(
                allocSize, modelArg | 4);

            *(s16 *)(entries + 0x02) = modelId;

            func_80013390_13F90(
                modelId, -1, 0, callback);

            func_800138EC_144EC(modelId, 6, 6);

            *(f32 *)(entries + 0x0C) = zero;
            *(f32 *)(entries + 0x08) = zero;
            *(f32 *)(entries + 0x04) = zero;
            *(s16 *)(entries + 0x00) = 0;

            i++;
            entries += 0x10;
        } while (i < count);
    }

    return slot;
}

extern s32 func_80013928_14528(s16 modelId);
extern void func_80013954_14554(s16 modelId, s32 value);

s16 func_8000D550_E150(s16 poolIndex, f32 x, f32 y, f32 z) {
    u8 *pool;
    u8 *entry;
    s16 i;
    s16 modelId;

    pool = (u8 *)D_800D6A00_D7600[poolIndex];
    entry = *(u8 **)(pool + 0x04);

    i = 0;
    if (*(s16 *)pool > 0) {
        do {
            modelId = *(s16 *)(entry + 0x02);

            if (func_80013928_14528(modelId) & 4) {
                break;
            }

            i++;
            entry += 0x10;
        } while (i < *(s16 *)pool);
    }

    if (i == *(s16 *)pool) {
        return -1;
    }

    modelId = *(s16 *)(entry + 0x02);

    Hu3DModelPosSet(modelId, x, y, z);
    func_80013954_14554(modelId, 0);
    func_800138EC_144EC(modelId, 6, 0);
    func_8001C258_1CE58(modelId, 4, 0);

    return modelId;
}

extern s16 func_8000D3E0_DFE0(
    s32 arg0, s32 arg1, s16 arg2, void (*arg3)(void));
extern void func_8000D694_E294(s16);

s16 func_8000D658_E258(s32 arg0, s16 arg1) {
    return func_8000D3E0_DFE0(
        0x85, arg0, arg1, func_8000D694_E294);
}

void func_8000D694_E294(s16 modelId) {
    u8 *hmfData;
    u8 *modelSetup;
    u8 *entry;
    u16 count;
    register s32 five __asm__("$22");
    s32 fullTurn;
    s16 frame;
    s16 activeFlag;
    s16 i;
    f32 zero;
    f32 angle;
    f32 size;

    hmfData = (u8 *)HmfModelData[modelId].hmf;
    modelSetup = (u8 *)HmfModelData[modelId].unkBC;
    count = *(u16 *)(hmfData + 0x1A);
    frame = *(s16 *)(modelSetup + 0x02);

    if (frame == 0) {
        *(u16 *)(modelSetup + 0x04) &= ~4;
        entry = *(u8 **)(modelSetup + 0x58);
        i = 0;

        if (frame < (s16)count) {
            zero = 0.0f;
            fullTurn = 360;
            five = 5;

            for (; i < (s16)count; i++, entry += 0x30) {
                *(f32 *)(entry + 0x14) = zero;
                *(f32 *)(entry + 0x10) = zero;
                *(f32 *)(entry + 0x0C) = zero;

                angle = (f32)((fullTurn / (s16)count) * i);

                *(f32 *)(entry + 0x18) = HuMathSin(angle);

                *(f32 *)(entry + 0x1C) =
                    (f32)(five - (rand8() & 0xF));

                *(f32 *)(entry + 0x20) = HuMathCos(angle);
                *(f32 *)(entry + 0x04) = 15.0f;
            }
        }
    }

    activeFlag = *(u16 *)(modelSetup + 0x04) & 4;

    if (activeFlag == 0) {
        entry = *(u8 **)(modelSetup + 0x58);

        frame = *(s16 *)(modelSetup + 0x02);
        size = HuMathCos((f32)frame * 11.25f) * 5.0f;

        i = 0;

        if (activeFlag < (s16)count) {
            for (; i < (s16)count; i++, entry += 0x30) {
                *(f32 *)(entry + 0x0C) +=
                    *(f32 *)(entry + 0x18) *
                    *(f32 *)(entry + 0x04);

                *(f32 *)(entry + 0x10) +=
                    *(f32 *)(entry + 0x1C);

                *(f32 *)(entry + 0x14) +=
                    *(f32 *)(entry + 0x20) *
                    *(f32 *)(entry + 0x04);

                *(f32 *)(entry + 0x08) = size;

                *(f32 *)(entry + 0x1C) -= 2.0f;
                *(f32 *)(entry + 0x04) *= 0.7f;
            }
        }

        frame = *(u16 *)(modelSetup + 0x02) + 1;
        *(u16 *)(modelSetup + 0x02) = frame;

        if (frame >= 8) {
            *(u16 *)(modelSetup + 0x04) |= 4;
            func_8001C258_1CE58(modelId, 4, 4);
        }
    }
}
