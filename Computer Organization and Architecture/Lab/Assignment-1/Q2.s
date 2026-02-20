addi t0,t0,30
addi t6,t0,1
addi t1,t1,2
addi x0,x0,0
addi t4,t4,1

for_loop:
    bge t1,t6,end_loop
    rem t3,t0,t1
    beq t3,x0,not_prime
    addi t1,t1,1
    jal x0,for_loop

not_prime:
    sub t4,t4,t4
    jal x0,end_loop

end_loop:
hi:
wi:
gi:
addi a0,a0,1
#
#
#