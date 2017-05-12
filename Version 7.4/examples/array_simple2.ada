STATES
q0 q1 q2 q3

INITIAL
q0

FINAL
q1 q2 q3

SYMBOLS
x

VARIABLES
a l N I i

TRANSITIONS
x q2
(or (and q3 (= I1 l1) (= l1 l0) (= N1 N0) (= l1 (+ I0 1)) (= i1 i0) (>= a1 (+ a0 3)) (<= l1 (- N1 1))) (and q2 (= l1 l0) (= N1 N0) (= i1 i0) (= I1 (+ I0 1)) (<= I1 (- l1 1)) (<= I1 (- N1 1)) (>= a1 (+ a0 3))) (and q1 (= l1 l0) (= I1 N1) (= N1 N0) (= N1 (+ I0 1)) (= i1 i0)))
x q3
(or (and q1 (= l1 l0) (= I1 N1) (= N1 N0) (= N1 (+ I0 1)) (= i1 i0)) (and q3 (= l1 l0) (= N1 N0) (= i1 i0) (= I1 (+ I0 1)) (<= I1 (- N1 1))))
x q0
(or (and q3 (= I1 1) (= l0 1) (= I0 0) (= l1 1) (= N1 N0) (= i1 i0) (>= N1 2) (>= a1 (+ a0 3))) (and q1 (= I1 1) (= N0 1) (= I0 0) (= l1 l0) (= N1 1) (= i1 i0)) (and q3 (= I1 1) (= I0 0) (= l1 l0) (= N1 N0) (= i1 i0) (>= N1 2) (<= l1 0)) (and q2 (= I1 1) (= I0 0) (= l1 l0) (= N1 N0) (= i1 i0) (>= l1 2) (>= N1 2) (>= a1 (+ a0 3))))
