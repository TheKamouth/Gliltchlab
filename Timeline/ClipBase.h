#ifndef ICLIP_H
#define ICLIP_H

#include <QUrl>

// rename file and class ClipBase
class ClipBase
{
public:
    ClipBase(QUrl filePath);

private:
    QUrl _fileUrl;
};

#endif // ICLIP_H
