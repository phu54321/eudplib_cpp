#pragma once

#include "TriggerAction.hpp"
#include "TriggerConstants.hpp"



Action NoAction();
Action Victory();
Action Defeat();
Action PreserveTrigger();
Action Wait(Expr Time);
Action PauseGame();
Action UnpauseGame();
Action Transmission(Unit Unit, Location location, String WAVName, Modifier timeMod, Expr time, String text, Expr alwaysDisplay);
Action PlayWAV(String WAVName);
Action DisplayText(String Text, Expr AlwaysDisplay);
Action CenterView(Location location);
Action CreateUnitWithProperties(Expr Count, Unit Unit, Location location, Player Player, UnitProperties Properties);
Action SetMissionObjectives(String Text);
Action SetSwitch(Switch Switch, SwitchAction State);
Action SetCountdownTimer(Modifier TimeModifier, Expr Time);
Action RunAIScript(AIScript Script);
Action RunAIScriptAt(AIScript Script, Location location);
Action LeaderBoardControl(Unit Unit, String Label);
Action LeaderBoardControlAt(Unit Unit, Location Location, String Label);
Action LeaderBoardResources(Resource ResourceType, String Label);
Action LeaderBoardKills(Unit Unit, String Label);
Action LeaderBoardScore(ScoreType ScoreType, String Label);
Action KillUnit(Unit Unit, Player Player);
Action KillUnitAt(Count Count, Unit Unit, Location location, Player ForPlayer);
Action RemoveUnit(Unit Unit, Player Player);
Action RemoveUnitAtLocation(Count Count, Unit Unit, Player ForPlayer, Location location);
Action SetResources(Player Player, Modifier Modifier, Expr Amount, Resource ResourceType);
Action SetScore(Player Player, Modifier Modifier, Expr Amount, ScoreType ScoreType);
Action MinimapPing(Location location);
Action TalkingPortrait(Unit Unit, Expr Time);
Action MuteUnitSpeech();
Action UnMuteUnitSpeech();
Action LeaderBoardComputerPlayers(PropState State);
Action LeaderBoardGoalControl(Expr Goal, Unit Unit, String Label);
Action LeaderBoardGoalControlAt(Expr Goal, Unit Unit, Location Location, String Label);
Action LeaderBoardGoalResources(Expr Goal, Resource ResourceType, String Label);
Action LeaderBoardGoalKills(Expr Goal, Unit Unit, String Label);
Action LeaderBoardGoalScore(Expr Goal, ScoreType ScoreType, String Label);
Action MoveLocation(Location location, Unit unit, Player Owner, Location DestLocation);
Action MoveUnit(Count Count, Unit UnitType, Player Owner, Location StartLocation, Location DestLocation);
Action LeaderBoardGreed(Expr Goal);
Action SetNextScenario(String ScenarioName);
Action SetDoodadState(PropState State, Unit Unit, Player Owner, Location location);
Action SetInvincibility(PropState State, Unit Unit, Player Owner, Location location);
Action CreateUnit(Expr Expr, Unit Unit, Location location, Player ForPlayer);
Action SetDeaths(Player Player, Modifier Modifier, Expr Expr, Unit Unit);
Action Order(Unit Unit, Player Owner, Location StartLocation, OrderType Order, Location DestLocation);
Action Comment(String Text);
Action GiveUnits(Count Count, Unit Unit, Player Owner, Location location, Player NewOwner);
Action ModifyUnitHitPoints(Count Count, Unit Unit, Player Owner, Location location, Expr Percent);
Action ModifyUnitEnergy(Count Count, Unit Unit, Player Owner, Location location, Expr Percent);
Action ModifyUnitShields(Count Count, Unit Unit, Player Owner, Location location, Expr Percent);
Action ModifyUnitResourceAmount(Count Count, Player Owner, Location location, Expr NewValue);
Action ModifyUnitHangarCount(Expr Add, Count Count, Unit Unit, Player Owner, Location location);
Action PauseTimer();
Action UnpauseTimer();
Action Draw();
Action SetAllianceStatus(Player Player, AllyStatus Status);
