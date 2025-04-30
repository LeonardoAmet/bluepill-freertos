.syntax unified
.cpu cortex-m3
.thumb

.global reset_handler
.type reset_handler, %function

reset_handler:
    b main
