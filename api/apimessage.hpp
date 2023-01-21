#ifndef VRTU_APIMESSAGE_HPP
#define VRTU_APIMESSAGE_HPP

#include <stdexcept>

namespace VRTU
{
    class ApiMessage
    {
    public:
        enum Category
        {
            CAT_EVENT,
            CAT_REQUEST,
            CAT_RESPONSE,
        };

        virtual ~ApiMessage() {}

    protected:
        explicit ApiMessage(Category aCat) noexcept
            : mCat(aCat)
        {
        }

        bool isRequest()  const noexcept { return mCat == CAT_REQUEST; }
        bool isResponse() const noexcept { return mCat == CAT_RESPONSE; }
        bool isEvent()    const noexcept { return mCat == CAT_EVENT; }

    private:
        Category mCat;


    };
}

#endif // APIMESSAGE_HPP
