//
// Created by Pd on 7/7/26.
//

#ifndef RAYLIB_GAME_TEMPLATE_SPELLS_H
#define RAYLIB_GAME_TEMPLATE_SPELLS_H

#include <map>
#include <string>

class Spells {
public:
    class Spell {
    public:
        virtual ~Spell() = default;
        virtual std::string getName() const { return "Invalid Spell"; }
        virtual Spell* clone() const { return new Spell(*this); }
    };

    class SPELL_FIRE : public Spell {
    public:
        std::string getName() const override { return "Fire Spell"; }
        Spell* clone() const override { return new SPELL_FIRE(*this); }
    };

    const std::map<std::string, Spell*> spells = {
        {"RRRRR", new SPELL_FIRE()},
        {"LLLLL", new SPELL_FIRE()}
    };

    ~Spells() {
        for (auto& pair : spells) {
            delete pair.second;
        }
    }
};

#endif //RAYLIB_GAME_TEMPLATE_SPELLS_H