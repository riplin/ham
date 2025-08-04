//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        GusCompatibilityPortHigh = 0x60,
        GusCompatibilityPortLow = 0x61,
        MidiAndSynthesizerPortHigh = 0x62,
        MidiAndSynthesizerPortLow = 0x63,
        CodecPortHigh = 0x64,
        CodecPortLow = 0x65,
        CdRomPortHigh = 0x60,
        CdRomPortLow = 0x61,
        AtapiPortHigh = 0x62,
        AtapiPortLow = 0x63,
        JoystickPortHigh = 0x60,
        JoystickPortLow = 0x61,
        EmulationPortHigh = 0x60,
        EmulationPortLow = 0x61,
        Mpu401PortHigh = 0x60,
        Mpu401PortLow = 0x61
    };
}

// Table 12-3
// PNP Address Control Registers
// +-------------+---------+---------+-------------+-----------------------------------+
// | Mnemonic    | Index   | LDN     | Default     | Description                       |
// +-------------+---------+---------+-------------+-----------------------------------+
// | P2X0HI      | 60h     | 0       | 00h         | P2X0HI[1:0] specifies P2XR[9:8]   |
// | P2X0LI      | 61h     | 0       | 00h         | P2X0LI[7:4] specifies P2XR[7:4]   |
// | P3X0HI      | 62h     | 0       | 00h         | P3X0HI[1:0] specifies P3XR[9:8]   |
// | P3X0LI      | 63h     | 0       | 00h         | P3X0LI[7:3] specifies P3XR[7:3]   |
// | PHCAI       | 64h     | 0       | 00h         | PHCAI[1:0] specifies PCODAR[9:8]  |
// | PLCAI       | 65h     | 0       | 00h         | PLCAI[7:2] specifies PCODAR[7:2]  |
// | PRAHI       | 60h     | 1       | 00h         | PRAHI[1:0] specifies PCDRAR[9:8]  |
// | PRALI       | 61h     | 1       | 00h         | PRALI[7:4] specifies PCDRAR[7:4]  |
// | PATAHI      | 62h     | 1       | 00h         | PATAHI[1:0] specifies PATAAR[9:8] |
// | PATALI      | 63h     | 1       | 00h         | PATALI[7:1] specifies PATAAR[7:1] |
// | P201HI      | 60h     | 2       | 00h         | P201HI[1:0] specifies P201AR[9:8] |
// | P201LI      | 61h     | 2       | 01h*        | P201LI[7:6] specifies P201AR[7:6] |
// | P388HI      | 60h     | 3       | 00h         | P388HI[1:0] specifies P388AR[9:8] |
// | P388LI      | 61h     | 3       | 08h*        | P388LI[7:6] specifies P388AR[7:6] |
// | P401HI      | 60h     | 4       | 00h         | P401HI[1:0] specifies P401AR[9:8] |
// | P401LI      | 61h     | 4       | 00h         | P401LI[7:1] specifies P401AR[7:1] |
// +-------------+---------+---------+-------------+-----------------------------------+

// Notes:
// 1. If P201AR[9:6] = 0h, then the value read back from P201LI is 00h (not 01h as shown).
// 2. If P388AR[9:6] = 0h, then the value read back from P388LI is 00h (not 08h as shown).

// All unused bits in the above PNP address control registers are reserved. All of the PNP
// address control registers shown in Table 12-3 are written through 0A79h (PNPWRP) and
// read through the PNP Read Data Port (PNPRDP). The value read back in the unspecified
// bits of all of the above registers is shown in the default column. For a description of the
// functions controlled by the various address blocks, see “I/O Address Spaces” on page 4-2.
// All of the groups of registers in Table 12-3 are used to describe an I/O address. When the
// programmable bits of any of these registers are zero (which is the power-up default), the
// addresses they describe are not decoded. Leaving the bits at zero is the PNP method of
// specifying that the range is not valid.

