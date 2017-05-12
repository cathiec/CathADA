STATES
q0 q1 q2

INITIAL
q0

FINAL
q1

SYMBOLS
init app down up in out

VARIABLES
c t

TRANSITIONS
init q0
(and q1 (= 0 c1))
in q1
(and q1 (<= t0 t1) (= c1 c0))
up q1
(and q1 (<= t0 t1) (= c1 c0))
app q1
(and q1 (<= t0 t1) (= c1 t0))
down q1
(and q2 (<= t0 t1) (= c1 t0)  (<= (- t1 c1) 5))
in q2
(and q2 (<= t0 t1) (= c1 c0)  (<= (- t1 c1) 5))
out q2
(and q2 (<= t0 t1) (= c1 c0)  (<= (- t1 c1) 5))
up q2
(and q1 (<= t0 t1) (= c1 c0))
