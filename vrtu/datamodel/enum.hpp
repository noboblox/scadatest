#ifndef VRTU_ENUM_HPP
#define VRTU_ENUM_HPP

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

namespace VRTU
{
    template <typename E, E InvalidValue>
    class Enum
    {
    public:
        Enum() = delete;

        struct Entry
        {
            E value;
            const char* label;
        };

        using Definition = std::vector<Entry>;

        static const char* Label(E arValue) noexcept
        {
            auto it = std::find_if(mDefinition.cbegin(),  mDefinition.cend(),
                                   [arValue](const auto& e){ return e.value == arValue; });

            return it != mDefinition.cend() ? it->label : "unknonw";
        }

        static E Value(const char* arLabel) noexcept
        {
            auto it = std::find_if(mDefinition.cbegin(),  mDefinition.cend(),
                                   [arLabel](const auto& e){ return !std::strcmp(e.label, arLabel); });

            return it != mDefinition.cend() ? it->value : InvalidValue;
        }

        static inline bool IsValid(E arValue) noexcept
        {
            return arValue != InvalidValue;
        }

        static inline E Value(const std::string& arLabel) noexcept
        {
            return Value(arLabel.c_str());
        }

    private:
        static const Definition mDefinition;
    };

} // namespace VRTU

#endif // VRTU_ENUM_HPP
