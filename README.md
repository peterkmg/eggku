# Eggku 🐰

Silly tui console app with haikus and ASCII art that explores POSIX syscalls.

<img width="459" height="301" alt="image" src="https://github.com/user-attachments/assets/0722efbc-495e-4b78-8293-e849e4d623e4" />


## Technical Details
- Double-buffered on-demand console rendering to help with flickering
- Raw terminal mode with POSIX syscalls
- ANSI escape sequences for colors/cursor control
- State machine-based menu system
- Direct file I/O for persistence

## Build & Run
`make build && make run`

