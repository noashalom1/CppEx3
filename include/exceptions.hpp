// Author: noapatito123@gmail.com
#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace coup
{

    class GameException : public std::runtime_error
    {
    public:
        explicit GameException(const std::string &msg) : std::runtime_error(msg) {}
    };

    class NotYourTurnException : public GameException
    {
    public:
        NotYourTurnException() : GameException("Not your turn.") {}
    };

    class SanctionedException : public GameException
    {
    public:
        SanctionedException() : GameException("You are sanctioned and cannot perform this action.") {}
    };

    class AlreadySanctionedException : public GameException
    {
    public:
        AlreadySanctionedException() : GameException("Target is already sanctioned.") {}
    };

    class NotEnoughCoinsException : public GameException
    {
    public:
        NotEnoughCoinsException(int required, int curr) : GameException("Not enough coins. Required: " + std::to_string(required) + ", but have: " + std::to_string(curr)) {}
    };

    class MaxPlayersExceededException : public GameException
    {
    public:
        MaxPlayersExceededException() : GameException("Cannot add more than 6 players.") {}
    };

    class DuplicateArrestException : public GameException
    {
    public:
        DuplicateArrestException() : GameException("Cannot arrest the same player twice in a row.") {}
    };

    class MustPerformCoupException : public GameException
    {
    public:
        MustPerformCoupException() : GameException("You must perform a COUP when you have 10 or more coins.") {}
    };

    class TargetNoCoinsException : public GameException
    {
    public:
        TargetNoCoinsException() : GameException("Target has no coins to arrest.") {}
    };

    class DuplicatePlayerNameException : public GameException
    {
    public:
        DuplicatePlayerNameException() : GameException("Player name already exists.") {}
    };

    class GameStillOngoingException : public GameException
    { //
    public:
        GameStillOngoingException() : GameException("Game is still ongoing, more than one player remains.") {}
    };

    class GameNotStartedException : public GameException
    {
    public:
        GameNotStartedException() : GameException("Not enough players to start the game.") {}
    };

    class PlayerNotFoundException : public GameException
    {
    public:
        explicit PlayerNotFoundException(const std::string &name) : GameException("Player not found: " + name) {}
    };

    class NoPlayersLeft : public GameException
    {
    public:
        NoPlayersLeft() : GameException("No players left.") {}
    };

    class UndoNotAllowedException : public GameException
    {
    public:
        UndoNotAllowedException(const std::string &target_name, const std::string &action)
            : GameException(target_name + " has not used " + action + ".") {}
    };

    class TargetIsAlreadyEliminated : public GameException
    {
    public:
        TargetIsAlreadyEliminated() : GameException("Target is already eliminated.") {}
    };

    class TargetIsEliminatedException : public GameException
    {
    public:
        TargetIsEliminatedException()
            : GameException("Cannot target an eliminated player.") {}
    };

    // כאשר שחקן כבר ביצע פעולה כלשהי בסיבוב הנוכחי
    class ActionAlreadyUsedThisRoundException : public GameException
    {
    public:
        ActionAlreadyUsedThisRoundException(const std::string &name, const std::string &action)
            : GameException(name + std::string(" has already used ") + action + std::string(" this round.")) {}
    };

    // כאשר השחקן המטרה לא מודח
    class TargetNotEliminatedException : public GameException
    {
    public:
        TargetNotEliminatedException() : GameException("Target is not eliminated.") {}
    };

    // כאשר השחקן שמנסה לבצע פעולה מודח בעצמו
    class PlayerEliminatedException : public GameException
    {
    public:
        explicit PlayerEliminatedException(const std::string &name)
            : GameException(name + " is eliminated.") {}
    };

    // כאשר מנסים לבטל פעולה ישנה מדי
    class ActionTooOldException : public GameException
    {
    public:
        explicit ActionTooOldException(const std::string &actor, const std::string &action)
            : GameException(actor + "'s " + action + " is too old.") {}
    };

    // כאשר שחקן מנסה לבטל את הפעולה של עצמו
    class CannotUndoOwnActionException : public GameException
    {
    public:
        explicit CannotUndoOwnActionException(const std::string &name, const std::string &action)
            : GameException(name + " cannot undo their own " + action + ".") {}
    };

    // כאשר אין פעולה אחרונה לביטול
    class NoRecentActionToUndoException : public GameException
    {
    public:
        explicit NoRecentActionToUndoException(const std::string &action)
            : GameException("No recent " + action + " by another player to undo.") {}
    };

    class InvalidBribeUndoException : public GameException
    {
    public:
        InvalidBribeUndoException()
            : GameException("Target has not done a bribe or has already undone it.") {}
    };

    class CannotTargetYourselfException : public GameException
    {
    public:
        CannotTargetYourselfException()
            : GameException("You cannot target yourself.") {}
    };

    class ArrestBlockedException : public GameException
    {
    public:
        ArrestBlockedException()
            : GameException("You are blocked from using ARREST this turn.") {}
    };

}

#endif
