//
// Created by William Zhao on 4/4/25.
//

#include "Camera.h"
#include "Spline.h"
#include <iostream>

Point cameraPosition = {0.0f, 0.0f, 5.0f};
Point cameraRollPitchYaw = {0.0f, 0.0f, 0.0f};
Point cameraTarget = {0.0f, 0.0f, 0.0f};
Point cameraUp = {0.0f, 1.0f, 0.0f};
float cameraSpeed = 5.0f; // Speed of the camera movement
float cameraSplinePosition = 0.0f; // Current position on the spline

void updateCameraPosition(float deltaTime) {
    cameraSplinePosition += cameraSpeed * deltaTime;
    if (cameraSplinePosition >= spline.numPoints) {
        cameraSplinePosition = 0.0f;
    }
    Point p = getPointOnSpline(cameraSplinePosition);

    cameraPosition.x = p.x;
    cameraPosition.y = p.y;
    cameraPosition.z = p.z;
}