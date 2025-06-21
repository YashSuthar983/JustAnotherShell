# JustAnotherShell

JustAnotherShell is a custom Unix-like shell implemented in modern C++23. It demonstrates command parsing, execution, and extensible built-in commands, providing a foundation for learning about shell internals and systems programming.

## Features

- **Command Parsing:** Tokenizes and parses user input into commands and arguments.
- **Built-in Commands:** Easily extendable with custom built-in commands via a registry.
- **External Command Execution:** Runs system commands using `execv` and manages processes with `fork`/`waitpid`.
- **Custom Readline:** Uses a custom readline implementation for user input (see `src/ReadlineCus/`).
- **Redirection Support:** Handles input/output redirection (work in progress).
- **Modular Design:** Organized into components: Scanner, Parser, Executer, InBuilt command registry, and more.

## Project Structure

- `src/` — Source code (core logic, built-ins, parser, executor, etc.)
  - `ReadlineCus/` — Custom readline implementation
- `CMakeLists.txt` — Build configuration
- `build/` — Build output (created after compilation)

## Build Instructions

### Requirements
- C++23 compatible compiler (e.g., g++ 13+)
- CMake 3.13+
- GNU Readline library

### Build
```bash
mkdir -p build
cd build
cmake ..
make
```

### Run
```bash
./shell
```

## License

This project is licensed under the GPL-3.0 License.