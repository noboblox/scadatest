#ifndef VRTU_ID_HPP
#define VRTU_ID_HPP

#include <string>

namespace VRTU
{
    class Id
    {
    public:
        static Id createUUID();
        std::string toString() const noexcept;

        explicit Id();
        explicit Id(const std::string& arValue) : mId(arValue) {}
        explicit Id(std::string&& arValue) : mId(std::move(arValue)) {}

        bool isNull() const;
        bool operator==(const Id& o) const noexcept { return mId == o.mId; }
        bool operator!=(const Id& o) const noexcept { return mId != o.mId; }
        bool operator>=(const Id& o) const noexcept { return mId >= o.mId; }
        bool operator<=(const Id& o) const noexcept { return mId <= o.mId; }
        bool operator> (const Id& o) const noexcept { return mId >  o.mId; }
        bool operator< (const Id& o) const noexcept { return mId <  o.mId; }

    private:
        std::string mId;
    };

} // namespace VRTU

#endif // VRTU_ID_HPP
