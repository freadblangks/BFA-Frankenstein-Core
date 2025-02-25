/*
 * Copyright (C) 2020 BfaCore
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
* Scripts for spells with SPELLFAMILY_HUNTER, SPELLFAMILY_PET and SPELLFAMILY_GENERIC spells used by hunter players.
* Ordered alphabetically using scriptname.
* Scriptnames of files in this file should be prefixed with "spell_hun_".
*/

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Creature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "PetPackets.h"
#include "PhasingHandler.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Unit.h"

enum HunterSpells
{
    SPELL_HUNTER_A_MURDER_OF_CROWS_1 = 131894,
    SPELL_HUNTER_A_MURDER_OF_CROWS_2 = 206505,
    SPELL_HUNTER_A_MURDER_OF_CROWS_DAMAGE = 131900,
    SPELL_HUNTER_ANIMAL_INSTINCTS = 204315,
    SPELL_HUNTER_ANIMAL_INSTINCTS_CHEETAH = 204324,
    SPELL_HUNTER_ANIMAL_INSTINCTS_MONGOOSE = 204333,
    SPELL_HUNTER_ANIMAL_INSTINCTS_RAPTOR = 204321,
    SPELL_HUNTER_ARCANE_SHOT = 185358,
    SPELL_HUNTER_ASPECT_OF_THE_CHEETAH_EFFECT_2 = 186258,
    SPELL_HUNTER_ASPECT_OF_THE_EAGLE = 186289,
    SPELL_HUNTER_AUTO_SHOT = 75,
    SPELL_HUNTER_BARRAGE = 120360,
    SPELL_HUNTER_BASIC_ATTACK_COST_MODIFIER = 62762,
    SPELL_HUNTER_BEAST_CLEAVE_AURA = 115939,
    SPELL_HUNTER_BEAST_CLEAVE_DAMAGE = 118459,
    SPELL_HUNTER_BEAST_CLEAVE_PROC = 118455,
    SPELL_HUNTER_BESTIAL_WRATH = 19574,
    SPELL_HUNTER_BLACK_ARROW = 194599,
    SPELL_HUNTER_BLINK_STRIKES = 130392,
    SPELL_HUNTER_BLINK_STRIKES_TELEPORT = 130393,
    SPELL_HUNTER_CAMOUFLAGE = 199483,
    SPELL_HUNTER_COBRA_SHOT = 193455,
    SPELL_HUNTER_DIRE_BEAST_GENERIC = 120679,
    SPELL_HUNTER_DIRE_FRENZY = 217200,
    SPELL_HUNTER_DIRE_FRENZY_DAMAGE = 217207,
    SPELL_HUNTER_DISENGAGE = 781,
    SPELL_HUNTER_EXHILARATION = 109304,
    SPELL_HUNTER_EXHILARATION_PET = 128594,
    SPELL_HUNTER_EXHILARATION_PET_AURA = 231546,
    SPELL_HUNTER_EXPLOSIVE_SHOT = 212431,
    SPELL_HUNTER_EXPLOSIVE_SHOT_DAMAGE = 212680,
    SPELL_HUNTER_EXPLOSIVE_SHOT_DETONATE = 212679,
    SPELL_HUNTER_FLANKING_STRIKE = 202800,
    SPELL_HUNTER_FLANKING_STRIKE_PROC = 204740,
    SPELL_HUNTER_FLANKING_STRIKE_PROC_UP = 206933,
    SPELL_HUNTER_FLARE_EFFECT = 28822,
    SPELL_HUNTER_FRENZY_STACKS = 19615,
    SPELL_HUNTER_HARPOON = 190925,
    SPELL_HUNTER_HARPOON_ROOT = 190927,
    SPELL_HUNTER_HUNTERS_MARK = 185987,
    SPELL_HUNTER_HUNTERS_MARK_AURA = 185365, //This one is the graphics (The arrow effect)
    SPELL_HUNTER_HUNTERS_MARK_AURA_2 = 185743, //This one is enabling "Marked Shot"
    SPELL_HUNTER_IMPROVED_MEND_PET = 24406,
    SPELL_HUNTER_INTIMIDATION_STUN = 24394,
    SPELL_HUNTER_INVIGORATION_TRIGGERED = 53398,
    SPELL_HUNTER_KILL_COMMAND = 34026,
    SPELL_HUNTER_KILL_COMMAND_CHARGE = 118171,
    SPELL_HUNTER_KILL_COMMAND_TRIGGER = 83381,
    SPELL_HUNTER_LACERATE = 185855,
    SPELL_HUNTER_LONE_WOLF = 155228,
    SPELL_HUNTER_MARKED_SHOT = 185901,
    SPELL_HUNTER_MARKED_SHOT_DAMAGE = 212621,
    SPELL_HUNTER_MARKING_TARGETS = 223138,
    SPELL_HUNTER_MASTERS_CALL_TRIGGERED = 62305,
    SPELL_HUNTER_MISDIRECTION = 34477,
    SPELL_HUNTER_MISDIRECTION_PROC = 35079,
    SPELL_HUNTER_MONGOOSE_BITE = 190928,
    SPELL_HUNTER_MONGOOSE_FURY = 190931,
    SPELL_HUNTER_MULTISHOT = 2643,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX = 55709,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF = 55711,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED = 54114,
    SPELL_HUNTER_PET_LAST_STAND_TRIGGERED = 53479,
    SPELL_HUNTER_POSTHAST = 109215,
    SPELL_HUNTER_POSTHAST_SPEED = 118922,
    SPELL_HUNTER_RANGERS_NET_INCREASE_SPEED = 206755,
    SPELL_HUNTER_RAPTOR_STRIKE = 186270,
    SPELL_HUNTER_SENTINEL = 206817,
    SPELL_HUNTER_SERPENT_STING = 87935,
    SPELL_HUNTER_SERPENT_STING_DAMAGE = 118253,
    SPELL_HUNTER_SPIKED_COLLAR = 53184,
    SPELL_HUNTER_STEADY_FOCUS = 193533,
    SPELL_HUNTER_STEADY_FOCUS_PROC = 193534,
    SPELL_HUNTER_STICKY_BOMB_PROC = 191244,
    SPELL_HUNTER_THOWING_AXES_DAMAGE = 200167,
    SPELL_HUNTER_TRAILBLAZER = 199921,
    SPELL_HUNTER_TRAILBLAZER_BUFF = 231390,
    SPELL_HUNTER_VULNERABLE = 187131,
    SPELL_HUNTER_WILD_CALL_AURA = 185791,
    SPELL_HUNTER_GENERIC_ENERGIZE_FOCUS = 91954,
    SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED = 54045,
    SPELL_HUNTER_READINESS = 23989,
    SPELL_DRAENEI_GIFT_OF_THE_NAARU = 59543,
    SPELL_HUNTER_FIRE = 82926,
    SPELL_ROAR_OF_SACRIFICE_TRIGGERED = 67481,
    SPELL_HUNTER_SNIPER_TRAINING_R1 = 53302,
    SPELL_HUNTER_SNIPER_TRAINING_BUFF_R1 = 64418,
    SPELL_HUNTER_STEADY_SHOT_FOCUS = 77443,
    SPELL_HUNTER_LOCK_AND_LOAD = 56453,
    SPELL_HUNTER_GLAIVE_TOSS_AURA = 117050,
    SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT = 120761,
    SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT = 121414,
    SPELL_HUNTER_GLAIVE_TOSS_RIGHT = 120755,
    SPELL_HUNTER_GLAIVE_TOSS_LEFT = 120756,
    SPELL_HUNTER_GLYPH_OF_STAMPEDE = 57902,
    SPELL_HUNTER_STAMPEDE_DAMAGE_REDUCTION = 130201,
    SPELL_HUNTER_BOND_SPIRIT_HEAL = 149254,
    SPELL_HUNTER_FOCUSING_SHOT = 152245,
    SPELL_HUNTER_FOCUSING_SHOT_AIM = 163485,
    SPELL_HUNTER_STEADY_FOCUS_TRIGGERED = 177668,
    SPELL_HUNTER_STEADY_SHOT = 56641,
    SPELL_HUNTER_BLINK_STRIKE = 130392,
    SPELL_HUNTER_BLINK_STRIKE_TELEPORT = 130393,
    SPELL_HUNTER_AURA_SHOOTING = 224729,
    SPELL_HUNTER_AIMED_SHOT = 19434,
    SPELL_HUNTER_GLYPH_OF_MISDIRECTION = 56829,
    //8.0
    SPELL_HUNTER_SCORCHING_WILDFIRE = 259496,
    SPELL_HUNTER_RAPID_FIRE = 257044,
    SPELL_HUNTER_RAPID_FIRE_MISSILE = 257045,
    SPELL_HUNTER_LETHAL_SHOTS = 260393,
    SPELL_HUNTER_CALLING_THE_SHOTS = 260404,
    SPELL_HUNTER_TRUESHOT = 288613,

};

enum AncientHysteriaSpells
{
    SPELL_MAGE_TEMPORAL_DISPLACEMENT = 80354,
    SPELL_SHAMAN_EXHAUSTION = 57723,
    SPELL_SHAMAN_SATED = 57724,
    SPELL_HUNTER_INSANITY = 95809
};

enum DireBeastSpells
{
    DIRE_BEAST_DREAD_WASTES = 126216,
    DIRE_BEAST_DUNGEONS = 132764,
    DIRE_BEAST_EASTERN_KINGDOMS = 122804,
    DIRE_BEAST_JADE_FOREST = 121118,
    DIRE_BEAST_KALIMDOR = 122802,
    DIRE_BEAST_KRASARANG_WILDS = 122809,
    DIRE_BEAST_KUN_LAI_SUMMIT = 126214,
    DIRE_BEAST_NORTHREND = 122807,
    DIRE_BEAST_OUTLAND = 122806,
    DIRE_BEAST_TOWNLONG_STEPPES = 126215,
    DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM = 126213,
    DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS = 122811,
};

// Harpoon - 190925
class spell_hun_harpoon : public SpellScriptLoader
{
public:
    spell_hun_harpoon() : SpellScriptLoader("spell_hun_harpoon") { }

    class spell_hun_harpoon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_harpoon_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_HARPOON) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_HARPOON_ROOT))
                return false;
            return true;
        }

        void CastHarpoon()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetExplTargetUnit();

            if (!player || !target)
                return;

            player->CastSpell(target, SPELL_HUNTER_HARPOON_ROOT, true);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetExplTargetUnit();

            if (!player || !target)
                return;

            WorldLocation pTarget = target->GetWorldLocation();

            float speedXY, speedZ;
            speedZ = 1.8f;
            speedXY = player->GetExactDist2d(&pTarget) * 10.0f / speedZ;
            player->GetMotionMaster()->MoveJump(pTarget, speedXY, speedZ, EVENT_JUMP);
        }

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasSpell(SPELL_HUNTER_POSTHAST))
                    player->CastSpell(player, SPELL_HUNTER_POSTHAST_SPEED, true);
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_hun_harpoon_SpellScript::CastHarpoon);
            OnEffectHitTarget += SpellEffectFn(spell_hun_harpoon_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            AfterCast += SpellCastFn(spell_hun_harpoon_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_harpoon_SpellScript();
    }
};

// Snake Hunter - 201078
class spell_hun_snake_hunter : public SpellScriptLoader
{
public:
    spell_hun_snake_hunter() : SpellScriptLoader("spell_hun_snake_hunter") { }

    class spell_hun_snake_hunter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_snake_hunter_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();

            if (!player)
                return;

            player->GetSpellHistory()->ResetCharges(sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE)->ChargeCategoryId);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_snake_hunter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_snake_hunter_SpellScript();
    }
};

// Way of the Mok'nathal - 201082
class spell_hun_way_of_the_moknathal : public SpellScriptLoader
{
public:
    spell_hun_way_of_the_moknathal() : SpellScriptLoader("spell_hun_way_of_the_moknathal") {}

    class spell_hun_way_of_the_moknathal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_way_of_the_moknathal_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_RAPTOR_STRIKE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_RAPTOR_STRIKE)
                return true;
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_way_of_the_moknathal_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_way_of_the_moknathal_AuraScript();
    }
};

