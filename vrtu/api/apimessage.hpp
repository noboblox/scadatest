#ifndef VRTU_APIMESSAGE_HPP
#define VRTU_APIMESSAGE_HPP

#include <functional>
#include <memory>

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
        bool isRequest()  const noexcept { return mCat == CAT_REQUEST; }
        bool isResponse() const noexcept { return mCat == CAT_RESPONSE; }
        bool isEvent()    const noexcept { return mCat == CAT_EVENT; }

    protected:
        explicit ApiMessage(Category aCat) noexcept
            : mCat(aCat)
        {
        }


    private:
        Category mCat;
    };

    using ApiMessageHandler = std::function<void(std::unique_ptr<const ApiMessage>&&)>;
}

#endif // APIMESSAGE_HPP
