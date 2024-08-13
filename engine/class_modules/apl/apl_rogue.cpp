#include "simulationcraft.hpp"
#include "class_modules/apl/apl_rogue.hpp"

namespace rogue_apl {

std::string potion( const player_t* p )
{
  return ( ( p->true_level >= 71 ) ? "tempered_potion_3" :
           ( p->true_level >= 61 ) ? "elemental_potion_of_ultimate_power_3" :
           ( p->true_level >= 51 ) ? "potion_of_spectral_agility" :
           ( p->true_level >= 40 ) ? "potion_of_unbridled_fury" :
           ( p->true_level >= 35 ) ? "draenic_agility" :
           "disabled" );
}

std::string flask( const player_t* p )
{
  return ( ( p->true_level >= 71 ) ? "flask_of_alchemical_chaos_3" :
           ( p->true_level >= 61 ) ? "iced_phial_of_corrupting_rage_3" :
           ( p->true_level >= 51 ) ? "spectral_flask_of_power" :
           ( p->true_level >= 40 ) ? "greater_flask_of_the_currents" :
           ( p->true_level >= 35 ) ? "greater_draenic_agility_flask" :
           "disabled" );
}

std::string food( const player_t* p )
{
  return ( ( p->true_level >= 71 ) ? "feast_of_the_divine_day" :
           ( p->true_level >= 61 ) ? "fated_fortune_cookie" :
           ( p->true_level >= 51 ) ? "feast_of_gluttonous_hedonism" :
           ( p->true_level >= 45 ) ? "famine_evaluator_and_snack_table" :
           ( p->true_level >= 40 ) ? "lavish_suramar_feast" :
           "disabled" );
}

std::string rune( const player_t* p )
{
  return ( ( p->true_level >= 80 ) ? "crystallized" :
           ( p->true_level >= 70 ) ? "draconic" :
           ( p->true_level >= 60 ) ? "veiled" :
           ( p->true_level >= 50 ) ? "battle_scarred" :
           ( p->true_level >= 45 ) ? "defiled" :
           ( p->true_level >= 40 ) ? "hyper" :
           "disabled" );
}

std::string temporary_enchant( const player_t* p )
{
  return ( ( p->true_level >= 71 ) ? "main_hand:algari_mana_oil_3/off_hand:algari_mana_oil_3" :
           ( p->true_level >= 61 ) ? "main_hand:buzzing_rune_3/off_hand:buzzing_rune_3" :
           ( p->true_level >= 51 ) ? "main_hand:shaded_sharpening_stone/off_hand:shaded_sharpening_stone" :
           "disabled" );
}

//assassination_apl_start
void assassination( player_t* p )
{
  action_priority_list_t* default_ = p->get_action_priority_list( "default" );
  action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
  action_priority_list_t* cds = p->get_action_priority_list( "cds" );
  action_priority_list_t* direct = p->get_action_priority_list( "direct" );
  action_priority_list_t* core_dot = p->get_action_priority_list( "core_dot" );
  action_priority_list_t* aoe_dot = p->get_action_priority_list( "aoe_dot" );
  action_priority_list_t* items = p->get_action_priority_list( "items" );
  action_priority_list_t* misc_cds = p->get_action_priority_list( "misc_cds" );
  action_priority_list_t* shiv = p->get_action_priority_list( "shiv" );
  action_priority_list_t* stealthed = p->get_action_priority_list( "stealthed" );
  action_priority_list_t* vanish = p->get_action_priority_list( "vanish" );

  precombat->add_action( "apply_poison" );
  precombat->add_action( "flask" );
  precombat->add_action( "augmentation" );
  precombat->add_action( "food" );
  precombat->add_action( "snapshot_stats" );
  precombat->add_action( "variable,name=trinket_sync_slot,value=1,if=trinket.1.has_stat.any_dps&(!trinket.2.has_stat.any_dps|trinket.1.cooldown.duration>=trinket.2.cooldown.duration)&!trinket.2.is.witherbarks_branch|trinket.1.is.witherbarks_branch", "Check which trinket slots have Stat Values" );
  precombat->add_action( "variable,name=trinket_sync_slot,value=2,if=trinket.2.has_stat.any_dps&(!trinket.1.has_stat.any_dps|trinket.2.cooldown.duration>trinket.1.cooldown.duration)&!trinket.1.is.witherbarks_branch|trinket.2.is.witherbarks_branch" );
  precombat->add_action( "variable,name=effective_spend_cp,value=cp_max_spend-2<?5*talent.hand_of_fate", "Determine combo point finish condition" );
  precombat->add_action( "stealth", "Pre-cast Slice and Dice if possible" );
  precombat->add_action( "slice_and_dice,precombat_seconds=1" );

  default_->add_action( "stealth", "Restealth if possible (no vulnerable enemies in combat)" );
  default_->add_action( "kick", "Interrupt on cooldown to allow simming interactions with that" );
  default_->add_action( "variable,name=single_target,value=spell_targets.fan_of_knives<2", "Conditional to check if there is only one enemy" );
  default_->add_action( "variable,name=regen_saturated,value=energy.regen_combined>35", "Combined Energy Regen needed to saturate" );
  default_->add_action( "variable,name=not_pooling,value=(dot.deathmark.ticking|dot.kingsbane.ticking|debuff.shiv.up)|(buff.envenom.up&buff.envenom.remains<=1)|energy.pct>=(40+30*talent.hand_of_fate-15*talent.vicious_venoms)|fight_remains<=20", "Check if we should be using our energy" );
  default_->add_action( "call_action_list,name=stealthed,if=stealthed.rogue|stealthed.improved_garrote|master_assassin_remains>0" );
  default_->add_action( "call_action_list,name=cds" );
  default_->add_action( "slice_and_dice,if=!buff.slice_and_dice.up&dot.rupture.ticking&combo_points>=1&(!buff.indiscriminate_carnage.up|variable.single_target)", "Put SnD up initially for Cut to the Chase, refresh with Envenom if at low duration" );
  default_->add_action( "envenom,if=buff.slice_and_dice.up&buff.slice_and_dice.remains<5&combo_points>=5" );
  default_->add_action( "call_action_list,name=core_dot" );
  default_->add_action( "call_action_list,name=aoe_dot,if=!variable.single_target" );
  default_->add_action( "call_action_list,name=direct" );
  default_->add_action( "arcane_torrent,if=energy.deficit>=15+energy.regen_combined" );
  default_->add_action( "arcane_pulse" );
  default_->add_action( "lights_judgment" );
  default_->add_action( "bag_of_tricks" );

  cds->add_action( "variable,name=deathmark_ma_condition,value=!talent.master_assassin.enabled|dot.garrote.ticking", "Cooldowns Wait on Deathmark for Garrote with MA and check for Kingsbane" );
  cds->add_action( "variable,name=deathmark_kingsbane_condition,value=!talent.kingsbane|cooldown.kingsbane.remains<=2" );
  cds->add_action( "variable,name=deathmark_condition,value=!stealthed.rogue&dot.rupture.ticking&buff.envenom.up&!debuff.deathmark.up&variable.deathmark_ma_condition&variable.deathmark_kingsbane_condition", "Deathmark to be used if not stealthed, Rupture is up, and all other talent conditions are satisfied" );
  cds->add_action( "call_action_list,name=items", "Usages for various special-case Trinkets and other Cantrips if applicable" );
  cds->add_action( "invoke_external_buff,name=power_infusion,if=dot.deathmark.ticking", "Invoke Externals to Deathmark" );
  cds->add_action( "deathmark,if=(variable.deathmark_condition&target.time_to_die>=10)|fight_remains<=20", "Cast Deathmark if the target will survive long enough" );
  cds->add_action( "call_action_list,name=shiv", "Check for Applicable Shiv usage" );
  cds->add_action( "kingsbane,if=(debuff.shiv.up|cooldown.shiv.remains<6)&buff.envenom.up&(cooldown.deathmark.remains>=50|dot.deathmark.ticking)|fight_remains<=15" );
  cds->add_action( "thistle_tea,if=!buff.thistle_tea.up&(((energy.deficit>=100+energy.regen_combined|charges>=3)&debuff.shiv.remains>=4)|spell_targets.fan_of_knives>=4&debuff.shiv.remains>=6)|fight_remains<charges*6", "Avoid overcapped energy, use with shiv, or dump charges at the end of a fight" );
  cds->add_action( "call_action_list,name=misc_cds", "Potion/Racials/Other misc cooldowns" );
  cds->add_action( "call_action_list,name=vanish,if=!stealthed.all&master_assassin_remains=0" );
  cds->add_action( "cold_blood,if=combo_points>=5&!dot.rupture.refreshable&!buff.edge_case.up&cooldown.deathmark.remains>10", "Cold Blood as an Envenom buff, avoiding potential waste through edge case" );

  direct->add_action( "envenom,if=!buff.darkest_night.up&effective_combo_points>=variable.effective_spend_cp&(variable.not_pooling|debuff.amplifying_poison.stack>=20|effective_combo_points>cp_max_spend|!variable.single_target)&!buff.vanish.up", "Direct Damage Abilities Envenom at applicable cp if not pooling, capped on amplifying poison stacks, on an animacharged CP, or in aoe." );
  direct->add_action( "envenom,if=buff.darkest_night.up&effective_combo_points>=cp_max_spend", "Special Envenom handling for Darkest Night" );
  direct->add_action( "variable,name=use_filler,value=combo_points.deficit>1|variable.not_pooling|!variable.single_target", "Check if we should be using a filler" );
  direct->add_action( "variable,name=use_caustic_filler,value=talent.caustic_spatter&dot.rupture.ticking&(!debuff.caustic_spatter.up|debuff.caustic_spatter.remains<=2)&combo_points.deficit>1&!variable.single_target", "Maintain Caustic Spatter" );
  direct->add_action( "mutilate,if=variable.use_caustic_filler" );
  direct->add_action( "ambush,if=variable.use_caustic_filler" );
  direct->add_action( "echoing_reprimand,if=variable.use_filler|fight_remains<20", "Use Echoing Reprimand on cooldown" );
  direct->add_action( "fan_of_knives,if=variable.use_filler&!priority_rotation&(spell_targets.fan_of_knives>=3|buff.clear_the_witnesses.up)", "Fan of Knives at 3+ targets or with clear the witnesses active" );
  direct->add_action( "fan_of_knives,target_if=!dot.deadly_poison_dot.ticking&(!priority_rotation|dot.garrote.ticking|dot.rupture.ticking),if=variable.use_filler&spell_targets.fan_of_knives>=3", "Fan of Knives to apply poisons if inactive on any target (or any bleeding targets with priority rotation) at 3T" );
  direct->add_action( "ambush,if=variable.use_filler&(buff.blindside.up|stealthed.rogue)&(!dot.kingsbane.ticking|debuff.deathmark.down|buff.blindside.up)", "Ambush on Blindside/Subterfuge. Do not use Ambush from stealth during Kingsbane & Deathmark." );
  direct->add_action( "mutilate,target_if=!dot.deadly_poison_dot.ticking&!debuff.amplifying_poison.up,if=variable.use_filler&spell_targets.fan_of_knives=2", "Tab-Mutilate to apply Deadly Poison at 2 targets" );
  direct->add_action( "mutilate,if=variable.use_filler", "Fallback Mutilate" );

  core_dot->add_action( "garrote,if=combo_points.deficit>=1&(pmultiplier<=1)&refreshable&target.time_to_die-remains>12", "Core damage over time abilities used everywhere  Maintain Garrote" );
  core_dot->add_action( "rupture,if=effective_combo_points>=variable.effective_spend_cp&(pmultiplier<=1)&refreshable&target.time_to_die-remains>(4+(talent.dashing_scoundrel*5)+(variable.regen_saturated*6))&!buff.darkest_night.up", "Maintain Rupture unless darkest night is up" );

  aoe_dot->add_action( "variable,name=scent_effective_max_stacks,value=(spell_targets.fan_of_knives*talent.scent_of_blood.rank*2)>?20", "AoE Damage over time abilities Check what the maximum Scent of Blood stacks is currently" );
  aoe_dot->add_action( "variable,name=scent_saturation,value=buff.scent_of_blood.stack>=variable.scent_effective_max_stacks", "We are Scent Saturated when our stack count is hitting the maximum" );
  aoe_dot->add_action( "crimson_tempest,target_if=min:remains,if=spell_targets>=3&refreshable&pmultiplier<=1&effective_combo_points>=variable.effective_spend_cp&energy.regen_combined>25&target.time_to_die-remains>6", "Crimson Tempest on 3+ Targets if we have enough energy regen" );
  aoe_dot->add_action( "garrote,cycle_targets=1,if=combo_points.deficit>=1&(pmultiplier<=1)&refreshable&!variable.regen_saturated&spell_targets.fan_of_knives>=2&target.time_to_die-remains>12", "Garrote upkeep, also uses it in AoE to reach energy saturation" );
  aoe_dot->add_action( "rupture,cycle_targets=1,if=effective_combo_points>=variable.effective_spend_cp&(pmultiplier<=1)&refreshable&(!variable.regen_saturated&(talent.scent_of_blood.rank=2|talent.scent_of_blood.rank<=1&(buff.indiscriminate_carnage.up|target.time_to_die-remains>15)))&target.time_to_die-remains>(4+(talent.dashing_scoundrel*5)+(variable.regen_saturated*6))&!buff.darkest_night.up", "Rupture upkeep, also uses it in AoE to reach energy saturation" );
  aoe_dot->add_action( "garrote,if=refreshable&combo_points.deficit>=1&(pmultiplier<=1|remains<=tick_time&spell_targets.fan_of_knives>=3)&(remains<=tick_time*2&spell_targets.fan_of_knives>=3)&(target.time_to_die-remains)>4&master_assassin_remains=0", "Garrote as a special generator for the last CP before a finisher for edge case handling" );

  items->add_action( "variable,name=base_trinket_condition,value=dot.rupture.ticking&cooldown.deathmark.remains<2|fight_remains<=22", "Special Case Trinkets" );
  items->add_action( "use_item,name=ashes_of_the_embersoul,use_off_gcd=1,if=(dot.kingsbane.ticking&dot.kingsbane.remains<=11)|fight_remains<=22" );
  items->add_action( "use_item,name=algethar_puzzle_box,use_off_gcd=1,if=variable.base_trinket_condition" );
  items->add_action( "use_item,name=treacherous_transmitter,use_off_gcd=1,if=variable.base_trinket_condition" );
  items->add_action( "do_treacherous_transmitter_task,use_off_gcd=1,if=dot.deathmark.ticking|fight_remains<=20" );
  items->add_action( "use_item,name=imperfect_ascendancy_serum,use_off_gcd=1,if=variable.base_trinket_condition" );
  items->add_action( "use_items,slots=trinket1,if=(variable.trinket_sync_slot=1&(debuff.deathmark.up|fight_remains<=20)|(variable.trinket_sync_slot=2&(!trinket.2.cooldown.ready|!debuff.deathmark.up&cooldown.deathmark.remains>20))|!variable.trinket_sync_slot)", "Fallback case for using stat trinkets" );
  items->add_action( "use_items,slots=trinket2,if=(variable.trinket_sync_slot=2&(debuff.deathmark.up|fight_remains<=20)|(variable.trinket_sync_slot=1&(!trinket.1.cooldown.ready|!debuff.deathmark.up&cooldown.deathmark.remains>20))|!variable.trinket_sync_slot)" );

  misc_cds->add_action( "potion,if=buff.bloodlust.react|fight_remains<30|debuff.deathmark.up", "Miscellaneous Cooldowns Potion" );
  misc_cds->add_action( "blood_fury,if=debuff.deathmark.up", "Various special racials to be synced with cooldowns" );
  misc_cds->add_action( "berserking,if=debuff.deathmark.up" );
  misc_cds->add_action( "fireblood,if=debuff.deathmark.up" );
  misc_cds->add_action( "ancestral_call,if=(!talent.kingsbane&debuff.deathmark.up&debuff.shiv.up)|(talent.kingsbane&debuff.deathmark.up&dot.kingsbane.ticking&dot.kingsbane.remains<8)" );

  shiv->add_action( "variable,name=shiv_condition,value=!debuff.shiv.up&dot.garrote.ticking&dot.rupture.ticking", "Shiv conditions" );
  shiv->add_action( "variable,name=shiv_kingsbane_condition,value=talent.kingsbane&buff.envenom.up&variable.shiv_condition" );
  shiv->add_action( "shiv,if=talent.arterial_precision&variable.shiv_condition&spell_targets.fan_of_knives>=4&dot.crimson_tempest.ticking|fight_remains<=charges*8", "Shiv for aoe with Arterial Precision" );
  shiv->add_action( "shiv,if=!talent.lightweight_shiv.enabled&variable.shiv_kingsbane_condition&(dot.kingsbane.ticking&dot.kingsbane.remains<8|cooldown.kingsbane.remains>=24)&(!talent.crimson_tempest.enabled|variable.single_target|dot.crimson_tempest.ticking)|fight_remains<=charges*8", "Shiv cases for Kingsbane" );
  shiv->add_action( "shiv,if=talent.lightweight_shiv.enabled&variable.shiv_kingsbane_condition&(dot.kingsbane.ticking|cooldown.kingsbane.remains<=1)|fight_remains<=charges*8" );
  shiv->add_action( "shiv,if=talent.arterial_precision&variable.shiv_condition&debuff.deathmark.up|fight_remains<=charges*8", "Fallback shiv for arterial during deathmark" );
  shiv->add_action( "shiv,if=!talent.kingsbane&!talent.arterial_precision&variable.shiv_condition&(!talent.crimson_tempest.enabled|variable.single_target|dot.crimson_tempest.ticking)|fight_remains<=charges*8", "Fallback if no special cases apply" );

  stealthed->add_action( "pool_resource,for_next=1", "Stealthed Actions" );
  stealthed->add_action( "ambush,if=!debuff.deathstalkers_mark.up&talent.deathstalkers_mark&!buff.darkest_night.up", "Apply Deathstalkers Mark if it has fallen off and we have no way to reapply otherwise" );
  stealthed->add_action( "shiv,if=talent.kingsbane&(dot.kingsbane.ticking|cooldown.kingsbane.up)&(!debuff.shiv.up&debuff.shiv.remains<1)&buff.envenom.up", "Make sure to have Shiv up during Kingsbane as a final check" );
  stealthed->add_action( "envenom,if=effective_combo_points>=variable.effective_spend_cp&dot.kingsbane.ticking&buff.envenom.remains<=3", "Envenom to maintain the buff during Subterfuge" );
  stealthed->add_action( "envenom,if=effective_combo_points>=variable.effective_spend_cp&buff.master_assassin_aura.up&variable.single_target", "Envenom during Master Assassin in single target" );
  stealthed->add_action( "rupture,target_if=effective_combo_points>=variable.effective_spend_cp&buff.indiscriminate_carnage.up&refreshable&(!variable.regen_saturated|!variable.scent_saturation|!dot.rupture.ticking)", "Rupture during Indiscriminate Carnage" );
  stealthed->add_action( "garrote,target_if=min:remains,if=stealthed.improved_garrote&(remains<12|pmultiplier<=1|(buff.indiscriminate_carnage.up&active_dot.garrote<spell_targets.fan_of_knives))&!variable.single_target&target.time_to_die-remains>2", "Improved Garrote: Apply or Refresh with buffed Garrotes, accounting for Indiscriminate Carnage" );
  stealthed->add_action( "garrote,if=stealthed.improved_garrote&(pmultiplier<=1|remains<12|!variable.single_target&buff.master_assassin_aura.remains<3)&combo_points.deficit>=1+2*talent.shrouded_suffocation" );

  vanish->add_action( "pool_resource,for_next=1,extra_amount=45", "Stealth Cooldowns Vanish Sync for Improved Garrote with Deathmark" );
  vanish->add_action( "vanish,if=!buff.fatebound_lucky_coin.up&(buff.fatebound_coin_tails.stack>=5|buff.fatebound_coin_heads.stack>=5)", "Vanish to fish for Fateful Ending if possible" );
  vanish->add_action( "vanish,if=!talent.master_assassin&!talent.indiscriminate_carnage&talent.improved_garrote&cooldown.garrote.up&(dot.garrote.pmultiplier<=1|dot.garrote.refreshable)&(debuff.deathmark.up|cooldown.deathmark.remains<4)&combo_points.deficit>=(spell_targets.fan_of_knives>?4)", "Vanish to spread Garrote during Deathmark without Indiscriminate Carnage" );
  vanish->add_action( "pool_resource,for_next=1,extra_amount=45" );
  vanish->add_action( "vanish,if=!talent.master_assassin&talent.indiscriminate_carnage&talent.improved_garrote&cooldown.garrote.up&(dot.garrote.pmultiplier<=1|dot.garrote.refreshable)&spell_targets.fan_of_knives>2&(target.time_to_die-remains>15|raid_event.adds.in>20)", "Vanish for cleaving Garrotes with Indiscriminate Carnage" );
  vanish->add_action( "vanish,if=!talent.improved_garrote&talent.master_assassin&!dot.rupture.refreshable&dot.garrote.remains>3&debuff.deathmark.up&(debuff.shiv.up|debuff.deathmark.remains<4)", "Vanish fallback for Master Assassin" );
  vanish->add_action( "vanish,if=talent.improved_garrote&cooldown.garrote.up&(dot.garrote.pmultiplier<=1|dot.garrote.refreshable)&(debuff.deathmark.up|cooldown.deathmark.remains<4)&raid_event.adds.in>30", "Vanish fallback for Improved Garrote during Deathmark if no add waves are expected" );
}
//assassination_apl_end

//outlaw_apl_start
void outlaw( player_t* p )
{
  action_priority_list_t* default_ = p->get_action_priority_list( "default" );
  action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
  action_priority_list_t* build = p->get_action_priority_list( "build" );
  action_priority_list_t* cds = p->get_action_priority_list( "cds" );
  action_priority_list_t* finish = p->get_action_priority_list( "finish" );
  action_priority_list_t* stealth = p->get_action_priority_list( "stealth" );
  action_priority_list_t* stealth_cds = p->get_action_priority_list( "stealth_cds" );

  precombat->add_action( "apply_poison,nonlethal=none,lethal=instant" );
  precombat->add_action( "flask" );
  precombat->add_action( "augmentation" );
  precombat->add_action( "food" );
  precombat->add_action( "snapshot_stats", "Snapshot raid buffed stats before combat begins and pre-potting is done." );
  precombat->add_action( "use_item,name=imperfect_ascendancy_serum" );
  precombat->add_action( "stealth,precombat_seconds=2" );
  precombat->add_action( "roll_the_bones,precombat_seconds=2" );
  precombat->add_action( "adrenaline_rush,precombat_seconds=1,if=talent.improved_adrenaline_rush" );
  precombat->add_action( "slice_and_dice,precombat_seconds=1" );
  precombat->add_action( "stealth" );

  default_->add_action( "stealth", "Restealth if possible (no vulnerable enemies in combat)" );
  default_->add_action( "kick", "Interrupt on cooldown to allow simming interactions with that" );
  default_->add_action( "variable,name=rtb_reroll,value=rtb_buffs.will_lose=(rtb_buffs.will_lose.buried_treasure+rtb_buffs.will_lose.grand_melee&spell_targets.blade_flurry<2&raid_event.adds.in>12&raid_event.adds.count<2)", "Variables that define the reroll rules for Roll the Bones  Default rule: reroll if the only buff that will be rolled away is Buried Treasure, or Grand Melee in single target without upcoming adds" );
  default_->add_action( "variable,name=rtb_reroll,if=talent.loaded_dice,value=rtb_buffs.will_lose=buff.loaded_dice.up", "If Loaded Dice is talented, then keep any 1 buff from Roll the Bones but roll it into 2 buffs when Loaded Dice is active" );
  default_->add_action( "variable,name=rtb_reroll,value=variable.rtb_reroll&rtb_buffs.longer=0|rtb_buffs.normal=0&rtb_buffs.longer>=1&rtb_buffs<6&rtb_buffs.max_remains<=39&!stealthed.all&buff.loaded_dice.up", "If all active Roll the Bones buffs are ahead of its container buff and have under 40s remaining, then reroll again with Loaded Dice active in an attempt to get even more buffs" );
  default_->add_action( "variable,name=rtb_reroll,op=reset,if=!(raid_event.adds.remains>12|raid_event.adds.up&(raid_event.adds.in-raid_event.adds.remains)<6|target.time_to_die>12)|fight_remains<12", "Avoid rerolls when we will not have time remaining on the fight or add wave to recoup the opportunity cost of the global" );
  default_->add_action( "variable,name=ambush_condition,value=(talent.hidden_opportunity|combo_points.deficit>=2+talent.improved_ambush+buff.broadside.up)&energy>=50" );
  default_->add_action( "variable,name=finish_condition,value=effective_combo_points>=cp_max_spend-1-(stealthed.all&talent.crackshot|(talent.hand_of_fate|talent.flawless_form)&talent.hidden_opportunity&(buff.audacity.up|buff.opportunity.up))", "Use finishers if at -1 from max combo points, or -2 in Stealth with Crackshot. With the hero trees, Hidden Opportunity builds also finish at -2 if Audacity or Opportunity is active" );
  default_->add_action( "variable,name=blade_flurry_sync,value=spell_targets.blade_flurry<2&raid_event.adds.in>20|buff.blade_flurry.remains>gcd", "With multiple targets, this variable is checked to decide whether some CDs should be synced with Blade Flurry" );
  default_->add_action( "call_action_list,name=cds" );
  default_->add_action( "call_action_list,name=stealth,if=stealthed.all", "High priority stealth list, will fall through if no conditions are met" );
  default_->add_action( "run_action_list,name=finish,if=variable.finish_condition" );
  default_->add_action( "call_action_list,name=build" );
  default_->add_action( "arcane_torrent,if=energy.base_deficit>=15+energy.regen" );
  default_->add_action( "arcane_pulse" );
  default_->add_action( "lights_judgment" );
  default_->add_action( "bag_of_tricks" );

  build->add_action( "echoing_reprimand", "Builders" );
  build->add_action( "ambush,if=talent.hidden_opportunity&buff.audacity.up", "High priority Ambush for Hidden Opportunity builds" );
  build->add_action( "pistol_shot,if=talent.fan_the_hammer&talent.audacity&talent.hidden_opportunity&buff.opportunity.up&!buff.audacity.up", "With Audacity + Hidden Opportunity + Fan the Hammer, consume Opportunity to proc Audacity any time Ambush is not available" );
  build->add_action( "pistol_shot,if=talent.fan_the_hammer&buff.opportunity.up&(buff.opportunity.stack>=buff.opportunity.max_stack|buff.opportunity.remains<2)", "With Fan the Hammer, consume Opportunity as a higher priority if at max stacks or if it will expire" );
  build->add_action( "pistol_shot,if=talent.fan_the_hammer&buff.opportunity.up&(combo_points.deficit>=(1+(talent.quick_draw+buff.broadside.up)*(talent.fan_the_hammer.rank+1))|combo_points<=talent.ruthlessness)", "With Fan the Hammer, consume Opportunity if it will not overcap CPs, or with 1 CP at minimum" );
  build->add_action( "pistol_shot,if=!talent.fan_the_hammer&buff.opportunity.up&(energy.base_deficit>energy.regen*1.5|combo_points.deficit<=1+buff.broadside.up|talent.quick_draw.enabled|talent.audacity.enabled&!buff.audacity.up)", "If not using Fan the Hammer, then consume Opportunity based on energy, when it will exactly cap CPs, or when using Quick Draw" );
  build->add_action( "pool_resource,for_next=1", "Fallback pooling just so Sinister Strike is never casted if Ambush is available for Hidden Opportunity builds" );
  build->add_action( "ambush,if=talent.hidden_opportunity" );
  build->add_action( "sinister_strike" );

  cds->add_action( "adrenaline_rush,if=!buff.adrenaline_rush.up&(!variable.finish_condition|!talent.improved_adrenaline_rush)|stealthed.all&talent.crackshot&talent.improved_adrenaline_rush&combo_points<=2", "Cooldowns  Use Adrenaline Rush if it is not active and the finisher condition is not met, but Crackshot builds can refresh it with 2cp or lower inside stealth" );
  cds->add_action( "blade_flurry,if=spell_targets>=2&buff.blade_flurry.remains<gcd", "Maintain Blade Flurry on 2+ targets" );
  cds->add_action( "blade_flurry,if=talent.deft_maneuvers&!variable.finish_condition&(spell_targets>=3&combo_points.deficit=spell_targets+buff.broadside.up|spell_targets>=5)", "With Deft Maneuvers, use Blade Flurry on cooldown at 5+ targets, or at 3-4 targets if missing combo points equal to the amount given" );
  cds->add_action( "roll_the_bones,if=variable.rtb_reroll|rtb_buffs=0|rtb_buffs.max_remains<=7&cooldown.vanish.ready&talent.crackshot", "Use Roll the Bones if reroll conditions are met, or with no buffs, or seven seconds early if about to enter a Vanish window with Crackshot" );
  cds->add_action( "keep_it_rolling,if=rtb_buffs>=4&(rtb_buffs.min_remains<2|buff.broadside.up)", "Use Keep it Rolling with any 4 buffs. If Broadside is not active, then wait until just before the lowest buff expires in an attempt to obtain it from Count the Odds." );
  cds->add_action( "ghostly_strike,if=combo_points<cp_max_spend" );
  cds->add_action( "use_item,name=manic_grieftorch,if=!stealthed.all&buff.between_the_eyes.up|fight_remains<=5", "Trinkets that should not be used during stealth and have higher priority than entering stealth" );
  cds->add_action( "use_item,name=beacon_to_the_beyond,if=!stealthed.all&buff.between_the_eyes.up|fight_remains<=5" );
  cds->add_action( "use_item,name=imperfect_ascendancy_serum,if=!stealthed.all|fight_remains<=22" );
  cds->add_action( "killing_spree,if=variable.finish_condition&!stealthed.all", "Killing Spree has higher priority than stealth cooldowns" );
  cds->add_action( "call_action_list,name=stealth_cds,if=!stealthed.all&(!talent.crackshot|cooldown.between_the_eyes.ready)", "Crackshot builds use stealth cooldowns if Between the Eyes is ready" );
  cds->add_action( "thistle_tea,if=!buff.thistle_tea.up&(energy.base_deficit>=150|fight_remains<charges*6)" );
  cds->add_action( "blade_rush,if=energy.base_time_to_max>4&!stealthed.all", "Use Blade Rush at minimal energy outside of stealth" );
  cds->add_action( "potion,if=buff.bloodlust.react|fight_remains<30|buff.adrenaline_rush.up" );
  cds->add_action( "blood_fury" );
  cds->add_action( "berserking" );
  cds->add_action( "fireblood" );
  cds->add_action( "ancestral_call" );
  cds->add_action( "use_item,name=elementium_pocket_anvil,use_off_gcd=1,if=gcd.remains<=action.sinister_strike.gcd%2", "Default conditions for usable items." );
  cds->add_action( "use_item,name=dragonfire_bomb_dispenser,use_off_gcd=1,if=gcd.remains<=action.sinister_strike.gcd%2&((!trinket.1.is.dragonfire_bomb_dispenser&trinket.1.cooldown.remains>10|trinket.2.cooldown.remains>10)|cooldown.dragonfire_bomb_dispenser.charges>2|fight_remains<20|!trinket.2.has_cooldown|!trinket.1.has_cooldown)", "Use Bomb Dispenser on cooldown, but hold if 2nd trinket is nearly off cooldown, unless at max charges or sim duration ends soon" );
  cds->add_action( "use_item,name=stormeaters_boon,if=spell_targets.blade_flurry>desired_targets|raid_event.adds.in>60|raid_event.adds.count<2|fight_remains<10" );
  cds->add_action( "use_item,name=enduring_dreadplate,if=spell_targets.blade_flurry>desired_targets|raid_event.adds.in>60|raid_event.adds.count<2|fight_remains<16" );
  cds->add_action( "use_item,name=windscar_whetstone,if=spell_targets.blade_flurry>desired_targets|raid_event.adds.in>60|raid_event.adds.count<2|fight_remains<7" );
  cds->add_action( "use_items,slots=trinket1,if=buff.between_the_eyes.up|trinket.1.has_stat.any_dps|fight_remains<=20" );
  cds->add_action( "use_items,slots=trinket2,if=buff.between_the_eyes.up|trinket.2.has_stat.any_dps|fight_remains<=20" );

  finish->add_action( "between_the_eyes,if=!talent.crackshot&(buff.between_the_eyes.remains<4|talent.improved_between_the_eyes|talent.greenskins_wickers)&!buff.greenskins_wickers.up", "Finishers  Use Between the Eyes to keep the crit buff up, but on cooldown if Improved/Greenskins, and avoid overriding Greenskins" );
  finish->add_action( "between_the_eyes,if=talent.crackshot&(cooldown.vanish.remains>45|talent.underhanded_upper_hand&talent.without_a_trace&(buff.adrenaline_rush.remains>10|buff.adrenaline_rush.down&cooldown.adrenaline_rush.remains>45))&(raid_event.adds.remains>8|raid_event.adds.in<raid_event.adds.remains|!raid_event.adds.up)", "Crackshot builds use Between the Eyes outside of Stealth if we are unlikely to enter a Stealth window before the next BtE cast or if we are unlikely to lose Adrenaline Rush uptime by hitting BtE before the next cast of Vanish" );
  finish->add_action( "slice_and_dice,if=buff.slice_and_dice.remains<fight_remains&refreshable" );
  finish->add_action( "cold_blood" );
  finish->add_action( "coup_de_grace" );
  finish->add_action( "dispatch" );

  stealth->add_action( "cold_blood,if=variable.finish_condition", "Stealth" );
  stealth->add_action( "pool_resource,for_next=1", "Ensure Crackshot BtE is not skipped because of low energy" );
  stealth->add_action( "between_the_eyes,if=variable.finish_condition&talent.crackshot&(!buff.shadowmeld.up|stealthed.rogue)", "High priority Between the Eyes for Crackshot, except not directly out of Shadowmeld" );
  stealth->add_action( "dispatch,if=variable.finish_condition" );
  stealth->add_action( "pistol_shot,if=talent.crackshot&talent.fan_the_hammer.rank>=2&buff.opportunity.stack>=6&(buff.broadside.up&combo_points<=1|buff.greenskins_wickers.up)", "2 Fan the Hammer Crackshot builds can consume Opportunity in stealth with max stacks, Broadside, and low CPs, or with Greenskins active" );
  stealth->add_action( "ambush,if=talent.hidden_opportunity" );

  stealth_cds->add_action( "vanish,if=talent.underhanded_upper_hand&talent.subterfuge&(buff.adrenaline_rush.up|!talent.without_a_trace&talent.crackshot)&(variable.finish_condition|!talent.crackshot&(variable.ambush_condition|!talent.hidden_opportunity))", "Stealth Cooldowns  Builds with Underhanded Upper Hand and Subterfuge (and Without a Trace for Crackshot) must use Vanish while Adrenaline Rush is active" );
  stealth_cds->add_action( "vanish,if=!talent.underhanded_upper_hand&talent.crackshot&variable.finish_condition", "Builds without Underhanded Upper Hand but with Crackshot must still use Vanish into Between the Eyes on cooldown" );
  stealth_cds->add_action( "vanish,if=!talent.underhanded_upper_hand&!talent.crackshot&talent.hidden_opportunity&!buff.audacity.up&buff.opportunity.stack<buff.opportunity.max_stack&variable.ambush_condition", "Builds without Underhanded Upper Hand and Crackshot but still Hidden Opportunity use Vanish into Ambush when Audacity is not active and under max Opportunity stacks" );
  stealth_cds->add_action( "vanish,if=!talent.underhanded_upper_hand&!talent.crackshot&!talent.hidden_opportunity&talent.fateful_ending&(!buff.fatebound_lucky_coin.up&(buff.fatebound_coin_tails.stack>=5|buff.fatebound_coin_heads.stack>=5)|buff.fatebound_lucky_coin.up&!cooldown.between_the_eyes.ready)", "Builds without Underhanded Upper Hand, Crackshot, and Hidden Opportunity but with Fatebound use Vanish at five stacks of either Fatebound coin in order to proc the Lucky Coin if it's not already active, and otherwise continue to Vanish into a Dispatch to proc Double Jeopardy on a biased coin" );
  stealth_cds->add_action( "vanish,if=!talent.underhanded_upper_hand&!talent.crackshot&!talent.hidden_opportunity&!talent.fateful_ending&talent.take_em_by_surprise&!buff.take_em_by_surprise.up", "Builds with none of the above can use Vanish to maintain Take 'em By Surprise" );
  stealth_cds->add_action( "shadowmeld,if=variable.finish_condition&!cooldown.vanish.ready" );
}
//outlaw_apl_end

//subtlety_apl_start
void subtlety( player_t* p )
{
  action_priority_list_t* default_ = p->get_action_priority_list( "default" );
  action_priority_list_t* precombat = p->get_action_priority_list( "precombat" );
  action_priority_list_t* build = p->get_action_priority_list( "build" );
  action_priority_list_t* cds = p->get_action_priority_list( "cds" );
  action_priority_list_t* finish = p->get_action_priority_list( "finish" );
  action_priority_list_t* stealth_cds = p->get_action_priority_list( "stealth_cds" );
  action_priority_list_t* stealthed = p->get_action_priority_list( "stealthed" );

  precombat->add_action( "apply_poison" );
  precombat->add_action( "flask" );
  precombat->add_action( "augmentation" );
  precombat->add_action( "food" );
  precombat->add_action( "snapshot_stats", "Snapshot raid buffed stats before combat begins and pre-potting is done." );
  precombat->add_action( "stealth" );
  precombat->add_action( "variable,name=algethar_puzzle_box_precombat_cast,value=3" );
  precombat->add_action( "use_item,name=algethar_puzzle_box" );
  precombat->add_action( "slice_and_dice,precombat_seconds=1" );

  default_->add_action( "stealth", "Restealth if possible (no vulnerable enemies in combat)" );
  default_->add_action( "kick", "Interrupt on cooldown to allow simming interactions with that" );
  default_->add_action( "variable,name=snd_condition,value=buff.slice_and_dice.up|spell_targets.shuriken_storm>=cp_max_spend", "Used to determine whether cooldowns wait for SnD based on targets." );
  default_->add_action( "call_action_list,name=cds", "Check CDs at first" );
  default_->add_action( "slice_and_dice,if=spell_targets.shuriken_storm<cp_max_spend&buff.slice_and_dice.remains<gcd.max&fight_remains>6&combo_points>=4", "Apply Slice and Dice at 4+ CP if it expires within the next GCD or is not up" );
  default_->add_action( "run_action_list,name=stealthed,if=stealthed.all", "Run fully switches to the Stealthed Rotation (by doing so, it forces pooling if nothing is available)." );
  default_->add_action( "variable,name=priority_rotation,value=priority_rotation", "Only change rotation if we have priority_rotation set." );
  default_->add_action( "variable,name=stealth_threshold,value=20+talent.vigor.rank*25+talent.thistle_tea*20+talent.shadowcraft*20", "Used to define when to use stealth CDs or builders" );
  default_->add_action( "variable,name=stealth_helper,value=energy>=variable.stealth_threshold" );
  default_->add_action( "variable,name=stealth_helper,value=energy.deficit<=variable.stealth_threshold,if=!talent.vigor|talent.shadowcraft" );
  default_->add_action( "call_action_list,name=stealth_cds,if=variable.stealth_helper|talent.invigorating_shadowdust", "Consider using a Stealth CD when reaching the energy threshold" );
  default_->add_action( "call_action_list,name=finish,if=effective_combo_points>=cp_max_spend" );
  default_->add_action( "call_action_list,name=finish,if=combo_points.deficit<=1|fight_remains<=1&effective_combo_points>=3", "Finish at maximum or close to maximum combo point value" );
  default_->add_action( "call_action_list,name=finish,if=spell_targets.shuriken_storm>=4&effective_combo_points>=4" );
  default_->add_action( "call_action_list,name=build,if=energy.deficit<=variable.stealth_threshold", "Use a builder when reaching the energy threshold" );
  default_->add_action( "arcane_torrent,if=energy.deficit>=15+energy.regen", "Lowest priority in all of the APL because it causes a GCD" );
  default_->add_action( "arcane_pulse" );
  default_->add_action( "lights_judgment" );
  default_->add_action( "bag_of_tricks" );

  build->add_action( "shuriken_storm,if=spell_targets>=2+(talent.gloomblade&buff.lingering_shadow.remains>=6|buff.perforated_veins.up)", "Builders  Keep using Shuriken Storm for Lingering Shadows on high stacks." );
  build->add_action( "gloomblade" );
  build->add_action( "backstab" );

  cds->add_action( "variable,name=trinket_conditions,value=(!equipped.witherbarks_branch|equipped.witherbarks_branch&trinket.witherbarks_branch.cooldown.remains<=8|equipped.bandolier_of_twisted_blades|talent.invigorating_shadowdust)", "Cooldowns  Helper Variable for Flagellation for trinket synchronisation" );
  cds->add_action( "cold_blood,if=!talent.secret_technique&combo_points>=5", "Cold Blood on 5 combo points when not playing Secret Technique" );
  cds->add_action( "sepsis,if=variable.snd_condition&target.time_to_die>=16&(buff.perforated_veins.up|!talent.perforated_veins)" );
  cds->add_action( "flagellation,target_if=max:target.time_to_die,if=variable.snd_condition&combo_points>=5&target.time_to_die>10&(variable.trinket_conditions&cooldown.shadow_blades.remains<=3|fight_remains<=28|cooldown.shadow_blades.remains>=14&talent.invigorating_shadowdust&talent.double_dance)&(!talent.invigorating_shadowdust|talent.sepsis|!talent.double_dance|talent.invigorating_shadowdust.rank=2&spell_targets.shuriken_storm>=2|cooldown.symbols_of_death.remains<=3|buff.symbols_of_death.remains>3)", "Defines Flagellation use in a stacked manner with trinkets and Shadow Blades" );
  cds->add_action( "symbols_of_death,if=variable.snd_condition&(!buff.the_rotten.up|!set_bonus.tier30_2pc)&buff.symbols_of_death.remains<=3&(!talent.flagellation|cooldown.flagellation.remains>10|buff.shadow_dance.remains>=2&talent.invigorating_shadowdust|cooldown.flagellation.up&combo_points>=5&!talent.invigorating_shadowdust)", "Align Symbols of Death to Flagellation." );
  cds->add_action( "shadow_blades,if=variable.snd_condition&(combo_points<=1|set_bonus.tier31_4pc)&(buff.flagellation_buff.up|buff.flagellation_persist.up|!talent.flagellation)", "Align Shadow Blades to Flagellation." );
  cds->add_action( "echoing_reprimand,if=variable.snd_condition&combo_points.deficit>=3", "ER during Shadow Dance." );
  cds->add_action( "shuriken_tornado,if=variable.snd_condition&buff.symbols_of_death.up&combo_points<=2&!buff.premeditation.up&(!talent.flagellation|cooldown.flagellation.remains>20)&spell_targets.shuriken_storm>=3", "Shuriken Tornado with Symbols of Death on 3 and more targets" );
  cds->add_action( "shuriken_tornado,if=variable.snd_condition&!buff.shadow_dance.up&!buff.flagellation_buff.up&!buff.flagellation_persist.up&!buff.shadow_blades.up&spell_targets.shuriken_storm<=2&!raid_event.adds.up", "Shuriken Tornado only outside of cooldowns" );
  cds->add_action( "shadow_dance,if=!buff.shadow_dance.up&fight_remains<=8+talent.subterfuge.enabled" );
  cds->add_action( "goremaws_bite,if=variable.snd_condition&combo_points.deficit>=3&(!cooldown.shadow_dance.up|talent.double_dance&buff.shadow_dance.up&!talent.invigorating_shadowdust|spell_targets.shuriken_storm<4&!talent.invigorating_shadowdust|talent.the_rotten|raid_event.adds.up)", "Goremaws Bite during Shadow Dance if possible." );
  cds->add_action( "thistle_tea,if=!buff.thistle_tea.up&cooldown.thistle_tea.charges_fractional>=2.5&buff.shadow_dance.remains>=4", "Thistle Tea during Shadow Dance when close to max stacks." );
  cds->add_action( "thistle_tea,if=!buff.thistle_tea.up&buff.shadow_dance.remains>=4&cooldown.secret_technique.remains<=10", "Thistle Tea during Shadow Dance when Secret Techniques is up." );
  cds->add_action( "thistle_tea,if=!buff.thistle_tea.up&(energy.deficit>=(100)|!buff.thistle_tea.up&fight_remains<=(6*cooldown.thistle_tea.charges))&(cooldown.symbols_of_death.remains>=3|buff.symbols_of_death.up)&combo_points.deficit>=2", "Thistle Tea for energy" );
  cds->add_action( "potion,if=buff.bloodlust.react|fight_remains<30|buff.symbols_of_death.up&(buff.shadow_blades.up|cooldown.shadow_blades.remains<=10)" );
  cds->add_action( "variable,name=racial_sync,value=buff.shadow_blades.up|!talent.shadow_blades&buff.symbols_of_death.up|fight_remains<20" );
  cds->add_action( "blood_fury,if=variable.racial_sync" );
  cds->add_action( "berserking,if=variable.racial_sync" );
  cds->add_action( "fireblood,if=variable.racial_sync" );
  cds->add_action( "ancestral_call,if=variable.racial_sync" );
  cds->add_action( "use_item,name=ashes_of_the_embersoul,if=(buff.cold_blood.up|(!talent.danse_macabre&buff.shadow_dance.up|buff.danse_macabre.stack>=3)&!talent.cold_blood)|fight_remains<10", "Sync specific trinkets to Flagellation or Shadow Dance." );
  cds->add_action( "use_item,name=witherbarks_branch,if=buff.flagellation_buff.up&talent.invigorating_shadowdust|buff.shadow_blades.up|equipped.bandolier_of_twisted_blades&raid_event.adds.up" );
  cds->add_action( "use_item,name=mirror_of_fractured_tomorrows,if=buff.shadow_dance.up&(target.time_to_die>=15|equipped.ashes_of_the_embersoul)" );
  cds->add_action( "use_item,name=beacon_to_the_beyond,use_off_gcd=1,if=!stealthed.all&(buff.deeper_daggers.up|!talent.deeper_daggers)&(!raid_event.adds.up|!equipped.stormeaters_boon|trinket.stormeaters_boon.cooldown.remains>20)" );
  cds->add_action( "use_item,name=manic_grieftorch,use_off_gcd=1,if=!stealthed.all&(!raid_event.adds.up|!equipped.stormeaters_boon|trinket.stormeaters_boon.cooldown.remains>20)" );
  cds->add_action( "invoke_external_buff,name=power_infusion,if=buff.shadow_dance.up", "PI" );
  cds->add_action( "use_items,if=!stealthed.all&(!trinket.mirror_of_fractured_tomorrows.cooldown.ready|!equipped.mirror_of_fractured_tomorrows)&(!trinket.ashes_of_the_embersoul.cooldown.ready|!equipped.ashes_of_the_embersoul)|fight_remains<10", "Default fallback for usable items: Use outside of Stealth/Shadow Dance." );

  finish->add_action( "variable,name=secret_condition,value=(action.gloomblade.used_for_danse|action.shadowstrike.used_for_danse|action.backstab.used_for_danse|action.shuriken_storm.used_for_danse)&(action.eviscerate.used_for_danse|action.black_powder.used_for_danse|action.rupture.used_for_danse)|!talent.danse_macabre", "Finisher  Defines what abilities need to be used for DM stacks before casting Secret Technique." );
  finish->add_action( "rupture,if=!dot.rupture.ticking&target.time_to_die-remains>6", "Apply Rupture if its not up." );
  finish->add_action( "variable,name=premed_snd_condition,value=talent.premeditation.enabled&spell_targets.shuriken_storm<5" );
  finish->add_action( "slice_and_dice,if=!stealthed.all&!variable.premed_snd_condition&spell_targets.shuriken_storm<6&!buff.shadow_dance.up&buff.slice_and_dice.remains<fight_remains&refreshable", "Refresh Slice and Dice outside of Shadow Dance." );
  finish->add_action( "variable,name=skip_rupture,value=buff.thistle_tea.up&spell_targets.shuriken_storm=1|buff.shadow_dance.up&(spell_targets.shuriken_storm=1|dot.rupture.ticking&spell_targets.shuriken_storm>=2)", "Variable to decide when not to use Rupture." );
  finish->add_action( "rupture,if=(!variable.skip_rupture|variable.priority_rotation)&target.time_to_die-remains>6&refreshable" );
  finish->add_action( "rupture,if=buff.finality_rupture.up&buff.shadow_dance.up&spell_targets.shuriken_storm<=4&!action.rupture.used_for_danse", "Refresh Rupture during Shadow Dance with Finality." );
  finish->add_action( "cold_blood,if=variable.secret_condition&cooldown.secret_technique.ready" );
  finish->add_action( "secret_technique,if=variable.secret_condition&(!talent.cold_blood|cooldown.cold_blood.remains>buff.shadow_dance.remains-2|!talent.improved_shadow_dance)", "Synchronizes Secret to Cold Blood if possible. Defaults to use once a builder and finisher is used." );
  finish->add_action( "rupture,cycle_targets=1,if=!variable.skip_rupture&!variable.priority_rotation&spell_targets.shuriken_storm>=2&target.time_to_die>=(2*combo_points)&refreshable", "Multidotting targets that will live long enough, refresh during pandemic." );
  finish->add_action( "rupture,if=!variable.skip_rupture&remains<cooldown.symbols_of_death.remains+10&cooldown.symbols_of_death.remains<=5&target.time_to_die-remains>cooldown.symbols_of_death.remains+5", "Refresh Rupture early if it will expire during Symbols. Do that refresh if SoD gets ready in the next 5s." );
  finish->add_action( "black_powder,if=!variable.priority_rotation&spell_targets>=3" );
  finish->add_action( "coup_de_grace" );
  finish->add_action( "eviscerate" );

  stealth_cds->add_action( "variable,name=shd_threshold,value=cooldown.shadow_dance.charges_fractional>=0.75+talent.double_dance", "Stealth Cooldowns  Helper Variable for Shadow Dance." );
  stealth_cds->add_action( "variable,name=rotten_cb,value=(!buff.the_rotten.up|!set_bonus.tier30_2pc)&(!talent.cold_blood|cooldown.cold_blood.remains<4|cooldown.cold_blood.remains>10)", "Helper variable to check for Cold Blood and The Rotten buff." );
  stealth_cds->add_action( "vanish,if=(combo_points.deficit>1|buff.shadow_blades.up&talent.invigorating_shadowdust)&!variable.shd_threshold&(cooldown.flagellation.remains>=60|!talent.flagellation|fight_remains<=(30*cooldown.vanish.charges))&(cooldown.symbols_of_death.remains>3|!set_bonus.tier30_2pc)&(cooldown.secret_technique.remains>=10|!talent.secret_technique|cooldown.vanish.charges>=2&talent.invigorating_shadowdust&(buff.the_rotten.up|!talent.the_rotten)&!raid_event.adds.up)", "Consider Flagellation, Symbols and Secret Technique cooldown when using Vanish with Shadow Dust." );
  stealth_cds->add_action( "pool_resource,for_next=1,extra_amount=40,if=race.night_elf", "Pool for Shadowmeld unless we are about to cap on Dance charges." );
  stealth_cds->add_action( "shadowmeld,if=energy>=40&energy.deficit>=10&!variable.shd_threshold&combo_points.deficit>4" );
  stealth_cds->add_action( "variable,name=shd_combo_points,value=combo_points.deficit>=3" );
  stealth_cds->add_action( "shadow_dance,if=(dot.rupture.ticking|talent.invigorating_shadowdust)&variable.rotten_cb&(!talent.the_first_dance|combo_points.deficit>=4|buff.shadow_blades.up)&(variable.shd_combo_points&variable.shd_threshold|(buff.shadow_blades.up|cooldown.symbols_of_death.up&!talent.sepsis|buff.symbols_of_death.remains>=4&!set_bonus.tier30_2pc|!buff.symbols_of_death.remains&set_bonus.tier30_2pc)&cooldown.secret_technique.remains<10+12*(!talent.invigorating_shadowdust|set_bonus.tier30_2pc))", "Shadow dance when Rupture is up and synchronize depending on talent choice." );

  stealthed->add_action( "shadowstrike,if=buff.stealth.up&(spell_targets.shuriken_storm<4|variable.priority_rotation)", "Stealthed Rotation  Always Strike from Stealth" );
  stealthed->add_action( "call_action_list,name=finish,if=effective_combo_points>=cp_max_spend", "Finish when on Animacharged combo points or max combo points." );
  stealthed->add_action( "call_action_list,name=finish,if=buff.shuriken_tornado.up&combo_points.deficit<=2" );
  stealthed->add_action( "call_action_list,name=finish,if=combo_points.deficit<=1+(talent.deeper_stratagem|talent.secret_stratagem)" );
  stealthed->add_action( "backstab,if=!buff.premeditation.up&buff.shadow_dance.remains>=3&buff.shadow_blades.up&!used_for_danse&talent.danse_macabre&spell_targets.shuriken_storm<=3&!buff.the_rotten.up", "Backstab for Danse Macabre stack generation during Shadowblades." );
  stealthed->add_action( "gloomblade,if=!buff.premeditation.up&buff.shadow_dance.remains>=3&buff.shadow_blades.up&!used_for_danse&talent.danse_macabre&spell_targets.shuriken_storm<=4", "Gloomblade for Danse Macabre stack generation during Shadowblades." );
  stealthed->add_action( "shadowstrike,if=!used_for_danse&buff.shadow_blades.up", "Shadow Strike for Danse Macabre stack generation during Shadowblades." );
  stealthed->add_action( "shuriken_storm,if=!buff.premeditation.up&spell_targets>=4" );
  stealthed->add_action( "shadowstrike" );
}
//subtlety_apl_end

} // namespace rogue_apl