// Mongoose Bite - 190928
class spell_hun_mongoose_bite : public SpellScriptLoader
{
public:
    spell_hun_mongoose_bite() : SpellScriptLoader("spell_hun_mongoose_bite") { }

    class spell_hun_mongoose_bite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_mongoose_bite_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE))
                return false;
            return true;
        }

        void ApplyBuff()
        {
            int32 dur = 0;
            if (Aura* aur = GetCaster()->GetAura(SPELL_HUNTER_MONGOOSE_FURY))
                dur = aur->GetDuration();

            GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_MONGOOSE_FURY, true);

            if (Aura* aur = GetCaster()->GetAura(SPELL_HUNTER_MONGOOSE_FURY))
                if (dur)
                    aur->SetDuration(dur);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_hun_mongoose_bite_SpellScript::ApplyBuff);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_mongoose_bite_SpellScript();
    }
};

// 90355 - Ancient Hysteria
class spell_hun_ancient_hysteria : public SpellScriptLoader
{
public:
    spell_hun_ancient_hysteria() : SpellScriptLoader("spell_hun_ancient_hysteria") { }

    class spell_hun_ancient_hysteria_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_ancient_hysteria_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_INSANITY)
                || !sSpellMgr->GetSpellInfo(SPELL_MAGE_TEMPORAL_DISPLACEMENT)
                || !sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EXHAUSTION)
                || !sSpellMgr->GetSpellInfo(SPELL_SHAMAN_SATED))
                return false;
            return true;
        }

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
        }

        void ApplyDebuff()
        {
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, SPELL_HUNTER_INSANITY, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_ancient_hysteria_SpellScript::RemoveInvalidTargets, EFFECT_ALL, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_hun_ancient_hysteria_SpellScript::ApplyDebuff);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_ancient_hysteria_SpellScript();
    }
};

// 53271 - Masters Call
class spell_hun_masters_call : public SpellScriptLoader
{
public:
    spell_hun_masters_call() : SpellScriptLoader("spell_hun_masters_call") { }

    class spell_hun_masters_call_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_masters_call_SpellScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MASTERS_CALL_TRIGGERED) ||
                !sSpellMgr->GetSpellInfo(spellInfo->GetEffect(EFFECT_0)->CalcValue()))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* ally = GetHitUnit())
                if (Player* caster = GetCaster()->ToPlayer())
                    if (Pet* target = caster->GetPet())
                    {
                        TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                        target->CastSpell(ally, GetEffectValue(), castMask);
                    }
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                // Cannot be processed while pet is dead
                TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                target->CastSpell(target, SPELL_HUNTER_MASTERS_CALL_TRIGGERED, castMask);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_masters_call_SpellScript();
    }
};

// 34477 - Misdirection
class spell_hun_misdirection : public SpellScriptLoader
{
public:
    spell_hun_misdirection() : SpellScriptLoader("spell_hun_misdirection") { }

    class spell_hun_misdirection_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_misdirection_AuraScript);

        bool _hasGlyph;
        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_HUNTER_MISDIRECTION_PROC });
        }

        void OnApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
            {
                _hasGlyph = false;
                return;
            }

            _hasGlyph = false;

            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetTarget())
                    if (Pet* pet = _player->GetPet())
                        if (pet->GetGUID() == target->GetGUID())
                            if (_player->HasAura(SPELL_HUNTER_GLYPH_OF_MISDIRECTION))
                                _hasGlyph = true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
                return;

            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (!GetDuration())
                {
                    _player->ResetRedirectThreat();

                    if (_hasGlyph)
                    {
                        if (_player->GetSpellHistory()->HasCooldown(SPELL_HUNTER_MISDIRECTION))
                            _player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_MISDIRECTION);
                        if (_player->GetSpellHistory()->HasCooldown(SPELL_HUNTER_MISDIRECTION_PROC))
                            _player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_MISDIRECTION_PROC);
                    }
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_misdirection_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_misdirection_AuraScript();
    }
};

// 35079 - Misdirection (Proc)
class spell_hun_misdirection_proc : public SpellScriptLoader
{
public:
    spell_hun_misdirection_proc() : SpellScriptLoader("spell_hun_misdirection_proc") { }

    class spell_hun_misdirection_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_misdirection_proc_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->ResetRedirectThreat();
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_proc_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_misdirection_proc_AuraScript();
    }
};

// 1515 - Tame Beast
class spell_hun_tame_beast : public SpellScriptLoader
{
public:
    spell_hun_tame_beast() : SpellScriptLoader("spell_hun_tame_beast") { }

    class spell_hun_tame_beast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_tame_beast_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster->IsPlayer())
                return SPELL_FAILED_DONT_REPORT;

            if (!GetExplTargetUnit())
                return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

            if (!caster->ToPlayer()->GetFirstUnusedActivePetSlot())
                return SPELL_FAILED_DONT_REPORT;

            if (Creature* target = GetExplTargetUnit()->ToCreature())
            {
                if (target->getLevel() > caster->getLevel())
                    return SPELL_FAILED_HIGHLEVEL;

                // use SMSG_PET_TAME_FAILURE?
                if (!target->GetCreatureTemplate()->IsTameable(caster->ToPlayer()->CanTameExoticPets()))
                    return SPELL_FAILED_BAD_TARGETS;

                if (!caster->GetPetGUID().IsEmpty())
                    return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                if (!caster->GetCharmGUID().IsEmpty())
                    return SPELL_FAILED_ALREADY_HAVE_CHARM;
            }
            else
                return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_tame_beast_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_tame_beast_SpellScript();
    }
};

// 53209 - chimera Shot
class spell_hun_chimera_shot : public SpellScriptLoader
{
public:
    spell_hun_chimera_shot() : SpellScriptLoader("spell_hun_chimera_shot") { }

    class spell_hun_chimera_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_chimera_shot_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (urand(0, 1) == 1)
                caster->CastSpell(target, 171454, true);
            else
                caster->CastSpell(target, 171457, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_chimera_shot_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_chimera_shot_SpellScript();
    }
};

// A Murder of Crows (Beast Mastery, Marksmanship) - 131894 and A Murder of Crows (Survival) - 206505
class spell_hun_a_murder_of_crows : public SpellScriptLoader
{
public:
    spell_hun_a_murder_of_crows() : SpellScriptLoader("spell_hun_a_murder_of_crows") { }

    class spell_hun_a_murder_of_crows_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_a_murder_of_crows_AuraScript);

        void OnTick(const AuraEffect* aurEff)
        {
            if (Unit* target = GetTarget())
            {
                if (aurEff->GetTickNumber() > 15)
                    return;

                if (Player* player = GetCaster()->ToPlayer())
                {
                    player->CastSpell(target, SPELL_HUNTER_A_MURDER_OF_CROWS_DAMAGE, true);
                }
            }
        }

        void HandleAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
                return;

            if (Unit* caster = GetCaster())
            {
                uint32 spec = caster->ToPlayer()->GetSpecializationId();

                if (spec == TALENT_SPEC_HUNTER_BEASTMASTER || spec == TALENT_SPEC_HUNTER_MARKSMAN)
                {
                    caster->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_A_MURDER_OF_CROWS_1, true);
                }
                if (spec == TALENT_SPEC_HUNTER_SURVIVAL)
                {
                    caster->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_A_MURDER_OF_CROWS_2, true);
                }
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_a_murder_of_crows_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_a_murder_of_crows_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_a_murder_of_crows_AuraScript();
    }
};

// Barrage - 120361
class spell_hun_barrage : public SpellScriptLoader
{
public:
    spell_hun_barrage() : SpellScriptLoader("spell_hun_barrage") { }

    class spell_hun_barrage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_barrage_SpellScript);

        void CheckLOS(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            Unit* caster = GetCaster();
            if (!caster)
                return;

            targets.remove_if([caster](WorldObject* objects) -> bool
            {
                if (!objects)
                    return true;

                if (!objects->IsWithinLOSInMap(caster))
                    return true;

                if (objects->ToUnit() && !caster->IsValidAttackTarget(objects->ToUnit()))
                    return true;

                return false;
            });
        }

        void HandleOnHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();

            if (!player || !target)
                return;

            float minDamage = 0.0f;
            float maxDamage = 0.0f;

            player->CalculateMinMaxDamage(RANGED_ATTACK, true, true, minDamage, maxDamage);

            int32 dmg = (minDamage + maxDamage) / 2 * 0.8f;

            if (!target->HasAura(SPELL_HUNTER_BARRAGE, player->GetGUID()))
                dmg /= 2;

            dmg = player->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(player, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            // Barrage now deals only 80% of normal damage against player-controlled targets.
            if (target->GetSpellModOwner())
                dmg = CalculatePct(dmg, 80);

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_barrage_SpellScript::CheckLOS, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_barrage_SpellScript::CheckLOS, EFFECT_1, TARGET_UNIT_CONE_ENEMY_24);
            OnHit += SpellHitFn(spell_hun_barrage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_barrage_SpellScript();
    }
};

// Beast Cleave - 115939
// Called by Multi-Shot - 2643
class spell_hun_beast_cleave : public SpellScriptLoader
{
public:
    spell_hun_beast_cleave() : SpellScriptLoader("spell_hun_beast_cleave") { }

    class spell_hun_beast_cleave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_beast_cleave_SpellScript);

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (player->HasAura(SPELL_HUNTER_BEAST_CLEAVE_AURA))
                    if (Pet* pet = player->GetPet())
                        player->CastSpell(pet, SPELL_HUNTER_BEAST_CLEAVE_PROC, true);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_hun_beast_cleave_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_beast_cleave_SpellScript();
    }
};

// Beast Cleave Proc - 118455
class spell_hun_beast_cleave_proc : public SpellScriptLoader
{
public:
    spell_hun_beast_cleave_proc() : SpellScriptLoader("spell_hun_beast_cleave_proc") { }

    class spell_hun_beast_cleave_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_beast_cleave_proc_AuraScript);

        void OnProc(const AuraEffect* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster())
                return;

            if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                return;

            if (eventInfo.GetDamageInfo()->GetSpellInfo() && eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_HUNTER_BEAST_CLEAVE_DAMAGE)
                return;

            if (Player* player = GetCaster()->ToPlayer())
            {
                if (GetTarget()->HasAura(aurEff->GetSpellInfo()->Id, player->GetGUID()))
                {
                    int32 bp = int32(eventInfo.GetDamageInfo()->GetDamage() * 0.75f);

                    GetTarget()->CastCustomSpell(GetTarget(), SPELL_HUNTER_BEAST_CLEAVE_DAMAGE, &bp, nullptr, nullptr, true);
                }
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_hun_beast_cleave_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_beast_cleave_proc_AuraScript();
    }
};

// Dire Beast - 120679
class spell_hun_dire_beast : public SpellScriptLoader
{
public:
    spell_hun_dire_beast() : SpellScriptLoader("spell_hun_dire_beast") { }

