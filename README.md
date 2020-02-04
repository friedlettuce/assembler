# assembler

#### Stages - Datapath
1. Fetch - from memory registers (pc counter)
2. Decode
3. Execute
4. Memory R/W (only need for load and store)
5. Write

#### Notes
- Every instruction is *32 bits*
- Index of operations
- Follow datapath slides ln5
- increment pc counter by 4
- RegisterWrite
- Load: load from address to register
- Store: store into memory address
- Memory address calculation for I format
- sw and lw addresses

###### Branch on equal
- extend sign and then shift left 2
- (PC + 4) + (immed & left shift 2)

###### Jump (J-Format)
- 6 for op 26 for target
1. Shift target left 2 and add for 4 to pc
