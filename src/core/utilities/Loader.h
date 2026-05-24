#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>
#include <memory>
#include <QString>
#include <QThread>

#include "engine/components/PointCloud.h"

std::vector<PointCloud::Point> loadPoints(const std::string& path);
std::shared_ptr<PointCloud> loadPointCloud(QOpenGLFunctions_4_5_Core* f, const std::string& path);

class LoaderThread : public QThread {
    Q_OBJECT
public:
    explicit LoaderThread(QObject* parent = nullptr) : QThread(parent) {}
    QString path;

signals:
    void done(std::vector<PointCloud::Point> points);

protected:
    void run() override {
        emit done(loadPoints(path.toStdString()));
    }
};

#endif // MODELLOADER_H
