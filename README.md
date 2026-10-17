*This project has been created as part of the 42 curriculum by mchopin*

**DESCRIPTION**

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


**INSTRUCTIONS**

To run this program:

git clone https://github.com/FreakOoo/codexion

run this command in the folder of your choice

then type 'make' in terminal and press enter.

now you can type in ./codexion 

followed by a space, the parameters of your liking in the following order:

```
./codexion number_of_coders time_to_burnout time_to_compile
           time_to_debug time_to_refactor number_of_compiles_required
           dongle_cooldown scheduler(fifo|edf)
```

this is reiterated at the top of main.c

example: `./codexion 5 800 100 50 50 3 20 fifo`

**RESOURCES**

- lots of youtube videos (especially oceano has a good one on deadlocks)
- geeksforgeeks (they have a great intro to multithreading)
- Deepseek and Claude for concept explanations and debugging (and to make my ranting clearer in this README.)


**Blocking cases handled**

- Deadlock: every coder takes their two dongles in the same global order
  (lower address first), so a circular wait can't form.
- Starvation: each dongle has a priority queue. Every request gets a ticket
  number (per dongle). FIFO ranks by ticket, EDF ranks by deadline
  (last compile start + time to burnout). Equal deadlines are broken by the
  lower ticket, so the result is always deterministic.
- Cooldown: on release, the dongle records when it is usable again
  (now + cooldown). The coder first in line sleeps until that moment.
- Wake-ups: a release wakes everyone waiting on that dongle. Each checks
  if they are first in line and the cooldown is over, otherwise they sleep again.
- Burnout: a monitor thread checks every ms, sets a shared dead flag,
  prints the burnout line, and wakes all sleeping coders so threads can be joined.
- Logs: one print mutex, and nothing is printed once the dead flag is set.

**Thread synchronization mechanisms**

- Mutex per dongle: protects its state (held, cooldown, queue), so two
  coders can't take the same dongle.
- Mutex per coder: protects last_compile and compiles, which the monitor reads.
- print_lock serializes output, dead_lock protects the dead / finished flags.
- Condition variable per dongle: coders sleep on it (cond_wait), and the
  first in line uses cond_timedwait to wake when the cooldown ends.
- Coders and monitor only communicate through the dead flag. The monitor
  broadcasts on every dongle (under its lock) after setting it, so no coder
  can miss it and sleep forever.
- Race avoided: checking "is it my turn?" and going to sleep both happen under
  the dongle's mutex (cond_wait releases it atomically), so a release can't
  slip in between and be missed.
