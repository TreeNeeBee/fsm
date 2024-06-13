# FSM Library

This library provides a simple and extensible implementation of a Finite State Machine (FSM) with state-specific event handling. This design is particularly suitable for systems like AUTOSAR, where applications are scheduled as periodic tasks, and state polling is done within these tasks.

## Features

- **State-specific event handling**: Each state handles its own events, ignoring irrelevant events.
- **Simple state transition**: Defined transitions between states with optional failure simulation using random conditions.
- **Easy integration**: Designed for integration with periodic task scheduling systems.

## Build && Run

To build and test this library, simply clone the repository.

```bash
git clone git@github.com:TreeNeeBee/fsm.git
cd fsm
mkdir build && cd build
cmake ..
make
```

you can run example with ```./fsmd```

## Usage

### Define States and Events

Define the states and events in your system. In this example, we have three states (A, B, C) and a standby state.

### Implement State

Each state should handle its specific events and define the transitions.

### Implement the FSM Class

The FSM class manages the current state and delegates event handling to the current state.

### Example Usage

Create an FSM instance and handle events.

### Example Scenario

In this example scenario, the state transitions are as follows:

- standby -> A: Keyboard input A
- A -> B: Keyboard input B
- A -> C: Keyboard input C
- B -> C: Keyboard input C
- C -> standby: Keyboard input S

Each state transition includes a random condition check to simulate the possibility of transition failure.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
