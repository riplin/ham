#include <conio.h>
#include <stdio.h>

#include <has/system/pic.h>
#include <has/testing/log.h>
#include <support/allocatr.h>
#include <ham/file/mod/mod.h>
#include <has/system/interrup.h>
#include <ham/drivers/gravis/shared/system.h>

void Handler()
{
    
}

volatile uint32_t s_Counter5 = 0;
void TestHandler5()
{
    Has::System::PIC::Control::ClearPendingInterrupt(5);
    s_Counter5 = s_Counter5 + 1;
}

volatile uint32_t s_Counter7 = 0;
void TestHandler7()
{
    Has::System::PIC::Control::ClearPendingInterrupt(7);
    s_Counter7 = s_Counter7 + 1;
}

volatile uint32_t s_Counter8 = 0;   //Control
void TestHandler8()
{
    Has::System::PIC::Control::ClearPendingInterrupt(8);
    s_Counter8 = s_Counter8 + 1;
}

volatile uint32_t s_Counter11 = 0;
void TestHandler11()
{
    Has::System::PIC::Control::ClearPendingInterrupt(11);
    s_Counter11 = s_Counter11 + 1;
}

volatile uint32_t s_Counter12 = 0;
void TestHandler12()
{
    Has::System::PIC::Control::ClearPendingInterrupt(12);
    s_Counter12 = s_Counter12 + 1;
}

volatile uint32_t s_Counter15 = 0;
void TestHandler15()
{
    Has::System::PIC::Control::ClearPendingInterrupt(15);
    s_Counter15 = s_Counter15 + 1;
}

