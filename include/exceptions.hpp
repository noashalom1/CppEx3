#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace coup
{

    class GameException : public std::runtime_error
    { //
    public:
        explicit GameException(const std::string &msg) : std::runtime_error(msg) {}
    };

    class NotYourTurnException : public GameException
    { //
    public:
        NotYourTurnException() : GameException("Not your turn.") {}
    };

    class SanctionedException : public GameException
    {
    public:
        SanctionedException() : GameException("You are sanctioned and cannot perform this action.") {}
    };

    class NotEnoughCoinsException : public GameException
    { //
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
    { //
    public:
        explicit PlayerNotFoundException(const std::string &name) : GameException("Player not found: " + name) {}
    };

    class NoPlayersLeft : public GameException
    {
    public:
        NoPlayersLeft() : GameException("No players left.") {}
    };

    class ActionBlockedException : public GameException
    {
    public:
        ActionBlockedException() : GameException("Coup blocked by General. You lost the coins.") {}
    };

    class UndoNotAllowedException : public GameException
    {
    public:
        UndoNotAllowedException() : GameException("Cannot undo: target did not perform tax.") {}
    };

    class UndoNotAllowed : public GameException
    {
    public:
        UndoNotAllowed() : GameException("Judge: undo bribe: target did not perform bribe") {}
    };

    class TargetIsAlreadyEliminated : public GameException
    {
    public:
        TargetIsAlreadyEliminated() : GameException("Target is already eliminated.") {}
    };

    // כאשר שחקן כבר ביצע פעולה כלשהי בסיבוב הנוכחי
    class ActionAlreadyUsedThisRoundException : public GameException
    {
    public:
        ActionAlreadyUsedThisRoundException(const std::string &name, const std::string &action)
            : GameException(name + std::string(" has already used ") + action + std::string(" this round.")) {}
    };
}

#endif
