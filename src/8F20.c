#include "common.h"
#include "graphics.h"

/* MP3_8F20_TYPED_STRUCTS */
/* Audio DMA nodes and scheduler task layout recovered from MIPS field offsets. */
typedef struct MP3DmaNode {
    ALLink link;
    u32 romAddress;
    u32 lastUsedFrame;
    u32 ramAddress;
} MP3DmaNode;

typedef struct MP3SchedulerMessages {
    OSMesgQueue queue;
    OSMesg messages[8];
} MP3SchedulerMessages;

typedef struct MP3AudioTask {
    OSTask task;
    OSMesgQueue *completionQueue;
    OSMesg completionMessage;
} MP3AudioTask;

typedef struct MP3AudioInitConfig {
    u8 padding00[0x1C];
    u32 outputFrequency;
    s32 maxVoices;
    s32 maxUpdates;
    u32 padding28;
    s32 *fxParams;
    s32 numAiBuffers;
    s32 numDmaBuffers;
    s32 dmaBufferSize;
    s32 bufferLifetime;
    s32 frequencyFactor;
    s32 frequencyDivisor;
    u32 flags;
    u8 padding4C[0x1C];
    void *piHandle;
} MP3AudioInitConfig;

typedef struct MP3AudioCommandEntry {
    void *commandList;
    u8 padding04[0x4C];
    s16 pendingCount;
    u8 padding52[2];
    struct MP3AudioCommandEntry *self;
} MP3AudioCommandEntry;

typedef struct MP3DmaHeads08320 {
    MP3DmaNode *active;
    MP3DmaNode *free;
} MP3DmaHeads08320;


extern OSIoMesg D_800ABAA0_main[];

extern MP3DmaNode *D_800ABA70_main;
extern MP3DmaHeads08320 MP3_dmaHeads08320 __asm__("D_800ABA70_main");
extern MP3DmaNode *D_800ABA74_main;
extern u32 D_800ABA78_main;
extern u32 D_800ABA7C_main;
extern u32 D_800ABA84_main;
extern OSMesgQueue D_800ABDA0_main;
extern MP3SchedulerMessages D_800AB9A0_main;
extern unkSchedStruct D_800A97C0_main;
extern void *D_800AB7D0_main;
extern u8 D_800CC3A0_main[];
extern u8 D_800AB9D8_main[];
extern s32 D_800ABA88_main;
extern u64 n_aspMain_code[];
extern u64 n_aspMain_data[];
extern ALGlobals D_800ABA10_main;
extern u32 D_800ABA8C_main;
extern u32 D_800ABA90_main;
extern u32 D_800ABA94_main;
extern u32 D_800ABA98_main;
extern u32 D_800ABE3C_main;
extern void *D_800ABE38_main;
extern void *D_800AB7D8_main[2];
extern MP3AudioCommandEntry *D_800AB7E0_main[3];
extern u32 D_800ABA80_main;

void *func_800010B8_main(s32 size);
void func_8006B720_main(ALGlobals *globals, ALSynConfig *config);

void *func_80008B94_main(void **arg0);
void RemoveSchedulerClient(unkSchedStruct *client);


