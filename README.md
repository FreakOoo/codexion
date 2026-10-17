_This project has been created as part of the 42 curriculum by mchopin_

*DESCRIPTION*

This project is a revamp of the classic philosophers problem. Its purpose is to 
teach how threads work.

In this example there are N amount of coders and they each need to compile
a certain amount of times.
To do so they need to hold two usb sticks and plug them into a quantum computer.
They compile, then debug, then refactor, and repeat.

The difficulty of this project is based around ressource management.
Between each coder there is 1 stick, and they have a death timer. 
If a coder does not compile within his burnout timer the simulation ends.

So the program needs to be very efficient for the coders to manage as much work
as possible without dying.

To do so, we have all the coders doing their work in parallel.
The point is to find ways to get them to do so without getting in each other's
way, like taking sticks someone else needs more desperately. 
And we need to avoid things like two coders taking the same stick at once.


*INSTRUCTIONS*

To run this program:

git clone https://github.com/FreakOoo/codexion

run this command in the folder of your choice

then type 'make' in terminal and press enter.

now you can type in ./codexion 

followed by a space, the parameters of your liking in the following order:

./codexion number_of_coders time_to_burnout time_to_compile
        time_to_debug time_to_refactor number_of_compiles_required
        dongle_cooldown scheduler(fifo|edf)

this is reiterated at the top of main.c

example: ./codexion 100 1 1 1 3 2 fifo

*RESSOURCES*

- lots of youtube videos (especially oceano has a good one on deadlocks)
- geeksforgeeks (they have a great intro to multithreading)
- Deepseek and Claude for concept explanations and debugging


*Blocking cases handled*

To avoid two coders grabbing the same dongle at once,
if all else fails and they are equally eligible to grab it 
(near impossible but still), then one gets priority just based on his ID.

once the scheduler was added all coders are given a permission slip with a priority ranknig

When a dongle finishes it's cooldown all coders in the waiting room wake up,
check if they are at the top of the priority list, if yes, grab the dongle,
if not, go back to sleep.

Based on if you choose FIFO (first in first out), or EDF which prioritises
the coder closest to death, the mechanism is slightly different.

*Thread sychronization mechanisms*


