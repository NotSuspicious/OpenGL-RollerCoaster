//
// Created by William Zhao on 4/7/25.
//

#include "Lighting.h"
#include <OpenGL/OpenGL.h>
#include "../../openGLHelper/vbo.h"
#include "../../openGLHelper/vao.h"
#include "../../openGLHelper/pipelineProgram.h"
#include "Spline.h"

Lighting::Lighting() {
    // Constructor
}

Lighting::~Lighting() {
    // Destructor
}

void Lighting::UpdateLightingUniform(PipelineProgram* pipelineProgram, const float* modelViewMatrix) {
    //Calculate the light direction in view space.
    float viewLightOutput[4];
    MultiplyMatrices(4,4,1, modelViewMatrix, LIGHT_DIRECTION.data(), viewLightOutput);
    Vector3 viewLightDirection = Vector3(viewLightOutput[0], viewLightOutput[1], viewLightOutput[2]);
    pipelineProgram->SetUniformVariable3fv("viewLightDirection", viewLightDirection.data());

    pipelineProgram->SetUniformVariable4fv("La", AMBIENT_COLOR.data());
    pipelineProgram->SetUniformVariable4fv("Ld", DIFFUSE_COLOR.data());
    pipelineProgram->SetUniformVariable4fv("Ls", SPECULAR_COLOR.data());

    pipelineProgram->SetUniformVariable4fv("ka", AMBIENT_REFLECTION_COEFFICIENT.data());
    pipelineProgram->SetUniformVariable4fv("kd", DIFFUSE_REFLECTION_COEFFICIENT.data());
    pipelineProgram->SetUniformVariable4fv("ks", SPECULAR_REFLECTION_COEFFICIENT.data());
    pipelineProgram->SetUniformVariablef("alpha", ALPHA);


}