    class spell_hun_dire_beast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_dire_beast_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    // Summon's skin is different function of Map or Zone ID
                    switch (player->GetZoneId())
                    {
                    case 5785: // The Jade Forest
                        player->CastSpell(target, DIRE_BEAST_JADE_FOREST, true);
                        break;
                    case 5805: // Valley of the Four Winds
                        player->CastSpell(target, DIRE_BEAST_VALLEY_OF_THE_FOUR_WINDS, true);
                        break;
                    case 5840: // Vale of Eternal Blossoms
                        player->CastSpell(target, DIRE_BEAST_VALE_OF_THE_ETERNAL_BLOSSOM, true);
                        break;
                    case 5841: // Kun-Lai Summit
                        player->CastSpell(target, DIRE_BEAST_KUN_LAI_SUMMIT, true);
                        break;
                    case 5842: // Townlong Steppes
                        player->CastSpell(target, DIRE_BEAST_TOWNLONG_STEPPES, true);
                        break;
                    case 6134: // Krasarang Wilds
                        player->CastSpell(target, DIRE_BEAST_KRASARANG_WILDS, true);
                        break;
                    case 6138: // Dread Wastes
                        player->CastSpell(target, DIRE_BEAST_DREAD_WASTES, true);
                        break;
                    default:
                    {
                        switch (player->GetMapId())
                        {
                        case 0: // Eastern Kingdoms
                            player->CastSpell(target, DIRE_BEAST_EASTERN_KINGDOMS, true);
                            break;
                        case 1: // Kalimdor
                            player->CastSpell(target, DIRE_BEAST_KALIMDOR, true);
                            break;
                        case 8: // Outland
                            player->CastSpell(target, DIRE_BEAST_OUTLAND, true);
                            break;
                        case 10: // Northrend
                            player->CastSpell(target, DIRE_BEAST_NORTHREND, true);
                            break;
                        default:
                            if (player->GetMap()->IsDungeon())
                                player->CastSpell(target, DIRE_BEAST_DUNGEONS, true);
                            else // Default
                                player->CastSpell(target, DIRE_BEAST_KALIMDOR, true);
                            break;
                        }
                        break;
                    }
                    }
                }
            }
        }

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasAura(SPELL_HUNTER_WILD_CALL_AURA))
                    player->RemoveAurasDueToSpell(SPELL_HUNTER_WILD_CALL_AURA);
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_dire_beast_SpellScript::HandleOnHit);
            AfterCast += SpellCastFn(spell_hun_dire_beast_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_dire_beast_SpellScript();
    }
};

// Kill Command - 34026
class spell_hun_kill_command : public SpellScriptLoader
{
public:
    spell_hun_kill_command() : SpellScriptLoader("spell_hun_kill_command") { }

