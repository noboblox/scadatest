#ifndef VRTU_APDU_HPP
#define VRTU_APDU_HPP

#include <memory>
#include "enum.hpp"

#include "iec60870_common.h"

namespace VRTU
{
    enum ApduType
    {
        APDU_TYPE_invalid,

        APDU_TYPE_I,
        APDU_TYPE_S,
        APDU_TYPE_U,
    };
    using EnumApduType = Enum<ApduType, APDU_TYPE_invalid>;

    enum ServiceType
    {
        SVC_invalid,

        STARTDT_ACT  = 0x07,
        STARTDT_CON  = 0x0B,
        STOPDT_ACT   = 0x13,
        STOPDT_CON   = 0x23,
        TESTFR_ACT   = 0x43,
        TESTFR_CON   = 0x83,
    };
    using EnumApduService = Enum<ServiceType, SVC_invalid>;


    class Apdu
    {
    public:
        Apdu(const sCS101_AppLayerParameters& arParameters, const uint8_t* arBuffer, std::size_t aSize);
        ~Apdu();

        ApduType GetType() const noexcept { return mType; }

        bool HasReceiveSequence() const noexcept;
        int GetReceiveSequence() const noexcept;

        bool HasSendSequence() const noexcept;
        int GetSendSequence() const noexcept;

        bool HasService() const noexcept;
        ServiceType GetService() const noexcept;

    private:
        ApduType mType = APDU_TYPE_invalid;
        uint8_t mHeader[6] = {};
        sCS101_StaticASDU mAsdu = {};
    };
}

#endif
