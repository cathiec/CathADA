STATES
q0 q1 q2 q3 q4 q5

INITIAL
q0

FINAL
q1

SYMBOLS
x

VARIABLES
a0 lp a1 a2 xp l N I x ip i

TRANSITIONS
x q2
(or (and q3 (= xp1 xp0) (= l1 l0) (= lp1 lp0) (= x1 x0) (= N1 N0) (= a01 a20) (= I1 ip1) (= i1 i0) (= ip1 (+ I0 1)) (= ip1 ip0) (>= N1 (+ ip1 1))) (and q2 (= l1 l0) (= xp1 xp0) (= lp1 lp0) (= x1 x0) (= N1 N0) (= a01 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1)) (<= I1 (- ip1 1))) (and q1 (= xp1 xp0) (= l1 l0) (= I1 N1) (= N1 N0) (= x1 x0) (= lp1 lp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)))
x q3
(or (and q1 (= x1 x0) (= l1 l0) (= xp1 xp0) (= I1 N1) (= N1 N0) (= lp1 lp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q3 (= l1 l0) (= xp1 xp0) (= N1 N0) (= lp1 lp0) (= x1 x0) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1))))
x q0
(or (and q4 (= I1 1) (= I0 0) (= l1 l0) (= lp1 lp0) (= N1 N0) (= x1 x0) (= xp1 a00) (= xp1 xp0) (= i1 i0) (= ip1 ip0) (= a01 a20) (<= ip1 (- lp1 1)) (>= N1 2) (>= ip1 2)) (and q5 (= I1 1) (= I0 0) (= l1 l0) (= N1 N0) (= xp1 a00) (= xp1 xp0) (= x1 x0) (= lp1 lp0) (= xp1 a20) (= i1 i0) (= ip1 ip0) (<= ip1 0) (>= N1 2) (>= lp1 2)) (and q1 (= I1 1) (= N0 1) (= I0 0) (= l1 l0) (= lp1 lp0) (= N1 1) (= xp1 a00) (= xp1 xp0) (= x1 x0) (= xp1 a20) (= i1 i0) (= ip1 ip0) (<= ip1 0) (>= lp1 1)) (and q3 (= I1 1) (= I0 0) (= l1 l0) (= x1 x0) (= lp1 lp0) (= N1 N0) (= xp1 xp0) (= xp1 a00) (= i1 i0) (= ip1 ip0) (<= ip1 0) (>= ip1 lp1) (>= N1 2)) (and q1 (= I1 1) (= N0 1) (= I0 0) (= l1 l0) (= x1 x0) (= xp1 a00) (= xp1 xp0) (= N1 1) (= lp1 lp0) (= i1 i0) (= ip1 ip0) (<= ip1 (- lp1 1)) (>= ip1 1)) (and q3 (= lp0 1) (= I1 1) (= I0 0) (= N1 N0) (= lp1 1) (= xp1 a00) (= xp1 xp0) (= l1 l0) (= x1 x0) (= xp1 a20) (= i1 i0) (= ip1 ip0) (<= ip1 0) (>= N1 2)) (and q1 (= N0 1) (= I1 1) (= I0 0) (= l1 l0) (= xp1 a00) (= xp1 xp0) (= x1 x0) (= lp1 lp0) (= N1 1) (= i1 i0) (= ip1 ip0) (<= ip1 (- lp1 1)) (<= lp1 0)) (and q3 (= I1 1) (= ip1 1) (= I0 0) (= ip0 1) (= N1 N0) (= xp1 a00) (= xp1 xp0) (= lp1 lp0) (= l1 l0) (= x1 x0) (= i1 i0) (= a01 a20) (>= N1 2) (<= lp1 1)) (and q2 (= I1 1) (= I0 0) (= x1 x0) (= xp1 xp0) (= xp1 a00) (= l1 l0) (= N1 N0) (= lp1 lp0) (= a01 a20) (= i1 i0) (= ip1 ip0) (>= ip1 lp1) (>= N1 2) (>= ip1 2)) (and q3 (= I1 1) (= I0 0) (= l1 l0) (= N1 N0) (= x1 x0) (= xp1 xp0) (= xp1 a00) (= lp1 lp0) (= i1 i0) (= ip1 ip0) (>= N1 2) (<= ip1 (- lp1 1)) (<= lp1 0)) (and q1 (= I1 1) (= N0 1) (= I0 0) (= x1 x0) (= N1 1) (= xp1 xp0) (= xp1 a00) (= lp1 lp0) (= l1 l0) (= i1 i0) (= ip1 ip0) (>= ip1 lp1)) (and q5 (= ip1 1) (= I1 1) (= I0 0) (= ip0 1) (= xp1 a00) (= xp1 xp0) (= x1 x0) (= l1 l0) (= N1 N0) (= lp1 lp0) (= i1 i0) (= a01 a20) (>= lp1 2) (>= N1 2)))
x q5
(or (and q3 (= a00 a20) (= l1 l0) (= x1 x0) (= xp1 xp0) (= I1 lp1) (= lp1 lp0) (= N1 N0) (= lp1 (+ I0 1)) (= i1 i0) (= ip1 ip0) (>= N1 (+ lp1 1))) (and q5 (= a00 a20) (= xp1 xp0) (= l1 l0) (= lp1 lp0) (= N1 N0) (= x1 x0) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- lp1 1)) (<= I1 (- N1 1))) (and q1 (= a00 a20) (= l1 l0) (= I1 N1) (= N1 N0) (= x1 x0) (= lp1 lp0) (= xp1 xp0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)))
x q4
(or (and q1 (= I1 N1) (= N1 N0) (= lp1 lp0) (= l1 l0) (= xp1 xp0) (= x1 x0) (= N1 (+ I0 1)) (= i1 i0) (= ip1 ip0)) (and q5 (= x1 x0) (= lp1 lp0) (= l1 l0) (= N1 N0) (= xp1 xp0) (= a01 a20) (= I1 ip1) (= i1 i0) (= ip1 (+ I0 1)) (= ip1 ip0) (>= N1 (+ ip1 1))) (and q4 (= lp1 lp0) (= l1 l0) (= x1 x0) (= N1 N0) (= xp1 xp0) (= a01 a20) (= i1 i0) (= I1 (+ I0 1)) (= ip1 ip0) (<= I1 (- N1 1)) (<= I1 (- ip1 1))))
