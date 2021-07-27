#ifndef USERDATAMANAGER_H
#define USERDATAMANAGER_H

#include <QObject>

class UserDataManager
{
public:
    UserDataManager();
    UserDataManager(UserDataManager const &) = delete;
    void operator=(UserDataManager const &) = delete;

    static UserDataManager & Instance()
    {
        static UserDataManager instance;
        return instance;
    }



private:

};

#endif // USERDATAMANAGER_H
