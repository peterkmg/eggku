# Eggku 🐰

Silly tui console app with haikus and ASCII art that explores POSIX syscalls.

## Technical Details
- Double-buffered on-demand console rendering to help with flickering
- Raw terminal mode with POSIX syscalls
- ANSI escape sequences for colors/cursor control
- State machine-based menu system
- Direct file I/O for persistence

## Build & Run
`make build && make run`

