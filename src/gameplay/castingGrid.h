#ifndef RAYLIB_GAME_TEMPLATE_CASTINGGRID_H
#define RAYLIB_GAME_TEMPLATE_CASTINGGRID_H

#include "raylib.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include "../enums.h"
#include <emscripten/emscripten.h>
#include "spells.h"
#include "../tools/soundManager.h"

class CastingGrid {
    Color lineColor = SKYBLUE;
    Color lineColorError = RED;
    Color lineColorGuard = LIGHTGRAY;
    Color lineColorGuardSecondary = GRAY;
    float lineLength = 40.0f;
    Vector2 targetPos{};
    bool previewValid = false;
    bool isOverlap = false;
    std::vector<Vector2> points;
    bool finished = false;
    Spells::Spell* spell{};

    int constraintX1, constraintY1, constraintX2, constraintY2;

    std::vector<std::vector<Vector2>> otherSpells;

    static bool onSegment(Vector2 p, Vector2 q, Vector2 r) {
        return q.x <= std::max(p.x, r.x) + 0.001f && q.x >= std::min(p.x, r.x) - 0.001f &&
               q.y <= std::max(p.y, r.y) + 0.001f && q.y >= std::min(p.y, r.y) - 0.001f;
    }

    static int orientation(Vector2 p, Vector2 q, Vector2 r) {
        float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (fabsf(val) < 0.001f) return 0;
        return (val > 0) ? 1 : 2;
    }

