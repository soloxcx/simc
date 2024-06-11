#include "class_modules/apl/apl_death_knight.hpp"

#include "player/action_priority_list.hpp"
#include "player/player.hpp"
#include "dbc/dbc.hpp"

namespace death_knight_apl
{

std::string potion( const player_t* p )
{
  std::string frost_potion = ( p->true_level >= 71 ) ? "tempered_potion_3" : "elemental_potion_of_ultimate_power_3";

  std::string unholy_potion = ( p->true_level >= 71 ) ? "tempered_potion_3" : "elemental_potion_of_ultimate_power_3";

  std::string blood_potion = ( p->true_level >= 71 ) ? "tempered_potion_3" : "elemental_potion_of_ultimate_power_3";

  switch ( p->specialization() )
  {
    case DEATH_KNIGHT_BLOOD:
      return blood_potion;
    case DEATH_KNIGHT_FROST:
      return frost_potion;
    default:
      return unholy_potion;
  }
}

std::string flask( const player_t* p )
{
  std::string flask_name = ( p->true_level >= 71 ) ? "flask_of_alchemical_chaos_3" : "iced_phial_of_corrupting_rage_3";

  // All specs use a strength flask as default
  return flask_name;
}

std::string food( const player_t* p )
{
  std::string frost_food;
  std::string unholy_food;
  std::string blood_food;

  if ( p->true_level >= 71 )
  {
    frost_food = "feast_of_the_divine_day";
    unholy_food = "feast_of_the_divine_day";
    blood_food = "feast_of_the_divine_day";
  }
  else
  {
    frost_food = "sizzling_seafood_medley";
    unholy_food = "sizzling_seafood_medley";
    blood_food = "great_cerulean_sea";
  }

  switch ( p->specialization() )
  {
    case DEATH_KNIGHT_BLOOD:
      return blood_food;
    case DEATH_KNIGHT_FROST:
      return frost_food;

    default:
      return unholy_food;
  }
}

std::string rune( const player_t* p )
{
  return ( p->true_level >= 71 ) ? "crystallized" : "draconic";
}

std::string temporary_enchant( const player_t* p )
{
  std::string frost_temporary_enchant = ( p->true_level >= 71 )
                                            ? "main_hand:algari_mana_oil_3/off_hand:algari_mana_oil_3"
                                            : "main_hand:buzzing_rune_3/off_hand:buzzing_rune_3";

  std::string unholy_temporary_enchant =
      ( p->true_level >= 71 ) ? "main_hand:algari_mana_oil_3" : "main_hand:howling_rune_3";

  std::string blood_temporary_enchant =
      ( p->true_level >= 71 ) ? "main_hand:algari_mana_oil_3" : "main_hand:howling_rune_3";

  switch ( p->specialization() )
  {
    case DEATH_KNIGHT_BLOOD:
      return blood_temporary_enchant;
    case DEATH_KNIGHT_FROST:
      return frost_temporary_enchant;

    default:
      return unholy_temporary_enchant;
  }
}

