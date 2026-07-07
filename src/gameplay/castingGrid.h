#ifndef RAYLIB_GAME_TEMPLATE_CASTINGGRID_H
#define RAYLIB_GAME_TEMPLATE_CASTINGGRID_H

#include "raylib.h"
#include <cmath>
#include <vector>
#include "../enums.h"
#include <emscripten/emscripten.h>

class CastingGrid {
    Color lineColor = SKYBLUE;
    Color lineColorError = RED;
    float lineLength = 40.0f;
    Vector2 targetPos{};
    bool previewValid = false;
    bool isOverlap = false;
    std::vector<Vector2> points;
    bool finished = false;

    [[nodiscard]] bool edgeExists(const Vector2& p1, const Vector2& p2) const {
        if (points.size() < 2) return false;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            const Vector2& a = points[i];
            const Vector2& b = points[i+1];
            if ((fabsf(a.x - p1.x) < 2.0f && fabsf(a.y - p1.y) < 2.0f &&
                 fabsf(b.x - p2.x) < 2.0f && fabsf(b.y - p2.y) < 2.0f) ||
                (fabsf(a.x - p2.x) < 2.0f && fabsf(a.y - p2.y) < 2.0f &&
                 fabsf(b.x - p1.x) < 2.0f && fabsf(b.y - p1.y) < 2.0f))
                return true;
        }
        return false;
    }

public:
    CastingGrid(Vector2 start) {
        points.push_back(start);
    }

    size_t getPointCount() const { return points.size(); }
    bool isFinished() const { return finished; }
    bool isOverlapPreview() const { return isOverlap && previewValid; }

    void update(Vector2 mousePos) {
        if (finished) return;

        if (points.size() < 2) {
            Vector2 dir = { mousePos.x - points.back().x,
                            mousePos.y - points.back().y };
            float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

            if (length >= lineLength) {
                dir.x = dir.x / length * lineLength;
                dir.y = dir.y / length * lineLength;
                targetPos = { points.back().x + dir.x, points.back().y + dir.y };

                previewValid = true;
                isOverlap = false;

                float distToTarget = sqrtf((mousePos.x - targetPos.x) * (mousePos.x - targetPos.x) +
                                           (mousePos.y - targetPos.y) * (mousePos.y - targetPos.y));
                if (distToTarget <= lineLength * 0.5f) {
                    points.push_back(targetPos);
                }
            } else {
                targetPos = mousePos;
                previewValid = false;
                isOverlap = false;
            }
            return;
        }

        const Vector2& last = points.back();
        const Vector2& prev = points[points.size() - 2];

        Vector2 prevDir = { last.x - prev.x, last.y - prev.y };
        float prevLen = sqrtf(prevDir.x * prevDir.x + prevDir.y * prevDir.y);
        if (prevLen == 0.0f) {
            previewValid = false;
            isOverlap = false;
            targetPos = last;
            return;
        }
        prevDir.x /= prevLen;
        prevDir.y /= prevLen;

        Vector2 toMouse = { mousePos.x - last.x, mousePos.y - last.y };
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
            chosenDir = angleLeft <= angleRight ? leftDir : rightDir;
        }

        bool longEnough = mouseDist >= lineLength;
        previewValid = false;
        isOverlap = false;

        if (longEnough && validAngle) {
            Vector2 candidate = {
                last.x + chosenDir.x * lineLength,
                last.y + chosenDir.y * lineLength
            };

            bool overlaps = edgeExists(last, candidate);
            previewValid = true;
            isOverlap = overlaps;
            targetPos = candidate;

            if (!overlaps) {
                float distToTarget = sqrtf((mousePos.x - targetPos.x) * (mousePos.x - targetPos.x) +
                                           (mousePos.y - targetPos.y) * (mousePos.y - targetPos.y));
                if (distToTarget <= lineLength * 0.5f) {
                    points.push_back(targetPos);
                }
            }
        } else {
            float stepLength = longEnough ? lineLength : mouseDist;
            targetPos = {
                last.x + chosenDir.x * stepLength,
                last.y + chosenDir.y * stepLength
            };
            previewValid = false;
        }
    }

    void commitOverlap() {
        if (!isOverlap || !previewValid || finished) return;
        points.push_back(targetPos);
        finished = true;
    }

    void render() const {
        if (points.size() == 1) {
            Color c = previewValid ? lineColor : lineColorError;
            DrawLineV(points[0], targetPos, c);
        } else {
            for (size_t i = 0; i < points.size() - 1; i++) {
                DrawLineV(points[i], points[i + 1], lineColor);
            }
            if (!finished) {
                Color c = previewValid ? lineColor : lineColorError;
                DrawLineV(points.back(), targetPos, c);
            }
        }
    }

    std::vector<CastingMoves> getMoves() const {
        std::vector<CastingMoves> moves;
        if (points.size() < 3) return moves;

        for (size_t i = 2; i < points.size(); ++i) {
            const Vector2& p0 = points[i-2];
            const Vector2& p1 = points[i-1];
            const Vector2& p2 = points[i];

            Vector2 segA = { p1.x - p0.x, p1.y - p0.y };
            Vector2 segB = { p2.x - p1.x, p2.y - p1.y };

            float cross = segA.x * segB.y - segA.y * segB.x;
            moves.push_back(cross > 0 ? CastingMoves::MOVE_RIGHT : CastingMoves::MOVE_LEFT);
        }
        return moves;
    }

    bool hasValidMoves() const {
        if (points.size() < 2) return true;

        const Vector2& last = points.back();
        const Vector2& prev = points[points.size() - 2];

        Vector2 dir = { last.x - prev.x, last.y - prev.y };
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len == 0.0f) return false;
        dir.x /= len; dir.y /= len;

        const float cos60 = 0.5f;
        const float sin60 = 0.8660254037844386f;

        Vector2 leftTarget = {
            last.x + (dir.x * cos60 - dir.y * sin60) * lineLength,
            last.y + (dir.x * sin60 + dir.y * cos60) * lineLength
        };
        Vector2 rightTarget = {
            last.x + (dir.x * cos60 + dir.y * sin60) * lineLength,
            last.y + (-dir.x * sin60 + dir.y * cos60) * lineLength
        };

        return !edgeExists(last, leftTarget) || !edgeExists(last, rightTarget);
    }
};

inline void evaluate(const std::vector<CastingMoves>& moves) {
    emscripten_log(0,"Evaluate");
    std::string str;
    for (auto move : moves) {
        switch (move) {
            case CastingMoves::MOVE_LEFT:
                str += "L";
                break;
            case CastingMoves::MOVE_RIGHT:
                str += "R";
                break;
        }
    }
    emscripten_log(0,str.c_str());
}

#endif