    class spell_hun_kill_command_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_kill_command_SpellScript);
        enum sspell {
            AnimalInstinctsReduction = 232646,
            AspectoftheBeast = 191384,
            BestialFerocity = 191413,
            BestialTenacity = 191414,
            BestialCunning = 191397,
            SpikedCollar = 53184,
            GreatStamina = 61688,
            Cornered = 53497
        };
        bool Validate(SpellInfo const* /*SpellEntry*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_KILL_COMMAND))
                return false;
            return true;
        }

        SpellCastResult CheckCastMeet()
        {
            Unit* pet = GetCaster()->GetGuardianPet();
            Unit* petTarget = GetExplTargetUnit();

            if (!pet || pet->isDead())
                return SPELL_FAILED_NO_PET;

            // pet has a target and target is within 5 yards and target is in line of sight
            if (!petTarget || !pet->IsWithinDist(petTarget, 40.0f, true) || !petTarget->IsWithinLOSInMap(pet))
                return SPELL_FAILED_DONT_REPORT;

            if (pet->HasAuraType(SPELL_AURA_MOD_STUN) || pet->HasAuraType(SPELL_AURA_MOD_CONFUSE) || pet->HasAuraType(SPELL_AURA_MOD_SILENCE) ||
                pet->HasAuraType(SPELL_AURA_MOD_FEAR) || pet->HasAuraType(SPELL_AURA_MOD_FEAR_2))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster()->IsPlayer())
            {
                if (Unit* pet = GetCaster()->GetGuardianPet())
                {
                    if (!pet)
                        return;

                    if (!GetExplTargetUnit())
                        return;
                    Unit* target = GetExplTargetUnit();
                    Player* player = GetCaster()->ToPlayer();

                    pet->CastSpell(GetExplTargetUnit(), SPELL_HUNTER_KILL_COMMAND_TRIGGER, true);

                    if (pet->GetVictim())
                    {
                        pet->AttackStop();
                        pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());
                    }
                    else
                        pet->ToCreature()->AI()->AttackStart(GetExplTargetUnit());
                     //pet->CastSpell(GetExplTargetUnit(), SPELL_HUNTER_KILL_COMMAND_CHARGE, true);

                    //191384 Aspect of the Beast 
                    if (GetCaster()->HasAura(AspectoftheBeast))
                    {
                        if (pet->HasAura(SpikedCollar))
                            player->CastSpell(target, BestialFerocity, true);
                        if (pet->HasAura(GreatStamina))
                            pet->CastSpell(pet, BestialTenacity, true);
                        if (pet->HasAura(Cornered))
                            player->CastSpell(target, BestialCunning, true);
                    }
                }
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_kill_command_SpellScript::CheckCastMeet);
            OnEffectHit += SpellEffectFn(spell_hun_kill_command_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_kill_command_SpellScript();
    }
};

// Kill Command (Damage) - 83381
class spell_hun_kill_command_proc : public SpellScriptLoader
{
public:
    spell_hun_kill_command_proc() : SpellScriptLoader("spell_hun_kill_command_proc") { }

    class spell_hun_kill_command_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_kill_command_proc_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* owner = caster->GetOwner();
            Unit* target = GetExplTargetUnit();

            // (1.5 * (rap * 3) * bmMastery * lowNerf * (1 + versability))
            int32 dmg = 4.5f * owner->m_unitData->RangedAttackPower;
            float lowNerf = float(std::min(int32(owner->getLevel()), 20) * 0.05f);

            if (Player const* ownerPlayer = owner->ToPlayer())
                dmg = AddPct(dmg, ownerPlayer->m_activePlayerData->Mastery);

            dmg *= lowNerf;

            dmg = caster->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(caster, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_kill_command_proc_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_kill_command_proc_SpellScript();
    }
};

// 198670 Piercing Shot - 
class spell_hun_piercing_shot : public SpellScript
{
    PrepareSpellScript(spell_hun_piercing_shot);

    void HandleOnHit()
    {
        uint32 damage = GetHitDamage();
        damage *= 2;
        SetHitDamage(damage);
            
        if (Unit* caster = GetCaster())
        {      
            Unit* target = GetHitUnit();

            if (!target)
            return;

            std::list<Unit*> targets;

            caster->GetAnyUnitListInRange(targets, caster->GetDistance(target));

            for (auto otherTarget : targets)
            {
                if (otherTarget != target)
                    if (!caster->IsFriendlyTo(otherTarget))
                        if (otherTarget->IsInBetween(caster, target, 5.0f))
                            caster->CastSpell(otherTarget, 213678, true);
            }
        }        
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_hun_piercing_shot::HandleOnHit);
    }
};

// Arcane Shot - 185358
class spell_hun_arcane_shot : public SpellScriptLoader
{
public:
    spell_hun_arcane_shot() : SpellScriptLoader("spell_hun_arcane_shot") { }

    class spell_hun_arcane_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_arcane_shot_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (caster->HasAura(SPELL_HUNTER_MARKING_TARGETS))
            {
                caster->CastSpell(target, SPELL_HUNTER_HUNTERS_MARK_AURA, true);
                caster->CastSpell(caster, SPELL_HUNTER_HUNTERS_MARK_AURA_2, true);
                caster->RemoveAurasDueToSpell(SPELL_HUNTER_MARKING_TARGETS);
            }

            if (caster->HasAura(SPELL_HUNTER_LETHAL_SHOTS) && roll_chance_f(20))
                if (caster->GetSpellHistory()->HasCooldown(SPELL_HUNTER_RAPID_FIRE))
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_RAPID_FIRE, -5000);

            if (caster->HasAura(SPELL_HUNTER_CALLING_THE_SHOTS))
                if (caster->GetSpellHistory()->HasCooldown(SPELL_HUNTER_TRUESHOT))
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_TRUESHOT, -2500);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_arcane_shot_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_arcane_shot_SpellScript();
    }
};

// Multi-Shot - 2643
class spell_hun_multi_shot : public SpellScriptLoader
{
public:
    spell_hun_multi_shot() : SpellScriptLoader("spell_hun_multi_shot") { }

    class spell_hun_multi_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_multi_shot_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_HUNTER_MARKING_TARGETS))
            {
                caster->CastSpell(caster, SPELL_HUNTER_HUNTERS_MARK_AURA_2, true);
                caster->RemoveAurasDueToSpell(SPELL_HUNTER_MARKING_TARGETS);

                if (Unit* target = GetHitUnit())
                    caster->CastSpell(target, SPELL_HUNTER_HUNTERS_MARK_AURA, true);
            }

            if (caster->HasAura(SPELL_HUNTER_LETHAL_SHOTS) && roll_chance_f(20))
                if (caster->GetSpellHistory()->HasCooldown(SPELL_HUNTER_RAPID_FIRE))
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_RAPID_FIRE, -5000);

            if (caster->HasAura(SPELL_HUNTER_CALLING_THE_SHOTS))
                if (caster->GetSpellHistory()->HasCooldown(SPELL_HUNTER_TRUESHOT))
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_TRUESHOT, -2500);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_multi_shot_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_multi_shot_SpellScript();
    }
};

// Marked Shot - 185901
class spell_hun_marked_shot : public SpellScriptLoader
{
public:
    spell_hun_marked_shot() : SpellScriptLoader("spell_hun_marked_shot") { }

    class spell_hun_marked_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_marked_shot_SpellScript);

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
            {
                std::list<Unit*> targetList;
                float radius = sSpellMgr->GetSpellInfo(SPELL_HUNTER_MARKED_SHOT)->GetEffect(0)->CalcRadius(caster);

                caster->GetAttackableUnitListInRange(targetList, radius);

                if (!targetList.empty())
                {
                    for (auto itr : targetList)
                    {
                        if (itr->HasAura(SPELL_HUNTER_HUNTERS_MARK_AURA, caster->GetGUID()))
                        {
                            caster->CastSpell(itr, SPELL_HUNTER_MARKED_SHOT_DAMAGE, true);
                            caster->CastSpell(itr, SPELL_HUNTER_VULNERABLE, true);
                            caster->RemoveAura(SPELL_HUNTER_HUNTERS_MARK_AURA_2);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_hun_marked_shot_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_marked_shot_SpellScript();
    }
};

// Aspect of the Cheetah - 186257
class spell_hun_aspect_of_the_cheetah : public SpellScriptLoader
{
public:
    spell_hun_aspect_of_the_cheetah() : SpellScriptLoader("spell_hun_aspect_of_the_cheetah") { }

    class spell_hun_aspect_of_the_cheetah_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_aspect_of_the_cheetah_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                caster->CastSpell(caster, SPELL_HUNTER_ASPECT_OF_THE_CHEETAH_EFFECT_2, true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_aspect_of_the_cheetah_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_aspect_of_the_cheetah_AuraScript();
    }
};

// Hunting Party - 212658
class spell_hun_hunting_party : public SpellScriptLoader
{
public:
    spell_hun_hunting_party() : SpellScriptLoader("spell_hun_hunting_party") { }

    class spell_hun_hunting_party_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_hunting_party_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*procInfo*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;
            if (!caster->ToPlayer())
                return;

            caster->ToPlayer()->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_EXHILARATION, -aurEff->GetBaseAmount() * IN_MILLISECONDS);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_hun_hunting_party_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_hunting_party_AuraScript();
    }
};

// Hunter's Mark - 185987
class spell_hun_hunters_mark : public SpellScriptLoader
{
public:
    spell_hun_hunters_mark() : SpellScriptLoader("spell_hun_hunters_mark") { }

    class spell_hun_hunters_mark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_hunters_mark_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_AUTO_SHOT)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_hunters_mark_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_hunters_mark_AuraScript();
    }
};

// Marking Targets - 223138
class spell_hun_marking_targets : public SpellScriptLoader
{
public:
    spell_hun_marking_targets() : SpellScriptLoader("spell_hun_marking_targets") { }

    class spell_hun_marking_targets_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_marking_targets_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            // Called by Multi-shoot & Arcane Shot
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_marking_targets_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_marking_targets_AuraScript();
    }
};

// Feign Death - 5384
class spell_hun_feign_death : public SpellScriptLoader
{
public:
    spell_hun_feign_death() : SpellScriptLoader("spell_hun_feign_death") { }

    class spell_hun_feign_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_feign_death_AuraScript);

        int32 health;
        int32 focus;

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            health = GetTarget()->GetHealth();
            focus = GetTarget()->GetPower(POWER_FOCUS);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (health && focus)
            {
                GetTarget()->SetHealth(health);
                GetTarget()->SetPower(POWER_FOCUS, focus);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_feign_death_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_feign_death_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_feign_death_AuraScript();
    }
};

// Intimidation - 19577
class spell_hun_intimidation : public SpellScript
{
    PrepareSpellScript(spell_hun_intimidation);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        Unit* target = caster->ToPlayer()->GetSelectedUnit();
        if (!caster || !target)
            return;

        caster->CastSpell(target, SPELL_HUNTER_INTIMIDATION_STUN, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hun_intimidation::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Killer Cobra - 199532
class spell_hun_killer_cobra : public SpellScriptLoader
{
public:
    spell_hun_killer_cobra() : SpellScriptLoader("spell_hun_killer_cobra") { }

    class spell_hun_killer_cobra_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_killer_cobra_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_COBRA_SHOT)
                return true;

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (Unit* caster = GetCaster())
            {
                if (caster->HasAura(SPELL_HUNTER_BESTIAL_WRATH))
                {
                    if (caster->GetSpellHistory()->HasCooldown(SPELL_HUNTER_KILL_COMMAND))
                        caster->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_KILL_COMMAND, true);
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_killer_cobra_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_hun_killer_cobra_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_killer_cobra_AuraScript();
    }
};

// Disengage - 781
class spell_hun_disengage : public SpellScriptLoader
{
public:
    spell_hun_disengage() : SpellScriptLoader("spell_hun_disengage") { }

    class spell_hun_disengage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_disengage_SpellScript);

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                uint32 spec = player->GetSpecializationId();

                if (player->HasSpell(SPELL_HUNTER_POSTHAST))
                {
                    if (spec == TALENT_SPEC_HUNTER_MARKSMAN || spec == TALENT_SPEC_HUNTER_BEASTMASTER)
                    {
                        player->RemoveMovementImpairingAuras();
                        player->CastSpell(player, SPELL_HUNTER_POSTHAST_SPEED, true);
                    }
                }
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_hun_disengage_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_disengage_SpellScript();
    }
};

// Farstrider - 199523
class spell_hun_farstrider : public SpellScriptLoader
{
public:
    spell_hun_farstrider() : SpellScriptLoader("spell_hun_farstrider") { }

    class spell_hun_farstrider_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_farstrider_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetHitMask() & PROC_HIT_CRITICAL)
                return true;

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasSpell(SPELL_HUNTER_DISENGAGE))
                    player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_DISENGAGE, true);

                if (player->HasSpell(SPELL_HUNTER_HARPOON))
                    player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_DISENGAGE, true);
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_farstrider_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_hun_farstrider_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_farstrider_AuraScript();
    }
};

// Throwing Axes - 200163
class spell_hun_throwing_axes : public SpellScript
{
    PrepareSpellScript(spell_hun_throwing_axes);

    void HandleOnCast()
    {
        Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
        if (!caster || !target)
            return;

        ObjectGuid targetGUID = target->GetGUID();
        uint8 throwCount = GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints;

        for (uint8 i = 0; i < throwCount; ++i)
        {
            caster->GetScheduler().Schedule(Milliseconds(500 * i), [targetGUID](TaskContext context)
            {
                if (Unit* caster = context.GetUnit())
                    if (Unit* target = ObjectAccessor::GetCreature(*caster, targetGUID))
                        caster->CastSpell(target, SPELL_HUNTER_THOWING_AXES_DAMAGE, false);
            });
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_hun_throwing_axes::HandleOnCast);
    }
};

// Ranger's Net - 200108
class spell_hun_rangers_net : public SpellScriptLoader
{
public:
    spell_hun_rangers_net() : SpellScriptLoader("spell_hun_rangers_net") {}

    class spell_hun_rangers_net_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_rangers_net_AuraScript);

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();

            caster->CastSpell(GetTarget(), SPELL_HUNTER_RANGERS_NET_INCREASE_SPEED, true);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_rangers_net_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_ROOT_2, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_rangers_net_AuraScript();
    }
};

// Sticky Bomb - 191241
class spell_hun_sticky_bomb : public SpellScriptLoader
{
public:
    spell_hun_sticky_bomb() : SpellScriptLoader("spell_hun_sticky_bomb") {}

    class spell_hun_sticky_bomb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_sticky_bomb_AuraScript);

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();

            caster->CastSpell(GetTarget(), SPELL_HUNTER_STICKY_BOMB_PROC, true);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_sticky_bomb_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_sticky_bomb_AuraScript();
    }
};

// Camouflage - 199483
class spell_hun_camouflage : public SpellScriptLoader
{
public:
    spell_hun_camouflage() : SpellScriptLoader("spell_hun_camouflage") { }

    class spell_hun_camouflage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_camouflage_AuraScript);

        void OnApply(AuraEffect const*, AuraEffectHandleModes)
        {
            if (GetCaster() && GetCaster()->IsPlayer())
                if (Unit* pet = GetCaster()->GetGuardianPet())
                    pet->CastSpell(pet, SPELL_HUNTER_CAMOUFLAGE, true);
        }

        void OnRemove(AuraEffect const*, AuraEffectHandleModes)
        {
            if (GetCaster() && GetCaster()->IsPlayer())
                if (Unit* pet = GetCaster()->GetGuardianPet())
                    pet->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_camouflage_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_camouflage_AuraScript();
    }
};

// Wild Call - 185789
class spell_hun_wild_call : public SpellScriptLoader
{
public:
    spell_hun_wild_call() : SpellScriptLoader("spell_hun_wild_call") { }

    class spell_hun_wild_call_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_wild_call_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_AUTO_SHOT && (eventInfo.GetHitMask() & PROC_HIT_CRITICAL))
                return true;

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->GetSpellHistory()->HasCooldown(SPELL_HUNTER_DIRE_BEAST_GENERIC))
                    player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_DIRE_BEAST_GENERIC, true);

                if (player->GetSpellHistory()->HasCooldown(SPELL_HUNTER_DIRE_FRENZY))
                    player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_DIRE_FRENZY, true);
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_wild_call_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_hun_wild_call_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_wild_call_AuraScript();
    }
};

// Bombardment - 35110
class spell_hun_bombardment : public SpellScriptLoader
{
public:
    spell_hun_bombardment() : SpellScriptLoader("spell_hun_bombardment") { }

    class spell_hun_bombardment_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_bombardment_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_MULTISHOT && (eventInfo.GetHitMask() & PROC_HIT_CRITICAL))
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_bombardment_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_bombardment_AuraScript();
    }
};

// Lock and Load - 194595
class spell_hun_lock_and_load : public SpellScriptLoader
{
public:
    spell_hun_lock_and_load() : SpellScriptLoader("spell_hun_lock_and_load") { }

    class spell_hun_lock_and_load_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_lock_and_load_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_AUTO_SHOT)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_lock_and_load_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_lock_and_load_AuraScript();
    }
};

// Sentinel - 206817
// AreaTriggerID - 9769
class at_hun_sentinel : public AreaTriggerEntityScript
{
public:
    at_hun_sentinel() : AreaTriggerEntityScript("at_hun_sentinel") {}

    struct at_hun_sentinelAI : AreaTriggerAI
    {
        at_hun_sentinelAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        void OnCreate() override
        {
            timeInterval = 6000;
        }

        int32 timeInterval;

        void OnUpdate(uint32 diff) override
        {
            timeInterval += diff;
            if (timeInterval < 6000)
                return;

            if (Unit* caster = at->GetCaster())
            {
                std::list<Unit*> targetList;
                float radius = sSpellMgr->GetSpellInfo(SPELL_HUNTER_SENTINEL)->GetEffect(0)->CalcRadius(caster);

                Trinity::AnyUnitInObjectRangeCheck l_Check(at, radius);
                Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> l_Searcher(at, targetList, l_Check);
                Cell::VisitAllObjects(at, l_Searcher, radius);

                for (Unit* l_Unit : targetList)

                {
                    caster->CastSpell(l_Unit, SPELL_HUNTER_HUNTERS_MARK_AURA, true);
                    caster->CastSpell(caster, SPELL_HUNTER_HUNTERS_MARK_AURA_2, true);

                    timeInterval -= 6000;
                }
            }
        }

        void OnRemove() override
        {
            if (Unit* caster = at->GetCaster())
            {
                std::list<Unit*> targetList;
                float radius = sSpellMgr->GetSpellInfo(SPELL_HUNTER_SENTINEL)->GetEffect(0)->CalcRadius(caster);

                Trinity::AnyUnitInObjectRangeCheck l_Check(at, radius);
                Trinity::UnitListSearcher<Trinity::AnyUnitInObjectRangeCheck> l_Searcher(at, targetList, l_Check);
                Cell::VisitAllObjects(at, l_Searcher, radius);

                for (Unit* l_Unit : targetList)
                    if (l_Unit != caster && caster->IsValidAttackTarget(l_Unit))
                    {
                        caster->CastSpell(l_Unit, SPELL_HUNTER_HUNTERS_MARK_AURA, true);
                        caster->CastSpell(caster, SPELL_HUNTER_HUNTERS_MARK_AURA_2, true);

                        timeInterval -= 6000;
                    }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_sentinelAI(areatrigger);
    }
};

// Dire Frenzy - 217200
class spell_hun_dire_frenzy : public SpellScriptLoader
{
public:
    spell_hun_dire_frenzy() : SpellScriptLoader("spell_hun_dire_frenzy") { }

    class spell_hun_dire_frenzy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_dire_frenzy_SpellScript);

        SpellCastResult CheckCastMeet()
        {
            Unit* pet = GetCaster()->GetGuardianPet();

            if (!pet || pet->isDead())
                return SPELL_FAILED_NO_PET;

            if (pet->HasAuraType(SPELL_AURA_MOD_STUN) || pet->HasAuraType(SPELL_AURA_MOD_CONFUSE) || pet->HasAuraType(SPELL_AURA_MOD_SILENCE) ||
                pet->HasAuraType(SPELL_AURA_MOD_FEAR) || pet->HasAuraType(SPELL_AURA_MOD_FEAR_2))
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void HandleOnCast()
        {
            Unit* caster = GetCaster()->GetGuardianPet();
            Unit* target = GetExplTargetUnit();
            if (!caster || !target)
                return;

            if (caster->GetVictim())
                caster->AttackStop();

            caster->ToCreature()->AI()->AttackStart(GetExplTargetUnit());

            ObjectGuid targetGuid = target->GetGUID();
            for (uint16 timer = 0; timer <= 800; timer += 200)
            {
                caster->GetScheduler().Schedule(Milliseconds(timer), [targetGuid](TaskContext context)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*context.GetUnit(), targetGuid))
                        GetContextUnit()->CastSpell(target, SPELL_HUNTER_DIRE_FRENZY_DAMAGE, false);
                });
            }
        }

        void HandleAfterCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->HasAura(SPELL_HUNTER_WILD_CALL_AURA))
                    player->RemoveAurasDueToSpell(SPELL_HUNTER_WILD_CALL_AURA);
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_dire_frenzy_SpellScript::CheckCastMeet);
            OnCast += SpellCastFn(spell_hun_dire_frenzy_SpellScript::HandleOnCast);
            AfterCast += SpellCastFn(spell_hun_dire_frenzy_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_dire_frenzy_SpellScript();
    }
};

// Flanking Strike - 202800
// Trigger for Animal Instincts - 204315
class spell_hun_flanking_strike : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike() : SpellScriptLoader("spell_hun_flanking_strike") { }

    class spell_hun_flanking_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (Unit* pet = GetCaster()->GetGuardianPet())
                {
                    if (!pet)
                        return;

                    Unit* target = GetExplTargetUnit();
                    if (!target)
                        return;

                    int32 dmg = GetHitDamage();

                    if (player->getAttackers().empty())
                    {
                        pet->ToCreature()->AI()->AttackStart(target);
                        pet->CastSpell(target, SPELL_HUNTER_FLANKING_STRIKE_PROC, true);
                        dmg *= 1.5f;
                    }
                    else
                    {
                        pet->ToCreature()->AI()->AttackStart(target);
                        pet->CastSpell(target, SPELL_HUNTER_FLANKING_STRIKE_PROC_UP, true);
                        pet->AddThreat(target, 400.0f);
                    }

                    dmg = player->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
                    dmg = target->SpellDamageBonusTaken(player, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

                    SetHitDamage(dmg);
                }
            }
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (caster->HasSpell(SPELL_HUNTER_ANIMAL_INSTINCTS))
            {
                uint32 roll = rand() % 4;

                if (roll == 3)
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_FLANKING_STRIKE, -3000);
                if (roll == 2)
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_MONGOOSE_BITE, -3000);
                else if (roll == 1)
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_ASPECT_OF_THE_EAGLE, -3000);
                else if (roll == 0)
                    caster->GetSpellHistory()->ModifyCooldown(SPELL_HUNTER_HARPOON, -3000);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            AfterCast += SpellCastFn(spell_hun_flanking_strike_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_SpellScript();
    }
};

// Flanking Strike (Pet - Damage) - 204740
class spell_hun_flanking_strike_proc : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike_proc() : SpellScriptLoader("spell_hun_flanking_strike_proc") { }

    class spell_hun_flanking_strike_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_proc_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* owner = caster->GetOwner();
            if (!owner)
                return;

            Unit* target = GetExplTargetUnit();
            if (!target)
                return;

            // (3.652 * (rap) * lowNerf * (1 + versability))
            int32 dmg = 3.652f * owner->m_unitData->RangedAttackPower;
            int32 lowNerf = std::min(int32(owner->getLevel()), 20) * 0.05f;
            dmg *= lowNerf;

            dmg = caster->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(caster, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_proc_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_proc_SpellScript();
    }
};

// Flanking Strike (Pet - Damage 50%) - 206933
class spell_hun_flanking_strike_proc_up : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike_proc_up() : SpellScriptLoader("spell_hun_flanking_strike_proc_up") { }

    class spell_hun_flanking_strike_proc_up_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_proc_up_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* owner = caster->GetOwner();
            if (!owner)
                return;

            Unit* target = GetExplTargetUnit();
            if (!target)
                return;

            // (3.652 * (rap) * lowNerf * (1 + versability))
            int32 dmg = 3.652f * owner->m_unitData->RangedAttackPower;
            int32 lowNerf = std::min(int32(owner->getLevel()), 20) * 0.05f;
            dmg *= lowNerf;
            dmg *= 1.5f;

            dmg = caster->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(caster, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_proc_up_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_proc_up_SpellScript();
    }
};

// Mortal Wounds - 201075
class spell_hun_mortal_wounds : public SpellScriptLoader
{
public:
    spell_hun_mortal_wounds() : SpellScriptLoader("spell_hun_mortal_wounds") { }

    class spell_hun_mortal_wounds_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_mortal_wounds_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if ((eventInfo.GetHitMask() & PROC_HIT_NONE) && eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_LACERATE)
                return true;

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (Player* player = GetCaster()->ToPlayer())
            {
                int32 chargeCatId = sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE)->ChargeCategoryId;

                if (SpellCategoryEntry const* mongooseBite = sSpellCategoryStore.LookupEntry(chargeCatId))
                {
                    player->GetSpellHistory()->RestoreCharge(chargeCatId);
                    player->GetSpellHistory()->ForceSendSetSpellCharges(mongooseBite);
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_mortal_wounds_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_hun_mortal_wounds_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_mortal_wounds_AuraScript();
    }
};

// Raptor Strike - 186270
class spell_hun_raptor_strike : public SpellScriptLoader
{
public:
    spell_hun_raptor_strike() : SpellScriptLoader("spell_hun_raptor_strike") { }

    class spell_hun_raptor_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_raptor_strike_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (caster->HasSpell(SPELL_HUNTER_SERPENT_STING))
                caster->CastSpell(target, SPELL_HUNTER_SERPENT_STING_DAMAGE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_raptor_strike_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_raptor_strike_SpellScript();
    }
};

// Carve - 187708
class spell_hun_carve : public SpellScriptLoader
{
public:
    spell_hun_carve() : SpellScriptLoader("spell_hun_carve") { }

    class spell_hun_carve_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_carve_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (caster->HasSpell(SPELL_HUNTER_SERPENT_STING))
                caster->CastSpell(target, SPELL_HUNTER_SERPENT_STING_DAMAGE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_carve_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_carve_SpellScript();
    }
};

// True Aim - 199527
class spell_hun_true_aim : public SpellScriptLoader
{
public:
    spell_hun_true_aim() : SpellScriptLoader("spell_hun_true_aim") { }

    class spell_hun_true_aim_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_true_aim_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_AIMED_SHOT || eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_ARCANE_SHOT)
                return true;

            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_true_aim_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_true_aim_AuraScript();
    }
};

// Explosive Shot - 212679
class spell_hun_explosive_shot_detonate : public SpellScript
{
    PrepareSpellScript(spell_hun_explosive_shot_detonate);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (AreaTrigger* at = GetCaster()->GetAreaTrigger(SPELL_HUNTER_EXPLOSIVE_SHOT))
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_HUNTER_EXPLOSIVE_SHOT);
            GetCaster()->CastSpell(at->GetPosition(), SPELL_HUNTER_EXPLOSIVE_SHOT_DAMAGE, true);
            at->Remove();
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hun_explosive_shot_detonate::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Exhilaration - 109304
class spell_hun_exhilaration : public SpellScript
{
    PrepareSpellScript(spell_hun_exhilaration);

    void HandeHeal(SpellEffIndex /*effIndex*/)
    {
        if (GetCaster()->HasAura(SPELL_HUNTER_EXHILARATION_PET_AURA))
            GetCaster()->CastSpell(nullptr, SPELL_HUNTER_EXHILARATION_PET, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hun_exhilaration::HandeHeal, EFFECT_0, SPELL_EFFECT_HEAL_PCT);
    }
};

