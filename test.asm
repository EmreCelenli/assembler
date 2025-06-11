// Simple program: R2 = 10 + (15 - 5)
@10
D=A
@15
D=D+A
@5
D=D-A
@2
M=D

// Loop example
(LOOP)
@i
M=M+1
@LOOP
D;JGT

// Pointer access
@arr
D=M
@3
A=D+A
D=M
@max
M=D