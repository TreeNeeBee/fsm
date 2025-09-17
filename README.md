# FSM Library

The **FSM Library** is a lightweight, extensible Finite State Machine (FSM) implementation in C, designed for embedded systems like AUTOSAR. It provides a macro-based interface for defining states, transitions, and event handlers, making it ideal for real-time applications with periodic task scheduling.

## Features

- **Macro-Based Configuration**: Use `DECLARE_SWC_FSM_CONTEXT`, `DECLARE_SWC_FSM_STATE`, and `DECLARE_SWC_FSM_TRANSITION` to define FSM contexts, states, and transitions declaratively.
- **State-Specific Event Handling**: Each state handles its own entry, routine, and exit actions.
- **Flexible Transitions**: Supports deterministic transitions via a transition table (`fsmTransTable`) with optional common and state-specific checks.
- **Fast Lookup Option**: Enable `FSM_STATE_FF` for O(1) state access using array indexing.
- **AUTOSAR Compatibility**: No dynamic memory allocation, minimal dependencies (only `<stdint.h>`), and C++ compatibility with `extern "C"`.
- **Single Header Design**: All functionality in `SWC_Fsm.h`, inspired by single-header libraries like `stb` and `miniz`.
- **Optional Implementation**: No need to define `FSM_IMPLEMENTATION` for header-only usage; defining it allows separating implementation into a single source file for better modularity.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`, `clang`, IAR, or Keil for AUTOSAR).
- Standard C library: `<stdint.h>` (no other dependencies required).

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/TreeNeeBee/fsm.git
   cd fsm
   ```

2. Include `SWC_Fsm.h` in your project:
   - Copy `SWC_Fsm.h` to your project directory.
   - **Header-Only Usage**: Simply include `SWC_Fsm.h` without defining `FSM_IMPLEMENTATION`. All functions are defined inline in the header, suitable for small projects or quick integration.
   - **Separated Implementation**: Define `FSM_IMPLEMENTATION` in **one** source file to include function implementations, reducing code duplication in larger projects:
     ```c
     #define FSM_IMPLEMENTATION
     #include "SWC_Fsm.h"
     ```

### Using `FSM_IMPLEMENTATION`

- **Do I Need `FSM_IMPLEMENTATION`?**
  - **No**: If you include `SWC_Fsm.h` without defining `FSM_IMPLEMENTATION`, all function implementations are included inline in the header. This is simpler for small projects or rapid prototyping, as it requires no additional setup.
  - **Yes**: Defining `FSM_IMPLEMENTATION` in one source file moves function implementations to that file, reducing binary size and compilation time in projects with multiple source files including `SWC_Fsm.h`.

- **Advantages of Defining `FSM_IMPLEMENTATION`**:
  - **Reduced Binary Size**: Prevents multiple inclusions of function implementations across translation units.
  - **Improved Compilation Speed**: Compiles function implementations only once in the designated source file.
  - **Better Modularity**: Separates interface (header) from implementation, making it easier to maintain large projects.
  - **Linker Optimization**: Avoids duplicate symbol errors when linking multiple object files.

- **Steps to Use `FSM_IMPLEMENTATION`**:
  1. Create a source file (e.g., `fsm_impl.c`) with:
     ```c
     #define FSM_IMPLEMENTATION
     #include "SWC_Fsm.h"
     ```
  2. Compile the implementation file separately:
     ```bash
     gcc -c fsm_impl.c -o fsm_impl.o
     ```
  3. Include `SWC_Fsm.h` in other source files without defining `FSM_IMPLEMENTATION`.
  4. Link all object files:
     ```bash
     gcc -o my_app main.o fsm_impl.o
     ```

### Building the Example

To build and run the example (`exampleFsm.c`):
```bash
cd examples
gcc -I.. -o fsmd exampleFsm.c
./fsmd
```

For fast state lookup (O(1)):
```bash
gcc -DFSM_STATE_FF -I.. -o fsmd exampleFsm.c
./fsmd
```

For separated implementation:
```bash
gcc -c fsm_impl.c -o fsm_impl.o
gcc -I.. -c exampleFsm.c -o exampleFsm.o
gcc -o fsmd exampleFsm.o fsm_impl.o
./fsmd
```

## Usage

### Defining States and Events

Define states and events in your application:
```c
typedef enum {
    FSM_STATE_STANDBY,
    FSM_STATE_A,
    FSM_STATE_B,
    FSM_STATE_C,
    FSM_STATE_COUNT
} fsm_state_t;

typedef enum {
    FSM_EVENT_KEY_A,
    FSM_EVENT_KEY_B,
    FSM_EVENT_KEY_C,
    FSM_EVENT_KEY_S,
    FSM_EVENT_NONE
} FSM_Event;
```

### Defining the FSM Context

Use the `DECLARE_SWC_FSM_CONTEXT` macro to define the FSM context, including states, transitions, and callbacks:
```c
// Common transition check
fsm_bool_t commonCheck(fsm_state_t from, fsm_state_t to, void *fsm) {
    return DEF_FSM_TRUE;
}

// State-specific transition check
fsm_bool_t transCheckAtoB(fsm_state_t from, fsm_state_t to, void *fsm) {
    return DEF_FSM_TRUE;
}

// State action callbacks
int32_t stateAEntry(void *fsm) { printf("Enter State A\n"); return 0; }
int32_t stateARoutine(void *fsm) { return 0; }
int32_t stateAExit(void *fsm) { printf("Exit State A\n"); return 0; }

#define FSM_STATE_LIST \
    DECLARE_SWC_FSM_STATE(FSM_STATE_A, stateAEntry, stateARoutine, stateAExit, 1000), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_STANDBY, NULL, NULL, NULL, 0), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_B, NULL, NULL, NULL, 0), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_C, NULL, NULL, NULL, 0)

#define FSM_TRANS_TABLE \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_STANDBY, FSM_STATE_A, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_A, FSM_STATE_B, transCheckAtoB), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_A, FSM_STATE_C, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_B, FSM_STATE_C, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_C, FSM_STATE_STANDBY, NULL)

// FSM context
DECLARE_SWC_FSM_CONTEXT(MyFSM, 1, DECLARE_SWC_FSM_STATES(FSM_STATE_LIST), DECLARE_SWC_FSM_TRANSITIONS(FSM_TRANS_TABLE), commonCheck, FSM_STATE_STANDBY, 1000, NULL, NULL, NULL)
```

