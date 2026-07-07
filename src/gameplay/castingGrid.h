//
// Created by Pd on 7/7/26.
//

#ifndef RAYLIB_GAME_TEMPLATE_CASTINGGRID_H
#define RAYLIB_GAME_TEMPLATE_CASTINGGRID_H

#include "raylib.h"
#include <cmath>
#include <vector>

class CastingGrid {
    Vector2 mousePos{};
    Color lineColor = SKYBLUE;
    Color lineColorError = RED;
    float lineLength = 40.0f;
    Vector2 targetPos{};
    bool longEnough = false;
    std::vector<Vector2> points;

public:
    CastingGrid(Vector2 _pos = Vector2()) {
        points.push_back(_pos);
    }

    void update() {
        Vector2 mouseAbsolute = GetMousePosition();


        if (points.size() < 2) {
            Vector2 dir = { mouseAbsolute.x - points.back().x,
                            mouseAbsolute.y - points.back().y };
            float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

            if (length >= lineLength) {
                dir.x = (dir.x / length) * lineLength;
                dir.y = (dir.y / length) * lineLength;
                targetPos = { points.back().x + dir.x, points.back().y + dir.y };
                longEnough = true;
            } else {
                targetPos = { points.back().x + dir.x, points.back().y + dir.y };
                longEnough = false;
            }
            return;
        }


        const Vector2& last = points[points.size() - 1];
        const Vector2& prev = points[points.size() - 2];


        Vector2 prevDir = { last.x - prev.x, last.y - prev.y };
        float prevLen = sqrtf(prevDir.x * prevDir.x + prevDir.y * prevDir.y);
        if (prevLen == 0.0f) {
            longEnough = false;
            targetPos = last;
            return;
        }
        prevDir.x /= prevLen;
        prevDir.y /= prevLen;


        Vector2 toMouse = { mouseAbsolute.x - last.x, mouseAbsolute.y - last.y };
        float mouseDist = sqrtf(toMouse.x * toMouse.x + toMouse.y * toMouse.y);
        Vector2 mouseDir = { 0.0f, 0.0f };
        if (mouseDist > 0.0f) {
            mouseDir.x = toMouse.x / mouseDist;
            mouseDir.y = toMouse.y / mouseDist;
        }


        const float cos60 = 0.5f;
        const float sin60 = 0.8660254037844386f;
        Vector2 leftDir = {
            prevDir.x * cos60 - prevDir.y * sin60,
            prevDir.x * sin60 + prevDir.y * cos60
        };
        Vector2 rightDir = {
            prevDir.x * cos60 + prevDir.y * sin60,
           -prevDir.x * sin60 + prevDir.y * cos60
        };


        float dotLeft  = mouseDir.x * leftDir.x  + mouseDir.y * leftDir.y;
        float dotRight = mouseDir.x * rightDir.x + mouseDir.y * rightDir.y;

        if (dotLeft  >  1.0f) dotLeft  =  1.0f;
        if (dotLeft  < -1.0f) dotLeft  = -1.0f;
        if (dotRight >  1.0f) dotRight =  1.0f;
        if (dotRight < -1.0f) dotRight = -1.0f;

        float angleLeft  = acosf(dotLeft)  * RAD2DEG;
        float angleRight = acosf(dotRight) * RAD2DEG;

        const float TOLERANCE = 10.0f;
        bool validAngle = false;
        Vector2 chosenDir = mouseDir;

        if (angleLeft <= TOLERANCE || angleRight <= TOLERANCE) {
            validAngle = true;
            if (angleLeft <= angleRight) {
                chosenDir = leftDir;
            } else {
                chosenDir = rightDir;
            }
        }

        float stepLength = (mouseDist < lineLength) ? mouseDist : lineLength;

        longEnough = (mouseDist >= lineLength && validAngle);

        targetPos = {
            last.x + chosenDir.x * stepLength,
            last.y + chosenDir.y * stepLength
        };
    }

    void render() {
        if (points.size() > 1) {
            for (int i = 0; i < points.size() - 1; i++) {
                DrawLineV(points[i], points[i + 1], lineColor);
            }
        }

        if (longEnough) {
            DrawLineV(points.back(), targetPos, lineColor);
        } else {
            DrawLineV(points.back(), targetPos, lineColorError);
        }
    }

    bool addPoint() {
        if (points.size() > 1 || longEnough) {
            if (longEnough)
                points.push_back(targetPos);
            return true;
        }
        return false;
    }
};

#endif // RAYLIB_GAME_TEMPLATE_CASTINGGRID_H

