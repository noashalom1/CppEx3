# COUP C++ Game Project

## 👤 Author
Noa Patito- noapatito123@gmail.com

---

## 🎮 About the Game

This project is a full-featured C++ implementation of the game **Coup**, with custom roles, advanced gameplay rules, and an interactive graphical user interface (GUI) using **SFML**.

Players take turns performing actions like `gather`, `tax`, `bribe`, `arrest`, `sanction`, `coup` and using unique abilities tied to their roles. The last surviving player wins.

---

## 🗂️ Folder Structure

```plaintext
.
├── include/
│   ├── gui/
│   │   └── GameGUI.hpp             # GUI class definition
│   ├── roles/                      # Header files for all player roles
│   │   ├── Baron.hpp
│   │   ├── General.hpp
│   │   ├── Governor.hpp
│   │   ├── Judge.hpp
│   │   ├── Merchant.hpp
│   │   └── Spy.hpp
│   ├── Button.hpp
│   ├── TextBox.hpp
│   ├── Game.hpp
│   ├── Player.hpp
│   ├── exceptions.hpp
│   └── doctest.h                   # Testing framework
│
├── src/
│   ├── gui/
│   │   ├── GameGUI.cpp
│   │   ├── InGame_GameGUI.cpp
│   │   ├── Draw_GameGUI.cpp
│   │   └── TargetSelection_GameGUI.cpp
│   ├── roles/
│   │   ├── Baron.cpp
│   │   ├── General.cpp
│   │   ├── Governor.cpp
│   │   ├── Judge.cpp
│   │   ├── Merchant.cpp
│   │   └── Spy.cpp
│   ├── Button.cpp
│   ├── TextBox.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   └── exceptions.cpp
│
├── tests/
│   ├── TestGame.cpp
│   ├── TestPlayer.cpp
│   └── TestRoles.cpp
│
├── arial.ttf                      # Font used in GUI
├── main.cpp                       # GUI entry point
├── Makefile
└── README.md
```

---
## 👥 Game Rules

### Objective
Eliminate all other players through strategic actions and role-based abilities. The last player remaining wins.

### Maximum 6 players
each assigned one of the following roles - randomly (can have more than one player with the same role):
  - Governor
  - Spy
  - Baron
  - General
  - Judge
  - Merchant

### Basic Actions
- **Gather**: +1 coin
- **Tax**: +2 coins (if Governor +3 coins)
- **Bribe**: Spend 4 coins to gain 2 extra turns 
- **Coup**: Spend 7 coins to eliminate a player 
- **Arrest**: Steal or reduce coins from another player (if target is General->he gets 1 coin back. If target is Merchant->he'll lost 2 coins, and the attacker will get nothing)
- **Sanction**: -3 coins, block target from acting tax and gather (if target is Baron->he gets 1 coin back. if target is Judge->the attacker pays 4 coins)

### Special Actions (1 per round per player-considered an out-of-turn actions)
- **Undo tax (by Governor only)**: undo the tax action performed by another player
- **Undo bribe (by Judge only)**: peek at another player’s coins and disable them using arrest on their next turn
- **Undo coup (by General only)**: pay 5 coins to prevent a coup against themselves or another player
- **Peek & disable (by Spy only)**: undo the bribe action performed by another player

### Additional Features
- `invest (by Baron only)`: can invest 3 coins and receive 6 coins in return
- `Merchant` gains 1 extra coin at the beginning of their turn if they have at least 3 coins
- Players cannot target themselves.
- Eliminated players are skipped in the turn order.
- A player with 10+ coins **must perform a coup**.
- Sanctioned players cannot act until their sanction is lifted.
- Bribed players get extra turns unless the bribe is undone.
- Undo actions have turn and role restrictions.
- Full memory safety using `std::shared_ptr`.

---

## 🖼️ GUI Overview

- Built using **SFML**.
- Setup screen for entering player names and roles
- Demo Game - a preloaded demo game with 6 players — one from each role — is available for quick testing and full feature demonstration.
- Action buttons (gather, tax, bribe, arrest, sanction, coup).
- Role-specific buttons (e.g., undo tax, peek & arrest).
- Visual display of player states, current turn, and messages.
- End screen showing the winner.


---
## 🧪 Testing & Memory Checks

### ✔️ Run Tests:
```bash
make test
```

### 🧠 Run Valgrind on Tests:
```bash
make valgrind
```
> Only test executables are analyzed by Valgrind (not GUI).

---

## 🖥️ Build & Run

### Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/noashalom1/CppEx3.git
   cd CppEx3
   ```

2. Build the GUI executable:

   ```bash
   make Main
   ```

3. Run the game:

   ```bash
   ./Main
   ```

> 💡 If you are using WSL or Linux, ensure that SFML is properly installed (`sudo apt install libsfml-dev`).
---

### Prerequisites

- C++17 compiler (e.g., `g++`)
- [SFML](https://www.sfml-dev.org/) library (Graphics, Window, System modules)

