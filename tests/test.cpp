//Copyright 2025-Present riplin

#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <iterator>
#include <sys/nearptr.h>
#include <ham/file/mod.h>
#include <ham/file/s3m.h>
#include <ham/file/song.h>
#include <has/system/pic.h>
#include <has/system/pit.h>
#include <hag/system/bda.h>
#include <has/testing/log.h>
#include <support/allocatr.h>
#include <ham/player/player.h>
#include <has/system/keyboard.h>
#include <has/system/interrup.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/modeset.h>
#include <ham/drivers/gravis/shared/system.h>
#include <ham/drivers/gravis/shared/gf1/voice/actvvoc.h>

Ham::File::Song* s_Song = nullptr;
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

namespace Border
{
    uint8_t Glyphs[] =
    {
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00111100,

        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000001,
        0b11000011,
        0b11000011,

        0b00111100,
        0b00111100,
        0b01111110,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,

        0b11000011,
        0b11000011,
        0b10000001,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,

        0b11111000,
        0b11111100,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111100,
        0b11111000,

        0b00000111,
        0b00000011,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000011,
        0b00000111,

        0b00011111,
        0b00111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b00111111,
        0b00011111,

        0b11100000,
        0b11000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b11000000,
        0b11100000,

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
        0b00000000,

        0b00111100,
        0b00111100,
        0b00111100,
        0b00111100,
        0b00111100,
        0b00111100,
        0b00111100,
        0b00111100,

        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        
        0b00000000,
        0b00000000,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,

        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111111,
        0b11111111
    };

    enum
    {
        Index = 0xC0,
        MiddleTop =                 Index + 0x00,
        MiddleTopNegative =         Index + 0x01,
        MiddleBottom =              Index + 0x02,
        MiddleBottomNegative =      Index + 0x03,
        MiddleLeft =                Index + 0x04,
        MiddleLeftNegative =        Index + 0x05,
        MiddleRight =               Index + 0x06,
        MiddleRightNegative =       Index + 0x07,

        TopLeft =                   Index + 0x08,
        TopLeftNegative =           Index + 0x09,
        TopRight =                  Index + 0x0a,
        TopRightNegative =          Index + 0x0b,
        BottomLeft =                Index + 0x0c,
        BottomLeftNegative =        Index + 0x0d,
        BottomRight =               Index + 0x0e,
        BottomRightNegative =       Index + 0x0f,

        MiddleVertical =            Index + 0x10,
        MiddleVerticalNegative =    Index + 0x11,
        MiddleHorizontal =          Index + 0x12,
        MiddleHorizontalNegative =  Index + 0x13,

        //Already defined in ASCII table
        Top =                   0xDF,
        TopNegative =           0xDC,
        Bottom =                0xDC,
        BottomNegative =        0xDF,
        Left =                  0xDD,
        LeftNegative =          0xDE,
        Right =                 0xDE,
        RightNegative =         0xDD,

        Count = sizeof(Glyphs) / (8 * sizeof(uint8_t))
    };
}

namespace VuMeter
{
    uint8_t Glyphs[] =
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

    enum
    {
        Index = 0xFB,

        Smaller =       Index + 0x00,
        Small =         Index + 0x01,
        Medium =        Index + 0x02,
        Large =         Index + 0x03,

        Count = sizeof(Glyphs) / (8 * sizeof(uint8_t))
    };
}

namespace Pattern
{
    uint8_t Glyphs[] =
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

    enum
    {
        Index = 0xF6,

        Dots = Index + 0x00,
        ColonDots = Index + 0x01,

        Count = sizeof(Glyphs) / (8 * sizeof(uint8_t))
    };
}

