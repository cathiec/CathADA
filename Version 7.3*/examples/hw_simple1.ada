STATES
q0 q1

INITIAL
q0

FINAL
q1

SYMBOLS
a b

VARIABLES
a m

TRANSITIONS
a q0
(and q1 (> m0 1) (= m1 m0) (= a1 0))
a q1
(and q1 (< a0 (- m0 1)) (= a1 (+ a0 1)) (= m0 m1))
b q1
(and q1 (= a0 (- m0 1)) (= a1 0) (= m0 m1))
