# pid-ball
Building my own PID controller from scratch using an Arduino Uno replica board, a tilting plane and a ping pong ball

## Inspiration
In my Control system university class I studied feed forward, feed back and PID control systems. This sparked my curiosity as to how hard it actually is to build a PID control system. I drew my inspiration from a youtube video in my reccomended feed that inplemented the same system, but I ended up doing my own version that strayed quite far from the original. There are tons of examples of such systems on youtube but I tried to do mine with as little research as possible, in order to be able to find my own sollutions for the various problems that would arise.

## First steps
My short term goal is to actually build the physical contraption and to implement a simple feed back control system. I'm using a cheap servo motor (Micro-Servo, 9g, SG90) as my actuator and an ultrasonic distance sensor (HC-SR04) to detect how far the ball is. I'm also using a ping pong ball to keep at a certain distance from the sensor as the "process". The whole thing is built out of Balsa wood and the "rails" the ball travels on are a couple of shortened dowel rods. To stick everything together a hot glue gun was used.

## PID
The term "PID" comes from Proportional, Integral and Derivative. A proportional system is a system that takes the system error as an input and outputs to the acutator a proportional command that brings the process closer to the refference input (or the wanted state). This is also known as a feed back system. The problem with this is that error is always part of the output, so the process will never be exactly as we want it. As a result an Integrator needs to be added, that "looks" into the past and compensates for this error. Another problem with such a system is that it tends to overshoot and oscilate around the wanted state, so a derivator that "looks" into the futre is implemented, in order to compensate for overshooting.
