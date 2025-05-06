# DAC81404_lib
A basic BETA library for TI's DAC81404 precision DAC and the associated DAC81404EVM

The channel D of DAC81404 in the Evaluation Module (DAC81404EVM) is problematic where the sense pins are "intentionally"(?) reversed. A solution can be found [here](https://e2e.ti.com/support/data-converters-group/data-converters/f/data-converters-forum/980539/dac81404evm-weird-behavior-of-one-dac-channel).

**See `main.cpp` for an working example. Compile with Arduino IDE or PlatformIO.**

**Platforms:**
I tested the code with Teensy 3.x & 4.x. The code should work for other platforms as well. Please report if something is amiss.

**Limitations:**
1. BETA versions - some bugs may exist. Please report if you encounter one. 
2. LDAC pin is not handled by the library.

**TODO:**
1. Code cleanup
2. Add more examples and follow standard Arduino library format

![20210228_201841](https://github.com/user-attachments/assets/a77609d5-51d9-47d9-9d79-06c053c6222c)
The DAC81404EVM is running the example code with a Teensy 4.1, with Channel D’s circuitry on the EVM modified.
