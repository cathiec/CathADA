STATES
q0 q1

INITIAL
q0

FINAL
q1

SYMBOLS
tau send_data rec_data send_ack rec_ack

VARIABLES
sbit rbit

TRANSITIONS
tau q0
(or q0 (and q1 (= sbit1 rbit1)))
send_data q0
q0
rec_data q0
q0
send_ack q0
q0
rec_ack q0
q0
