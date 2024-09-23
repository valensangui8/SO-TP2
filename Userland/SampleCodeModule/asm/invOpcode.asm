GLOBAL invalidOpcode

section .text

invalidOpcode:
    rdpmc
    ret
