#include "apdu.hpp"
#include "iec60870_common.h"
#include "cs101_asdu_internal.h"

template<> const VRTU::EnumApduType::Definition VRTU::EnumApduType::mDefinition = {
    {VRTU::APDU_TYPE_I, "I"},
    {VRTU::APDU_TYPE_U, "U"},
    {VRTU::APDU_TYPE_S, "S"},
};

template<> const VRTU::EnumApduService::Definition VRTU::EnumApduService::mDefinition = {
    {VRTU::TESTFR_ACT , "TESTFR act" },
    {VRTU::TESTFR_CON , "TESTFR con" },
    {VRTU::STARTDT_ACT, "STARTDT act"},
    {VRTU::STARTDT_CON, "STARTDT con"},
    {VRTU::STOPDT_ACT , "STOPDT act" },
    {VRTU::STOPDT_CON , "STOPDT con" },
};

namespace VRTU
{
    Apdu::Apdu(const sCS101_AppLayerParameters& arParameters, const uint8_t* arBuffer, std::size_t aSize)
    {
        if (aSize < (int) sizeof(mHeader))
            return;

        std::memcpy(mHeader, arBuffer, sizeof(mHeader));

        if (mHeader[0] != 0x68)
            return;

        if (mHeader[1] != aSize - 2)
            return;

        int type = mHeader[2] & 0x03;

        if (type == 0x03)
        {
            mType = APDU_TYPE_U;
        }
        else if (type == 0x01)
        {
            mType = APDU_TYPE_S;
        }
        else
        {
            std::unique_ptr<sCS101_ASDU, void(*)(sCS101_ASDU*)> p_asdu(nullptr, CS101_ASDU_destroy);
            p_asdu.reset(CS101_ASDU_createFromBuffer(const_cast<sCS101_AppLayerParameters*> (&arParameters),
                                                     const_cast<uint8_t*> (arBuffer + sizeof(mHeader)),
                                                     aSize - sizeof(mHeader)));
            if (!p_asdu)
                return;

            CS101_ASDU_clone(p_asdu.get(), &mAsdu);
            mType = APDU_TYPE_I;
        }
    }

    Apdu::~Apdu()
    {
    }

    static int DecodeSequence(int lowByte, int highByte)
    {
        int result = (lowByte >> 1);
        result += (highByte << 8);
        return result;
    }

    bool Apdu::HasReceiveSequence() const noexcept
    {
        return mType == APDU_TYPE_I || mType == APDU_TYPE_S;
    }

    int Apdu::GetReceiveSequence() const noexcept
    {
        if (!HasReceiveSequence())
            return 0;

        return DecodeSequence(mHeader[4], mHeader[5]);
    }

    bool Apdu::HasSendSequence() const noexcept
    {
        return mType == APDU_TYPE_I;
    }

    int Apdu::GetSendSequence() const noexcept
    {
        if (!HasSendSequence())
            return 0;

        return DecodeSequence(mHeader[2], mHeader[3]);
    }

    bool Apdu::HasService() const noexcept
    {
        return mType == APDU_TYPE_U;
    }

    ServiceType Apdu::GetService() const noexcept
    {
        switch (mHeader[2])
        {
        case STARTDT_ACT:
        case STARTDT_CON:
        case STOPDT_ACT:
        case STOPDT_CON:
        case TESTFR_ACT:
        case TESTFR_CON:
            return static_cast<ServiceType> (mHeader[2]);
        default:
            return SVC_invalid;
        }
    }
}
