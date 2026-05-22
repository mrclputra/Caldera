#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <memory>
#include <string>
#include <QOpenGLFunctions_4_5_Core>
#include "engine/components/PointCloud.h"

class ModelLoader {
public:
    static std::shared_ptr<PointCloud> load(
        QOpenGLFunctions_4_5_Core* f,
        const std::string& path
    );
};

#endif // MODELLOADER_H