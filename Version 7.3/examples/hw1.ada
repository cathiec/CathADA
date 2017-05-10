STATES
q0 q1 q2 q3 q4 q5

INITIAL
q0

FINAL
q1 q2 q3 q4 q5

SYMBOLS
a r

VARIABLES
a m

TRANSITIONS
a q0
(and q1 (> m0 1) (= m1 m0) (= a1 0))
a q2
(or (and q4 (< a0 (- m0 1)) (= a1 (+ a0 1)) (= m0 m1)) (and q1 (= a1 0) (= m0 m1)) (and q5 (= a0 m0) (= a1 a0) (= m0 m1)))
r q2
(and q4 (= a0 (- m0 1)) (= a1 0) (= m0 m1))
a q4
(or (and q2 (= a1 a0) (= m0 m1)) (and q3 (= a1 0) (= m0 m1)) (and q5 (= a0 m0) (= a1 a0) (= m0 m1)))
a q1
(or (and q2 (= a1 a0) (= m0 m1)) (and q3 (= a1 0) (= m0 m1)))
a q3
(or (and q4 (= a1 a0) (= m0 m1)) (and q1 (= a1 0) (= m0 m1)))
