//
// Created by William Zhao on 4/4/25.
//

#include "Camera.h"
#include "Spline.h"
#include <iostream>

Point cameraPosition = {0.0f, 0.0f, 0.0f};
Point cameraRollPitchYaw = {0.0f, 0.0f, 0.0f};
Point cameraTarget = {0.0f, 0.0f, 0.0f};
Point cameraUp = {0.0f, 1.0f, 0.0f};
float cameraSpeed = 0.5f; // Speed of the camera movement
float cameraSplinePosition = 0.0f; // Current position on the spline

void updateCameraPosition(float deltaTime) {
    cameraSplinePosition += cameraSpeed * deltaTime;
    float cameraSplineNextPos = cameraSplinePosition+0.4f;
    if (cameraSplinePosition >= spline.numPoints) {
        cameraSplinePosition = 0.0f;
    }
    if (cameraSplineNextPos >= spline.numPoints) {
        cameraSplineNextPos = cameraSplinePosition;
    }
    int cameraSplineIndex = static_cast<int>(cameraSplinePosition);

    Vector3 p = getPointOnSpline(cameraSplinePosition);
	Vector3 tangent = splineTangents[getSplineIndex(cameraSplinePosition)];
    Vector3 pNext = getNextPointOnSpline(cameraSplineNextPos);

    cameraPosition.x = p.x;
    cameraPosition.y = p.y;
    cameraPosition.z = p.z;

    cameraTarget.x = p.x + tangent.x;
    cameraTarget.y = p.y + tangent.y;
    cameraTarget.z = p.z + tangent.z;

    cameraUp.x = splineCrossSections[cameraSplineIndex].normal.x;
    cameraUp.y = splineCrossSections[cameraSplineIndex].normal.y;
    cameraUp.z = splineCrossSections[cameraSplineIndex].normal.z;
}