int main(int argc, const char** argv)
{
    using namespace Has::System;
    using namespace Ham::Gravis::Shared;
    using namespace Ham::File;

    LOG_CONFIGURE("test.txt");

    Support::Allocator allocator;

    if (argc != 2)
    {
        printf("test <modfile.mod>\n");
        return -1;
    }

    Mod* mod = Mod::Load(allocator, argv[1]);

    if (mod == nullptr)
    {
        printf("Error loading mod file.\n");
        return -1;
    }

    // InterruptTable::SetupHandler(5, TestHandler5);
    // InterruptTable::SetupHandler(7, TestHandler7);
    // InterruptTable::SetupHandler(8, TestHandler8);
    // InterruptTable::SetupHandler(11, TestHandler11);
    // InterruptTable::SetupHandler(12, TestHandler12);
    // InterruptTable::SetupHandler(15, TestHandler15);
    // PIC::Mask::UnmaskInterrupt(5);
    // PIC::Mask::UnmaskInterrupt(7);
    // PIC::Mask::UnmaskInterrupt(8);
    // PIC::Mask::UnmaskInterrupt(11);
    // PIC::Mask::UnmaskInterrupt(12);
    // PIC::Mask::UnmaskInterrupt(15);

    // uint32_t prev5 = -1;
    // uint32_t prev7 = -1;
    // uint32_t prev8 = -1;
    // uint32_t prev11 = -1;
    // uint32_t prev12 = -1;
    // uint32_t prev15 = -1;

    auto result = Function::System::Initialize(allocator);

    if (result != Function::System::InitializeError::Success)
    {
        printf("Error: %s\n", Function::System::InitializeError::ToString(result));
        Mod::Destroy(mod);
        mod = nullptr;
        return -1;
    }

    /*
    printf("Name     : %s\n", mod->GetName());
    printf("Channels : %i\n", mod->GetChannelCount());

    for (uint8_t i = 0; i < mod->GetSampleCount(); ++i)
    {
        printf("Sample Name %2i: %s\n", i, mod->GetSampleName(i));
        printf("Length        : %i\n", mod->GetSampleLength(i));
        printf("Fine tune     : %i\n", mod->GetSampleFineTune(i));
        printf("Volume        : %i\n", mod->GetSampleVolume(i));
        printf("Loop start    : %i\n", mod->GetSampleLoopStart(i));
        printf("Loop end      : %i\n", mod->GetSampleLoopEnd(i));
    }

    printf("Orders   : %i\n", mod->GetOrderCount());

    for (uint8_t i = 0; i < mod->GetOrderCount(); ++i)
    {
        printf("Order %2i: %i\n", i, mod->GetOrder(i));
    }

    printf("Patterns: %i\n", mod->GetPatternCount());

    for (uint8_t i = 0; i < mod->GetPatternCount(); ++i)
    {
        Mod::Note* pattern = mod->GetPattern(i);
        printf("Pattern %i:\n", i);

        for (uint8_t row = 0; row < 64; ++row)
        {
            Mod::Note* curRow = pattern + (row * mod->GetChannelCount());
            for (uint8_t channel = 0; channel < mod->GetChannelCount(); ++channel)
            {
                Mod::Note* curNote = curRow + channel;
                const char* note = "   ";
                char sample[3];
                if (curNote->Sample != 0)
                    sprintf(sample, "%02X", curNote->Sample);
                else
                    sprintf(sample, "  ");

                char parameter[3];
                if (curNote->Parameter != 0)
                    sprintf(parameter, "%02X", curNote->Parameter);
                else
                    sprintf(parameter, "  ");

                if (curNote->Note != 0xffff)
                    note = mod->GetNoteName(curNote->Note);

                char effect[2];
                
                if (curNote->Effect != 0)
                    sprintf(effect, "%1X", curNote->Effect);
                else
                    sprintf(effect, " ");
                printf("| %s %s %s %s ", sample, note, effect, parameter);
            }
            printf("|\n");
        }
    }
    */

    //TODO: proper memory management
    GF1::Global::DramIOAddress_t address = 16;
    for (uint8_t i = 0; i < mod->GetSampleCount(); ++i)
    {
        if (mod->GetSampleLength(i) == 0)
            continue;

        mod->SetSampleAddress(i, address);
        Function::System::UploadSound(address, mod->GetSampleData(i), mod->GetSampleLength(i));

        address += mod->GetSampleLength(i);
        printf("Sample %i: length: %i, loop start: %i, loop end: %i\n", i, mod->GetSampleLength(i), mod->GetSampleLoopStart(i), mod->GetSampleLoopEnd(i));
    }

    GF1::Page_t voice = 0;
    for (uint8_t i = 0; i < mod->GetSampleCount(); ++i)
    {
        if (mod->GetSampleLength(i) == 0)
            continue;

        Function::System::SetLinearVolume(voice, 511);

        GF1::Global::DramIOAddress_t loopEnd = mod->GetSampleAddress(i) + mod->GetSampleLength(i);
        if (mod->GetSampleLoopEnd(i) != 0)
            loopEnd = mod->GetSampleAddress(i) + mod->GetSampleLoopEnd(i);
        GF1::Global::DramIOAddress_t loopStart = mod->GetSampleAddress(i) + mod->GetSampleLoopStart(i);

        printf("Play: %i, start: 0x%06lX, loopStart: 0x%06lX, loopEnd: 0x%06lX\n", i, mod->GetSampleAddress(i), loopStart, loopEnd - 1);

        Function::System::PlayVoice(voice, mod->GetSampleAddress(i), loopStart, loopEnd,
            GF1::Voice::VoiceControl::Bits8 |
            GF1::Voice::VoiceControl::Forward |
            GF1::Voice::VoiceControl::LoopToBegin | //No sound if this isn't set. Also, only plays once. Go figure.
            GF1::Voice::VoiceControl::InterruptEnable | //Still no interrupts...
            GF1::Voice::VoiceControl::Play);

        ++voice;
        break;
    }

    Function::System::SetTimer1Handler(Handler, 50);

    printf("\nTest that timers are running:\n");
    do
    {
        // if ((s_Counter5 != prev5) ||
        //     (s_Counter7 != prev7) ||
        //     (s_Counter8 != prev8) ||
        //     (s_Counter11 != prev11) ||
        //     (s_Counter12 != prev12) ||
        //     (s_Counter15 != prev15))
        // {
        //     prev5 = s_Counter5;
        //     prev7 = s_Counter7;
        //     prev8 = s_Counter8;
        //     prev11 = s_Counter11;
        //     prev12 = s_Counter12;
        //     prev15 = s_Counter15;
        //     printf("C5: %li, C7: %li, C8: %li, C11: %li, C12: %li, C15: %li\r", prev5, prev7, prev8, prev11, prev12, prev15);
        // }
    } while (!kbhit());

    Mod::Destroy(mod);
    mod = nullptr;

    Function::System::Shutdown();

    // PIC::Mask::MaskInterrupt(5);
    // PIC::Mask::MaskInterrupt(7);
    // PIC::Mask::MaskInterrupt(11);
    // PIC::Mask::MaskInterrupt(12);
    // PIC::Mask::MaskInterrupt(15);
    // InterruptTable::RestoreHandler(5);
    // InterruptTable::RestoreHandler(7);
    // InterruptTable::RestoreHandler(8);
    // InterruptTable::RestoreHandler(11);
    // InterruptTable::RestoreHandler(12);
    // InterruptTable::RestoreHandler(15);

    return 0;
}
