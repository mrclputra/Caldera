#pragma once

#include <memory>
#include <string>
#include <QOpenGLFunctions_4_5_Core>
#include "engine/components/Object.h"

// i dont think this necessitates a class, maybe we can just make the function static --marcel
class ModelLoader {
public:
    static std::shared_ptr<Object> load(
        QOpenGLFunctions_4_5_Core* f,
        const std::string& path
    );
};