// Table 4-1 InterWave Address Spaces
// +------------+------------------------------------------------------------+----------------------------------------------------+-----------+
// | Mnemonic   | Description                                                | Base Address (SA11–SA0)†                           | Ref. Page |
// +------------+------------------------------------------------------------+----------------------------------------------------+-----------+
// | P2XR       | GUS-Compatibility—A block of ten addresses                 | 0*, 0*, P2X0HI[1:0], P2X0LI[7:4], X, X, X, X       | 12-25     |
// |            | within 16 spaces used primarily for compatibility with     |                                                    |           |
// |            | existing sound cards. The four least-significant bits      |                                                    |           |
// |            | of the address span the 16 spaces, but only P2XR+0,        |                                                    |           |
// |            | P2XR+6, and P2XR+8 through F are used.                     |                                                    |           |
// | P3XR       | MIDI and Synthesizer—A block of eight                      | 0*, 0*, P3X0HI[1:0], P3X0LI[7:3], X, X, X          | 12-25     |
// |            | consecutive addresses used primarily to address the        |                                                    |           |
// |            | synthesizer and MIDI functions. The three                  |                                                    |           |
// |            | least-significant bits span the eight spaces, but          |                                                    |           |
// |            | P3XR+6 is not used.                                        |                                                    |           |
// | PCODAR     | Codec—A block of four consecutive addresses used           | 0*, 0*, PHCAI[1:0], PLCAI[7:2], X, X               | 12-25     |
// |            | to address the codec function.                             |                                                    |           |
// | PCDRAR     | External Interface—A block of eight consecutive            | 0*, 0*, PRAHI[1:0], PRALI[7:3], X, X, X            | 12-25     |
// |            | addresses used for accesses to the external                |                                                    |           |
// |            | CD-ROM interface. When this I/O block is decoded,          |                                                    |           |
// |            | it causes the EX_CS signal to become active.               |                                                    |           |
// | PATAAR     | ATAPI—A block of two consecutive addresses used            | 0*, 0*, PATAHI[1:0], PATALI[7:1], X                | 12-25     |
// |            | in conjunction with PCDRAR to communicate with             |                                                    |           |
// |            | an ATAPI CD-ROM. When this I/O block is decoded,           |                                                    |           |
// |            | it causes the EX_CS signal to become active.               |                                                    |           |
// | PIDXR      | Plug and Play Index register—The single-byte               | 279h                                               | 12-21     |
// |            | index to all of the standard PNP-ISA registers.            |                                                    |           |
// | PNPWRP     | Plug and Play Write Data Port—The single-byte              | A79h                                               | 12-21     |
// |            | data port through which all of the standard PNP-ISA        |                                                    |           |
// |            | registers are written.                                     |                                                    |           |
// | PNPRDP     | Plug and Play Read Data Port—The single-byte               | 0, 0, PSRPAI[7:0], 1, 1                            | 12-21     |
// |            | data port through which all of the standard PNP-ISA        |                                                    |           |
// |            | registers are read.                                        |                                                    |           |
// | UGPA1I,    | General Purpose Address Registers 1 and                    |                                                    | 12-11,    |
// | UGPA2I     | 2—The general purpose registers (UGP1I and                 |                                                    | 12-25     |
// | (P401AR)   | UGP2I) are single-byte registers used for                  |                                                    |           |
// |            | compatibility with legacy sound cards. These               |                                                    |           |
// |            | registers are typically placed in two consecutive          |                                                    |           |
// |            | locations and pointed to by P401AR, a standard PNP         |                                                    |           |
// |            | register that specifies the location of these two          |                                                    |           |
// |            | emulation addresses. P401AR is necessary to                |                                                    |           |
// |            | communicate to the PNP BIOS and to application             |                                                    |           |
// |            | software that these addresses are being used.              |                                                    |           |
// | P201AR     | Game Control (Joystick)—Points to a single-byte            | 0*, 0*, P201HI[1:0], P201LI[7:6], 0, 0, 0, 0, 0, 1 | 12-25     |
// |            | block that is used for game control (the Game Control      |                                                    |           |
// |            | register (GCCR)). This port is typically set to address    |                                                    |           |
// |            | 201h to match the legacy location of this function.        |                                                    |           |
// | P388AR     | AdLib Emulation—Points to a two-byte block that            | 0*, 0*, P388HI[1:0], P388LI[7:6], 0, 0, 1, 0, 0, X | 12-25     |
// |            | is used to capture activity for legacy AdLib registers;    |                                                    |           |
// |            | however, this block has little value unless it is set to   |                                                    |           |
// |            | the legacy position of 388h–389h.                          |                                                    |           |
// +------------+------------------------------------------------------------+----------------------------------------------------+-----------+

