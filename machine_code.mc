# windows x64 calling convention
  # rcx holds address of the beginning of program instruction
  # rax holds the size (in bytes) of allocated space for program instructions

# code for testing some aspects (doesn't make sense overall)
48 89 cf              # mov    rdi,rcx
48 c7 c0 01 00 00 00  # mov    rax,0x1
48 ff c0              # inc    rax
48 83 f8 17           # cmp    rax,0x17
75 f7                 # jne    0xa
48 89 f8              # mov    rax,rdi
48 89 d0              # mov    rax,rdx
c3                    # ret
