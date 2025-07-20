#include <conio.h>
#include <stdio.h>
#include <has/system/pic.h>
#include <has/system/pit.h>
#include <has/testing/log.h>
#include <support/allocatr.h>
#include <ham/file/mod/mod.h>
#include <ham/player/player.h>
#include <has/system/interrup.h>
#include <ham/drivers/gravis/shared/system.h>
#include <ham/drivers/gravis/shared/gf1/voice/actvvoc.h>

Ham::File::Mod* s_Mod = nullptr;
Ham::Player::Player* s_Player = nullptr;

void Handler()
{
    s_Player->Tick();
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

    s_Mod = Mod::Load(allocator, argv[1]);

    if (s_Mod == nullptr)
    {
        printf("Error loading mod file.\n");
        return -1;
    }

    auto result = Function::System::Initialize(allocator);

    if (result != Function::System::InitializeError::Success)
    {
        printf("Error: %s\n", Function::System::InitializeError::ToString(result));
        Mod::Destroy(s_Mod);
        s_Mod = nullptr;
        return -1;
    }

    s_Player = ::new(allocator.Allocate(sizeof(Ham::Player::Player))) Ham::Player::Player(s_Mod, [](uint8_t RefreshRateInHz)
    {
        SYS_ClearInterrupts();
        uint16_t divisor = PIT::CalculateDivisor(RefreshRateInHz);
        PIT::Data::WriteChannel0(uint8_t(divisor));
        PIT::Data::WriteChannel0(uint8_t(divisor >> 8));
        SYS_RestoreInterrupts();
    });

    //Function::System::SetTimer1Handler(Handler, 50);

    //TODO: would very much prefer to use the GUS timers, but I can't seem to get those interrupts to fire. :(
    InterruptTable::ChainHandler(8, Handler);
    SYS_ClearInterrupts();
    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel0);
    uint16_t divisor = PIT::CalculateDivisor(50);
    PIT::Data::WriteChannel0(uint8_t(divisor));
    PIT::Data::WriteChannel0(uint8_t(divisor >> 8));
    SYS_RestoreInterrupts();

    static const char* effect = "_123456789ABCDEF";
    static const char* hex = "0123456789ABCDEF";

    s_Player->Play();

    volatile uint8_t row = -1;
    do
    {
        if (row != s_Player->GetCurrentRow())
        {
            uint8_t currentOrder = s_Mod->GetOrder(s_Player->GetCurrentOrderIndex());
            Mod::Note* currentPattern = s_Mod->GetPattern(currentOrder);
            Mod::Note* currentRow = currentPattern + s_Player->GetCurrentRow() * s_Mod->GetChannelCount();
            const char* note0 = currentRow[0].Note != 0xFFFF ? s_Mod->GetNoteName(currentRow[0].Note) : "___";
            const char* note1 = currentRow[1].Note != 0xFFFF ? s_Mod->GetNoteName(currentRow[1].Note) : "___";
            const char* note2 = currentRow[2].Note != 0xFFFF ? s_Mod->GetNoteName(currentRow[2].Note) : "___";
            const char* note3 = currentRow[3].Note != 0xFFFF ? s_Mod->GetNoteName(currentRow[3].Note) : "___";
            char effect0 = (currentRow[0].Parameter == 0) ? effect[currentRow[0].Effect] : hex[currentRow[0].Effect];
            char effect1 = (currentRow[1].Parameter == 0) ? effect[currentRow[1].Effect] : hex[currentRow[1].Effect];
            char effect2 = (currentRow[2].Parameter == 0) ? effect[currentRow[2].Effect] : hex[currentRow[2].Effect];
            char effect3 = (currentRow[3].Parameter == 0) ? effect[currentRow[3].Effect] : hex[currentRow[3].Effect];

            char param0[3] = { '_', '_', '\0' };
            char param1[3] = { '_', '_', '\0' };
            char param2[3] = { '_', '_', '\0' };
            char param3[3] = { '_', '_', '\0' };

            if ((currentRow[0].Parameter != 0) || (currentRow[0].Effect != 0))
            {
                param0[0] = hex[currentRow[0].Parameter >> 4];
                param0[1] = hex[currentRow[0].Parameter & 0x0f];
            }
            if ((currentRow[1].Parameter != 0) || (currentRow[1].Effect != 0))
            {
                param1[0] = hex[currentRow[1].Parameter >> 4];
                param1[1] = hex[currentRow[1].Parameter & 0x0f];
            }
            if ((currentRow[2].Parameter != 0) || (currentRow[2].Effect != 0))
            {
                param2[0] = hex[currentRow[2].Parameter >> 4];
                param2[1] = hex[currentRow[2].Parameter & 0x0f];
            }
            if ((currentRow[3].Parameter != 0) || (currentRow[3].Effect != 0))
            {
                param3[0] = hex[currentRow[3].Parameter >> 4];
                param3[1] = hex[currentRow[3].Parameter & 0x0f];
            }

            printf("[%3i/%2i] | %s %c%s | %s %c%s | %s %c%s | %s %c%s |\r", 
                s_Player->GetCurrentOrderIndex(), s_Player->GetCurrentRow(), note0, effect0, param0, note1, effect1, param1, note2, effect2, param2, note3, effect3, param3);
            LOG("Play", "[%3i/%2i] | %s %c%s | %s %c%s | %s %c%s | %s %c%s |", s_Player->GetCurrentOrderIndex(), s_Player->GetCurrentRow(), note0, effect0, param0, note1, effect1, param1, note2, effect2, param2, note3, effect3, param3);
            row = s_Player->GetCurrentRow();
        }
    } while (!kbhit());

    s_Player->Stop();

    Function::System::Shutdown();

    InterruptTable::UnchainHandler(8);

    SYS_ClearInterrupts();
    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel0);
    PIT::Data::WriteChannel0(0);
    PIT::Data::WriteChannel0(0);
    SYS_RestoreInterrupts();

    s_Player->~Player();
    allocator.Free(s_Player);
    Mod::Destroy(s_Mod);

    return 0;
}
