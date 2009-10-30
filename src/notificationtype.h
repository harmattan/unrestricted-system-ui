#ifndef NOTIFICATIONTYPE_H
#define NOTIFICATIONTYPE_H

// temporary event-types for notifications, defined as "what I want".
// correct types aren't specified yet by duihome.
// type is always mapped to 'new-message' --> shows envelope icon in info banner.

namespace NotificationType {

enum Type
{
    error = 0,
    info,
    warning
};

}

#endif // NOTIFICATIONTYPE_H
