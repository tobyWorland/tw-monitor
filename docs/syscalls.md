# Syscalls

You should treat system calls as if they were function calls: Don't
rely on R0-R3, R12 to keep their values. (But LR will keep its value)

## SVC 0 (Exit)
- R0: Exit code

## SVC 1 (Put decimal, new line, no width)
- R0: Holds the number to print to the display