    static bool segmentsIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2) {
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false;
    }

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

    [[nodiscard]] bool doesEdgeIntersectOtherSpells(const Vector2& p1, const Vector2& p2) const {
        for (const auto& spellPoints : otherSpells) {
            if (spellPoints.size() < 2) continue;
            for (size_t i = 0; i < spellPoints.size() - 1; ++i) {
                if (segmentsIntersect(p1, p2, spellPoints[i], spellPoints[i+1]))
                    return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool isEdgeBlocked(const Vector2& p1, const Vector2& p2) const {
        return edgeExists(p1, p2) || doesEdgeIntersectOtherSpells(p1, p2);
    }

public:
    CastingGrid(Vector2 start, int x1, int y1, int x2, int y2)
        : constraintX1(std::min(x1, x2)), constraintY1(std::min(y1, y2)),
          constraintX2(std::max(x1, x2)), constraintY2(std::max(y1, y2))
    {
        if (!isInsideConstraint(start)) return;
        points.push_back(start);
        SoundManager::Play(SoundManager::SPELL_DRAW);
    }

    size_t getPointCount() const { return points.size(); }
    std::vector<Vector2> getPoints() const { return points; }
    Spells::Spell* getSpell() const { return spell; }
    bool isFinished() const { return finished; }
    bool isOverlapPreview() const { return isOverlap && previewValid; }

    bool isInsideConstraint(Vector2 p) const {
        return p.x >= constraintX1 && p.x <= constraintX2 &&
               p.y >= constraintY1 && p.y <= constraintY2;
    }

    void update(Vector2 mousePos, const std::vector<std::vector<Vector2>>& otherSpellsParam) {
        otherSpells = otherSpellsParam;

        if (finished) return;

        if (points.size() < 2) {
            Vector2 dir = { mousePos.x - points.back().x,
                            mousePos.y - points.back().y };
            float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

            if (length >= lineLength) {
                dir.x = dir.x / length * lineLength;
                dir.y = dir.y / length * lineLength;
                targetPos = { points.back().x + dir.x, points.back().y + dir.y };

                bool inside = isInsideConstraint(targetPos);
                bool externalOverlap = inside && doesEdgeIntersectOtherSpells(points.back(), targetPos);

                previewValid = inside && !externalOverlap;
                isOverlap = false;

                float distToTarget = sqrtf((mousePos.x - targetPos.x) * (mousePos.x - targetPos.x) +
                                           (mousePos.y - targetPos.y) * (mousePos.y - targetPos.y));
                if (previewValid && distToTarget <= lineLength * 0.5f) {
                    points.push_back(targetPos);
                    SoundManager::Play(SoundManager::SPELL_DRAW);
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

            if (!isInsideConstraint(candidate)) {
                previewValid = false;
                isOverlap = false;
                targetPos = candidate;
            } else {
                bool ownOverlap = edgeExists(last, candidate);
                bool externalOverlap = doesEdgeIntersectOtherSpells(last, candidate);

                if (externalOverlap) {
                    previewValid = false;
                    isOverlap = false;
                    targetPos = candidate;
                } else if (ownOverlap) {
                    previewValid = true;
                    isOverlap = true;
                    targetPos = candidate;
                } else {
                    previewValid = true;
                    isOverlap = false;
                    targetPos = candidate;

                    float distToTarget = sqrtf((mousePos.x - targetPos.x) * (mousePos.x - targetPos.x) +
                                               (mousePos.y - targetPos.y) * (mousePos.y - targetPos.y));
                    if (distToTarget <= lineLength * 0.5f) {
                        points.push_back(targetPos);
                        SoundManager::Play(SoundManager::SPELL_DRAW);
                    }
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
        SoundManager::Play(SoundManager::SPELL_DRAW);
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

            if (!finished && points.size() >= 2) {
                const Vector2& last = points.back();
                const Vector2& prev = points[points.size() - 2];
                Vector2 dir = { last.x - prev.x, last.y - prev.y };
                float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                if (len > 0.0f) {
                    dir.x /= len; dir.y /= len;
                    const float cos60 = 0.5f;
                    const float sin60 = 0.8660254037844386f;

                    Vector2 leftDir = {
                        dir.x * cos60 - dir.y * sin60,
                        dir.x * sin60 + dir.y * cos60
                    };
                    Vector2 rightDir = {
                        dir.x * cos60 + dir.y * sin60,
                       -dir.x * sin60 + dir.y * cos60
                    };

                    Vector2 leftEnd  = { last.x + leftDir.x  * lineLength,
                                         last.y + leftDir.y  * lineLength };
                    Vector2 rightEnd = { last.x + rightDir.x * lineLength,
                                         last.y + rightDir.y * lineLength };

                    if (!isEdgeBlocked(last, leftEnd) && isInsideConstraint(leftEnd))
                        DrawLineV(last, leftEnd, lineColorGuard);
                    if (!isEdgeBlocked(last, rightEnd) && isInsideConstraint(rightEnd))
                        DrawLineV(last, rightEnd, lineColorGuard);

                    if (!isEdgeBlocked(last, leftEnd) && isInsideConstraint(leftEnd)) {
                        Vector2 leftSecondaryLeft = {
                            leftDir.x * cos60 - leftDir.y * sin60,
                            leftDir.x * sin60 + leftDir.y * cos60
                        };
                        Vector2 leftSecondaryRight = {
                            leftDir.x * cos60 + leftDir.y * sin60,
                           -leftDir.x * sin60 + leftDir.y * cos60
                        };
                        Vector2 leftSecLeftEnd  = { leftEnd.x + leftSecondaryLeft.x  * lineLength,
                                                    leftEnd.y + leftSecondaryLeft.y  * lineLength };
                        Vector2 leftSecRightEnd = { leftEnd.x + leftSecondaryRight.x * lineLength,
                                                    leftEnd.y + leftSecondaryRight.y * lineLength };
                        if (!isEdgeBlocked(leftEnd, leftSecLeftEnd) && isInsideConstraint(leftSecLeftEnd))
                            DrawLineV(leftEnd, leftSecLeftEnd, lineColorGuardSecondary);
                        if (!isEdgeBlocked(leftEnd, leftSecRightEnd) && isInsideConstraint(leftSecRightEnd))
                            DrawLineV(leftEnd, leftSecRightEnd, lineColorGuardSecondary);
                    }

                    if (!isEdgeBlocked(last, rightEnd) && isInsideConstraint(rightEnd)) {
                        Vector2 rightSecondaryLeft = {
                            rightDir.x * cos60 - rightDir.y * sin60,
                            rightDir.x * sin60 + rightDir.y * cos60
                        };
                        Vector2 rightSecondaryRight = {
                            rightDir.x * cos60 + rightDir.y * sin60,
                           -rightDir.x * sin60 + rightDir.y * cos60
                        };
                        Vector2 rightSecLeftEnd  = { rightEnd.x + rightSecondaryLeft.x  * lineLength,
                                                     rightEnd.y + rightSecondaryLeft.y  * lineLength };
                        Vector2 rightSecRightEnd = { rightEnd.x + rightSecondaryRight.x * lineLength,
                                                     rightEnd.y + rightSecondaryRight.y * lineLength };
                        if (!isEdgeBlocked(rightEnd, rightSecLeftEnd) && isInsideConstraint(rightSecLeftEnd))
                            DrawLineV(rightEnd, rightSecLeftEnd, lineColorGuardSecondary);
                        if (!isEdgeBlocked(rightEnd, rightSecRightEnd) && isInsideConstraint(rightSecRightEnd))
                            DrawLineV(rightEnd, rightSecRightEnd, lineColorGuardSecondary);
                    }
                }

                Color c = previewValid ? lineColor : lineColorError;
                DrawLineV(points.back(), targetPos, c);
            }
        }
    }

    void cleanRender() const {
        if (points.size() < 2) return;
        for (size_t i = 0; i < points.size() - 1; i++) {
            DrawLineV(points[i], points[i + 1], lineColor);
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

        return (!isEdgeBlocked(last, leftTarget)  && isInsideConstraint(leftTarget)) ||
               (!isEdgeBlocked(last, rightTarget) && isInsideConstraint(rightTarget));
    }

    Spells::Spell* evaluate() {
        std::string str;
        for (auto move : getMoves()) {
            switch (move) {
                case CastingMoves::MOVE_LEFT:  str += "L"; break;
                case CastingMoves::MOVE_RIGHT: str += "R"; break;
            }
        }
        emscripten_log(0, str.c_str());

        static Spells spells;

        auto it = spells.spells.find(str);
        if (it != spells.spells.end()) {
            SoundManager::Play(SoundManager::SPELL_SUCCESS);
            spell = it->second->clone();
            return spell;
        }
        SoundManager::Play(SoundManager::SPELL_FAIL);
        return new Spells::Spell();
    }
};

#endif