### Example Usage

Below is an example (`exampleFsm.c`) demonstrating FSM initialization and event-driven transitions:
```c
#include "SWC_Fsm.h"
#include <stdio.h>

// Common transition check
fsm_bool_t commonCheck(fsm_state_t from, fsm_state_t to, void *fsm) {
    return DEF_FSM_TRUE;
}

// State-specific transition check
fsm_bool_t transCheckAtoB(fsm_state_t from, fsm_state_t to, void *fsm) {
    return DEF_FSM_TRUE;
}

// State action callbacks
int32_t stateAEntry(void *fsm) { printf("Enter State A\n"); return 0; }
int32_t stateARoutine(void *fsm) { return 0; }
int32_t stateAExit(void *fsm) { printf("Exit State A\n"); return 0; }

// Define states and transitions
#define FSM_STATE_LIST \
    DECLARE_SWC_FSM_STATE(FSM_STATE_A, stateAEntry, stateARoutine, stateAExit, 1000), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_STANDBY, NULL, NULL, NULL, 0), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_B, NULL, NULL, NULL, 0), \
    DECLARE_SWC_FSM_STATE(FSM_STATE_C, NULL, NULL, NULL, 0)

#define FSM_TRANS_TABLE \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_STANDBY, FSM_STATE_A, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_A, FSM_STATE_B, transCheckAtoB), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_A, FSM_STATE_C, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_B, FSM_STATE_C, NULL), \
    DECLARE_SWC_FSM_TRANSITION(FSM_STATE_C, FSM_STATE_STANDBY, NULL)

// Declare FSM context
DECLARE_SWC_FSM_CONTEXT(MyFSM, 1, DECLARE_SWC_FSM_STATES(FSM_STATE_LIST), DECLARE_SWC_FSM_TRANSITIONS(FSM_TRANS_TABLE), commonCheck, FSM_STATE_STANDBY, 1000, NULL, NULL, NULL)

int main(void) {
    SWCFsmContext *context = DECLARE_SWC_FSM_CONTEXT_REF(MyFSM);

    if (!swcFsmInit(context)) {
        printf("FSM 初始化失败\n");
        return 1;
    }

    printf("Transition to A\n");
    swcFsmTransTo(FSM_STATE_A, DEF_FSM_FALSE, context);
    printf("Transition to B\n");
    swcFsmTransTo(FSM_STATE_B, DEF_FSM_FALSE, context);
    printf("Transition to C\n");
    swcFsmTransTo(FSM_STATE_C, DEF_FSM_FALSE, context);
    printf("Transition to Standby\n");
    swcFsmTransTo(FSM_STATE_STANDBY, DEF_FSM_FALSE, context);

    swcFsmExit(context);
    return 0;
}
```

### State Transition Diagram

The example implements the following transitions:
- **Standby → A**: Triggered by `swcFsmTransTo(FSM_STATE_A, ...)`.
- **A → B**: Triggered by `swcFsmTransTo(FSM_STATE_B, ...)` with `transCheckAtoB`.
- **A → C**: Triggered by `swcFsmTransTo(FSM_STATE_C, ...)`.
- **B → C**: Triggered by `swcFsmTransTo(FSM_STATE_C, ...)`.
- **C → Standby**: Triggered by `swcFsmTransTo(FSM_STATE_STANDBY, ...)`.

Each transition can include a common check (`fsmCommonCheck`) and state-specific checks (`transCheck`).

## AUTOSAR Compatibility

- **No Dynamic Memory**: Uses static state and transition tables.
- **Minimal Dependencies**: Only requires `<stdint.h>`; defines `NULL` and `fsm_bool_t` internally.
- **C++ Support**: Wrapped in `extern "C"` for C++ compatibility.
- **Configurable**: Supports `FSM_STATE_FF` for O(1) lookup and custom intervals for periodic tasks.

## FAQ

### Do I need to define `FSM_IMPLEMENTATION`?
No, you can include `SWC_Fsm.h` without defining `FSM_IMPLEMENTATION` for header-only usage, where all functions are inline. Define `FSM_IMPLEMENTATION` in one source file for larger projects to reduce binary size and improve modularity.

### How do I enable fast state lookup?
Define `FSM_STATE_FF` during compilation for O(1) state access. Ensure `fsmStateList` is an array indexed by `fsm_state_t`:
```c
static SWCFsmStateItem stateList[FSM_STATE_COUNT] = { ... };
```

### Can I use this in AUTOSAR?
Yes, the library is designed for AUTOSAR:
- No dynamic allocation.
- Minimal dependencies.
- Supports custom state and transition definitions via macros.

### How do I add new states or transitions?
Use `DECLARE_SWC_FSM_STATE` and `DECLARE_SWC_FSM_TRANSITION` in the `FSM_STATE_LIST` and `FSM_TRANS_TABLE` macros, respectively.

## Contributing

Contributions are welcome! Please:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Commit changes (`git commit -m 'Add your feature'`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