// 194386
class aura_hun_volley : public AuraScript
{
    PrepareAuraScript(aura_hun_volley);

    bool CheckProc(ProcEventInfo& /*eventInfo*/)
    {
        return GetTarget()->GetPower(POWER_FOCUS) >= GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints;
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        GetTarget()->ModifyPower(POWER_FOCUS, -GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(aura_hun_volley::CheckProc);
        OnEffectProc += AuraEffectProcFn(aura_hun_volley::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 199921
class aura_trailblazer : public AuraScript
{
    PrepareAuraScript(aura_trailblazer);

    void EffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        RescheduleBuff();
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
    {
        RescheduleBuff();
    }

    void RescheduleBuff()
    {
        Unit* target = GetTarget();
        target->RemoveAurasDueToSpell(SPELL_HUNTER_TRAILBLAZER_BUFF);
        target->GetScheduler().CancelGroup(SPELL_HUNTER_TRAILBLAZER);
        target->GetScheduler().Schedule(Milliseconds(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints), SPELL_HUNTER_TRAILBLAZER, [](TaskContext context)
        {
            GetContextUnit()->CastSpell(GetContextUnit(), SPELL_HUNTER_TRAILBLAZER_BUFF, true);
        });
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(aura_trailblazer::EffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectProc += AuraEffectProcFn(aura_trailblazer::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// Heart of the Phoenix (Pet) - 55709
class spell_hun_pet_heart_of_the_phoenix : public SpellScriptLoader
{
public:
    spell_hun_pet_heart_of_the_phoenix() : SpellScriptLoader("spell_hun_pet_heart_of_the_phoenix") { }

    class spell_hun_pet_heart_of_the_phoenix_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_heart_of_the_phoenix_SpellScript);

        bool Load() override
        {
            if (!GetCaster()->IsPet())
                return false;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* owner = caster->GetOwner())
                if (!caster->HasAura(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                {
                    owner->CastCustomSpell(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED, SPELLVALUE_BASE_POINT0, 100, caster, true);
                    caster->CastSpell(caster, SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF, true);
                }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_heart_of_the_phoenix_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_pet_heart_of_the_phoenix_SpellScript();
    }
};

// Thunderstomp (Pet) - 63900
class spell_hun_pet_thunderstomp : public SpellScriptLoader
{
public:
    spell_hun_pet_thunderstomp() : SpellScriptLoader("spell_hun_pet_thunderstomp") { }

    class spell_hun_pet_thunderstomp_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_thunderstomp_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* owner = GetCaster()->GetOwner();
            Unit* target = GetHitUnit();

            if (!owner || !target)
                return;

            int32 dmg = int32(1.5f * (owner->m_unitData->RangedAttackPower * 0.250f));

            dmg = caster->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(caster, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_thunderstomp_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_pet_thunderstomp_SpellScript();
    }
};

// Basic Attack + Blink Strikes
// Called by Smack - 49966, Bite - 17253, Claw - 16827
class spell_hun_pet_basic_attack : public SpellScriptLoader
{
public:
    spell_hun_pet_basic_attack() : SpellScriptLoader("spell_hun_pet_basic_attack") { }

    class spell_hun_pet_basic_attack_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_basic_attack_SpellScript);

        SpellCastResult CheckCastRange()
        {
            Unit* caster = GetCaster();

            if (caster == nullptr)
                return SPELL_FAILED_DONT_REPORT;

            Unit* owner = caster->GetOwner();

            if (owner == nullptr)
                return SPELL_FAILED_DONT_REPORT;

            Unit* target = GetExplTargetUnit();

            if (target == nullptr)
                return SPELL_FAILED_DONT_REPORT;

            if (owner->HasSpell(SPELL_HUNTER_BLINK_STRIKES))
            {
                if (owner->ToPlayer()->GetSpellHistory()->HasCooldown(SPELL_HUNTER_BLINK_STRIKES) && caster->GetDistance(target) > 10.0f)
                    return SPELL_FAILED_OUT_OF_RANGE;

                if ((caster->HasAuraType(SPELL_AURA_MOD_ROOT) || caster->HasAuraType(SPELL_AURA_MOD_STUN)) && caster->GetDistance(target) > 5.0f)
                    return SPELL_FAILED_ROOTED;

                if (!owner->ToPlayer()->GetSpellHistory()->HasCooldown(SPELL_HUNTER_BLINK_STRIKES) && target->IsWithinLOSInMap(caster) &&
                    caster->GetDistance(target) > 10.0f && caster->GetDistance(target) < 30.0f && /*!caster->IsInRoots() && */!caster->HasAuraType(SPELL_AURA_MOD_STUN))
                {
                    caster->CastSpell(target, SPELL_HUNTER_BLINK_STRIKES_TELEPORT, true);

                    if (caster->ToCreature()->IsAIEnabled && caster->ToPet())
                    {
                        caster->ToPet()->ClearUnitState(UNIT_STATE_FOLLOW);
                        if (caster->ToPet()->GetVictim())
                            caster->ToPet()->AttackStop();
                        caster->GetMotionMaster()->Clear();
                        caster->ToPet()->GetCharmInfo()->SetIsCommandAttack(true);
                        caster->ToPet()->GetCharmInfo()->SetIsAtStay(false);
                        caster->ToPet()->GetCharmInfo()->SetIsReturning(false);
                        caster->ToPet()->GetCharmInfo()->SetIsFollowing(false);

                        caster->ToCreature()->AI()->AttackStart(target);
                    }

                    owner->ToPlayer()->GetSpellHistory()->AddCooldown(SPELL_HUNTER_BLINK_STRIKES, 0, std::chrono::seconds(20));
                }
            }
            return SPELL_CAST_OK;
        }

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Pet* pet = GetCaster()->ToPet())
            {
                if (Unit* owner = GetCaster()->GetOwner())
                {
                    Unit* target = GetHitUnit();
                    if (!target)
                        return;

                    // (1.5 * 1 * 1 * (Ranged attack power * 0.333) * (1 + $versadmg))
                    int32 dmg = (owner->m_unitData->RangedAttackPower * 0.333f);

                    SpellInfo const* CostModifier = sSpellMgr->GetSpellInfo(SPELL_HUNTER_BASIC_ATTACK_COST_MODIFIER);
                    SpellInfo const* SpikedCollar = sSpellMgr->GetSpellInfo(SPELL_HUNTER_SPIKED_COLLAR);

                    // Increases the damage done by your pet's Basic Attacks by 10%
                    if (pet->HasAura(SPELL_HUNTER_SPIKED_COLLAR) && SpikedCollar != nullptr)
                        AddPct(dmg, SpikedCollar->GetEffect(EFFECT_0)->BasePoints);

                    // Deals 100% more damage and costs 100% more Focus when your pet has 50 or more Focus.
                    if (pet->GetPower(POWER_FOCUS) + 25 >= 50)
                    {
                        if (CostModifier != nullptr)
                            dmg += CalculatePct(dmg, CostModifier->GetEffect(EFFECT_1)->BasePoints);
                        pet->EnergizeBySpell(pet, GetSpellInfo()->Id, -25, POWER_FOCUS);
                    }

                    dmg *= pet->GetModifierValue(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_PCT);

                    dmg = pet->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
                    dmg = target->SpellDamageBonusTaken(pet, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

                    SetHitDamage(dmg);
                }
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_pet_basic_attack_SpellScript::CheckCastRange);
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_basic_attack_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_pet_basic_attack_SpellScript();
    }
};

// Cobra Spit (Pet) - 206685
class spell_hun_pet_cobra_spit : public SpellScriptLoader
{
public:
    spell_hun_pet_cobra_spit() : SpellScriptLoader("spell_hun_pet_cobra_spit") { }

    class spell_hun_pet_cobra_spit_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_cobra_spit_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* owner = caster->GetOwner();
            if (!owner)
                return;

            Unit* target = GetExplTargetUnit();
            if (!target)
                return;

            // (1 + AP * 0,2)
            int32 dmg = 1 + owner->m_unitData->RangedAttackPower * 0.2f;

            dmg = caster->SpellDamageBonusDone(target, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));
            dmg = target->SpellDamageBonusTaken(caster, GetSpellInfo(), dmg, SPELL_DIRECT_DAMAGE, GetEffectInfo(EFFECT_0));

            SetHitDamage(dmg);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_cobra_spit_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_pet_cobra_spit_SpellScript();
    }
};

// 883 - Call Pet 1
// 83242 - Call Pet 2
// 83243 - Call Pet 3
// 83244 - Call Pet 4
// 83245 - Call Pet 5
class spell_hun_call_pet : public SpellScript
{
    PrepareSpellScript(spell_hun_call_pet);

    SpellCastResult CheckCast()
    {
        return GetCaster()->HasAura(SPELL_HUNTER_LONE_WOLF) ? SPELL_FAILED_SPELL_UNAVAILABLE : SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_hun_call_pet::CheckCast);
    }
};

// Flare - 1543
// AreaTriggerID - 510
class at_hun_flare : public AreaTriggerEntityScript
{
public:
    at_hun_flare() : AreaTriggerEntityScript("at_hun_flare") {}

    struct at_hun_flareAI : AreaTriggerAI
    {
        at_hun_flareAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        void OnCreate() override
        {
            Unit* caster = at->GetCaster();
            if (!caster)
                return;

            if (caster->GetTypeId() != TYPEID_PLAYER)
                return;

            if (TempSummon* tempSumm = caster->SummonCreature(WORLD_TRIGGER, at->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 200))
            {
                tempSumm->SetFaction(caster->getFaction());
                tempSumm->SetSummonerGUID(caster->GetGUID());
                PhasingHandler::InheritPhaseShift(tempSumm, caster);
                caster->CastSpell(tempSumm, SPELL_HUNTER_FLARE_EFFECT, true);
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_flareAI(areatrigger);
    }
};

// Exposive Trap - 191433
// AreaTriggerID - 1613
class at_hun_explosive_trap : public AreaTriggerEntityScript
{
public:

    at_hun_explosive_trap() : AreaTriggerEntityScript("at_hun_explosive_trap") { }

    struct at_hun_explosive_trapAI : AreaTriggerAI
    {
        int32 timeInterval;

        enum UsedSpells
        {
            SPELL_HUNTER_EXPLOSIVE_TRAP_DAMAGE = 13812
        };

        at_hun_explosive_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
            timeInterval = 200;
        }

        void OnCreate() override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!caster->ToPlayer())
                return;

            for (auto itr : at->GetInsideUnits())
            {
                Unit* target = ObjectAccessor::GetUnit(*caster, itr);
                if (!caster->IsFriendlyTo(target))
                {
                    if (TempSummon* tempSumm = caster->SummonCreature(WORLD_TRIGGER, at->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 200))
                    {
                        tempSumm->SetFaction(caster->getFaction());
                        tempSumm->SetSummonerGUID(caster->GetGUID());
                        PhasingHandler::InheritPhaseShift(tempSumm, caster);
                        caster->CastSpell(tempSumm, SPELL_HUNTER_EXPLOSIVE_TRAP_DAMAGE, true);
                        at->Remove();
                    }
                }
            }
        }

        void OnUnitEnter(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster || !unit)
                return;

            if (!caster->ToPlayer())
                return;

            if (!caster->IsFriendlyTo(unit))
            {
                if (TempSummon* tempSumm = caster->SummonCreature(WORLD_TRIGGER, at->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 200))
                {
                    tempSumm->SetFaction(caster->getFaction());
                    tempSumm->SetSummonerGUID(caster->GetGUID());
                    PhasingHandler::InheritPhaseShift(tempSumm, caster);
                    caster->CastSpell(tempSumm, SPELL_HUNTER_EXPLOSIVE_TRAP_DAMAGE, true);
                    at->Remove();
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_explosive_trapAI(areatrigger);
    }
};

// Freezing Trap - 187650
// AreaTriggerID - 9170
class at_hun_freezing_trap : public AreaTriggerEntityScript
{
public:

    at_hun_freezing_trap() : AreaTriggerEntityScript("at_hun_freezing_trap") { }

    struct at_hun_freezing_trapAI : AreaTriggerAI
    {
        int32 timeInterval;

        enum UsedSpells
        {
            SPELL_HUNTER_FREEZING_TRAP_STUN = 3355
        };

        at_hun_freezing_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
            timeInterval = 200;
        }

        void OnCreate() override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!caster->ToPlayer())
                return;

            for (auto itr : at->GetInsideUnits())
            {
                Unit* target = ObjectAccessor::GetUnit(*caster, itr);
                if (!caster->IsFriendlyTo(target))
                {
                    caster->CastSpell(target, SPELL_HUNTER_FREEZING_TRAP_STUN, true);
                    at->Remove();
                    return;
                }
            }
        }

        void OnUnitEnter(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster || !unit)
                return;

            if (!caster->ToPlayer())
                return;

            if (!caster->IsFriendlyTo(unit))
            {
                caster->CastSpell(unit, SPELL_HUNTER_FREEZING_TRAP_STUN, true);
                at->Remove();
                return;
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_freezing_trapAI(areatrigger);
    }
};

// Tar Trap (not activated) - 187698
// AreaTriggerID - 4435
class at_hun_tar_trap_not_activated : public AreaTriggerEntityScript
{
public:

    at_hun_tar_trap_not_activated() : AreaTriggerEntityScript("at_hun_tar_trap_not_activated") { }

    struct at_hun_tar_trap_not_activatedAI : AreaTriggerAI
    {
        int32 timeInterval;

        enum UsedSpells
        {
            SPELL_HUNTER_ACTIVATE_TAR_TRAP = 187700
        };

        at_hun_tar_trap_not_activatedAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
            timeInterval = 200;
        }

        void OnCreate() override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!caster->ToPlayer())
                return;

            for (auto itr : at->GetInsideUnits())
            {
                Unit* target = ObjectAccessor::GetUnit(*caster, itr);
                if (!caster->IsFriendlyTo(target))
                    if (TempSummon* tempSumm = caster->SummonCreature(WORLD_TRIGGER, at->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 60000))
                    {
                        tempSumm->SetFaction(caster->getFaction());
                        tempSumm->SetSummonerGUID(caster->GetGUID());
                        PhasingHandler::InheritPhaseShift(tempSumm, caster);
                        caster->CastSpell(tempSumm, SPELL_HUNTER_ACTIVATE_TAR_TRAP, true);
                        at->Remove();
                    }
            }
        }

