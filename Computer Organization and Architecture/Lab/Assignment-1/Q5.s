addi x6,x6,0
addi x1,x1,1
addi x4,x4,14
addi x5,x5,0

for_loop:
    beq x4,x6,16
    add x5,x5,x4
    sub x4,x4,x1
    jal x0,for_loop
end_loop:

# Below lines are just for convenience
addi x1,x1,0
addi x1,x1,0
addi x1,x1,0