s32 func_80008320_main(MP3AudioInitConfig *config, ALHeap *heap) {
    ALSynConfig synthConfig;
    MP3DmaNode *dmaNodes;
    MP3AudioCommandEntry *audioEntry;
    f32 sampleCount;
    s32 i;

    synthConfig.outputRate = osAiSetFrequency(config->outputFrequency);
    D_800ABA98_main = config->flags;

    sampleCount = (f32)config->frequencyFactor * (f32)synthConfig.outputRate /
                  (f32)config->frequencyDivisor;

    D_800ABA94_main = (s32)sampleCount;
    if ((f32)D_800ABA94_main < sampleCount) {
        D_800ABA94_main++;
    }

    D_800ABA94_main = ((D_800ABA94_main / 184) + 1) * 184;
    D_800ABA8C_main = D_800ABA94_main - 184;
    D_800ABA90_main = D_800ABA94_main;
    D_800ABE3C_main = 1;

    if (config->fxParams != NULL) {
        synthConfig.fxType = AL_FX_CUSTOM;
        synthConfig.params = config->fxParams;
    } else {
        synthConfig.fxType = AL_FX_NONE;
        synthConfig.params = NULL;
    }

    synthConfig.maxVVoices = synthConfig.maxPVoices = config->maxVoices;
    synthConfig.maxUpdates = config->maxUpdates;
    synthConfig.dmaproc = (void *)func_80008B94_main;
    synthConfig.heap = heap;
    synthConfig.maxFXbusses = 0;
    func_8006B720_main(&D_800ABA10_main, &synthConfig);

    D_800ABA88_main = 0;
    D_800ABA7C_main = 0;
    D_800ABA78_main = 0;
    D_800ABA80_main = config->dmaBufferSize;
    D_800ABA84_main = config->bufferLifetime;

    dmaNodes = func_800010B8_main(
        config->numDmaBuffers * sizeof(MP3DmaNode)
    );
    if (dmaNodes == NULL) {
        return 1;
    }

    dmaNodes[config->numDmaBuffers - 1].link.next = NULL;
    dmaNodes[0].link.next = NULL;
    dmaNodes[0].link.prev = NULL;

    for (i = 0; i < config->numDmaBuffers - 1; i++) {
        alLink(&dmaNodes[i + 1].link, &dmaNodes[i].link);

        dmaNodes[i].ramAddress =
            (u32)func_800010B8_main(D_800ABA80_main);

        if (dmaNodes[i].ramAddress == 0) {
            return 1;
        }
    }

    dmaNodes[i].ramAddress =
        (u32)func_800010B8_main(D_800ABA80_main);

    if (dmaNodes[i].ramAddress == 0) {
        return 1;
    }

    MP3_dmaHeads08320.active = NULL;
    MP3_dmaHeads08320.free = dmaNodes;
    D_800ABE38_main = config->piHandle;

    for (i = 0; i < 2; i++) {
        D_800AB7D8_main[i] =
            func_800010B8_main(config->numAiBuffers * 8);

        if (D_800AB7D8_main[i] == NULL) {
            return 2;
        }
    }

    for (i = 0; i < 3; i++) {
        D_800AB7E0_main[i] = func_800010B8_main(
            sizeof(MP3AudioCommandEntry)
        );

        if (D_800AB7E0_main[i] == NULL) {
            return 3;
        }

        D_800AB7E0_main[i]->pendingCount = 0;
        D_800AB7E0_main[i]->self = D_800AB7E0_main[i];

        D_800AB7E0_main[i]->commandList =
            func_800010B8_main(D_800ABA90_main * 4);

        if (D_800AB7E0_main[i]->commandList == NULL) {
            return 4;
        }
    }

    return 0;
}


INCLUDE_ASM("asm/nonmatchings/8F20", func_80008678_main);

INCLUDE_ASM("asm/nonmatchings/8F20", func_8000870C_main);

INCLUDE_ASM("asm/nonmatchings/8F20", func_80008750_main);

INCLUDE_ASM("asm/nonmatchings/8F20", func_8000889C_main);

void func_800089D8_main(void) {
}


u32 func_800089E0_main(u32 inputRomAddress, u32 requestSize) {
    register MP3DmaNode *node __asm__("$17");
    register u32 romAddress __asm__("$18") = inputRomAddress;
    MP3DmaNode *previous = NULL;
    MP3DmaNode *active;
    MP3DmaNode **activeHead;
    MP3DmaNode **freeHead;
    OSIoMesg *message;

    u32 requestEnd = romAddress + requestSize;
    register u32 ramAddress __asm__("$19");
    u32 alignment;
    s32 messageIndex;


    node = D_800ABA70_main;

    if (node != NULL) {
        u32 dmaSize = D_800ABA80_main;

        while (node != NULL) {

            u32 cachedStart = node->romAddress;
            s32 segmentEnd = (s32)(cachedStart + dmaSize);

            if (romAddress < cachedStart) {
                break;
            }

            if (segmentEnd < (s32)requestEnd) {
                previous = node;
                node = (MP3DmaNode *)node->link.next;
                continue;
            }

                node->lastUsedFrame = D_800ABA78_main;

                {
                    register u32 address __asm__("$4") =
                        romAddress + node->ramAddress;
                    return osVirtualToPhysical(
                        (void *)(address - node->romAddress));
                }
        }
    }
    freeHead = &D_800ABA74_main;
    __asm__ volatile ("" : "=r"(freeHead) : "0"(freeHead));
    node = *freeHead;

    if (node == NULL) {
        return osVirtualToPhysical(freeHead[-1]);
    }

    D_800ABA74_main = (MP3DmaNode *)node->link.next;

    alUnlink(&node->link);

    if (previous != NULL) {
        alLink(&node->link, &previous->link);
    } else {
        activeHead = &D_800ABA70_main;
        __asm__ volatile ("" : "=r"(activeHead) : "0"(activeHead));
        active = *activeHead;

        if (active != NULL) {
            previous = active;
            *activeHead = node;
            node->link.next = &previous->link;
            node->link.prev = NULL;
            previous->link.prev = &node->link;
        } else {
            D_800ABA70_main = node;
            node->link.next = NULL;
            node->link.prev = NULL;
        }
    }


    ramAddress = node->ramAddress;

    alignment = romAddress & 1;
    romAddress -= alignment;

    node->romAddress = romAddress;
    node->lastUsedFrame = D_800ABA78_main;

    messageIndex = D_800ABA7C_main++;
    message = &D_800ABAA0_main[messageIndex];

    message->hdr.pri = OS_MESG_PRI_NORMAL;
    message->hdr.retQueue = &D_800ABDA0_main;
    message->dramAddr = (void *)ramAddress;
    message->devAddr = romAddress;
    message->size = D_800ABA80_main;

    osEPiStartDma(
        (OSPiHandle *)D_800ABE38_main,
        message,
        OS_READ
    );

    return osVirtualToPhysical((void *)ramAddress)
        + alignment;
}


