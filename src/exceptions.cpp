#include "exceptions.hpp"

namespace coup
{
    namespace GameExceptionStrings
    {
        // Static error messages
        const char *NotYourTurn = "Not your turn.";
        const std::string Sanctioned = "You are sanctioned and cannot perform this action.";
        const std::string AlreadySanctioned = "Target is already sanctioned.";
        const std::string MaxPlayersExceeded = "Cannot add more than 6 players.";
        const std::string DuplicateArrest = "Cannot arrest the same player twice in a row.";
        const std::string MustPerformCoup = "You must perform a COUP when you have 10 or more coins.";
        const std::string TargetNoCoins = "Target has no coins to arrest.";
        const std::string DuplicatePlayerName = "Player name already exists.";
        const std::string GameStillOngoing = "Game is still ongoing, more than one player remains.";
        const std::string GameNotStarted = "Not enough players to start the game.";
        const std::string NoPlayersLeft = "No players left.";
        const std::string TargetIsAlreadyEliminated = "Target is already eliminated.";
        const std::string TargetIsEliminated = "Cannot target an eliminated player.";
        const std::string TargetNotEliminated = "Target is not eliminated.";
        const std::string InvalidBribeUndo = "Target has not done a bribe or has already undone it.";
        const std::string CannotTargetYourself = "You cannot target yourself.";
        const std::string ArrestBlocked = "You are blocked from using ARREST this turn.";

        // Dynamic messages
        std::string NotEnoughCoins(int required, int curr)
        {
            return "Not enough coins. Required: " + std::to_string(required) + ", but have: " + std::to_string(curr);
        }

        std::string PlayerNotFound(const std::string &name)
        {
            return "Player not found: " + name;
        }

        std::string UndoNotAllowed(const std::string &target_name, const std::string &action)
        {
            return target_name + " has not used " + action + ".";
        }

        std::string ActionAlreadyUsedThisRound(const std::string &name, const std::string &action)
        {
            return name + " has already used " + action + " this round.";
        }

        std::string PlayerEliminated(const std::string &name)
        {
            return name + " is eliminated.";
        }
        std::string ActionTooOld(const std::string &actor, const std::string &action)
        {
            return actor + "'s " + action + " is too old.";
        }
        std::string CannotUndoOwnAction(const std::string &name, const std::string &action)
        {
            return name + " cannot undo their own " + action + ".";
        }
        std::string NoRecentActionToUndo(const std::string &action)
        {
            return "No recent " + action + " by another player to undo.";
        }
        std::string NoCoupToUndo(const std::string &target)
        {
            return "No coup was found to undo for " + target + ".";
        }
    }
}