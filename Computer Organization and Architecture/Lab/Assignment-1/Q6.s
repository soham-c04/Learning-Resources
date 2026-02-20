lui x0,0
addi x1,x0,0
addi x4,x0,85
addi x5,x0,0
addi x6,x0,1
addi x7,x0,8
addi x8,x4,0

for_loop:
    bge x6,x7,end_loop
    srli x9,x8,7
    slli x9,x9,7
    xor x8,x8,x9
    slli x8,x8,1
    srli x9,x9,7
    add x8,x8,x9
    addi x6,x6,1
    beq x4,x8,equal
    jal x0,for_loop
equal:
    addi x5,x5,1
    jal x0,for_loop
end_loop:

# Below lines are just for convenience
addi x1,x1,0
addi x1,x1,0
addi x1,x1,0