namespace Panning
{
    uint8_t Glyphs[] = 
    {
        0b00000000,
        0b00000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00000100,
        0b00000100,
        0b00000100,
        0b00000100,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b10101010,
        0b00000000,

        0b00000000,
        0b00000000,
        0b00000001,
        0b00000001,
        0b00000001,
        0b00000001,
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

    enum
    {
        Index = 0xB0,

        Pan0 = Index + 0x00,
        Pan1 = Index + 0x01,
        Pan2 = Index + 0x02,
        Pan3 = Index + 0x03,
        Pan4 = Index + 0x04,
        Pan5 = Index + 0x05,
        Pan6 = Index + 0x06,
        Pan7 = Index + 0x07,
        Empty = Index + 0x08,

        Count = sizeof(Glyphs) / (8 * sizeof(uint8_t))
    };
}

namespace Progress
{
    uint8_t Glyphs[] =
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

    enum
    {
        Index = 0xE8,

        Step1 = Index + 0x00,
        Step2 = Index + 0x01,
        Step3 = Index + 0x02,
        Step4 = Index + 0x03,
        Step5 = Index + 0x04,
        Step6 = Index + 0x05,
        Step7 = Index + 0x06,
        Step8 = Index + 0x07,

        Count = sizeof(Glyphs) / (8 * sizeof(uint8_t))
    };
}

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
    VGA::ModeSetting::UploadCharacterGlyphs(Border::Count - 1, Border::Glyphs, Border::Index, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs(VuMeter::Count - 1, VuMeter::Glyphs, VuMeter::Index, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs(Pattern::Count - 1, Pattern::Glyphs, Pattern::Index, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs(Panning::Count - 1, Panning::Glyphs, Panning::Index, 8, 0);
    VGA::ModeSetting::UploadCharacterGlyphs(Progress::Count - 1, Progress::Glyphs, Progress::Index, 8, 0);

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
        topLeft[0] = Border::TopNegative;
        topLeft[1] = color;
        bottomLeft[0] = Border::BottomNegative;
        bottomLeft[1] = color;

        topLeft += 2;
        bottomLeft += 2;
    }

    left[0] = Border::TopLeftNegative;
    left[1] = color;
    left += (screenWidth << 1);

    right[0] = Border::TopRightNegative;
    right[1] = color;
    right += (screenWidth << 1);

    for (uint16_t y = y0 + 1; y < y1; ++y)
    {
        left[0] = Border::LeftNegative;
        left[1] = color;
        right[0] = Border::RightNegative;
        right[1] = color;
        left += (screenWidth << 1);
        right += (screenWidth << 1);
    }

    left[0] = Border::BottomLeftNegative;
    left[1] = color;
    right[0] = Border::BottomRightNegative;
    right[1] = color;
}

void DrawSkinnyColumn(uint16_t x, uint16_t y0, uint16_t y1, uint8_t color, uint16_t screenWidth)
{
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();
    uint8_t* left = screenPtr + ((y0 * screenWidth + x) << 1);

    left[0] = Border::MiddleTopNegative;
    left[1] = color;
    left += (screenWidth << 1);

    for (uint16_t y = y0 + 1; y < y1; ++y)
    {
        left[0] = Border::MiddleVerticalNegative;
        left[1] = color;
        left += (screenWidth << 1);
    }

    left[0] = Border::MiddleBottomNegative;
    left[1] = color;
}

void DrawBlankColumn(uint16_t x, uint16_t y0, uint16_t y1, uint8_t color, uint16_t screenWidth)
{
    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();
    uint8_t* topLeft = screenPtr + ((y0 * screenWidth + x) << 1);
    uint8_t* bottomLeft = screenPtr + ((y1 * screenWidth + x) << 1);

    topLeft[0] = Border::TopNegative;
    topLeft[1] = color;
    bottomLeft[0] = Border::BottomNegative;
    bottomLeft[1] = color;
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

    //|00000000001111111111222222222233333333334444444444555555555566666666667777777777
    //|01234567890123456789012345678901234567890123456789012345678901234567890123456789
    //|---------+---------+---------+---------+---------+---------+---------+---------+|
    //|XX SS NNN EPP SS NNN EPP SS NNN EPP SS NNN EPP SS NNN EPP SS NNN EPP SS NNN EPP |

    DrawBorder(0, 10, 79, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(0, 11, 0, 47, ' ', (Colors::Black << 4) | Colors::DarkGrey, width);

    for (uint16_t i = 0; i < width - 1; ++i)
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
    
    DrawBlankColumn(0, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);

    DrawSkinnyColumn(2, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(13, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(24, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(35, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(46, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(57, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    DrawSkinnyColumn(68, 10, 48, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);

    // Focus row
    FillRectangle(0, 29, width - 1, 29, ' ', (Colors::DarkRoyalPurple << 4) | Colors::White, width);

    // Song info
    DrawBorder(7, 1, 33, 5, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(8, 2, 32, 4, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::Cream, width);

    // VU meters
    DrawBorder(62, 2, 79, 8, (Colors::DarkRoyalPurple << 4) | Colors::DarkerRoyalPurple, width);
    FillRectangle(63, 3, 78, 7, VuMeter::Smaller, (Colors::DarkerRoyalPurple << 4) | Colors::Black, width);

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
    WriteString("Order:", 1, 4, (Colors::DarkRoyalPurple << 4) | Colors::White);

    WriteString("Samples", 35, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Bal", 59, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);
    WriteString("Volume", 63, 1, (Colors::DarkRoyalPurple << 4) | Colors::White);

    WriteString(s_Song->GetName(), 8, 2, (Colors::DarkerRoyalPurple << 4) | Colors::Cream);

    WriteString("ESC/Q:Quit Space/P/Pause:Pause/Resume 1,2,3,4:Mute", 0, 49);
}

static const char* s_Hex = "0123456789ABCDEF";
static const char* s_S3mEffect = "0xxGHxxE8ODBxCSTADEFIKLQUVS";
static const char* s_Effect = s_Hex;

static const char* s_NoteNames[12]
{
    "C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "A-", "A#", "B-"
};

void WriteRow(uint8_t* rowPtr, const Ham::File::Song::NoteData* currentNote, uint8_t currentRowIndex, bool drawVolume)
{
    using namespace Has;
    using namespace Ham::File;

    rowPtr[0] = '0' + (currentRowIndex / 10);
    rowPtr[2] = '0' + (currentRowIndex % 10);
    uint8_t* channelPtr = rowPtr + 6;
    for (uint16_t channel = 0; channel < min<uint16_t>(7, s_Song->GetChannelCount()); ++channel)
    {
        uint8_t sample[2] = { Pattern::Dots, Pattern::Dots };
        if (currentNote[channel].Instrument != 0)
        {
            sample[0] = s_Hex[currentNote[channel].Instrument >> 4];
            sample[1] = s_Hex[currentNote[channel].Instrument & 0x0F];
        }

        uint8_t noteName[3] = { Pattern::Dots, Pattern::Dots, Pattern::Dots };
        if (currentNote[channel].Note < Note::KeyOff)
        {
            uint8_t idx = currentNote[channel].Note % 12;
            uint8_t oct = currentNote[channel].Note / 12;
            noteName[0] = s_NoteNames[idx][0];
            noteName[1] = s_NoteNames[idx][1];
            noteName[2] = s_Hex[oct];
        }
        
        uint8_t effect = Pattern::Dots;
        uint8_t parameter[2] = { Pattern::Dots, Pattern::Dots };
        if ((currentNote[channel].Effect != 0) || (currentNote[channel].Parameter != 0))
        {
            effect = s_Effect[currentNote[channel].Effect];
            parameter[0] = s_Hex[currentNote[channel].Parameter >> 4];
            parameter[1] = s_Hex[currentNote[channel].Parameter & 0x0F];
        }

        channelPtr[0] = sample[0];
        channelPtr[2] = sample[1];

        channelPtr[4] = Pattern::ColonDots;

        channelPtr[6] = noteName[0];
        channelPtr[8] = noteName[1];
        channelPtr[10] = noteName[2];

        channelPtr[12] = Pattern::ColonDots;

        channelPtr[14] = effect;
        channelPtr[16] = parameter[0];
        channelPtr[18] = parameter[1];

        channelPtr += 22;
    }
}

void ClearRow(uint8_t* rowPtr)
{
    using namespace Has;

    rowPtr[0] = Pattern::Dots;
    rowPtr[2] = Pattern::Dots;
    uint8_t* channelPtr = rowPtr + 6;
    for (uint16_t channel = 0; channel < min<uint16_t>(7, s_Song->GetChannelCount()); ++channel)
    {
        channelPtr[0] = Pattern::Dots;
        channelPtr[2] = Pattern::Dots;
        channelPtr[4] = Pattern::ColonDots;
        channelPtr[6] = Pattern::Dots;
        channelPtr[8] = Pattern::Dots;
        channelPtr[10] = Pattern::Dots;
        channelPtr[12] = Pattern::ColonDots;
        channelPtr[14] = Pattern::Dots;
        channelPtr[16] = Pattern::Dots;
        channelPtr[18] = Pattern::Dots;

        channelPtr += 22;
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

    uint8_t cOI = currentOrderIndex;
    char order[4] =  {' ', ' ', ' ', '\0'};
    uint8_t orderS = cOI % 10;
    cOI /= 10;
    uint8_t orderT = cOI % 10;
    uint8_t orderH = cOI / 10;
    order[0] = ((orderH > 0) && (orderT > 0)) ? orderH + '0' : ' ';
    order[1] = (orderT > 0) ? orderT + '0' : ' ';
    order[2] = orderS + '0';
    WriteString(order, 8, 4);

    uint8_t patternIndex = s_Song->GetOrder(currentOrderIndex);
    const Song::NoteData* currentPattern = s_Song->GetPatternNotes(patternIndex);

    uint8_t* screenPtr = FARPointer(0xB800, 0x0000).ToPointer<uint8_t>();
    int16_t screenStartRow = 18 - int16_t(currentRowIndex);
    uint8_t dataStartRow = 0;
    if (screenStartRow < 0)
    {
        dataStartRow = abs(screenStartRow);
        screenStartRow = 0;
    }
    uint8_t rowCount = min(s_Song->GetPatternRowCount(patternIndex) - dataStartRow, 37 - screenStartRow);
    screenStartRow += 11;
    for (uint16_t clearRow = 11; clearRow < screenStartRow; ++clearRow)
    {
        uint8_t* rowPtr = screenPtr + 160 * clearRow;
        ClearRow(rowPtr);
    }

    for (uint16_t row = 0; row < rowCount; ++row)
    {
        const Song::NoteData* currentRow = currentPattern + (dataStartRow + row) * s_Song->GetChannelCount();
        uint8_t* rowPtr = screenPtr + 160 * (screenStartRow + row);
        WriteRow(rowPtr, currentRow, dataStartRow + row, (screenStartRow + row) == 29);
    }
    for (uint16_t clearRow = screenStartRow + rowCount; clearRow < 48; ++clearRow)
    {
        uint8_t* rowPtr = screenPtr + 160 * clearRow;
        ClearRow(rowPtr);
    }

    // Trigger VU meters
    const Song::NoteData* vuRow = currentPattern + currentRowIndex * s_Song->GetChannelCount();
    for (uint8_t channel = 0; channel < min<uint8_t>(s_Song->GetChannelCount(), 5); ++channel)
    {
        if (vuRow[channel].Note != Note::NotSet)
            channelVolumes[channel] = s_Player->GetChannelVolume(channel);
    }

    //Sample box
    FillRectangle(35, 3, 56, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::Cream, 80);

    for (uint8_t channel = 0; channel < min<uint8_t>(s_Song->GetChannelCount(), 5); ++channel)
    {
        uint8_t sample = s_Player->GetChannelSample(channel);
        if (sample != 0)
        {
            WriteString(s_Song->GetInstrumentName(sample - 1), 35, 3 + channel);
        }
    }

    //Panning box
    for (uint8_t channel = 0; channel < min<uint8_t>(s_Song->GetChannelCount(), 5); ++channel)
    {
        uint8_t pan = s_Player->GetChannelBalance(channel);
        if (pan < 0x08)
        {
            screenPtr[160 * 3 + 118 + channel * 160 + 0] = Panning::Index + pan;
            screenPtr[160 * 3 + 120 + channel * 160 + 0] = Panning::Empty;
        }
        else
        {
            screenPtr[160 * 3 + 118 + channel * 160 + 0] = Panning::Empty;
            screenPtr[160 * 3 + 120 + channel * 160 + 0] = Panning::Index + (pan & 0x07);
        }
    }

    //Progress box
    FillRectangle(1, 7, 32, 7, ' ', (Colors::DarkerRoyalPurple << 4) | Colors::BabyBlue, 80);

    uint32_t progress = (((currentOrderIndex << 6) + currentRowIndex) * 32 * 8) / (s_Song->GetOrderCount() << 6);
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
            vuPtr[(vol << 1) + 0] = VuMeter::Smaller;
            vuPtr[(vol << 1) + 1] = (Colors::DarkerRoyalPurple << 4) | Colors::Black;
        }

        for (uint8_t vol = 0; vol < (channelVolumes[channel] >> 2); ++vol)
        {
            vuPtr[(vol << 1) + 0] = VuMeter::Large;
            vuPtr[(vol << 1) + 1] = s_Player->IsChannelMuted(channel) ? ((Colors::DarkerRoyalPurple << 4) | Colors::Grey) : vuColors[vol];
        }
        uint8_t partialVol = channelVolumes[channel] & 0x03;
        if (partialVol)
        {
            uint8_t vol = channelVolumes[channel] >> 2;
            vuPtr[(vol << 1) + 0] = VuMeter::Index + partialVol;
            vuPtr[(vol << 1) + 1] = s_Player->IsChannelMuted(channel) ? ((Colors::DarkerRoyalPurple << 4) | Colors::Grey) : vuColors[vol];
        }
        channelVolumes[channel] = max<int8_t>(channelVolumes[channel] - 1, 0);
    }
}

typedef uint8_t FileType_t;
namespace FileType
{
    enum
    {
        Invalid = 0x00,
        Mod = 0x01,
        S3m = 0x02
    };
}


void split(const std::string &s, char delim, std::vector<std::string>& result)
{
    std::istringstream iss(s);
    std::string item;

    while (std::getline(iss, item, delim))
        if (!item.empty()) result.push_back(item);
}

std::string tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](uint8_t c){ return std::tolower(c); });
    return s;
}

FileType_t DetermineFileType(const char* fileName)
{
    std::string name = tolower(fileName);

    std::vector<std::string> items;
    split(name, '.', items);
    
    if (!items.empty())
    {
        std::string ext = items[items.size() - 1];
        if (ext == "mod")
            return FileType::Mod;
        if (ext == "s3m")
            return FileType::S3m;
    }
    return FileType::Invalid;
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
        printf("test <musicfile.ext>\n");
        printf("Where ext is: mod or s3m\n");
        return -1;
    }
    switch(DetermineFileType(argv[1]))
    {
    case FileType::Mod:
        s_Song = Mod::Load(allocator, argv[1]);
        s_Effect = s_Hex;
        break;
    case FileType::S3m:
        s_Song = S3m::Load(allocator, argv[1]);
        s_Effect = s_S3mEffect;
        break;
    default:
        printf("Unknown file format: %s", argv[1]);
        return -1;
    }

    if (s_Song == nullptr)
    {
        printf("Failed to load: %s\n", argv[1]);
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
        Song::Destroy(s_Song);
        s_Song = nullptr;
        return -1;
    }

    Function::System::Configure(s_Song->GetChannelCount());

    s_Player = ::new(allocator.Allocate(sizeof(Ham::Player::Player))) Ham::Player::Player(allocator, s_Song, [](uint8_t RefreshRateInHz)
    {
        SYS_ClearInterrupts();
        uint16_t divisor = PIT::CalculateDivisor(RefreshRateInHz);
        PIT::Data::WriteChannel0(uint8_t(divisor));
        PIT::Data::WriteChannel0(uint8_t(divisor >> 8));
        SYS_RestoreInterrupts();
    });

    //Function::System::SetTimer1Handler(Handler, 50);

    struct
    {
        volatile bool Pause = false;
        volatile bool Quit = false;
        volatile bool Mute1 = false;
        volatile bool Mute2 = false;
        volatile bool Mute3 = false;
        volatile bool Mute4 = false;
    } Command;

    KB::SetScanCodeHandler([&](KB::ScanCode_t scanCode)
    {
        Command.Mute1 = Command.Mute1 | (scanCode == KB::ScanCodeSet1::One);
        Command.Mute2 = Command.Mute2 | (scanCode == KB::ScanCodeSet1::Two);
        Command.Mute3 = Command.Mute3 | (scanCode == KB::ScanCodeSet1::Three);
        Command.Mute4 = Command.Mute4 | (scanCode == KB::ScanCodeSet1::Four);
        Command.Pause = Command.Pause | (scanCode == KB::ScanCodeSet1::Pause);
        Command.Pause = Command.Pause | (scanCode == KB::ScanCodeSet1::P);
        Command.Pause = Command.Pause | (scanCode == KB::ScanCodeSet1::Spacebar);

        // Better to respond to the release event when quitting so we don't get sticky keys.
        Command.Quit = Command.Quit | (scanCode == (KB::ScanCodeSet1::Q | KB::ScanCodeSet1::Released));
        Command.Quit = Command.Quit | (scanCode == (KB::ScanCodeSet1::Escape | KB::ScanCodeSet1::Released));
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
        if (Command.Mute1)
        {
            s_Player->ToggleChannelMute(0);
            Command.Mute1 = false;
        }
        if (Command.Mute2)
        {
            s_Player->ToggleChannelMute(1);
            Command.Mute2 = false;
        }
        if (Command.Mute3)
        {
            s_Player->ToggleChannelMute(2);
            Command.Mute3 = false;
        }
        if (Command.Mute4)
        {
            s_Player->ToggleChannelMute(3);
            Command.Mute4 = false;
        }
        if (Command.Pause)
        {
            if (s_Player->IsPaused())
                s_Player->Resume();
            else
                s_Player->Pause();
            Command.Pause = false;
        }

        SYS_ClearInterrupts();
        uint8_t currentTick = s_Player->GetCurrentTick();
        uint8_t currentRow = s_Player->GetCurrentRow();
        SYS_RestoreInterrupts();

        if ((row != currentRow) && (currentTick == 0))
        {
            WriteTick0();
            row = currentRow;
            LOG("Test", "WriteTick0: Updated Tick %i row %i on screen", currentTick, currentRow);
        }

        if (tick != currentTick)
        {
            WriteTickX();
            tick = currentTick;
            LOG("Test", "WriteTickX: Updated Tick %i row %i on screen", currentTick, currentRow);
        }
    } while (!Command.Quit);

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
    Song::Destroy(s_Song);

    VGA::ModeSetting::SetVideoMode(80, 25, VGA::ModeSetting::BitsPerPixel::Bpp4, VGA::ModeSetting::Flags::Text);
    VGA::ModeSetting::Shutdown();

    return 0;
}
