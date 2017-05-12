STATES
q0 q1 q2 q3 q4 q5 q6

INITIAL
q0

FINAL
q1

SYMBOLS
init app down up in out

VARIABLES
a b c t

TRANSITIONS
init q0
(and q1 (= 0 a1) (= 0 b1) (= 0 c1))
app q1
(and q2 (<= t0 t1) (= a1 t0) (= b1 t0) (= c1 t0) (<= (- t1 a1) 5) (< (- t1 b1) 2))
in q2
(and q3 (<= t0 t1) (= a1 a0) (= b1 b0) (= c1 c0) (<= (- t1 a1) 5) (<= 2 (- t0 a0)) (< (- t0 a0) 5))
in q4
(and q5 (<= t0 t1) (= a1 a0) (= b1 b0) (= c1 c0) (<= (- t1 a1) 5) (<= 2 (- t0 a0)) (< (- t0 a0) 5))
down q2
(and q4 (<= t0 t1) (= a1 a0) (= b1 b0) (= c1 t0) (<= (- t1 a1) 5) (<= 1 (- t0 b0)) (< (- t0 b0) 2))
down q3
(and q5 (<= t0 t1) (= a1 a0) (= b1 b0) (= c1 t0) (<= (- t1 a1) 5) (<= 1 (- t0 b0)) (< (- t0 b0) 2))
out q5
(and q6 (<= t0 t1) (= a1 a0) (= b1 t0) (= c1 c0) (<= (- t0 a0) 5) (<= (- t1 b1) 1))
up q6
(and q1 (<= t0 t1) (= a1 a0) (= b1 b0) (= c1 c0)  (<= (- t0 b0) 1))
