//Copyright 2025-Present riplin

#include <conio.h>
#include <stdio.h>
#include <sys/nearptr.h>
#include <has/system/pic.h>
#include <has/system/pit.h>
#include <hag/system/bda.h>
#include <has/testing/log.h>
#include <support/allocatr.h>
#include <ham/file/mod/mod.h>
#include <ham/player/player.h>
#include <has/system/keyboard.h>
#include <has/system/interrup.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/modeset.h>
#include <ham/drivers/gravis/shared/system.h>
#include <ham/drivers/gravis/shared/gf1/voice/actvvoc.h>

Ham::File::Mod* s_Mod = nullptr;
Ham::Player::Player* s_Player = nullptr;

uint8_t palette[16 * 3] =
{
    0x00, 0x00, 0x00, // Black
    0x07, 0x00, 0x0b, // Darker Royal Purple
    0x0f, 0x00, 0x16, // Dark Royal Purple
    0x19, 0x00, 0x26, // Royal Purple
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x22, 0x33, 0x3C, // Baby Blue
    0x1E, 0x2F, 0x1F, // Iguana Green
    0x23, 0x35, 0x1F, // Pistachio
    0x3E, 0x35, 0x1B, // Crayola Orange Red
    0x3F, 0x2D, 0x13, // Pastel Orange
    0x3F, 0x1A, 0x18, // Pastel Red
    0x0F, 0x0F, 0x0F, // Dark Grey
    0x1F, 0x1F, 0x1F, // Grey
    0x3A, 0x38, 0x33, // Cream
    0x3F, 0x3F, 0x3F  // White
};

namespace Colors
{
    enum
    {
        Black = 0x00,
        DarkerRoyalPurple = 0x01,
        DarkRoyalPurple = 0x02,
        RoyalPurple = 0x03,

        BabyBlue = 0x06,
        IguanaGreen = 0x07,
        Pistachio = 0x08,
        CrayolaOrangeRed = 0x09,
        PastelOrange = 0x0a,
        PastelRed = 0x0b,
        DarkGrey = 0x0c,
        Grey = 0x0d,
        Cream = 0x0e,
        White = 0x0f
    };
};

uint8_t borderCharacterGlyphs[] =
{
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111100,
    0b11111000,
    0b11110000,
    0b11110000,
    
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00001111,
    
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b00111111,
    0b00011111,
    0b00001111,
    0b00001111,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11110000,

    0b11110000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,

    0b00001111,
    0b00001111,
    0b00000111,
    0b00000011,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00001111,
    0b00001111,
    0b00011111,
    0b00111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,

    0b11110000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t vuMeterGlyphs[] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00111100,
    0b01111110,
    0b01111110,
    0b00111100,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00000000
};

uint8_t patternGlyphs[] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01100110,
    0b00000000,

    0b00000000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00000000,
    0b01100110,
    0b00000000
};

uint8_t panningGlyphs[] = 
{
    0b00000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00000001,
    0b00000001,
    0b00000001,
    0b00000001,
    0b00000000,
    0b10101010,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10101010,
    0b00000000,
};

uint8_t progressGlyphs[] =
{
    0b00000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,

    0b00000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b00000000,

    0b00000000,
    0b11100000,
    0b11100000,
    0b11100000,
    0b11100000,
    0b11100000,
    0b11100000,
    0b00000000,

    0b00000000,
    0b11110000,
    0b11110000,
    0b11110000,
    0b11110000,
    0b11110000,
    0b11110000,
    0b00000000,

    0b00000000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b00000000,

    0b00000000,
    0b11111100,
    0b11111100,
    0b11111100,
    0b11111100,
    0b11111100,
    0b11111100,
    0b00000000,

    0b00000000,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b00000000,

    0b00000000,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b00000000,
};