        void OnUnitEnter(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster || !unit)
                return;

            if (!caster->ToPlayer())
                return;

            if (!caster->IsFriendlyTo(unit))
            {
                if (TempSummon* tempSumm = caster->SummonCreature(WORLD_TRIGGER, at->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 60000))
                {
                    tempSumm->SetFaction(caster->getFaction());
                    tempSumm->SetSummonerGUID(caster->GetGUID());
                    PhasingHandler::InheritPhaseShift(tempSumm, caster);
                    caster->CastSpell(tempSumm, SPELL_HUNTER_ACTIVATE_TAR_TRAP, true);
                    at->Remove();
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_tar_trap_not_activatedAI(areatrigger);
    }
};

// Tar Trap (activated) - 187700
// AreaTriggerID - 4436
class at_hun_tar_trap_activated : public AreaTriggerEntityScript
{
public:
    at_hun_tar_trap_activated() : AreaTriggerEntityScript("at_hun_tar_trap_activated") { }

    struct at_hun_tar_trap_activatedAI : AreaTriggerAI
    {
        int32 timeInterval;

        enum UsedSpells
        {
            SPELL_HUNTER_TAR_TRAP_SLOW = 135299
        };

        at_hun_tar_trap_activatedAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
            timeInterval = 200;
        }

        void OnCreate() override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!caster->ToPlayer())
                return;

            for (auto itr : at->GetInsideUnits())
            {
                Unit* target = ObjectAccessor::GetUnit(*caster, itr);
                if (!caster->IsFriendlyTo(target))
                {
                    caster->CastSpell(target, SPELL_HUNTER_TAR_TRAP_SLOW, true);
                }
            }
        }

        void OnUnitEnter(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster || !unit)
                return;

            if (!caster->ToPlayer())
                return;

            if (!caster->IsFriendlyTo(unit))
            {
                caster->CastSpell(unit, SPELL_HUNTER_TAR_TRAP_SLOW, true);
            }
        }

        void OnUnitExit(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster || !unit)
                return;

            if (!caster->ToPlayer())
                return;

            if (unit->HasAura(SPELL_HUNTER_TAR_TRAP_SLOW) && unit->GetAura(SPELL_HUNTER_TAR_TRAP_SLOW)->GetCaster() == caster)
                unit->RemoveAura(SPELL_HUNTER_TAR_TRAP_SLOW);
        }

        void OnRemove() override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!caster->ToPlayer())
                return;

            for (auto itr : at->GetInsideUnits())
            {
                Unit* target = ObjectAccessor::GetUnit(*caster, itr);
                if (target->HasAura(SPELL_HUNTER_TAR_TRAP_SLOW) && target->GetAura(SPELL_HUNTER_TAR_TRAP_SLOW)->GetCaster() == caster)
                    target->RemoveAura(SPELL_HUNTER_TAR_TRAP_SLOW);
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_tar_trap_activatedAI(areatrigger);
    }
};

// Binding Shot - 109248
// AreaTriggerID - 1524
class at_hun_binding_shot : public AreaTriggerEntityScript
{
public:

    at_hun_binding_shot() : AreaTriggerEntityScript("at_hun_binding_shot") { }

    struct at_hun_binding_shotAI : AreaTriggerAI
    {
        enum UsedSpells
        {
            SPELL_HUNTER_BINDING_SHOT_AURA = 117405,
            SPELL_HUNTER_BINDING_SHOT_STUN = 117526,
            SPELL_HUNTER_BINDING_SHOT_IMMUNE = 117553,
            SPELL_HUNTER_BINDING_SHOT_VISUAL_1 = 118306,
            SPELL_HUNDER_BINDING_SHOT_VISUAL_2 = 117614
        };

        at_hun_binding_shotAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        void OnUnitEnter(Unit* unit) override
        {
            Unit* caster = at->GetCaster();

            if (!caster)
                return;

            if (!unit)
                return;

            if (!caster->IsFriendlyTo(unit))
            {
                unit->CastSpell(unit, SPELL_HUNTER_BINDING_SHOT_AURA, true);
            }
        }

