//
// Created by William Zhao on 4/4/25.
//
#ifndef CAMERA_H
#define CAMERA_H

#include "Spline.h"
#include "../../openGLHelper/openGLMatrix.h"

extern Vector3 cameraPosition;
extern Point cameraRollPitchYaw;
extern Point cameraTarget;
extern Point cameraUp;
extern float cameraSpeed;
extern float cameraSplinePosition;

void updateCameraPosition(float deltaTime);
#endif //CAMERA_H
