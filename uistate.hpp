#ifndef UISTATE_HPP
#define UISTATE_HPP
#include <QObject>
#include "vrtu/id.hpp"

struct UiState
{
public:
    VRTU::Id server = VRTU::Id();
    std::string serverIp = "";
    int serverPort = 0;
};

#endif // UISTATE_HPP
