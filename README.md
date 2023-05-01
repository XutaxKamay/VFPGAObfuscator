# FPGA Simulator

This is a very early version of a FPGA simulator done for fun right now. It can only run logic gates in multiples stages.
The end goal would be to use it as an obfuscation system and a way to learn how a CPU could work, where you can upload the bitstream to the FPGA from networks in an encrypted form.
For example you would be able to run your custom virtual asynchronous soft-core where the CPU decrypts itself its instructions for funny things and CTFs built only from logic gates that runs in parallel inside your host CPU (GPU?). Or totally invent new stuffs.

With time (quite a lot of it), I plan to implement these things:
-  A similar HDL programming language where you can implement your own truth tables to write your own kind of operators to interact with variables (or anything else you want to call it for), because writing manually the logic gates and interacting with them each others from scratch might be too much of an overkill and quite a long task.
- Give an example of a small virtual soft-core connected to a virtual LCD and a virtual keyboard.
- Create logic gates that wouldn't be possible in real life.
- I have no idea much yet, but this is the best way I found to learn about hardware without spending any pennies, but I'm sure it will end up useful in the future years as a reference, at least for myself.
