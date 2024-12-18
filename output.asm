ldi r0 5
sta x
ldi r0 10
sta y
push r0
pop r1
jmp_if_less THEN_LABEL
push r0
pop r1
sub r0 r1
sta x
jmp END_LABEL
THEN_LABEL:
push r0
pop r1
add r0 r1
sta x
END_LABEL:
