STATES
q0 q1 q2 q3 q4 q5 q6 q7 q8 q9 q10 q11 q12 q13

INITIAL
q0

SYMBOLS
tau send_data rec_data send_ack rec_ack epsilon

VARIABLES
N sdata bdata sbit bbit sack back rdata rtbit rbit t T x y

TRANSITIONS
tau q0
(and q1 (= 0 t1) (< 0 T1) (< 0 N1) (= 0 sdata1) (= 0 bdata1) (= 0 sbit1) (= 0 bbit1) (= 0 sack1) (= 0 back1) (= 0 rdata1) (= 0 rtbit1) (= 0 rbit1) (= 0 x1) (= 0 y1))
tau q1
(and q2 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 (+ sdata0 1)) (= bdata0 bdata1) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 x0) (= y1 y0))
tau q2
(and q3 (<= t0 t1) (= T0 T1) (= N0 N1) (or (and (< N0 sdata0) (= sdata1 (+ sdata0 1))) (and (<= sdata0 N0) (= sdata1 sdata0))) (= bdata0 bdata1) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 x0) (= y1 y0))
send_data q3
(and q4 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit1 sbit0) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 t0) (<= t1 (+ x1 T0)) (= y1 t0))
tau q4
(and q5 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x0 T0) (= x1 x0) (= y1 y0))
tau q5
(and q3 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (< x0 T0) (= x1 x0) (= y1 y0))
rec_data q4
(and q6 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 bdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata1 bdata0) (= rtbit1 bbit0) (= rbit0 rbit1) (= x1 x0) (< y0 T0) (= y1 y0))
rec_data q5
(and q7 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 bdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata1 bdata0) (= rtbit1 bbit0) (= rbit0 rbit1) (= x1 x0) (< y0 T0) (= y1 y0))
send_ack q6
(and q8 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 bdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back1 rbit0) (= rdata1 rdata0) (= rbit0 rbit1) (= x1 x0) (= y1 t0))
send_ack q7
(and q10 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 bdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back1 rbit0) (= rdata1 rdata0) (= rbit0 rbit1) (= x1 x0) (= y1 t0))
tau q10
(and q3 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (< x0 T0) (= x1 x0) (= y1 y0))
rec_ack q8
(and q9 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 bdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack1 back0) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 x0) (< y0 T0) (= y1 y0))
tau q9
(or (and q3 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (not (= sack0 sbit0)) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 x0) (= y1 y0)) (and q1 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit1 (- 1 sbit0)) (= bbit0 bbit1) (= sack0 sbit0) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 x0) (= y1 y0)))
send_data q7
(and q12 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit1 sbit0) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x1 t0) (<= t1 (+ x1 T0)) (= y1 t0))
tau q12
(or (and q11 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x0 T0) (= x1 x0) (= y1 y0)) (and q6 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (< x0 T0) (= x1 x0) (= y1 y0)))
tau q11
(and q7 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (< x0 T0) (= x1 x0) (= y1 y0))
tau q8
(and q13 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (< x0 T0) (= x1 x0) (= y1 y0))
tau q13
(and q3 (<= t0 t1) (= T0 T1) (= N0 N1) (= sdata1 sdata0) (= bdata1 sdata0) (= sbit0 sbit1) (= bbit0 bbit1) (= sack0 sack1) (= back0 back1) (= rdata0 rdata1) (= rtbit0 rtbit1) (= rbit0 rbit1) (= x0 T0) (= x1 x0) (= y1 y0))
epsilon q0
q0
epsilon q1
true
epsilon q2
q2
epsilon q3
q3
epsilon q4
q4
epsilon q5
q5
epsilon q6
q6
epsilon q7
q7
epsilon q8
q8
epsilon q9
q9
epsilon q10
q10
epsilon q11
q11
epsilon q12
q12
epsilon q13
q13