  //blood_apl_start
  void blood( player_t* p )
  {
    action_priority_list_t* default_ = p->get_action_priority_list( "default" );
    action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
    action_priority_list_t* drw_up = p->get_action_priority_list( "drw_up" );
    action_priority_list_t* racials = p->get_action_priority_list( "racials" );
    action_priority_list_t* standard = p->get_action_priority_list( "standard" );
    action_priority_list_t* trinkets = p->get_action_priority_list( "trinkets" );

    precombat->add_action( "flask" );
    precombat->add_action( "food" );
    precombat->add_action( "augmentation" );
    precombat->add_action( "snapshot_stats" );
    precombat->add_action( "variable,name=trinket_1_buffs,value=trinket.1.has_use_buff|(trinket.1.has_buff.strength|trinket.1.has_buff.mastery|trinket.1.has_buff.versatility|trinket.1.has_buff.haste|trinket.1.has_buff.crit)|trinket.1.is.mirror_of_fractured_tomorrows" );
    precombat->add_action( "variable,name=trinket_2_buffs,value=trinket.2.has_use_buff|(trinket.2.has_buff.strength|trinket.2.has_buff.mastery|trinket.2.has_buff.versatility|trinket.2.has_buff.haste|trinket.2.has_buff.crit)|trinket.2.is.mirror_of_fractured_tomorrows" );
    precombat->add_action( "variable,name=trinket_1_exclude,value=trinket.1.is.ruby_whelp_shell|trinket.1.is.whispering_incarnate_icon" );
    precombat->add_action( "variable,name=trinket_2_exclude,value=trinket.2.is.ruby_whelp_shell|trinket.2.is.whispering_incarnate_icon" );
    precombat->add_action( "variable,name=damage_trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_2_buffs&trinket.2.ilvl>=trinket.1.ilvl|variable.trinket_1_buffs" );

    default_->add_action( "auto_attack" );
    default_->add_action( "variable,name=death_strike_dump_amount,value=65" );
    default_->add_action( "variable,name=bone_shield_refresh_value,value=4,op=setif,condition=talent.consumption.enabled|talent.blooddrinker.enabled,value_else=5" );
    default_->add_action( "mind_freeze,if=target.debuff.casting.react" );
    default_->add_action( "invoke_external_buff,name=power_infusion,if=buff.dancing_rune_weapon.up|!talent.dancing_rune_weapon", "Use <a href='https://www.wowhead.com/spell=10060/power-infusion'>Power Infusion</a> while <a href='https://www.wowhead.com/spell=49028/dancing-rune-weapon'>Dancing Rune Weapon</a> is up, or on cooldown if <a href='https://www.wowhead.com/spell=49028/dancing-rune-weapon'>Dancing Rune Weapon</a> is not talented" );
    default_->add_action( "potion,if=buff.dancing_rune_weapon.up" );
    default_->add_action( "call_action_list,name=trinkets" );
    default_->add_action( "raise_dead" );
    default_->add_action( "reapers_mark" );

    default_->add_action( "icebound_fortitude,if=!(buff.dancing_rune_weapon.up|buff.vampiric_blood.up)&(target.cooldown.pause_action.remains>=8|target.cooldown.pause_action.duration>0)" );
    default_->add_action( "vampiric_blood,if=!(buff.dancing_rune_weapon.up|buff.icebound_fortitude.up|buff.vampiric_blood.up)&(target.cooldown.pause_action.remains>=13|target.cooldown.pause_action.duration>0)" );
    default_->add_action( "deaths_caress,if=!buff.bone_shield.up" );
    default_->add_action( "death_and_decay,if=!death_and_decay.ticking&(talent.unholy_ground|talent.sanguine_ground|spell_targets.death_and_decay>3|buff.crimson_scourge.up)" );
    default_->add_action( "death_strike,if=buff.coagulopathy.remains<=gcd|buff.icy_talons.remains<=gcd|runic_power>=variable.death_strike_dump_amount|runic_power.deficit<=variable.heart_strike_rp|target.time_to_die<10" );
    default_->add_action( "blooddrinker,if=!buff.dancing_rune_weapon.up" );
    default_->add_action( "call_action_list,name=racials" );
    default_->add_action( "sacrificial_pact,if=!buff.dancing_rune_weapon.up&(pet.ghoul.remains<2|target.time_to_die<gcd)" );
    default_->add_action( "blood_tap,if=(rune<=2&rune.time_to_4>gcd&charges_fractional>=1.8)|rune.time_to_3>gcd" );
    default_->add_action( "gorefiends_grasp,if=talent.tightening_grasp.enabled" );
    default_->add_action( "empower_rune_weapon,if=rune<6&runic_power.deficit>5" );
    default_->add_action( "abomination_limb" );
    default_->add_action( "dancing_rune_weapon,if=!buff.dancing_rune_weapon.up" );
    default_->add_action( "run_action_list,name=drw_up,if=buff.dancing_rune_weapon.up" );
    default_->add_action( "call_action_list,name=standard" );

    drw_up->add_action( "blood_boil,if=!dot.blood_plague.ticking" );
    drw_up->add_action( "tombstone,if=buff.bone_shield.stack>5&rune>=2&runic_power.deficit>=30&!talent.shattering_bone|(talent.shattering_bone.enabled&death_and_decay.ticking)" );
    drw_up->add_action( "death_strike,if=buff.coagulopathy.remains<=gcd|buff.icy_talons.remains<=gcd" );
    drw_up->add_action( "marrowrend,if=(buff.bone_shield.remains<=4|buff.bone_shield.stack<variable.bone_shield_refresh_value)&runic_power.deficit>20" );
    drw_up->add_action( "soul_reaper,if=active_enemies=1&target.time_to_pct_35<5&target.time_to_die>(dot.soul_reaper.remains+5)" );
    drw_up->add_action( "soul_reaper,target_if=min:dot.soul_reaper.remains,if=target.time_to_pct_35<5&active_enemies>=2&target.time_to_die>(dot.soul_reaper.remains+5)" );
    drw_up->add_action( "death_and_decay,if=!death_and_decay.ticking&(talent.sanguine_ground|talent.unholy_ground)" );
    drw_up->add_action( "blood_boil,if=spell_targets.blood_boil>2&charges_fractional>=1.1" );
    drw_up->add_action( "variable,name=heart_strike_rp_drw,value=(25+spell_targets.heart_strike*talent.heartbreaker.enabled*2)" );
    drw_up->add_action( "death_strike,if=runic_power.deficit<=variable.heart_strike_rp_drw|runic_power>=variable.death_strike_dump_amount" );
    drw_up->add_action( "consumption" );
    drw_up->add_action( "blood_boil,if=charges_fractional>=1.1&buff.hemostasis.stack<5" );
    drw_up->add_action( "heart_strike,if=rune.time_to_2<gcd|runic_power.deficit>=variable.heart_strike_rp_drw" );

    racials->add_action( "blood_fury,if=cooldown.dancing_rune_weapon.ready&(!cooldown.blooddrinker.ready|!talent.blooddrinker.enabled)" );
    racials->add_action( "berserking" );
    racials->add_action( "arcane_pulse,if=active_enemies>=2|rune<1&runic_power.deficit>60" );
    racials->add_action( "lights_judgment,if=buff.unholy_strength.up" );
    racials->add_action( "ancestral_call" );
    racials->add_action( "fireblood" );
    racials->add_action( "bag_of_tricks" );
    racials->add_action( "arcane_torrent,if=runic_power.deficit>20" );

    standard->add_action( "tombstone,if=buff.bone_shield.stack>5&rune>=2&runic_power.deficit>=30&!talent.shattering_bone|(talent.shattering_bone.enabled&death_and_decay.ticking)&cooldown.dancing_rune_weapon.remains>=25" );
    standard->add_action( "variable,name=heart_strike_rp,value=(10+spell_targets.heart_strike*talent.heartbreaker.enabled*2)" );
    standard->add_action( "death_strike,if=buff.coagulopathy.remains<=gcd|buff.icy_talons.remains<=gcd|runic_power>=variable.death_strike_dump_amount|runic_power.deficit<=variable.heart_strike_rp|target.time_to_die<10" );
    standard->add_action( "deaths_caress,if=(buff.bone_shield.remains<=4|(buff.bone_shield.stack<variable.bone_shield_refresh_value+1))&runic_power.deficit>10&!(talent.insatiable_blade&cooldown.dancing_rune_weapon.remains<buff.bone_shield.remains)&!talent.consumption.enabled&!talent.blooddrinker.enabled&rune.time_to_3>gcd" );
    standard->add_action( "marrowrend,if=(buff.bone_shield.remains<=4|buff.bone_shield.stack<variable.bone_shield_refresh_value)&runic_power.deficit>20&!(talent.insatiable_blade&cooldown.dancing_rune_weapon.remains<buff.bone_shield.remains)" );
    standard->add_action( "consumption" );
    standard->add_action( "soul_reaper,if=active_enemies=1&target.time_to_pct_35<5&target.time_to_die>(dot.soul_reaper.remains+5)" );
    standard->add_action( "soul_reaper,target_if=min:dot.soul_reaper.remains,if=target.time_to_pct_35<5&active_enemies>=2&target.time_to_die>(dot.soul_reaper.remains+5)" );
    standard->add_action( "bonestorm,if=buff.bone_shield.stack>=5" );
    standard->add_action( "blood_boil,if=charges_fractional>=1.8&(buff.hemostasis.stack<=(5-spell_targets.blood_boil)|spell_targets.blood_boil>2)" );
    standard->add_action( "heart_strike,if=rune.time_to_4<gcd" );
    standard->add_action( "blood_boil,if=charges_fractional>=1.1" );
    standard->add_action( "heart_strike,if=(rune>1&(rune.time_to_3<gcd|buff.bone_shield.stack>7))" );

    trinkets->add_action( "use_item,name=fyralath_the_dreamrender,if=dot.mark_of_fyralath.ticking", "Trinkets" );
    trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=!variable.trinket_1_buffs&(variable.damage_trinket_priority=1|trinket.2.cooldown.remains|!trinket.2.has_cooldown)", "Prioritize damage dealing on use trinkets over trinkets that give buffs" );
    trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=!variable.trinket_2_buffs&(variable.damage_trinket_priority=2|trinket.1.cooldown.remains|!trinket.1.has_cooldown)" );
    trinkets->add_action( "use_item,use_off_gcd=1,slot=main_hand,if=!equipped.fyralath_the_dreamrender&(variable.trinket_1_buffs|trinket.1.cooldown.remains)&(variable.trinket_2_buffs|trinket.2.cooldown.remains)" );
    trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=variable.trinket_1_buffs&(buff.dancing_rune_weapon.up|!talent.dancing_rune_weapon|cooldown.dancing_rune_weapon.remains>20)&(variable.trinket_2_exclude|trinket.2.cooldown.remains|!trinket.2.has_cooldown|variable.trinket_2_buffs)" );
    trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=variable.trinket_2_buffs&(buff.dancing_rune_weapon.up|!talent.dancing_rune_weapon|cooldown.dancing_rune_weapon.remains>20)&(variable.trinket_1_exclude|trinket.1.cooldown.remains|!trinket.1.has_cooldown|variable.trinket_1_buffs)" );
  }
  //blood_apl_end

//frost_apl_start
void frost( player_t* p )
{
  action_priority_list_t* default_ = p->get_action_priority_list( "default" );
  action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
  action_priority_list_t* aoe = p->get_action_priority_list( "aoe" );
  action_priority_list_t* breath = p->get_action_priority_list( "breath" );
  action_priority_list_t* breath_oblit = p->get_action_priority_list( "breath_oblit" );
  action_priority_list_t* cold_heart = p->get_action_priority_list( "cold_heart" );
  action_priority_list_t* cooldowns = p->get_action_priority_list( "cooldowns" );
  action_priority_list_t* high_prio_actions = p->get_action_priority_list( "high_prio_actions" );
  action_priority_list_t* obliteration = p->get_action_priority_list( "obliteration" );
  action_priority_list_t* racials = p->get_action_priority_list( "racials" );
  action_priority_list_t* single_target = p->get_action_priority_list( "single_target" );
  action_priority_list_t* trinkets = p->get_action_priority_list( "trinkets" );
  action_priority_list_t* variables = p->get_action_priority_list( "variables" );

  precombat->add_action( "flask" );
  precombat->add_action( "food" );
  precombat->add_action( "augmentation" );
  precombat->add_action( "snapshot_stats", "Snapshot raid buffed stats before combat begins and pre-potting is done." );
  precombat->add_action( "variable,name=trinket_1_exclude,value=trinket.1.is.ruby_whelp_shell|trinket.1.is.whispering_incarnate_icon" );
  precombat->add_action( "variable,name=trinket_2_exclude,value=trinket.2.is.ruby_whelp_shell|trinket.2.is.whispering_incarnate_icon" );
  precombat->add_action( "variable,name=trinket_1_sync,op=setif,value=1,value_else=0.5,condition=trinket.1.has_use_buff&(talent.pillar_of_frost&!talent.breath_of_sindragosa&(trinket.1.cooldown.duration%%cooldown.pillar_of_frost.duration=0)|talent.breath_of_sindragosa&(cooldown.breath_of_sindragosa.duration%%trinket.1.cooldown.duration=0))", "Evaluates a trinkets cooldown, divided by pillar of frost, empower rune weapon, or breath of sindragosa's cooldown. If it's value has no remainder return 1, else return 0.5." );
  precombat->add_action( "variable,name=trinket_2_sync,op=setif,value=1,value_else=0.5,condition=trinket.2.has_use_buff&(talent.pillar_of_frost&!talent.breath_of_sindragosa&(trinket.2.cooldown.duration%%cooldown.pillar_of_frost.duration=0)|talent.breath_of_sindragosa&(cooldown.breath_of_sindragosa.duration%%trinket.2.cooldown.duration=0))" );
  precombat->add_action( "variable,name=trinket_1_buffs,value=trinket.1.has_use_buff|(trinket.1.has_buff.strength|trinket.1.has_buff.mastery|trinket.1.has_buff.versatility|trinket.1.has_buff.haste|trinket.1.has_buff.crit&!variable.trinket_1_exclude)" );
  precombat->add_action( "variable,name=trinket_2_buffs,value=trinket.2.has_use_buff|(trinket.2.has_buff.strength|trinket.2.has_buff.mastery|trinket.2.has_buff.versatility|trinket.2.has_buff.haste|trinket.2.has_buff.crit&!variable.trinket_2_exclude)" );
  precombat->add_action( "variable,name=trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_1_buffs&variable.trinket_2_buffs&(trinket.2.has_cooldown&!variable.trinket_2_exclude|!trinket.1.has_cooldown)|variable.trinket_2_buffs&((trinket.2.cooldown.duration%trinket.2.proc.any_dps.duration)*(1.5+trinket.2.has_buff.strength)*(variable.trinket_2_sync))>((trinket.1.cooldown.duration%trinket.1.proc.any_dps.duration)*(1.5+trinket.1.has_buff.strength)*(variable.trinket_1_sync)*(1+((trinket.1.ilvl-trinket.2.ilvl)%100)))" );
  precombat->add_action( "variable,name=damage_trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_1_buffs&!variable.trinket_2_buffs&trinket.2.ilvl>=trinket.1.ilvl" );
  precombat->add_action( "variable,name=trinket_1_manual,value=trinket.1.is.algethar_puzzle_box" );
  precombat->add_action( "variable,name=trinket_2_manual,value=trinket.2.is.algethar_puzzle_box" );
  precombat->add_action( "variable,name=rw_buffs,value=talent.gathering_storm|talent.everfrost" );
  precombat->add_action( "variable,name=2h_check,value=main_hand.2h" );

  default_->add_action( "auto_attack" );
  default_->add_action( "call_action_list,name=variables", "Choose Action list to run" );
  default_->add_action( "call_action_list,name=trinkets" );
  default_->add_action( "call_action_list,name=high_prio_actions" );
  default_->add_action( "call_action_list,name=cooldowns" );
  default_->add_action( "call_action_list,name=racials" );
  default_->add_action( "call_action_list,name=cold_heart,if=talent.cold_heart&(!buff.killing_machine.up|talent.breath_of_sindragosa)&((debuff.razorice.stack=5|!death_knight.runeforge.razorice&!talent.glacial_advance&!talent.avalanche&!talent.arctic_assault)|fight_remains<=gcd)" );
  default_->add_action( "run_action_list,name=breath_oblit,if=buff.breath_of_sindragosa.up&talent.obliteration&buff.pillar_of_frost.up" );
  default_->add_action( "run_action_list,name=breath,if=buff.breath_of_sindragosa.up&(!talent.obliteration|talent.obliteration&!buff.pillar_of_frost.up)" );
  default_->add_action( "run_action_list,name=obliteration,if=talent.obliteration&buff.pillar_of_frost.up&!buff.breath_of_sindragosa.up" );
  default_->add_action( "call_action_list,name=aoe,if=active_enemies>=2" );
  default_->add_action( "call_action_list,name=single_target,if=active_enemies=1" );

  aoe->add_action( "howling_blast,if=buff.rime.react|!dot.frost_fever.ticking", "AoE Action List" );
  aoe->add_action( "glacial_advance,if=!variable.pooling_runic_power&variable.rp_buffs" );
  aoe->add_action( "frostscythe,if=!death_and_decay.ticking&equipped.fyralath_the_dreamrender&(cooldown.rage_of_fyralath_417131.remains<3|!dot.mark_of_fyralath.ticking)" );
  aoe->add_action( "obliterate,if=buff.killing_machine.react&talent.cleaving_strikes&death_and_decay.ticking" );
  aoe->add_action( "glacial_advance,if=!variable.pooling_runic_power" );
  aoe->add_action( "obliterate" );
  aoe->add_action( "frost_strike,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=!variable.pooling_runic_power&!talent.glacial_advance" );
  aoe->add_action( "horn_of_winter,if=rune<2&runic_power.deficit>25" );
  aoe->add_action( "arcane_torrent,if=runic_power.deficit>25" );

  breath->add_action( "howling_blast,if=variable.rime_buffs&runic_power>(45-(talent.rage_of_the_frozen_champion*8))", "Breath Active Rotation" );
  breath->add_action( "horn_of_winter,if=rune<2&runic_power.deficit>25" );
  breath->add_action( "frostscythe" );
  breath->add_action( "obliterate,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=buff.killing_machine.react|runic_power.deficit>40|buff.pillar_of_frost.up" );
  breath->add_action( "remorseless_winter,if=runic_power<36&rune.time_to_2>runic_power%18" );
  breath->add_action( "death_and_decay,if=variable.st_planning&talent.unholy_ground&!death_and_decay.ticking&runic_power.deficit>=10|runic_power<36&rune.time_to_2>runic_power%18" );
  breath->add_action( "howling_blast,if=runic_power<36&rune.time_to_2>runic_power%18" );
  breath->add_action( "obliterate,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=runic_power.deficit>25" );
  breath->add_action( "howling_blast,if=buff.rime.react" );
  breath->add_action( "arcane_torrent,if=runic_power<60" );

  breath_oblit->add_action( "frostscythe", "Breath & Obliteration Active Rotation" );
  breath_oblit->add_action( "obliterate,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=buff.killing_machine.up" );
  breath_oblit->add_action( "howling_blast,if=buff.rime.react" );
  breath_oblit->add_action( "howling_blast,if=!buff.killing_machine.up" );
  breath_oblit->add_action( "horn_of_winter,if=runic_power.deficit>25" );
  breath_oblit->add_action( "arcane_torrent,if=runic_power.deficit>20" );

  cold_heart->add_action( "chains_of_ice,if=fight_remains<gcd&(rune<2|!buff.killing_machine.up&(!variable.2h_check&buff.cold_heart.stack>=4|variable.2h_check&buff.cold_heart.stack>8)|buff.killing_machine.up&(!variable.2h_check&buff.cold_heart.stack>8|variable.2h_check&buff.cold_heart.stack>10))", "Cold Heart" );
  cold_heart->add_action( "chains_of_ice,if=!talent.obliteration&buff.pillar_of_frost.up&buff.cold_heart.stack>=10&(buff.pillar_of_frost.remains<gcd*(1+(talent.frostwyrms_fury&cooldown.frostwyrms_fury.ready))|buff.unholy_strength.up&buff.unholy_strength.remains<gcd)" );
  cold_heart->add_action( "chains_of_ice,if=!talent.obliteration&death_knight.runeforge.fallen_crusader&!buff.pillar_of_frost.up&cooldown.pillar_of_frost.remains_expected>15&(buff.cold_heart.stack>=10&buff.unholy_strength.up|buff.cold_heart.stack>=13)" );
  cold_heart->add_action( "chains_of_ice,if=!talent.obliteration&!death_knight.runeforge.fallen_crusader&buff.cold_heart.stack>=10&!buff.pillar_of_frost.up&cooldown.pillar_of_frost.remains_expected>20" );
  cold_heart->add_action( "chains_of_ice,if=talent.obliteration&!buff.pillar_of_frost.up&(buff.cold_heart.stack>=14&buff.unholy_strength.up|buff.cold_heart.stack>=19|cooldown.pillar_of_frost.remains_expected<3&buff.cold_heart.stack>=14)" );

  cooldowns->add_action( "potion,if=(talent.pillar_of_frost&buff.pillar_of_frost.up&(talent.obliteration&buff.pillar_of_frost.remains<6|!talent.obliteration)|!talent.pillar_of_frost&buff.empower_rune_weapon.up|!talent.pillar_of_frost&!talent.empower_rune_weapon|active_enemies>=2&buff.pillar_of_frost.up)|fight_remains<25", "Cooldowns" );
  cooldowns->add_action( "empower_rune_weapon,if=talent.obliteration&!buff.empower_rune_weapon.up&rune<6&((cooldown.pillar_of_frost.remains_expected<7&buff.bloodlust.up)|((active_enemies>=2&(!raid_event.adds.exists|raid_event.adds.exists&raid_event.adds.remains>12)|variable.st_planning)&cooldown.pillar_of_frost.ready))|fight_remains<20" );
  cooldowns->add_action( "empower_rune_weapon,use_off_gcd=1,if=buff.breath_of_sindragosa.up&!buff.empower_rune_weapon.up&runic_power<70&rune<3" );
  cooldowns->add_action( "empower_rune_weapon,use_off_gcd=1,if=!talent.breath_of_sindragosa&!talent.obliteration&!buff.empower_rune_weapon.up&rune<5&(cooldown.pillar_of_frost.remains_expected<7|buff.pillar_of_frost.up|!talent.pillar_of_frost)" );
  cooldowns->add_action( "abomination_limb,if=talent.obliteration&!buff.pillar_of_frost.up&cooldown.pillar_of_frost.remains<3&(variable.adds_remain|variable.st_planning)|fight_remains<15" );
  cooldowns->add_action( "abomination_limb,if=talent.breath_of_sindragosa&(variable.adds_remain|variable.st_planning)" );
  cooldowns->add_action( "abomination_limb,if=!talent.breath_of_sindragosa&!talent.obliteration&(variable.adds_remain|variable.st_planning)" );
  cooldowns->add_action( "chill_streak,if=!talent.cleaving_strikes|active_enemies<=5" );
  cooldowns->add_action( "pillar_of_frost,if=talent.obliteration&(variable.adds_remain|variable.st_planning)&(buff.empower_rune_weapon.up|cooldown.empower_rune_weapon.remains)|fight_remains<12" );
  cooldowns->add_action( "pillar_of_frost,if=talent.breath_of_sindragosa&(variable.adds_remain|variable.st_planning)&(!talent.icecap&(runic_power>70|cooldown.breath_of_sindragosa.remains>40)|talent.icecap&(cooldown.breath_of_sindragosa.remains>5))|buff.breath_of_sindragosa.up" );
  cooldowns->add_action( "pillar_of_frost,if=talent.icecap&!talent.obliteration&!talent.breath_of_sindragosa&(variable.adds_remain|variable.st_planning)" );
  cooldowns->add_action( "breath_of_sindragosa,if=!buff.breath_of_sindragosa.up&cooldown.empower_rune_weapon.remains_expected<15&(variable.adds_remain|variable.st_planning|fight_remains<30)" );
  cooldowns->add_action( "frostwyrms_fury,if=hero_tree.rider_of_the_apocalypse&talent.apocalypse_now&talent.pillar_of_frost&buff.pillar_of_frost.up" );
  cooldowns->add_action( "frostwyrms_fury,if=active_enemies=1&(talent.pillar_of_frost&buff.pillar_of_frost.remains<gcd*2&buff.pillar_of_frost.up&!talent.obliteration|!talent.pillar_of_frost)&(!raid_event.adds.exists|(raid_event.adds.in>15+raid_event.adds.duration|talent.absolute_zero&raid_event.adds.in>15+raid_event.adds.duration))|fight_remains<3" );
  cooldowns->add_action( "frostwyrms_fury,if=active_enemies>=2&(talent.pillar_of_frost&buff.pillar_of_frost.up|raid_event.adds.exists&raid_event.adds.up&raid_event.adds.in>cooldown.pillar_of_frost.remains_expected-raid_event.adds.in-raid_event.adds.duration)&(buff.pillar_of_frost.remains<gcd*2|raid_event.adds.exists&raid_event.adds.remains<gcd*2)" );
  cooldowns->add_action( "frostwyrms_fury,if=talent.obliteration&(talent.pillar_of_frost&buff.pillar_of_frost.up&!variable.2h_check|!buff.pillar_of_frost.up&variable.2h_check&cooldown.pillar_of_frost.remains|!talent.pillar_of_frost)&((buff.pillar_of_frost.remains<gcd|buff.unholy_strength.up&buff.unholy_strength.remains<gcd)&(debuff.razorice.stack=5|!death_knight.runeforge.razorice&!talent.glacial_advance))" );
  cooldowns->add_action( "raise_dead" );
  cooldowns->add_action( "soul_reaper,if=fight_remains>5&target.time_to_pct_35<5&active_enemies<=2&(talent.obliteration&(buff.pillar_of_frost.up&!buff.killing_machine.react&rune>2|!buff.pillar_of_frost.up)|talent.breath_of_sindragosa&(buff.breath_of_sindragosa.up&runic_power>50|!buff.breath_of_sindragosa.up)|!talent.breath_of_sindragosa&!talent.obliteration)" );
  cooldowns->add_action( "any_dnd,if=!death_and_decay.ticking&!buff.mograines_might.up&variable.adds_remain&(buff.pillar_of_frost.up&buff.pillar_of_frost.remains>5&buff.pillar_of_frost.remains<11|!buff.pillar_of_frost.up&cooldown.pillar_of_frost.remains>10|fight_remains<11)&(active_enemies>5|talent.cleaving_strikes&active_enemies>=2)" );
  cooldowns->add_action( "reapers_mark" );

  high_prio_actions->add_action( "invoke_external_buff,name=power_infusion,if=(buff.pillar_of_frost.up|!talent.pillar_of_frost)&(talent.obliteration|talent.breath_of_sindragosa&buff.breath_of_sindragosa.up|!talent.breath_of_sindragosa&!talent.obliteration)", "High Priority Actions Use <a href='https://www.wowhead.com/spell=10060/power-infusion'>Power Infusion</a> while <a href='https://www.wowhead.com/spell=51271/pillar-of-frost'>Pillar of Frost</a> is up, as well as <a href='https://www.wowhead.com/spell=152279/breath-of-sindragosa'>Breath of Sindragosa</a> or on cooldown if <a href='https://www.wowhead.com/spell=51271/pillar-of-frost'>Pillar of Frost</a> and <a href='https://www.wowhead.com/spell=152279/breath-of-sindragosa'>Breath of Sindragosa</a> are not talented" );
  high_prio_actions->add_action( "mind_freeze,if=target.debuff.casting.react", "Interrupt" );
  high_prio_actions->add_action( "antimagic_shell,if=runic_power.deficit>40&death_knight.first_ams_cast<time" );
  high_prio_actions->add_action( "howling_blast,if=!dot.frost_fever.ticking&active_enemies>=2&((!talent.obliteration|talent.obliteration&(!cooldown.pillar_of_frost.ready|buff.pillar_of_frost.up&!buff.killing_machine.react))|(equipped.fyralath_the_dreamrender&!dot.mark_of_fyralath.ticking))", "Maintain Frost Fever, Icy Talons and Unleashed Frenzy" );
  high_prio_actions->add_action( "glacial_advance,if=active_enemies>=2&variable.rp_buffs&talent.obliteration&talent.breath_of_sindragosa&!buff.pillar_of_frost.up&!buff.breath_of_sindragosa.up&cooldown.breath_of_sindragosa.remains>variable.breath_pooling_time" );
  high_prio_actions->add_action( "glacial_advance,if=active_enemies>=2&variable.rp_buffs&talent.breath_of_sindragosa&!buff.breath_of_sindragosa.up&cooldown.breath_of_sindragosa.remains>variable.breath_pooling_time" );
  high_prio_actions->add_action( "glacial_advance,if=active_enemies>=2&variable.rp_buffs&!talent.breath_of_sindragosa&talent.obliteration&!buff.pillar_of_frost.up" );
  high_prio_actions->add_action( "frost_strike,if=active_enemies=1&variable.rp_buffs&talent.obliteration&talent.breath_of_sindragosa&!buff.pillar_of_frost.up&!buff.breath_of_sindragosa.up&cooldown.breath_of_sindragosa.remains>variable.breath_pooling_time" );
  high_prio_actions->add_action( "frost_strike,if=active_enemies=1&variable.rp_buffs&talent.breath_of_sindragosa&!buff.breath_of_sindragosa.up&cooldown.breath_of_sindragosa.remains>variable.breath_pooling_time" );
  high_prio_actions->add_action( "frost_strike,if=active_enemies=1&variable.rp_buffs&!talent.breath_of_sindragosa&talent.obliteration&!buff.pillar_of_frost.up" );
  high_prio_actions->add_action( "remorseless_winter,if=variable.rw_buffs|variable.adds_remain" );

  obliteration->add_action( "howling_blast,if=buff.killing_machine.stack<2&buff.pillar_of_frost.remains<gcd&buff.rime.react", "Obliteration Active Rotation" );
  obliteration->add_action( "frost_strike,if=(active_enemies<=1|!talent.glacial_advance)&buff.killing_machine.react<2&buff.pillar_of_frost.remains<gcd&!death_and_decay.ticking" );
  obliteration->add_action( "glacial_advance,if=buff.killing_machine.react<2&buff.pillar_of_frost.remains<gcd&!death_and_decay.ticking" );
  obliteration->add_action( "frostscythe" );
  obliteration->add_action( "obliterate,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=buff.killing_machine.react" );
  obliteration->add_action( "howling_blast,if=!buff.killing_machine.react&(!dot.frost_fever.ticking)" );
  obliteration->add_action( "glacial_advance,if=!buff.killing_machine.react&(!death_knight.runeforge.razorice&(!talent.avalanche|debuff.razorice.stack<5|debuff.razorice.remains<gcd*3)|((variable.rp_buffs|rune<2)&active_enemies>1))" );
  obliteration->add_action( "frost_strike,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=!buff.killing_machine.react&(rune<2|variable.rp_buffs|debuff.razorice.stack=5&talent.shattering_blade)&!variable.pooling_runic_power&(!talent.glacial_advance|active_enemies=1)" );
  obliteration->add_action( "howling_blast,if=buff.rime.react&!buff.killing_machine.react" );
  obliteration->add_action( "frost_strike,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=!buff.killing_machine.react&!variable.pooling_runic_power&(!talent.glacial_advance|active_enemies=1)" );
  obliteration->add_action( "howling_blast,if=!buff.killing_machine.react&runic_power<30" );
  obliteration->add_action( "arcane_torrent,if=rune<1&runic_power<30" );
  obliteration->add_action( "glacial_advance,if=!variable.pooling_runic_power&active_enemies>=2" );
  obliteration->add_action( "frost_strike,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice,if=!variable.pooling_runic_power&(!talent.glacial_advance|active_enemies=1)" );
  obliteration->add_action( "howling_blast,if=buff.rime.react" );
  obliteration->add_action( "obliterate,target_if=max:(debuff.razorice.stack+1)%(debuff.razorice.remains+1)*death_knight.runeforge.razorice" );

  racials->add_action( "blood_fury,if=variable.cooldown_check", "Racial Abilities" );
  racials->add_action( "berserking,if=variable.cooldown_check" );
  racials->add_action( "arcane_pulse,if=variable.cooldown_check" );
  racials->add_action( "lights_judgment,if=variable.cooldown_check" );
  racials->add_action( "ancestral_call,if=variable.cooldown_check" );
  racials->add_action( "fireblood,if=variable.cooldown_check" );
  racials->add_action( "bag_of_tricks,if=talent.obliteration&!buff.pillar_of_frost.up&buff.unholy_strength.up" );
  racials->add_action( "bag_of_tricks,if=!talent.obliteration&buff.pillar_of_frost.up&(buff.unholy_strength.up&buff.unholy_strength.remains<gcd*3|buff.pillar_of_frost.remains<gcd*3)" );

  single_target->add_action( "frost_strike,if=buff.killing_machine.react<2&runic_power.deficit<20&!variable.2h_check", "Single Target Rotation" );
  single_target->add_action( "frostscythe" );
  single_target->add_action( "obliterate,if=buff.killing_machine.react" );
  single_target->add_action( "howling_blast,if=buff.rime.react&talent.icebreaker.rank=2" );
  single_target->add_action( "horn_of_winter,if=rune<4&runic_power.deficit>25&talent.obliteration&talent.breath_of_sindragosa" );
  single_target->add_action( "frost_strike,if=!variable.pooling_runic_power&(variable.rp_buffs|runic_power.deficit<25|debuff.razorice.stack=5&talent.shattering_blade)" );
  single_target->add_action( "howling_blast,if=variable.rime_buffs" );
  single_target->add_action( "glacial_advance,if=!variable.pooling_runic_power&!death_knight.runeforge.razorice&(debuff.razorice.stack<5|debuff.razorice.remains<gcd*3)" );
  single_target->add_action( "obliterate,if=!variable.pooling_runes" );
  single_target->add_action( "horn_of_winter,if=rune<4&runic_power.deficit>25&(!talent.breath_of_sindragosa|cooldown.breath_of_sindragosa.remains>cooldown.horn_of_winter.duration)" );
  single_target->add_action( "arcane_torrent,if=runic_power.deficit>20" );
  single_target->add_action( "frost_strike,if=!variable.pooling_runic_power" );

  trinkets->add_action( "use_item,name=fyralath_the_dreamrender,if=dot.mark_of_fyralath.ticking&!buff.pillar_of_frost.up&!buff.empower_rune_weapon.up&!death_and_decay.ticking&(active_enemies<2|dot.frost_fever.ticking)", "Trinkets" );
  trinkets->add_action( "use_item,use_off_gcd=1,name=algethar_puzzle_box,if=!buff.pillar_of_frost.up&cooldown.pillar_of_frost.remains<2&(!talent.breath_of_sindragosa|runic_power>60&(buff.breath_of_sindragosa.up|cooldown.breath_of_sindragosa.remains<2))" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=variable.trinket_1_buffs&!variable.trinket_1_manual&(talent.obliteration&buff.pillar_of_frost.remains>10&(!variable.rp_buffs|cooldown.empower_rune_weapon.max_charges<2&buff.empower_rune_weapon.up)|!talent.obliteration)&(!buff.pillar_of_frost.up&trinket.1.cast_time>0|!trinket.1.cast_time>0)&(buff.breath_of_sindragosa.up|buff.pillar_of_frost.up)&(variable.trinket_2_exclude|!trinket.2.has_cooldown|trinket.2.cooldown.remains|variable.trinket_priority=1)|trinket.1.proc.any_dps.duration>=fight_remains", "Trinkets The trinket with the highest estimated value, will be used first and paired with Pillar of Frost." );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=variable.trinket_2_buffs&!variable.trinket_2_manual&(talent.obliteration&buff.pillar_of_frost.remains>10&(!variable.rp_buffs|cooldown.empower_rune_weapon.max_charges<2&buff.empower_rune_weapon.up)|!talent.obliteration)&(!buff.pillar_of_frost.up&trinket.2.cast_time>0|!trinket.2.cast_time>0)&(buff.breath_of_sindragosa.up|buff.pillar_of_frost.up)&(variable.trinket_1_exclude|!trinket.1.has_cooldown|trinket.1.cooldown.remains|variable.trinket_priority=2)|trinket.2.proc.any_dps.duration>=fight_remains" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=!variable.trinket_1_buffs&!variable.trinket_1_manual&((variable.damage_trinket_priority=1|trinket.2.cooldown.remains)|(trinket.1.cast_time>0&!buff.pillar_of_frost.up|!trinket.1.cast_time>0&cooldown.pillar_of_frost.remains>20)|talent.pillar_of_frost&cooldown.pillar_of_frost.remains_expected>20|!talent.pillar_of_frost)", "If only one on use trinket provides a buff, use the other on cooldown. Or if neither trinket provides a buff, use both on cooldown." );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=!variable.trinket_2_buffs&!variable.trinket_2_manual&((variable.damage_trinket_priority=2|trinket.1.cooldown.remains)|(trinket.2.cast_time>0&!buff.pillar_of_frost.up|!trinket.2.cast_time>0&cooldown.pillar_of_frost.remains>20)|talent.pillar_of_frost&cooldown.pillar_of_frost.remains_expected>20|!talent.pillar_of_frost)" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=main_hand,if=!equipped.fyralath_the_dreamrender&(!variable.trinket_1_buffs|trinket.1.cooldown.remains)&(!variable.trinket_2_buffs|trinket.2.cooldown.remains)" );

  variables->add_action( "variable,name=st_planning,value=active_enemies=1&(raid_event.adds.in>15|!raid_event.adds.exists)", "Variables" );
  variables->add_action( "variable,name=adds_remain,value=active_enemies>=2&(!raid_event.adds.exists|raid_event.adds.exists&raid_event.adds.remains>5)" );
  variables->add_action( "variable,name=rime_buffs,value=buff.rime.react&(talent.rage_of_the_frozen_champion|talent.avalanche|talent.icebreaker)" );
  variables->add_action( "variable,name=rp_buffs,value=talent.unleashed_frenzy&(buff.unleashed_frenzy.remains<gcd.max*3|buff.unleashed_frenzy.stack<3)|talent.icy_talons&(buff.icy_talons.remains<gcd.max*3|buff.icy_talons.stack<3)" );
  variables->add_action( "variable,name=cooldown_check,value=talent.pillar_of_frost&buff.pillar_of_frost.up&(talent.obliteration&buff.pillar_of_frost.remains>10|!talent.obliteration)|!talent.pillar_of_frost&buff.empower_rune_weapon.up|!talent.pillar_of_frost&!talent.empower_rune_weapon|active_enemies>=2&buff.pillar_of_frost.up" );
  variables->add_action( "variable,name=oblit_pooling_time,op=setif,value=((cooldown.pillar_of_frost.remains_expected+1)%gcd.max)%((rune+3)*(runic_power+5))*100,value_else=3,condition=runic_power<35&rune<2&cooldown.pillar_of_frost.remains_expected<10", "Formulaic approach to determine the time before these abilities come off cooldown that the simulation should star to pool resources. Capped at 15s in the run_action_list call." );
  variables->add_action( "variable,name=breath_pooling_time,op=setif,value=((cooldown.breath_of_sindragosa.remains+1)%gcd.max)%((rune+1)*(runic_power+20))*100,value_else=2,condition=runic_power.deficit>10&cooldown.breath_of_sindragosa.remains<10" );
  variables->add_action( "variable,name=pooling_runes,value=rune<4&talent.obliteration&cooldown.pillar_of_frost.remains_expected<variable.oblit_pooling_time" );
  variables->add_action( "variable,name=pooling_runic_power,value=talent.breath_of_sindragosa&cooldown.breath_of_sindragosa.remains<variable.breath_pooling_time|talent.obliteration&runic_power<35&cooldown.pillar_of_frost.remains_expected<variable.oblit_pooling_time" );
}
//frost_apl_end

//unholy_apl_start
void unholy( player_t* p )
{
  action_priority_list_t* default_ = p->get_action_priority_list( "default" );
  action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
  action_priority_list_t* aoe = p->get_action_priority_list( "aoe" );
  action_priority_list_t* aoe_burst = p->get_action_priority_list( "aoe_burst" );
  action_priority_list_t* cooldowns = p->get_action_priority_list( "cooldowns" );
  action_priority_list_t* st = p->get_action_priority_list( "st" );
  action_priority_list_t* racials = p->get_action_priority_list( "racials" );
  action_priority_list_t* trinkets = p->get_action_priority_list( "trinkets" );
  action_priority_list_t* variables = p->get_action_priority_list( "variables" );

  precombat->add_action( "flask" );
  precombat->add_action( "food" );
  precombat->add_action( "augmentation" );
  precombat->add_action( "snapshot_stats" );
  precombat->add_action( "raise_dead" );
  precombat->add_action( "army_of_the_dead,precombat_time=2" );
  precombat->add_action( "variable,name=trinket_1_buffs,value=trinket.1.has_use_buff" );
  precombat->add_action( "variable,name=trinket_2_buffs,value=trinket.2.has_use_buff" );
  precombat->add_action( "variable,name=trinket_1_sync,op=setif,value=1,value_else=0.5,condition=variable.trinket_1_buffs&(talent.apocalypse&trinket.1.cooldown.duration%%cooldown.apocalypse.duration=0|talent.dark_transformation&trinket.1.cooldown.duration%%cooldown.dark_transformation.duration=0)" );
  precombat->add_action( "variable,name=trinket_2_sync,op=setif,value=1,value_else=0.5,condition=variable.trinket_2_buffs&(talent.apocalypse&trinket.2.cooldown.duration%%cooldown.apocalypse.duration=0|talent.dark_transformation&trinket.2.cooldown.duration%%cooldown.dark_transformation.duration=0)" );
  precombat->add_action( "variable,name=trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_1_buffs&variable.trinket_2_buffs&(trinket.2.has_cooldown|!trinket.1.has_cooldown)|variable.trinket_2_buffs&((trinket.2.cooldown.duration%trinket.2.proc.any_dps.duration)*(1.5+trinket.2.has_buff.strength)*(variable.trinket_2_sync))>((trinket.1.cooldown.duration%trinket.1.proc.any_dps.duration)*(1.5+trinket.1.has_buff.strength)*(variable.trinket_1_sync)*(1+((trinket.1.ilvl-trinket.2.ilvl)%100)))" );
  precombat->add_action( "variable,name=damage_trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_1_buffs&!variable.trinket_2_buffs&trinket.2.ilvl>=trinket.1.ilvl" );

  default_->add_action( "auto_attack" );
  default_->add_action( "call_action_list,name=variables", "Call Action Lists" );
  default_->add_action( "call_action_list,name=trinkets" );
  default_->add_action( "call_action_list,name=racials" );
  default_->add_action( "call_action_list,name=cooldowns" );
  default_->add_action( "call_action_list,name=aoe_burst,if=active_enemies>=2&buff.death_and_decay.up" );
  default_->add_action( "call_action_list,name=aoe,if=active_enemies>=2&!buff.death_and_decay.up" );
  default_->add_action( "call_action_list,name=st,if=active_enemies=1" );

  aoe->add_action( "any_dnd,if=(!talent.bursting_sores|death_knight.fwounded_targets=active_enemies|death_knight.fwounded_targets>=8|raid_event.adds.exists&raid_event.adds.remains<=11&raid_event.adds.remains>5)&(!talent.defile|talent.defile&buff.defile.remains<gcd)", "AOE" );
  aoe->add_action( "epidemic,if=buff.sudden_doom.react&talent.doomed_bidding&talent.menacing_magus" );
  aoe->add_action( "festering_strike,if=debuff.festering_wound.stack<4&(buff.festering_scythe.react|cooldown.apocalypse.remains<variable.apoc_timing)" );
  aoe->add_action( "wound_spender,target_if=max:debuff.festering_wound.stack,if=debuff.festering_wound.stack>=1&cooldown.apocalypse.remains>variable.apoc_timing" );
  aoe->add_action( "epidemic,if=!variable.pooling_runic_power" );
  aoe->add_action( "festering_strike,target_if=min:debuff.festering_wound.stack,if=debuff.festering_wound.stack<4" );

  aoe_burst->add_action( "epidemic,if=!variable.pooling_runic_power&(active_enemies>=6|runic_power.deficit<30|buff.festermight.stack=20|buff.sudden_doom.react)", "AoE Burst" );
  aoe_burst->add_action( "wound_spender,target_if=max:debuff.festering_wound.stack,if=debuff.festering_wound.stack>=1" );
  aoe_burst->add_action( "epidemic,if=!variable.pooling_runic_power|fight_remains<10" );
  aoe_burst->add_action( "festering_strike,if=debuff.festering_wound.stack<4&buff.festering_scythe.react" );
  aoe_burst->add_action( "wound_spender" );

  cooldowns->add_action( "army_of_the_dead,if=(variable.st_planning|variable.adds_remain)&(talent.commander_of_the_dead&cooldown.dark_transformation.remains<5|!talent.commander_of_the_dead)|fight_remains<35", "Cooldowns" );
  cooldowns->add_action( "raise_abomination,if=(variable.st_planning|variable.adds_remain)&(talent.commander_of_the_dead&cooldown.dark_transformation.remains<gcd*2|!talent.commander_of_the_dead)|fight_remains<30" );
  cooldowns->add_action( "defile,if=(variable.st_planning|variable.adds_remain)&(buff.defile.remains<gcd&(pet.gargoyle.active|pet.army_ghoul.active|pet.apoc_ghoul.active|pet.abomination.active))" );
  cooldowns->add_action( "unholy_blight,if=(variable.st_planning|variable.adds_remain)" );
  cooldowns->add_action( "dark_transformation,if=(variable.st_planning|variable.adds_remain)&(talent.gift_of_the_sanlayn&(talent.apocalypse&pet.apoc_ghoul.active|!talent.apocalypse)|!talent.gift_of_the_sanlayn&talent.apocalypse&cooldown.apocalypse.remains<8|!talent.apocalypse)" );
  cooldowns->add_action( "summon_gargoyle,use_off_gcd=1,if=(variable.st_planning|variable.adds_remain)&(buff.commander_of_the_dead.up|!talent.commander_of_the_dead)&runic_power>=40" );
  cooldowns->add_action( "unholy_assault,if=(variable.st_planning|variable.adds_remain)&(cooldown.apocalypse.remains<gcd*2|!talent.apocalypse)" );
  cooldowns->add_action( "apocalypse,if=(variable.st_planning|variable.adds_remain)&(debuff.festering_wound.stack>=4)" );
  cooldowns->add_action( "outbreak,target_if=target.time_to_die>dot.virulent_plague.remains&(dot.virulent_plague.refreshable|talent.superstrain&(dot.frost_fever.refreshable&!talent.vampiric_strike|dot.blood_plague.refreshable))&((!talent.unholy_blight|talent.unholy_blight&cooldown.unholy_blight.remains>15%((talent.superstrain*3)+(talent.plaguebringer*2)+(talent.ebon_fever*2)))|(!talent.raise_abomination|talent.raise_abomination&cooldown.raise_abomination.remains>15%((talent.superstrain*3)+(talent.plaguebringer*2)+(talent.ebon_fever*2))))" );
  cooldowns->add_action( "abomination_limb,if=(variable.st_planning|variable.adds_remain)&(active_enemies>=2|!buff.dark_transformation.up&!buff.sudden_doom.react&debuff.festering_wound.stack<=2)" );

  st->add_action( "death_coil,if=buff.sudden_doom.react&buff.gift_of_the_sanlayn.remains&(talent.doomed_bidding|talent.rotten_touch)", "Single Taget" );
  st->add_action( "soul_reaper,if=target.health.pct<=35&!buff.gift_of_the_sanlayn.up&fight_remains>5" );
  st->add_action( "festering_strike,if=(debuff.festering_wound.stack<4&cooldown.apocalypse.remains<variable.apoc_timing)|(talent.gift_of_the_sanlayn&!buff.gift_of_the_sanlayn.up|!talent.gift_of_the_sanlayn)&(buff.festering_scythe.react|debuff.festering_wound.stack<=2)" );
  st->add_action( "death_coil,if=(buff.sudden_doom.react&debuff.festering_wound.stack>=1|rune<2|runic_power.deficit<30&!talent.vampiric_strike)" );
  st->add_action( "wound_spender,if=(debuff.festering_wound.stack>=3&cooldown.apocalypse.remains>variable.apoc_timing)|buff.vampiric_strike.react&cooldown.apocalypse.remains>variable.apoc_timing" );
  st->add_action( "death_coil,if=!variable.pooling_runic_power&debuff.death_rot.remains<gcd" );
  st->add_action( "wound_spender,if=debuff.festering_wound.stack>4" );
  st->add_action( "death_coil,if=!variable.pooling_runic_power" );

  racials->add_action( "arcane_torrent,if=runic_power.deficit>20&(cooldown.summon_gargoyle.remains<gcd&time>15|!talent.summon_gargoyle.enabled|pet.gargoyle.active&rune<2&debuff.festering_wound.stack<1)", "Racials" );
  racials->add_action( "blood_fury,if=(buff.blood_fury.duration+3>=pet.gargoyle.remains&pet.gargoyle.active)|(!talent.summon_gargoyle|cooldown.summon_gargoyle.remains>60)&(pet.army_ghoul.active&pet.army_ghoul.remains<=buff.blood_fury.duration+3|pet.apoc_ghoul.active&pet.apoc_ghoul.remains<=buff.blood_fury.duration+3|active_enemies>=2&death_and_decay.ticking)|fight_remains<=buff.blood_fury.duration+3" );
  racials->add_action( "berserking,if=(buff.berserking.duration+3>=pet.gargoyle.remains&pet.gargoyle.active)|(!talent.summon_gargoyle|cooldown.summon_gargoyle.remains>60)&(pet.army_ghoul.active&pet.army_ghoul.remains<=buff.berserking.duration+3|pet.apoc_ghoul.active&pet.apoc_ghoul.remains<=buff.berserking.duration+3|active_enemies>=2&death_and_decay.ticking)|fight_remains<=buff.berserking.duration+3" );
  racials->add_action( "lights_judgment,if=buff.unholy_strength.up&(!talent.festermight|buff.festermight.remains<target.time_to_die|buff.unholy_strength.remains<target.time_to_die)" );
  racials->add_action( "ancestral_call,if=(18>=pet.gargoyle.remains&pet.gargoyle.active)|(!talent.summon_gargoyle|cooldown.summon_gargoyle.remains>60)&(pet.army_ghoul.active&pet.army_ghoul.remains<=18|pet.apoc_ghoul.active&pet.apoc_ghoul.remains<=18|active_enemies>=2&death_and_decay.ticking)|fight_remains<=18" );
  racials->add_action( "arcane_pulse,if=active_enemies>=2|(rune.deficit>=5&runic_power.deficit>=60)" );
  racials->add_action( "fireblood,if=(buff.fireblood.duration+3>=pet.gargoyle.remains&pet.gargoyle.active)|(!talent.summon_gargoyle|cooldown.summon_gargoyle.remains>60)&(pet.army_ghoul.active&pet.army_ghoul.remains<=buff.fireblood.duration+3|pet.apoc_ghoul.active&pet.apoc_ghoul.remains<=buff.fireblood.duration+3|active_enemies>=2&death_and_decay.ticking)|fight_remains<=buff.fireblood.duration+3" );
  racials->add_action( "bag_of_tricks,if=active_enemies=1&(buff.unholy_strength.up|fight_remains<5)" );

  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=variable.trinket_1_buffs&((!talent.summon_gargoyle&((!talent.army_of_the_dead|cooldown.army_of_the_dead.remains>60|death_knight.disable_aotd|talent.raise_abomination&cooldown.raise_abomination.remains>60)&(pet.apoc_ghoul.active|(!talent.apocalypse|active_enemies>=2)&buff.dark_transformation.up)|pet.army_ghoul.active|pet.abomination.active)|talent.summon_gargoyle&pet.gargoyle.active|cooldown.summon_gargoyle.remains>80)&(pet.apoc_ghoul.active|(!talent.apocalypse|active_enemies>=2)&buff.dark_transformation.up)&(variable.trinket_priority=1|trinket.2.cooldown.remains|!trinket.2.has_cooldown))|trinket.1.proc.any_dps.duration>=fight_remains", "Trinkets" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=variable.trinket_2_buffs&((!talent.summon_gargoyle&((!talent.army_of_the_dead|cooldown.army_of_the_dead.remains>60|death_knight.disable_aotd|talent.raise_abomination&cooldown.raise_abomination.remains>60)&(pet.apoc_ghoul.active|(!talent.apocalypse|active_enemies>=2)&buff.dark_transformation.up)|pet.army_ghoul.active|pet.abomination.active)|talent.summon_gargoyle&pet.gargoyle.active|cooldown.summon_gargoyle.remains>80)&(pet.apoc_ghoul.active|(!talent.apocalypse|active_enemies>=2)&buff.dark_transformation.up)&(variable.trinket_priority=2|trinket.1.cooldown.remains|!trinket.1.has_cooldown))|trinket.2.proc.any_dps.duration>=fight_remains" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket1,if=!variable.trinket_1_buffs&(variable.damage_trinket_priority=1|trinket.2.cooldown.remains|!trinket.2.has_cooldown|!talent.summon_gargoyle&!talent.army_of_the_dead|!talent.summon_gargoyle&talent.army_of_the_dead&cooldown.army_of_the_dead.remains_expected>20|!talent.summon_gargoyle&!talent.army_of_the_dead&cooldown.dark_transformation.remains>20|cooldown.summon_gargoyle.remains>20&!pet.gargoyle.active)|fight_remains<15" );
  trinkets->add_action( "use_item,use_off_gcd=1,slot=trinket2,if=!variable.trinket_2_buffs&(variable.damage_trinket_priority=2|trinket.1.cooldown.remains|!trinket.1.has_cooldown|!talent.summon_gargoyle&!talent.army_of_the_dead|!talent.summon_gargoyle&talent.army_of_the_dead&cooldown.army_of_the_dead.remains_expected>20|!talent.summon_gargoyle&!talent.army_of_the_dead&cooldown.dark_transformation.remains>20|cooldown.summon_gargoyle.remains>20&!pet.gargoyle.active)|fight_remains<15" );

  variables->add_action( "variable,name=apoc_timing,op=setif,value=5,value_else=2,condition=cooldown.apocalypse.remains<10&debuff.festering_wound.stack<=4&cooldown.unholy_assault.remains>10", "Variables" );
  variables->add_action( "variable,name=pooling_runic_power,op=setif,value=1,value_else=0,condition=talent.vile_contagion&cooldown.vile_contagion.remains<3&runic_power<60&!variable.st_planning" );
  variables->add_action( "variable,name=st_planning,op=setif,value=1,value_else=0,condition=active_enemies=1&(!raid_event.adds.exists|raid_event.adds.in>15)" );
  variables->add_action( "variable,name=adds_remain,op=setif,value=1,value_else=0,condition=active_enemies>=2&(!raid_event.adds.exists|raid_event.adds.exists&raid_event.adds.remains>6)" );
}
//unholy_apl_end

}  // namespace death_knight_apl
