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
    pipelineProgram->Bind();

}