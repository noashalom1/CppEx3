// Author: noapatito123@gmail.com
#pragma once

#include <stdexcept>
#include <string>

namespace coup
{
    // Base class for all custom exceptions
    class GameException : public std::runtime_error
    {
    public:
        GameException(const std::string &msg) : std::runtime_error(msg) {}
        virtual ~GameException() {}
    };

    // Centralized strings for all exception messages
    namespace GameExceptionStrings
    {
        extern const char *NotYourTurn;
        extern const std::string Sanctioned;
        extern const std::string AlreadySanctioned;
        extern const std::string MaxPlayersExceeded;
        extern const std::string DuplicateArrest;
        extern const std::string MustPerformCoup;
        extern const std::string TargetNoCoins;
        extern const std::string DuplicatePlayerName;
        extern const std::string GameStillOngoing;
        extern const std::string GameNotStarted;
        extern const std::string NoPlayersLeft;
        extern const std::string TargetIsAlreadyEliminated;
        extern const std::string TargetIsEliminated;
        extern const std::string TargetNotEliminated;
        extern const std::string InvalidBribeUndo;
        extern const std::string CannotTargetYourself;
        extern const std::string ArrestBlocked;


        // Dynamic messages
        std::string NotEnoughCoins(int required, int curr);
        std::string PlayerNotFound(const std::string &name);
        std::string UndoNotAllowed(const std::string &target_name, const std::string &action);
        std::string ActionAlreadyUsedThisRound(const std::string &name, const std::string &action);
        std::string PlayerEliminated(const std::string &name);
        std::string ActionTooOld(const std::string &actor, const std::string &action);
        std::string CannotUndoOwnAction(const std::string &name, const std::string &action);
        std::string NoRecentActionToUndo(const std::string &action);
        std::string NoCoupToUndo(const std::string &target);
    }

    // === Specific Exceptions ===

    class NotYourTurnException : public GameException
    {
    public:
        NotYourTurnException() : GameException(GameExceptionStrings::NotYourTurn) {}
    };

    class SanctionedException : public GameException
    {
    public:
        SanctionedException() : GameException(GameExceptionStrings::Sanctioned) {}
    };

    class AlreadySanctionedException : public GameException
    {
    public:
        AlreadySanctionedException() : GameException(GameExceptionStrings::AlreadySanctioned) {}
    };

    class NotEnoughCoinsException : public GameException
    {
    public:
        NotEnoughCoinsException(int required, int current)
            : GameException(GameExceptionStrings::NotEnoughCoins(required, current)) {}
    };

    class MaxPlayersExceededException : public GameException
    {
    public:
        MaxPlayersExceededException() : GameException(GameExceptionStrings::MaxPlayersExceeded) {}
    };

    class DuplicateArrestException : public GameException
    {
    public:
        DuplicateArrestException() : GameException(GameExceptionStrings::DuplicateArrest) {}
    };

    class MustPerformCoupException : public GameException
    {
    public:
        MustPerformCoupException() : GameException(GameExceptionStrings::MustPerformCoup) {}
    };

    class TargetNoCoinsException : public GameException
    {
    public:
        TargetNoCoinsException() : GameException(GameExceptionStrings::TargetNoCoins) {}
    };

    class DuplicatePlayerNameException : public GameException
    {
    public:
        DuplicatePlayerNameException() : GameException(GameExceptionStrings::DuplicatePlayerName) {}
    };

    class GameStillOngoingException : public GameException
    {
    public:
        GameStillOngoingException() : GameException(GameExceptionStrings::GameStillOngoing) {}
    };

    class GameNotStartedException : public GameException
    {
    public:
        GameNotStartedException() : GameException(GameExceptionStrings::GameNotStarted) {}
    };

    class NoPlayersLeftException : public GameException
    {
    public:
        NoPlayersLeftException() : GameException(GameExceptionStrings::NoPlayersLeft) {}
    };

    class TargetIsAlreadyEliminatedException : public GameException
    {
    public:
        TargetIsAlreadyEliminatedException() : GameException(GameExceptionStrings::TargetIsAlreadyEliminated) {}
    };

    class TargetIsEliminatedException : public GameException
    {
    public:
        TargetIsEliminatedException() : GameException(GameExceptionStrings::TargetIsEliminated) {}
    };

    class TargetNotEliminatedException : public GameException
    {
    public:
        TargetNotEliminatedException() : GameException(GameExceptionStrings::TargetNotEliminated) {}
    };

    class PlayerNotFoundException : public GameException
    {
    public:
        PlayerNotFoundException(const std::string &name)
            : GameException(GameExceptionStrings::PlayerNotFound(name)) {}
    };

    class UndoNotAllowedException : public GameException
    {
    public:
        UndoNotAllowedException(const std::string &name, const std::string &action)
            : GameException(GameExceptionStrings::UndoNotAllowed(name, action)) {}
    };

    class ActionAlreadyUsedThisRoundException : public GameException
    {
    public:
        ActionAlreadyUsedThisRoundException(const std::string &name, const std::string &action)
            : GameException(GameExceptionStrings::ActionAlreadyUsedThisRound(name, action)) {}
    };

    class PlayerEliminatedException : public GameException
    {
    public:
        PlayerEliminatedException(const std::string &name)
            : GameException(GameExceptionStrings::PlayerEliminated(name)) {}
    };

    class ActionTooOldException : public GameException
    {
    public:
        ActionTooOldException(const std::string &actor, const std::string &action)
            : GameException(GameExceptionStrings::ActionTooOld(actor, action)) {}
    };

    class CannotUndoOwnActionException : public GameException
    {
    public:
        CannotUndoOwnActionException(const std::string &name, const std::string &action)
            : GameException(GameExceptionStrings::CannotUndoOwnAction(name, action)) {}
    };

    class NoRecentActionToUndoException : public GameException
    {
    public:
        NoRecentActionToUndoException(const std::string &action)
            : GameException(GameExceptionStrings::NoRecentActionToUndo(action)) {}
    };

    class NoCoupToUndoException : public GameException
    {
    public:
        NoCoupToUndoException(const std::string &target)
            : GameException(GameExceptionStrings::NoCoupToUndo(target)) {}
    };

    class InvalidBribeUndoException : public GameException
    {
    public:
        InvalidBribeUndoException() : GameException(GameExceptionStrings::InvalidBribeUndo) {}
    };

    class CannotTargetYourselfException : public GameException
    {
    public:
        CannotTargetYourselfException() : GameException(GameExceptionStrings::CannotTargetYourself) {}
    };

    class ArrestBlockedException : public GameException
    {
    public:
        ArrestBlockedException() : GameException(GameExceptionStrings::ArrestBlocked) {}
    };

}

