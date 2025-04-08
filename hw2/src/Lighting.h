//
// Created by William Zhao on 4/7/25.
// Phong Lighting Implementation
//

#ifndef LIGHTING_H
#define LIGHTING_H

#include "Math.h"


class Lighting {
public:
    Lighting();
    ~Lighting();
    void UpdateLightingUniform(class PipelineProgram* pipelineProgram, const float* modelViewMatrix);
    private:
        const Vector4 AMBIENT_COLOR = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
        const Vector4 DIFFUSE_COLOR = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
        const Vector4 SPECULAR_COLOR = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
        const Vector4 LIGHT_DIRECTION = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
        const Vector4 AMBIENT_REFLECTION_COEFFICIENT = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
        const Vector4 DIFFUSE_REFLECTION_COEFFICIENT = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
        const Vector4 SPECULAR_REFLECTION_COEFFICIENT = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
        const float ALPHA = 32.0f;
};



#endif //LIGHTING_H