bool SetupScreen()
{
    using namespace Hag;
    using namespace Hag::System;

    LOG("Test", "Setting video mode...");
    // Stock 80x50 text mode.
    VGA::ModeSetting::SetVideoError_t error = VGA::ModeSetting::SetVideoMode(80, 50, VGA::ModeSetting::BitsPerPixel::Bpp4, VGA::ModeSetting::Flags::Text);
    if (error != VGA::ModeSetting::SetVideoError::Success)
    {
        LOG("Test", "Error setting video mode: %s", VGA::ModeSetting::SetVideoError::ToString(error));
        return false;
    }
    VGA::ModeSetting::SetCursor(false, 0, 0);

    // Turn screen off for a momen and set to 8 dot per character.
    VGA::Sequencer::ClockingMode::Write(
        VGA::Sequencer::ClockingMode::Read() | VGA::Sequencer::ClockingMode::DotClockSelect | VGA::Sequencer::ClockingMode::ScreenOff);

    // Setup our colors.
    VGA::ModeSetting::SetColors(sizeof(palette) / (3 * sizeof(uint8_t)), palette, 0);

    // Reset attribute index register.
    VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06);
    
    // Turn off blinking.
    VGA::AttributeController::AttributeMode_t attributeMode = VGA::AttributeController::AttributeMode::Read();
    attributeMode &= ~VGA::AttributeController::AttributeMode::EnableBlinking;

    // Reset attribute index register.
    VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06);
    VGA::AttributeController::AttributeMode::Write(attributeMode);

    // Make it point to our colors in the VGA palette.
    for (uint8_t i = 0; i < 16; ++i)
        VGA::AttributeController::Palette::Write(i, i);

    // We're done. Re-enable the attribute controller.
    VGA::AttributeControllerIndex::Write(VGA::AttributeControllerIndex::EnableVideoDisplay);

    // Font changes
    VGA::ModeSetting::UploadCharacterGlyphs((sizeof(borderCharacterGlyphs) / (8 * sizeof(uint8_t))) - 1, borderCharacterGlyphs, 0xD3, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs((sizeof(vuMeterGlyphs) / (8 * sizeof(uint8_t))) - 1, vuMeterGlyphs, 0xFB, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs((sizeof(patternGlyphs) / (8 * sizeof(uint8_t))) - 1, patternGlyphs, 0xF6, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs((sizeof(progressGlyphs) / (8 * sizeof(uint8_t))) - 1, progressGlyphs, 0xE8, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs((sizeof(panningGlyphs) / (8 * sizeof(uint8_t))) - 1, panningGlyphs, 0xC0, 8, 0);

    // Turn screen back on.
    VGA::Sequencer::ClockingMode::Write(
        VGA::Sequencer::ClockingMode::Read() & ~VGA::Sequencer::ClockingMode::ScreenOff);

    return true;
}

void WriteString(const char* str, uint8_t x, uint8_t y)
{
    using namespace Hag::System;
    uint16_t pos = ((BDA::NumberOfScreenColumns::Get() << 1) * y) + (uint16_t(x) << 1);
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>() + pos;
    while (*str != '\0')
    {
        *screenPtr = *(str++);
        screenPtr += 2;
    }
}

void WriteString(const char* str, uint8_t x, uint8_t y, uint8_t colors)
{
    using namespace Hag::System;
    uint16_t pos = ((BDA::NumberOfScreenColumns::Get() << 1) * y) + (uint16_t(x) << 1);
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>() + pos;
    while (*str != '\0')
    {
        *(screenPtr++) = *(str++);
        *(screenPtr++) = colors;
    }
}

void DrawBorder(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color, uint16_t screenWidth)
{
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();

    uint8_t* topLeft = screenPtr + ((y0 * screenWidth + x0) << 1);
    uint8_t* bottomLeft = screenPtr + ((y1 * screenWidth + x0) << 1);

    uint8_t* left = topLeft;
    uint8_t* right = left + ((x1 - x0) << 1);

    //Skip corner
    topLeft += 2;
    bottomLeft += 2;
    for (uint16_t x = x0 + 1; x < x1; ++x)
    {
        topLeft[0] = 0xDC;
        topLeft[1] = color;
        bottomLeft[0] = 0xDF;
        bottomLeft[1] = color;

        topLeft += 2;
        bottomLeft += 2;
    }

    left[0] = 0xD4;
    left[1] = color;
    left += (screenWidth << 1);

    right[0] = 0xD6;
    right[1] = color;
    right += (screenWidth << 1);

    for (uint16_t y = y0 + 1; y < y1; ++y)
    {
        left[0] = 0xDE;
        left[1] = color;
        right[0] = 0xDD;
        right[1] = color;
        left += (screenWidth << 1);
        right += (screenWidth << 1);
    }

    left[0] = 0xD8;
    left[1] = color;
    right[0] = 0xDA;
    right[1] = color;
}

void FillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t value, uint8_t color, uint16_t screenWidth)
{
    uint16_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint16_t>();
    uint16_t* ptr = screenPtr + y0 * screenWidth + x0;
    uint16_t val = (uint16_t(color) << 8) | value;

    uint16_t xDelta = screenWidth - ((x1 - x0) + 1);
    for (uint16_t y = y0; y <= y1; ++y)
    {
        for (uint16_t x = x0; x <= x1; ++x)
        {
            *(ptr++) = val;
        }
        ptr += xDelta;
    }
}

void WriteStatic()
{
    uint16_t width = 80;
    uint16_t height = 50;

    // Header and footer
    FillRectangle(0, 0, width - 1, 0, ' ', (Colors::RoyalPurple << 4) | Colors::White, width);
    FillRectangle(0, height - 1, width - 1, height - 1, ' ', (Colors::RoyalPurple << 4) | Colors::White, width);

    // Body
    FillRectangle(0, 1, width - 1, 9, ' ', (Colors::DarkRoyalPurple << 4) | Colors::White, width);

    // Rows
    FillRectangle(0, 11, width - 1, 47, ' ', (Colors::Black << 4) | Colors::DarkGrey, width);

    // Highlight rows
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();

    for (uint16_t i = 0; i < width; ++i)
    {
        screenPtr[(13 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(17 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(21 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(25 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(33 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(37 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(41 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
        screenPtr[(45 * width * 2) + (i << 1) + 1] = (Colors::Black << 4) | Colors::Grey;
    }

    //|00000000001111111111222222222233333333334444444444555555555566666666667777777777
    //|01234567890123456789012345678901234567890123456789012345678901234567890123456789
    //|---------+---------+---------+---------+---------+---------+---------+---------+|
    //| XX   SS NNN EPP VV  SS NNN EPP VV  SS NNN EPP VV  SS NNN EPP VV  SS NNN EPP VV |

    DrawBorder( 0, 10,  3, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawBorder( 5, 10, 19, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawBorder(20, 10, 34, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawBorder(35, 10, 49, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawBorder(50, 10, 64, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawBorder(65, 10, 79, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);

    FillRectangle(4, 10, 4, 48, ' ', (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);

    // Focus row
    FillRectangle(0, 29, width - 1, 29, ' ', (Colors::DarkRoyalPurple << 4) | Colors::White, width);

    // Song info
    DrawBorder(7, 1, 28, 4, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(8, 2, 27, 3, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::Cream, width);

    // VU meters
    DrawBorder(62, 2, 79, 8, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(63, 3, 78, 7, 0xFB, (Colors::DarkerRoyalPurple << 4) | Colors::Black, width);

    // Sample names
    DrawBorder(34, 2, 57, 8, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(35, 3, 56, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::Cream, width);

    // Panning
    DrawBorder(58, 2, 61, 8, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(59, 3, 60, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::BabyBlue, width);

    // Progress box
    DrawBorder(0, 6, 33, 8, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(1, 7, 32, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::BabyBlue, width);

    // Write static text to screen.
    WriteString("Test player 0.1 beta", 0, 0);
    WriteString("(c) 2025 riplin", 80 - 15, 0);
    WriteString("Song:", 1, 2, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Tempo:", 1, 3, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("bpm:", 8, 3, (Colors::DarkerRoyalPurple << 4) | Colors::DarkGrey);
    WriteString("speed:", 16, 3, (Colors::DarkerRoyalPurple << 4) | Colors::DarkGrey);

    WriteString("Progress", 1, 5, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Samples", 35, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Bal", 59, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Volume", 63, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);

    WriteString(s_Mod->GetName(), 8, 2, (Colors::DarkerRoyalPurple << 4) | Colors::Cream);

}

static const char* s_Hex = "0123456789ABCDEF";

void WriteRow(uint8_t* rowPtr, Ham::File::Mod::Note* currentNote, uint8_t currentRowIndex, bool drawVolume)
{
    using namespace Has;
    rowPtr[2] = '0' + (currentRowIndex / 10);
    rowPtr[4] = '0' + (currentRowIndex % 10);
    uint8_t* channelPtr = rowPtr + 12;
    for (uint16_t channel = 0; channel < min<uint16_t>(5, s_Mod->GetChannelCount()); ++channel)
    {
        uint8_t sample[2] = { 0xF6, 0xF6 };
        if (currentNote[channel].Sample != 0)
        {
            sample[0] = s_Hex[currentNote[channel].Sample >> 4];
            sample[1] = s_Hex[currentNote[channel].Sample & 0x0F];
        }

        static const uint8_t noteDefault[3] = { 0xF6, 0xF6, 0xF6 };
        const uint8_t* noteName = noteDefault;
        if (currentNote[channel].Note != 0xFFFF)
        {
            noteName = (const uint8_t*)s_Mod->GetNoteName(currentNote[channel].Note);
        }
        
        uint8_t effect = 0xF6;
        uint8_t parameter[2] = { 0xF6, 0xF6 };
        if ((currentNote[channel].Effect != 0) || (currentNote[channel].Parameter != 0))
        {
            effect = s_Hex[currentNote[channel].Effect];
            parameter[0] = s_Hex[currentNote[channel].Parameter >> 4];
            parameter[1] = s_Hex[currentNote[channel].Parameter & 0x0F];
        }

        uint8_t vol[2] = { 0xF6, 0xF6 };
        if (drawVolume)
        {
            uint8_t curVol = s_Player->GetChannelVolume(channel);
            vol[0] = s_Hex[curVol >> 4];
            vol[1] = s_Hex[curVol & 0x0F];
        }

        channelPtr[0] = sample[0];
        channelPtr[2] = sample[1];

        channelPtr[4] = 0xF7;

        channelPtr[6] = noteName[0];
        channelPtr[8] = noteName[1];
        channelPtr[10] = noteName[2];

        channelPtr[12] = 0xF7;

        channelPtr[14] = effect;
        channelPtr[16] = parameter[0];
        channelPtr[18] = parameter[1];

        channelPtr[20] = 0xF7;

        channelPtr[22] = vol[0];
        channelPtr[24] = vol[1];

        channelPtr += 30;
    }
}

void ClearRow(uint8_t* rowPtr)
{
    using namespace Has;

    rowPtr[2] = 0xF6;
    rowPtr[4] = 0xF6;
    uint8_t* channelPtr = rowPtr + 12;
    for (uint16_t channel = 0; channel < min<uint16_t>(5, s_Mod->GetChannelCount()); ++channel)
    {
        channelPtr[0] = 0xF6;
        channelPtr[2] = 0xF6;
        channelPtr[4] = 0xF7;
        channelPtr[6] = 0xF6;
        channelPtr[8] = 0xF6;
        channelPtr[10] = 0xF6;
        channelPtr[12] = 0xF7;
        channelPtr[14] = 0xF6;
        channelPtr[16] = 0xF6;
        channelPtr[18] = 0xF6;
        channelPtr[20] = 0xF7;
        channelPtr[22] = 0xF6;
        channelPtr[24] = 0xF6;

        channelPtr += 30;
    }
}

static int8_t channelVolumes[5] = { 0 };

void WriteTick0()
{
    using namespace Has;
    using namespace Ham::File;
    // Update the once per row stuff.
    SYS_ClearInterrupts();
    uint8_t currentOrderIndex = s_Player->GetCurrentOrderIndex();
    uint8_t currentRowIndex = s_Player->GetCurrentRow();
    uint8_t currentSpeed = s_Player->GetSpeed();
    uint8_t currentBpm = s_Player->GetBpm();
    SYS_RestoreInterrupts();

    char speed[3] = {' ', ' ', '\0'};
    uint8_t speedT = currentSpeed / 10;
    uint8_t speedS = currentSpeed % 10;
    speed[0] = (speedT > 0) ? speedT + '0' : ' ';
    speed[1] = speedS + '0';
    WriteString(speed, 22, 3);

    char bpm[4] = {' ', ' ', ' ', '\0'};
    uint8_t bpmS = currentBpm % 10;
    currentBpm /= 10;
    uint8_t bpmT = currentBpm % 10;
    uint8_t bpmH = currentBpm / 10;
    bpm[0] = (bpmH > 0) ? bpmH + '0' : ' ';
    bpm[1] = bpmT + '0';
    bpm[2] = bpmS + '0';
    WriteString(bpm, 12, 3);

    uint8_t patternIndex = s_Mod->GetOrder(currentOrderIndex);
    Mod::Note* currentPattern = s_Mod->GetPattern(patternIndex);

    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();
    int16_t screenStartRow = 18 - int16_t(currentRowIndex);
    uint8_t dataStartRow = 0;
    if (screenStartRow < 0)
    {
        dataStartRow = abs(screenStartRow);
        screenStartRow = 0;
    }
    uint8_t rowCount = min(64 - dataStartRow, 37 - screenStartRow);
    screenStartRow += 11;
    for (uint16_t clearRow = 11; clearRow < screenStartRow; ++clearRow)
    {
        uint8_t* rowPtr = screenPtr + 160 * clearRow;
        ClearRow(rowPtr);
    }

    for (uint16_t row = 0; row < rowCount; ++row)
    {
        Mod::Note* currentRow = currentPattern + (dataStartRow + row) * s_Mod->GetChannelCount();
        uint8_t* rowPtr = screenPtr + 160 * (screenStartRow + row);
        WriteRow(rowPtr, currentRow, dataStartRow + row, (screenStartRow + row) == 29);
    }
    for (uint16_t clearRow = screenStartRow + rowCount; clearRow < 48; ++clearRow)
    {
        uint8_t* rowPtr = screenPtr + 160 * clearRow;
        ClearRow(rowPtr);
    }

    // Trigger VU meters
    Mod::Note* vuRow = currentPattern + currentRowIndex * s_Mod->GetChannelCount();
    for (uint8_t channel = 0; channel < min<uint8_t>(s_Mod->GetChannelCount(), 5); ++channel)
    {
        if (vuRow[channel].Note != 0xFFFF)
            channelVolumes[channel] = s_Player->GetChannelVolume(channel);
    }

    //Sample box
    FillRectangle(35, 3, 56, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::Cream, 80);

    for (uint8_t channel = 0; channel < min<uint8_t>(s_Mod->GetChannelCount(), 5); ++channel)
    {
        uint8_t sample = s_Player->GetChannelSample(channel);
        if (sample != 0)
        {
            WriteString(s_Mod->GetSampleName(sample - 1), 35, 3 + channel);
        }
    }

    //Panning box
    for (uint8_t channel = 0; channel < min<uint8_t>(s_Mod->GetChannelCount(), 5); ++channel)
    {
        uint8_t pan = s_Player->GetChannelBalance(channel);
        if (pan < 0x08)
        {
            screenPtr[160 * 3 + 118 + channel * 160 + 0] = 0xC0 + pan;
            screenPtr[160 * 3 + 120 + channel * 160 + 0] = 0xC0 + 8;
        }
        else
        {
            screenPtr[160 * 3 + 118 + channel * 160 + 0] = 0xC0 + 8;
            screenPtr[160 * 3 + 120 + channel * 160 + 0] = 0xC0 + (pan & 0x07);
        }
    }

    //Progress box
    FillRectangle(1, 7, 32, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::BabyBlue, 80);

    uint32_t progress = (((currentOrderIndex << 6) + currentRowIndex) * 32 * 8) / (s_Mod->GetOrderCount() << 6);
    for (uint16_t i = 0; i < (progress >> 3); ++i)
    {
        screenPtr[160 * 7 + 2 + i*2 + 0] = 0xEF;
    }

    uint8_t subProgress = (progress & 0x07);
    if (subProgress != 0)
    {
        screenPtr[160 * 7 + 2 + (progress >> 3)*2 + 0] = 0xE7 + subProgress;
    }
}

uint8_t vuColors[16] =
{
    (Colors::DarkerRoyalPurple << 4) | Colors::IguanaGreen,
    (Colors::DarkerRoyalPurple << 4) | Colors::IguanaGreen,
    (Colors::DarkerRoyalPurple << 4) | Colors::IguanaGreen,
    (Colors::DarkerRoyalPurple << 4) | Colors::IguanaGreen,
    (Colors::DarkerRoyalPurple << 4) | Colors::IguanaGreen,
    (Colors::DarkerRoyalPurple << 4) | Colors::Pistachio,
    (Colors::DarkerRoyalPurple << 4) | Colors::Pistachio,
    (Colors::DarkerRoyalPurple << 4) | Colors::Pistachio,
    (Colors::DarkerRoyalPurple << 4) | Colors::Pistachio,
    (Colors::DarkerRoyalPurple << 4) | Colors::CrayolaOrangeRed,
    (Colors::DarkerRoyalPurple << 4) | Colors::CrayolaOrangeRed,
    (Colors::DarkerRoyalPurple << 4) | Colors::CrayolaOrangeRed,
    (Colors::DarkerRoyalPurple << 4) | Colors::PastelOrange,
    (Colors::DarkerRoyalPurple << 4) | Colors::PastelOrange,
    (Colors::DarkerRoyalPurple << 4) | Colors::PastelRed,
    (Colors::DarkerRoyalPurple << 4) | Colors::PastelRed
};

void WriteTickX()
{
    using namespace Has;
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();

    // Update the high frequency stuff.
    for (uint8_t channel = 0; channel < 5; ++channel)
    {
        uint8_t* vuPtr = screenPtr + 160 * 3 + 126 + channel * 160;
        for (uint8_t vol = 0; vol < 16; ++vol)
        {
            vuPtr[(vol << 1) + 0] = 0xFB;
            vuPtr[(vol << 1) + 1] = (Colors::DarkerRoyalPurple << 4) | Colors::Black;
        }

        for (uint8_t vol = 0; vol < (channelVolumes[channel] >> 2); ++vol)
        {
            vuPtr[(vol << 1) + 0] = 0xFE;
            vuPtr[(vol << 1) + 1] = s_Player->IsChannelMuted(channel) ? ((Colors::DarkerRoyalPurple << 4) | Colors::Grey) : vuColors[vol];
        }
        uint8_t partialVol = channelVolumes[channel] & 0x03;
        if (partialVol)
        {
            uint8_t vol = channelVolumes[channel] >> 2;
            vuPtr[(vol << 1) + 0] = 0xFB + partialVol;
            vuPtr[(vol << 1) + 1] = s_Player->IsChannelMuted(channel) ? ((Colors::DarkerRoyalPurple << 4) | Colors::Grey) : vuColors[vol];
        }
        channelVolumes[channel] = max<int8_t>(channelVolumes[channel] - 1, 0);
    }
}

void Handler()
{
    s_Player->Tick();
}

int main(int argc, const char** argv)
{
    __djgpp_nearptr_enable();
    using namespace Hag;
    using namespace Ham::File;
    using namespace Has::System;
    using namespace Ham::Gravis::Shared;

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
        return -1;
    }

    LOG("Test", "Initializing VGA driver");
    if (!VGA::ModeSetting::Initialize(allocator))
    {
        LOG("Test", "VGA failed to initialize");
    }

    if (!SetupScreen())
    {
        LOG("Test", "Failed to set up screen");
        return -1;
    }

    WriteStatic();

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

    volatile bool Pause = false;
    volatile bool Quit = false;
    volatile bool Mute1 = false;
    volatile bool Mute2 = false;
    volatile bool Mute3 = false;
    volatile bool Mute4 = false;

    KB::SetScanCodeHandler([&](KB::ScanCode_t scanCode)
    {
        Mute1 = Mute1 | (scanCode == KB::ScanCodeSet1::One);
        Mute2 = Mute2 | (scanCode == KB::ScanCodeSet1::Two);
        Mute3 = Mute3 | (scanCode == KB::ScanCodeSet1::Three);
        Mute4 = Mute4 | (scanCode == KB::ScanCodeSet1::Four);
        Pause = Pause | (scanCode == KB::ScanCodeSet1::Pause);
        Pause = Pause | (scanCode == KB::ScanCodeSet1::Spacebar);
        Quit = Quit | (scanCode == KB::ScanCodeSet1::Escape);
    });

    KB::InstallKeyboardHandler();

    //TODO: would very much prefer to use the GUS timers, but I can't seem to get those interrupts to fire. :(
    InterruptTable::ChainHandler(8, Handler);
    SYS_ClearInterrupts();
    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel0);
    uint16_t divisor = PIT::CalculateDivisor(50);
    PIT::Data::WriteChannel0(uint8_t(divisor));
    PIT::Data::WriteChannel0(uint8_t(divisor >> 8));
    SYS_RestoreInterrupts();

    s_Player->Play();

    volatile uint8_t tick = -1;
    volatile uint8_t row = -1;

    do
    {
        if (Mute1)
        {
            s_Player->ToggleChannelMute(0);
            Mute1 = false;
        }
        if (Mute2)
        {
            s_Player->ToggleChannelMute(1);
            Mute2 = false;
        }
        if (Mute3)
        {
            s_Player->ToggleChannelMute(2);
            Mute3 = false;
        }
        if (Mute4)
        {
            s_Player->ToggleChannelMute(3);
            Mute4 = false;
        }
        if (Pause)
        {
            if (s_Player->IsPaused())
                s_Player->Resume();
            else
                s_Player->Pause();
            Pause = false;
        }
        if (row != s_Player->GetCurrentRow())
        {
            WriteTick0();
            row = s_Player->GetCurrentRow();
        }
        if (tick != s_Player->GetCurrentTick())
        {
            WriteTickX();
            tick = s_Player->GetCurrentTick();
        }
    } while (!Quit);

    s_Player->Stop();

    KB::RemoveKeyboardHandler();

    InterruptTable::UnchainHandler(8);

    Function::System::Shutdown();

    SYS_ClearInterrupts();
    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel0);
    PIT::Data::WriteChannel0(0);
    PIT::Data::WriteChannel0(0);
    SYS_RestoreInterrupts();

    s_Player->~Player();
    allocator.Free(s_Player);
    Mod::Destroy(s_Mod);

    VGA::ModeSetting::SetVideoMode(80, 25, VGA::ModeSetting::BitsPerPixel::Bpp4, VGA::ModeSetting::Flags::Text);
    VGA::ModeSetting::Shutdown();

    return 0;
}