        void OnUnitExit(Unit* unit) override
        {
            if (!unit || !at->GetCaster())
                return;

            Position pos = at->GetPosition();

            // Need to check range also, since when the trigger is removed, this get called as well.
            if (unit->HasAura(SPELL_HUNTER_BINDING_SHOT_AURA) && unit->GetExactDist(&pos) >= 5.0f)
            {
                unit->RemoveAura(SPELL_HUNTER_BINDING_SHOT_AURA);
                at->GetCaster()->CastSpell(unit, SPELL_HUNTER_BINDING_SHOT_STUN, true);
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_binding_shotAI(areatrigger);
    }
};

// Caltrops - 194277 (194278)
// AreaTriggerID - 5084
class at_hun_caltrops : public AreaTriggerEntityScript
{
public:
    at_hun_caltrops() : AreaTriggerEntityScript("at_hun_caltrops") { }

    struct at_hun_caltropsAI : AreaTriggerAI
    {
        int32 timeInterval;

        enum UsedSpells
        {
            SPELL_HUNTER_CALTROPS_AURA = 194279
        };

        at_hun_caltropsAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger)
        {
            // How often should the action be executed
            timeInterval = 1000;
        }

        void OnUpdate(uint32 p_Time) override
        {
            Unit* caster = at->GetCaster();
            if (!caster)
                return;

            if (caster->GetTypeId() != TYPEID_PLAYER)
                return;

            // Check if we can handle actions
            timeInterval += p_Time;
            if (timeInterval < 1000)
                return;

            for (auto guid : at->GetInsideUnits())
            {
                if (Unit* unit = ObjectAccessor::GetUnit(*caster, guid))
                {
                    if (!caster->IsFriendlyTo(unit))
                    {
                        caster->CastSpell(unit, SPELL_HUNTER_CALTROPS_AURA, true);
                    }
                }
            }

            timeInterval -= 1000;
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_hun_caltropsAI(areatrigger);
    }
};

// 194599 Black Arrow - 
// Reset Cooldown on creature kill
class PlayerScript_black_arrow : public PlayerScript
{
public:
    PlayerScript_black_arrow() :PlayerScript("PlayerScript_black_arrow") {}

    void OnCreatureKill(Player* Player, Creature* /*Creature*/)
    {
        if (Player->HasSpell(SPELL_HUNTER_BLACK_ARROW))
        {
            if (Player->GetSpellHistory()->HasCooldown(SPELL_HUNTER_BLACK_ARROW))
                Player->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_BLACK_ARROW, true);
        }
    }

    void OnPVPKill(Player* killer, Player* /*killed*/)
    {
        if (killer->HasSpell(SPELL_HUNTER_BLACK_ARROW))
        {
            if (killer->GetSpellHistory()->HasCooldown(SPELL_HUNTER_BLACK_ARROW))
                killer->GetSpellHistory()->ResetCooldown(SPELL_HUNTER_BLACK_ARROW, true);
        }
    }
};

// 53480 - Roar of Sacrifice
class spell_hun_roar_of_sacrifice : public SpellScriptLoader
{
public:
    spell_hun_roar_of_sacrifice() : SpellScriptLoader("spell_hun_roar_of_sacrifice") { }

    class spell_hun_roar_of_sacrifice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_roar_of_sacrifice_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_ROAR_OF_SACRIFICE_TRIGGERED });
        }

        bool CheckProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !(damageInfo->GetSchoolMask() & aurEff->GetMiscValue()))
                return false;

            if (!GetCaster())
                return false;

            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            uint32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            eventInfo.GetActor()->CastCustomSpell(SPELL_ROAR_OF_SACRIFICE_TRIGGERED, SPELLVALUE_BASE_POINT0, damage, GetCaster(), TRIGGERED_FULL_MASK, nullptr, aurEff);
        }

        void Register() override
        {
            DoCheckEffectProc += AuraCheckEffectProcFn(spell_hun_roar_of_sacrifice_AuraScript::CheckProc, EFFECT_1, SPELL_AURA_DUMMY);
            OnEffectProc += AuraEffectProcFn(spell_hun_roar_of_sacrifice_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_roar_of_sacrifice_AuraScript();
    }
};

// 37506 - Scatter Shot
class spell_hun_scatter_shot : public SpellScriptLoader
{
public:
    spell_hun_scatter_shot() : SpellScriptLoader("spell_hun_scatter_shot") { }

    class spell_hun_scatter_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_scatter_shot_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* caster = GetCaster()->ToPlayer();
            // break Auto Shot and autohit
            caster->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
            caster->AttackStop();
            caster->SendAttackSwingCancelAttack();
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_scatter_shot_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_scatter_shot_SpellScript();
    }
};

// Glaive Toss (damage) - 120761 and 121414
class spell_hun_glaive_toss_damage : public SpellScriptLoader
{
public:
    spell_hun_glaive_toss_damage() : SpellScriptLoader("spell_hun_glaive_toss_damage") { }

    class spell_hun_glaive_toss_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_glaive_toss_damage_SpellScript);

        ObjectGuid mainTargetGUID;

        bool Load()
        {
            mainTargetGUID = ObjectGuid::Empty;
            return true;
        }

        void CorrectDamageRange(std::list<WorldObject*>& targets)
        {
            targets.clear();

            std::list<Unit*> targetList;
            float radius = 50.0f;

            GetCaster()->GetAnyUnitListInRange(targetList, radius);

            for (auto itr : targetList)
            {
                if (itr->HasAura(SPELL_HUNTER_GLAIVE_TOSS_AURA))
                {
                    mainTargetGUID = itr->GetGUID();
                    break;
                }
            }

            if (!mainTargetGUID)
                return;

            Unit* target = ObjectAccessor::GetUnit(*GetCaster(), mainTargetGUID);
            if (!target)
                return;

            targets.push_back(target);

            for (auto itr : targetList)
                if (itr->IsInBetween(GetCaster(), target, 5.0f))
                    if (!GetCaster()->IsFriendlyTo(itr))
                        targets.push_back(itr);
        }

        void CorrectSnareRange(std::list<WorldObject*>& targets)
        {
            targets.clear();

            std::list<Unit*> targetList;
            float radius = 50.0f;

            GetCaster()->GetAnyUnitListInRange(targetList, radius);

            for (auto itr : targetList)
            {
                if (itr->HasAura(SPELL_HUNTER_GLAIVE_TOSS_AURA))
                {
                    mainTargetGUID = itr->GetGUID();
                    break;
                }
            }

            if (!mainTargetGUID)
                return;

            if (!mainTargetGUID)
                return;

            Unit* target = ObjectAccessor::GetUnit(*GetCaster(), mainTargetGUID);
            if (!target)
                return;

            targets.push_back(target);

            for (auto itr : targetList)
                if (itr->IsInBetween(GetCaster(), target, 5.0f))
                    if (!GetCaster()->IsFriendlyTo(itr))
                        targets.push_back(itr);
        }

        void OnDamage()
        {
            if (!mainTargetGUID)
                return;

            Unit* target = ObjectAccessor::GetUnit(*GetCaster(), mainTargetGUID);
            if (!target)
                return;

            if (GetHitUnit())
                if (GetHitUnit() == target)
                    SetHitDamage(GetHitDamage() * 4);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectDamageRange, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::CorrectSnareRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnHit += SpellHitFn(spell_hun_glaive_toss_damage_SpellScript::OnDamage);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_hun_glaive_toss_damage_SpellScript();
    }
};

// Glaive Toss (Missile data) - 120755 and 120756
class spell_hun_glaive_toss_missile : public SpellScriptLoader
{
public:
    spell_hun_glaive_toss_missile() : SpellScriptLoader("spell_hun_glaive_toss_missile") { }

    class spell_hun_glaive_toss_missile_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_glaive_toss_missile_SpellScript);

        void HandleAfterCast()
        {
            if (GetSpellInfo()->Id == SPELL_HUNTER_GLAIVE_TOSS_RIGHT)
            {
                if (Player* plr = GetCaster()->ToPlayer())
                    plr->CastSpell(plr, SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                else if (GetOriginalCaster())
                {
                    if (Player* caster = GetOriginalCaster()->ToPlayer())
                        caster->CastSpell(caster, SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_RIGHT, true);
                }
            }
            else
            {
                if (Player* plr = GetCaster()->ToPlayer())
                    plr->CastSpell(plr, SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                else if (GetOriginalCaster())
                {
                    if (Player* caster = GetOriginalCaster()->ToPlayer())
                        caster->CastSpell(caster, SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_AND_SNARE_LEFT, true);
                }
            }

            if (Unit* target = GetExplTargetUnit())
                if (GetCaster() == GetOriginalCaster())
                    GetCaster()->AddAura(SPELL_HUNTER_GLAIVE_TOSS_AURA, target);
        }

        void HandleOnHit()
        {
            if (GetSpellInfo()->Id == SPELL_HUNTER_GLAIVE_TOSS_RIGHT)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster == GetOriginalCaster())
                            target->CastSpell(caster, SPELL_HUNTER_GLAIVE_TOSS_LEFT, true, nullptr, 0, caster->GetGUID());
            }
            else
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster == GetOriginalCaster())
                            target->CastSpell(caster, SPELL_HUNTER_GLAIVE_TOSS_RIGHT, true, nullptr, 0, caster->GetGUID());
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_hun_glaive_toss_missile_SpellScript::HandleAfterCast);
            OnHit += SpellHitFn(spell_hun_glaive_toss_missile_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_hun_glaive_toss_missile_SpellScript();
    }
};

// Blink Strike - 130392
class spell_hun_blink_strike : public SpellScriptLoader
{
public:
    spell_hun_blink_strike() : SpellScriptLoader("spell_hun_blink_strike") { }

    class spell_hun_blink_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_blink_strike_SpellScript);

        void HandleOnHit()
        {
            if (GetHitUnit() && GetCaster() && GetCaster()->GetOwner())
                if (GetCaster()->GetOwner()->HasAura(SPELL_HUNTER_BLINK_STRIKE))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_HUNTER_BLINK_STRIKE_TELEPORT, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_hun_blink_strike_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_hun_blink_strike_SpellScript();
    }
};

enum LoneWolfes
{
    LoneWolfAura = 164273,

    ///< Stats auras
    LoneWolfMastery = 160198,
    LoneWolfStamina = 160199,
    LoneWolfCritical = 160200,
    LoneWolfHaste = 160203,
    LoneWolfSpellPower = 160205,
    LoneWolfPrimarStats = 160206,
    LoneWolfVersatility = 172967,
    LoneWolfMultistrike = 172968
};

static uint32 const g_BuffSpells[8] =
{
    LoneWolfes::LoneWolfMastery,
    LoneWolfes::LoneWolfStamina,
    LoneWolfes::LoneWolfCritical,
    LoneWolfes::LoneWolfHaste,
    LoneWolfes::LoneWolfSpellPower,
    LoneWolfes::LoneWolfPrimarStats,
    LoneWolfes::LoneWolfVersatility,
    LoneWolfes::LoneWolfMultistrike
};


// 155228 - Lone Wolf
class spell_hun_lone_wolf : public SpellScriptLoader
{
public:
    spell_hun_lone_wolf() : SpellScriptLoader("spell_hun_lone_wolf") { }

