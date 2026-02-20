lui x0,0
lui x1,0
addi x2,x2,1
li x3,755
lui x4,0

for_loop:
    beq x3,x0,end_loop
    andi x5,x3,1
    add x4,x4,x5
    srli x3,x3,1
    jal x0,for_loop
end_loop:

# Below lines are just for convenience
addi x1,x1,0
addi x1,x1,0
addi x1,x1,0