/* MP3_8B94_TARGET_TEST: candidate based on six original MIPS instructions */
/* Audio DMA callback defined above. */

void *func_80008B94_main(void **arg0) {
    *arg0 = &D_800ABA70_main;
    return (void *)func_800089E0_main;
}


void func_80008BAC_main(void) {
    s32 i;
    OSMesg message;
    MP3DmaNode *node;
    MP3DmaNode *next;
    MP3DmaNode **head;

    for (i = 0; (u32)i < D_800ABA7C_main; i++) {
        osRecvMesg(&D_800ABDA0_main, &message, OS_MESG_NOBLOCK);
    }

    node = D_800ABA70_main;
    if (node != NULL) {
        head = &D_800ABA70_main;
        do {

            next = (MP3DmaNode *)node->link.next;

            if ((u32)(node->lastUsedFrame + D_800ABA84_main)
                < D_800ABA78_main) {

                if (*head == node) {
                    *head = next;
                }

                alUnlink(&node->link);

                if (D_800ABA74_main != NULL) {
                    alLink(&node->link, &D_800ABA74_main->link);
                } else {
                    D_800ABA74_main = node;
                    node->link.next = NULL;
                    node->link.prev = NULL;
                }
            }

            node = next;
        } while (node != NULL);

    }

    D_800ABA7C_main = 0;
    D_800ABA78_main++;
}


void func_80008CC0_main(void *unused, s32 removeClient) {
    if (removeClient != 0) {
        RemoveSchedulerClient(&D_800A97C0_main);
    } else {
        osCreateMesgQueue(&D_800AB9A0_main.queue, D_800AB9A0_main.messages, 8);
        D_800AB7D0_main = D_800CC3A0_main;
        AddSchedulerClient(&D_800A97C0_main, &D_800AB9A0_main.queue, 3);
    }
}


void func_80008D28_main(
    MP3AudioTask *audioTask,
    OSMesg message,
    u32 commandEnd
) {
    u32 *commandStarts;
    u32 *currentStart;
    u32 index;

    audioTask->completionQueue =
        (OSMesgQueue *)D_800AB9D8_main;

    audioTask->completionMessage = message;

    index = D_800ABA88_main;
    commandStarts = (u32 *)(D_800AB9D8_main - 0x200);
    currentStart = &commandStarts[index];

    audioTask->task.t.data_ptr = (u64 *)*currentStart;

    audioTask->task.t.data_size =
        ((s32)(commandEnd - *currentStart) >> 3) << 3;

    audioTask->task.t.type = 2;

    audioTask->task.t.ucode_boot =
        (u64 *)rspbootTextStart;

    audioTask->task.t.ucode_boot_size =
        (u32)gspF3DEX2_fifoTextStart -
        (u32)rspbootTextStart;

    audioTask->task.t.flags = 0;
    audioTask->task.t.ucode = n_aspMain_code;
    audioTask->task.t.ucode_data = n_aspMain_data;
    audioTask->task.t.ucode_data_size = 0x800;

    audioTask->task.t.dram_stack = NULL;
    audioTask->task.t.dram_stack_size = 0;
    audioTask->task.t.output_buff = NULL;
    audioTask->task.t.output_buff_size = NULL;
    audioTask->task.t.yield_data_ptr = NULL;
    audioTask->task.t.yield_data_size = 0;
}