    class spell_hun_lone_wolf_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_lone_wolf_AuraScript);

        void OnApply(AuraEffect const*, AuraEffectHandleModes)
        {
            if (!GetCaster())
                return;

            if (Player* player = GetCaster()->ToPlayer())
            {
                player->LearnSpell(LoneWolfes::LoneWolfMastery, false);
                player->LearnSpell(LoneWolfes::LoneWolfStamina, false);
                player->LearnSpell(LoneWolfes::LoneWolfCritical, false);
                player->LearnSpell(LoneWolfes::LoneWolfHaste, false);
                player->LearnSpell(LoneWolfes::LoneWolfSpellPower, false);
                player->LearnSpell(LoneWolfes::LoneWolfPrimarStats, false);
                player->LearnSpell(LoneWolfes::LoneWolfVersatility, false);
                player->LearnSpell(LoneWolfes::LoneWolfMultistrike, false);
            }
        }

        void OnRemove(AuraEffect const*, AuraEffectHandleModes)
        {
            if (!GetCaster())
                return;

            if (Player* player = GetCaster()->ToPlayer())
            {
                player->RemoveSpell(LoneWolfes::LoneWolfMastery);
                player->RemoveSpell(LoneWolfes::LoneWolfStamina);
                player->RemoveSpell(LoneWolfes::LoneWolfCritical);
                player->RemoveSpell(LoneWolfes::LoneWolfHaste);
                player->RemoveSpell(LoneWolfes::LoneWolfSpellPower);
                player->RemoveSpell(LoneWolfes::LoneWolfPrimarStats);
                player->RemoveSpell(LoneWolfes::LoneWolfVersatility);
                player->RemoveSpell(LoneWolfes::LoneWolfMultistrike);
                player->RemoveAura(LoneWolfes::LoneWolfAura);
            }
        }

        void OnUpdate(uint32, AuraEffect* aurEff)
        {
            if (!GetUnitOwner())
                return;

            Player* player = GetUnitOwner()->ToPlayer();

            if (player == nullptr)
                return;

            Pet* pet = player->GetPet();

            if (pet != nullptr)
            {
                player->RemoveAura(LoneWolfes::LoneWolfAura);

                aurEff->ChangeAmount(0, true, true);

                if (AuraEffect* auraEffect = aurEff->GetBase()->GetEffect(EFFECT_1))
                    auraEffect->ChangeAmount(0, true, true);

                for (uint8 I = 0; I < 8; ++I)
                    player->RemoveAura(g_BuffSpells[I]);
            }
            else
            {
                if (!player->HasAura(LoneWolfes::LoneWolfAura))
                {
                    player->CastSpell(player, LoneWolfes::LoneWolfAura, true);

                    aurEff->ChangeAmount(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints, true, true);

                    if (AuraEffect* auraEffect = aurEff->GetBase()->GetEffect(EFFECT_1))
                        auraEffect->ChangeAmount(GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints, true, true);
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_hun_lone_wolf_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_lone_wolf_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        //    OnEffectUpdate += AuraEffectUpdateFn(spell_hun_lone_wolf_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_lone_wolf_AuraScript();
    }
};


// 204147 - Windburst
class spell_hun_windburst : public SpellScriptLoader
{
public:
    spell_hun_windburst() : SpellScriptLoader("spell_hun_windburst") { }

    class spell_hun_windburst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_windburst_SpellScript);

        void HandleHit()
        {
            if (Unit* caster = GetCaster())
            {
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                caster->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 204475, true);
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_hun_windburst_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_windburst_SpellScript();
    }
};

// Called by 186270 - 187708
// 87935 - Serpent Sting
class spell_hun_serpent_sting : public SpellScriptLoader
{
public:
    spell_hun_serpent_sting() : SpellScriptLoader("spell_hun_serpent_sting") { }

    class spell_hun_serpent_sting_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_serpent_sting_SpellScript);

        void EffectDummy()
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                    caster->CastSpell(target, 118253, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_hun_serpent_sting_SpellScript::EffectDummy);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_serpent_sting_SpellScript();
    }
};

// 186265 - Aspect of the turtle
class spell_hun_aspect_of_the_turtle : public SpellScriptLoader
{
public:
    spell_hun_aspect_of_the_turtle() : SpellScriptLoader("spell_hun_aspect_of_the_turtle") { }

    class spell_hun_aspect_of_the_turtle_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_aspect_of_the_turtle_AuraScript);

        void OnApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                caster->AddUnitFlag(UnitFlags(UNIT_FLAG_PACIFIED));
                caster->AttackStop();
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->RemoveUnitFlag(UnitFlags(UNIT_FLAG_PACIFIED));
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_aspect_of_the_turtle_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DEFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_aspect_of_the_turtle_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DEFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_aspect_of_the_turtle_AuraScript();
    }
};

// 19434 - Aimed shot
class spell_hun_aimed_shot : public SpellScript
{
    PrepareSpellScript(spell_hun_aimed_shot);

    void HandleDamage(SpellEffIndex effIndex)
    {
        float distance = 30.0f;
        int32 damagePct = 50;
        std::list<Unit*> targetList;
        std::list<Unit*> victimList;
        bool canApplyDamage = true;

        if (Player* modOwner = GetCaster()->GetSpellModOwner())
        {
            if (modOwner->HasAura(199522))
            {
                if (Unit* mainTarget = GetHitUnit())
                {
                    mainTarget->GetAnyUnitListInRange(targetList, distance);

                    if (!targetList.empty())
                    {
                        for (auto target : targetList)
                        {
                            if (!modOwner->IsFriendlyTo(target))
                            {
                                if (target == mainTarget)
                                    continue;

                                 if (target->HasAura(187131))
                                    canApplyDamage = false;

                                    victimList.push_back(target);
                            }
                        }
                        if (canApplyDamage)
                            damagePct += 15;

                        for (auto victim : victimList)
                        {
                            int32 castTime = 0;
                            mainTarget->ModSpellCastTime(GetSpellInfo(), castTime);
                            mainTarget->CastCustomSpell(victim, 164340, &damagePct, NULL, NULL, true, NULL, NULL, modOwner->GetGUID());
                        }
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_hun_aimed_shot::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 205434 - Flanking Strike Pet
class spell_hun_flanking_strike_pet : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike_pet() : SpellScriptLoader("spell_hun_flanking_strike_pet") { }

    class spell_hun_flanking_strike_pet_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_pet_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetHitDamage();

            if (Unit* target = GetHitUnit())
            {
                if (Unit* caster = GetCaster())
                {
                    if (target->isAttackingPlayer() && target->GetTarget() != caster->GetGUID())
                        damage += CalculatePct(damage, GetSpellInfo()->GetEffect(EFFECT_2)->BasePoints);
                }
            }

            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_pet_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_pet_SpellScript();
    }
};

// 212431 - Explosive Shot
class spell_hun_explosive_shot_aura : public SpellScriptLoader
{
public:
    spell_hun_explosive_shot_aura() : SpellScriptLoader("spell_hun_explosive_shot_aura") { }

    class spell_hun_explosive_shot_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_explosive_shot_aura_AuraScript);

        void OnApply(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* aurTarget = GetTarget();

            if (!aurTarget)
                return;

            if (aurTarget)
            {
                if (aurTarget->GetTypeId() == TYPEID_UNIT)
                    if (aurTarget->GetEntry() != 70)
                    {
                        if (aurTarget->HasAura(GetId()))
                            aurTarget->RemoveAurasDueToSpell(GetId());
                    }
                    else
                        return;
            }

            if (Unit* caster = GetCaster())
            {
                if (Creature* creature = caster->SummonCreature(70, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 15000))
                {
                    if (Unit* target = caster->ToPlayer()->GetSelectedUnit())
                    {
                        creature->SetOwnerGUID(caster->GetGUID());
                        caster->AddAura(GetId(), creature);
                        creature->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 8.14286f);          // Normal speed
                    }
                }
            }
        }

        void OnApply1(AuraEffect const*  /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
            {
                if (target->GetTypeId() == TYPEID_UNIT)
                    if (target->GetEntry() != 70)
                        if (target->HasAura(GetId()))
                            target->RemoveAurasDueToSpell(GetId());
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_explosive_shot_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectApply += AuraEffectApplyFn(spell_hun_explosive_shot_aura_AuraScript::OnApply1, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hun_explosive_shot_aura_AuraScript();
    }
};

// 214579 - Sidewinders
class spell_hun_sidewinders : public SpellScriptLoader
{
public:
    spell_hun_sidewinders() : SpellScriptLoader("spell_hun_sidewinders") { }

    class spell_hun_sidewinders_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_sidewinders_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                    caster->CastSpell(target, 187131, true);
        }

        void HandleDummy1(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                {
                    caster->CastSpell(target, 214581, true);
                    caster->SendPlaySpellVisual(*target, target->GetOrientation(), 56931, 0, 0, 18.0f, false);
                }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_sidewinders_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_hun_sidewinders_SpellScript::HandleDummy1, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_sidewinders_SpellScript();
    }
};

// 186387 Bursting Shot --  
class spell_bursting_shot : public SpellScriptLoader
{
public:
    spell_bursting_shot() : SpellScriptLoader("spell_bursting_shot") { }

    class spell_bursting_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_bursting_shot_SpellScript);

        void HandleAfterHit()
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(GetHitUnit(), SPELL_HUNTER_AURA_SHOOTING, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_bursting_shot_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_bursting_shot_SpellScript();
    }
};

//19574
class spell_hun_bestial_wrath : public SpellScript
{
    PrepareSpellScript(spell_hun_bestial_wrath);

    void OnActivate()
    {
        if (Unit* caster = GetCaster())
        {
            if (Player* player = caster->ToPlayer())
            {
                if (Pet* pet = player->GetPet())
                    pet->AddAura(19574);
            }
        }
    }

   void Register() override
   {
        OnCast += SpellCastFn(spell_hun_bestial_wrath::OnActivate);
   }
};

void AddSC_hunter_spell_scripts()
{
    new spell_hun_harpoon();
    new spell_hun_snake_hunter();
    new spell_hun_way_of_the_moknathal();
    new spell_hun_mongoose_bite();
    new spell_hun_ancient_hysteria();
    //new spell_hun_invigoration();
    new spell_hun_masters_call();
    new spell_hun_misdirection();
    new spell_hun_misdirection_proc();
    new spell_hun_tame_beast();
    new spell_hun_chimera_shot();
    new spell_hun_a_murder_of_crows();
    new spell_hun_barrage();
    new spell_hun_beast_cleave();
    new spell_hun_beast_cleave_proc();
    new spell_hun_dire_beast();
    new spell_hun_kill_command();
    new spell_hun_kill_command_proc();
    RegisterSpellScript(spell_hun_piercing_shot);
    new spell_hun_arcane_shot();
    new spell_hun_multi_shot();
    new spell_hun_marked_shot();
    new spell_hun_aspect_of_the_cheetah();
    new spell_hun_hunting_party();
    new spell_hun_hunters_mark();
    new spell_hun_marking_targets();
    new spell_hun_feign_death();
    RegisterSpellScript(spell_hun_intimidation);
    new spell_hun_killer_cobra();
    new spell_hun_disengage();
    new spell_hun_farstrider();
    RegisterSpellScript(spell_hun_throwing_axes);
    new spell_hun_rangers_net();
    new spell_hun_sticky_bomb();
    new spell_hun_camouflage();
    new spell_hun_wild_call();
    new spell_hun_bombardment();
    new spell_hun_lock_and_load();
    new spell_hun_dire_frenzy();
    new spell_hun_flanking_strike();
    new spell_hun_flanking_strike_proc();
    new spell_hun_flanking_strike_proc_up();
    new spell_hun_mortal_wounds();
    new spell_hun_raptor_strike();
    new spell_hun_carve();
    new spell_hun_true_aim();
    RegisterSpellScript(spell_hun_explosive_shot_detonate);
    RegisterSpellScript(spell_hun_exhilaration);
    RegisterAuraScript(aura_hun_volley);
    RegisterAuraScript(aura_trailblazer);
    RegisterSpellScript(spell_hun_call_pet);
    new spell_hun_roar_of_sacrifice();
    new spell_hun_scatter_shot();
    new spell_hun_glaive_toss_damage();
    new spell_hun_glaive_toss_missile();
    new spell_hun_blink_strike();
    new spell_hun_lone_wolf();
    new spell_hun_windburst();
    new spell_hun_serpent_sting();
    new spell_hun_aspect_of_the_turtle();
    RegisterSpellScript(spell_hun_aimed_shot);
    new spell_hun_flanking_strike_pet();
    new spell_hun_explosive_shot_aura();
    new spell_hun_sidewinders();
    new spell_bursting_shot();
    new spell_hun_pet_heart_of_the_phoenix();
    new spell_hun_pet_thunderstomp();
    new spell_hun_pet_basic_attack();
    new spell_hun_pet_cobra_spit();
    new at_hun_flare();
    new at_hun_explosive_trap();
    new at_hun_freezing_trap();
    new at_hun_tar_trap_activated();
    new at_hun_tar_trap_not_activated();
    new at_hun_binding_shot();
    new at_hun_caltrops();
    new at_hun_sentinel(); 
    new PlayerScript_black_arrow();
    RegisterSpellScript(spell_hun_bestial_wrath);
}