// Notes:
// † SA refers to system address pins.
// * SA11–SA10 may or may not be decoded based on the state of EX_CS at power up.

typedef Has::System::PnP::Data_t AddressControl_t;
namespace AddressControl
{
    enum
    {
        GusCompatibilityPortHigh = 0x03,
        GusCompatibilityPortLow = 0xF0,
        MidiAndSynthesizerPortHigh = 0x01,
        MidiAndSynthesizerPortLow = 0xF8,
        CodecPortHigh = 0x03,
        CodecPortLow = 0xFC,
        CdRomPortHigh = 0x03,
        CdRomPortLow = 0xF8,
        AtapiPortHigh = 0x03,
        AtapiPortLow = 0xFE,
        JoystickPortHigh = 0x03,
        JoystickPortLow = 0xC0,
        EmulationPortHigh = 0x03,
        EmulationPortLow = 0xC0,
        Mpu401PortHigh = 0xFF,
        Mpu401PortLow = 0xFF
    };

    inline Has::System::PnP::Register_t ReadGusCompatibilityPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::GusCompatibilityPortHigh)) << 8) |
            Data::Read(Register::GusCompatibilityPortLow);
    }

    inline Has::System::PnP::Register_t ReadMidiAndSynthesizerPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::MidiAndSynthesizerPortHigh)) << 8) |
            Data::Read(Register::MidiAndSynthesizerPortLow);
    }

    inline Has::System::PnP::Register_t ReadCodecPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::CodecPortHigh)) << 8) |
            Data::Read(Register::CodecPortLow);
    }

    inline Has::System::PnP::Register_t ReadCdRomPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::CdRomPortHigh)) << 8) |
            Data::Read(Register::CdRomPortLow);
    }

    inline Has::System::PnP::Register_t ReadAtapiPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::AtapiPortHigh)) << 8) |
            Data::Read(Register::AtapiPortLow);
    }

    inline Has::System::PnP::Register_t ReadJoystickPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::JoystickPortHigh)) << 8) |
            Data::Read(Register::JoystickPortLow);
    }

    inline Has::System::PnP::Register_t ReadEmulationPort()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::EmulationPortHigh)) << 8) |
            Data::Read(Register::EmulationPortLow);
    }

    inline Has::System::PnP::Register_t ReadMpu401Port()
    {
        using namespace Has::System::PnP;
        return (Register_t(Data::Read(Register::Mpu401PortHigh)) << 8) |
            Data::Read(Register::Mpu401PortLow);
    }

    inline void WriteGusCompatibilityPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::GusCompatibilityPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::GusCompatibilityPortLow, AddressControl_t(port));
    }

    inline void WriteMidiAndSynthesizerPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::MidiAndSynthesizerPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::MidiAndSynthesizerPortLow, AddressControl_t(port));
    }

    inline void WriteCodecPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::CodecPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::CodecPortLow, AddressControl_t(port));
    }

    inline void WriteCdRomPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::CdRomPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::CdRomPortLow, AddressControl_t(port));
    }

    inline void WriteAtapiPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::AtapiPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::AtapiPortLow, AddressControl_t(port));
    }

    inline void WriteJoystickPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::JoystickPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::JoystickPortLow, AddressControl_t(port));
    }

    inline void WriteEmulationPort(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::EmulationPortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::EmulationPortLow, AddressControl_t(port));
    }

    inline void WriteMpu401Port(Has::System::PnP::Register_t port)
    {
        using namespace Has::System::PnP;
        Data::Write(Register::Mpu401PortHigh, AddressControl_t(port >> 8));
        Data::Write(Register::Mpu401PortLow, AddressControl_t(port));
    }

}

}