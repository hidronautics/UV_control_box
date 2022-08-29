#ifndef ISERVERDATA_H
#define ISERVERDATA_H

#include <QDataStream>
#include <QByteArray>

#include "ibasicdata.h"
#include "pultcontrolsystemprotocols.h"

class IServerData : public IBasicData {
public:
    IServerData();

    FromPult generateFullMessage();

    void parseFullMessage(ToPult message);
};

#endif // ISERVERDATA_H
