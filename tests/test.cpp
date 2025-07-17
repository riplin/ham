#include <conio.h>
#include <stdio.h>

#include <support/allocatr.h>
#include <ham/file/mod/mod.h>
#include <ham/drivers/gravis/shared/system.h>

volatile uint32_t s_Counter = 0;

void Handler()
{
    s_Counter = s_Counter + 1;
}

int main(int argc, const char** argv)
{
    using namespace Ham::Gravis::Shared;
    using namespace Ham::File;

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

    uint32_t prev = -1;

    if (!Function::System::SetTimer1Handler(Handler, 50))
    {
        printf("Failed!\n");
    }

    do
    {
        if (s_Counter != prev)
        {
            printf("Counter: %li\r", s_Counter);
            prev = s_Counter;
        }
    } while (!kbhit());

    Mod::Destroy(mod);
    mod = nullptr;

    Function::System::Shutdown();

    return 0;
}
