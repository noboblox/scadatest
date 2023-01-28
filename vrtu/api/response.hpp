#ifndef VRTU_RESPONSE_HPP
#define VRTU_RESPONSE_HPP

#include "apimessage.hpp"

namespace VRTU
{
    class Response : public VRTU::ApiMessage
    {
    public:
        enum ResultType
        {
            RT_VOID,
            RT_INT,
        };

        explicit Response() noexcept
            : VRTU::ApiMessage(ApiMessage::CAT_RESPONSE),
              mError(0),
              mType(RT_VOID)
        {
        }

        explicit Response(std::size_t aError) noexcept
            : VRTU::ApiMessage(ApiMessage::CAT_RESPONSE),
              mError(aError),
              mType(RT_VOID)
        {
        }

        bool success() const noexcept { return mError == 0; }
        bool failure() const noexcept { return !success(); }

        void clear() noexcept { mType = RT_VOID; }
        void setInt(int value) noexcept
        {
            mResult.i = value;
            mType = RT_INT;
        }

        bool getInt(int& result) const noexcept
        {
            if (mType != RT_INT)
                return false;

            result = mResult.i;
            return true;
        }

    private:
        union Result
        {
            int i;
        };

        std::size_t mError;
        ResultType mType;
        Result mResult;
    };
}
#endif // RESPONSE_HPP
