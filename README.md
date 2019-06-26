# Kaleidoscope-Simulator

A development, debugging and testing API for virtual Kaleidoscope firmware builds.

## Scope of this plugin

The scope of Kaleidoscope-Simulator is to provide an API that allows to bundle
integration tests with the keyboard firmware sketch file.

Kaleidoscope-Simulator relies on the two libraries [Papilio](https://github.com/CapeLeidokos/Papilio.git) and
[Aglais](https://github.com/CapeLeidokos/Aglais.git) for the simulation
infrastructure and the I/O recorder data format that is used to
recode keyboard sessions and replay them with the simulator.

For a description of both aspects please refer to the README.md files of both dependencies.

## A brief example

The following code snipped could be added to Kaleidoscope's stock
firmware sketch file (****.ino) to run
a very simple firmware test.

```cpp
// ... at end of the firmware file

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "Kaleidoscope-Simulator.h"

KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {
   
void runSimulator(Simulator &simulator) {
   
   using namespace actions;

   auto test = simulator.newTest("A simple test");

   // Have some action in order to trigger a keyboard report.
   //
   simulator.tapKey(2, 1); // (row = 2, col = 1) -> A
   
   // Run a single scan cycle (during which the key will be detected as
   // tapped and a keyboard report will be generated). Assert that the 
   // letter 'a' is part of this keyboard report
   //
   simulator.cycleExpectReports(AssertKeycodesActive{Key_A});
}

} // namespace simulator
} // namespace kaleidoscope

#endif
```

This very simple test checks if a keycode for the letter 'a' is present in a keyboard
report that is generated as a reaction on a key at matrix position (row = 2, col = 1) being tapped 
(pressed and immediately released). 

This test implies the standard QWERTY keyboard layout to active for the 
letter 'a' being associated with the key at the respective matrix position on layer 0.

Although a very simple test, it could already catch multiple types
of firmware programming issues, among those keymapping problems or reports accidentally not being
generated.

## A simple test in greater detail

Let's look again at the above example, now with focus on the way the test is
defined using Kaleidoscope-Simulator's API. 

We will walk through the test line by line.

First, we have to make sure that the compiler only sees our test in virtual firmware
builds. This is important as the simulator API can not be used in actual 
firmware builds for the target platform. That's because most target platforms resources
are simply too limited.

```cpp
#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
```

Next, we bring the simulator API into scope.

```cpp
#include "Kaleidoscope-Simulator.h"
```

It is good custom to define code in namespaces to avoid symbol naming conflicts.

```cpp
namespace kaleidoscope {
namespace simulator {
```

The test method as the standardized name `runTest` and a pre-defined signature.
You are free to structure your tests if necessary by introducing additional
test methods which can be called from `runSimulator(...)`. Please note that the `Simulator` object
is the central object in simulation and testing. It e.g. coordinates timing and action handling.

```cpp
void runSimulator(Simulator &simulator) {
```
   
The `using` statement is for mere convenience as all action classes live in namespace 
`actions`. Otherwise we would need to write `actions::AssertKeycodesActive{Key_A}`
instead of the more concise `AssertKeycodesActive{Key_A}`.

```cpp
   using namespace actions;
```

Next, we generate a test object. 
It's only purpose lies in its lifetime that is defined by the scope where
it is generated. It serves to group simulation and testing instructions and checks
if a set of actions that are associated with a test are valid.

```cpp
   auto test = simulator.newTest("A simple test");
```

Tap a key at a given matrix position.

```cpp
   simulator.tapKey(2, 1); // (row = 2, col = 1) -> A
```

Finally, we run a scan cycle. This will call Arduinos `loop()`
function under the hood. During this scan cycle the keyboard matrix
is checked for keys being pressed or released. That is when our key at
position (2, 1) is detected as having been tapped and a keyboard report is 
issued. We pass an action `AssertKeycodesActive` that will check if key 'a' is active 
in the generated keyboard report.

```cpp
   simulator.cycleExpectReports(AssertKeycodesActive{Key_A});
```

That's it. Close scopes and terminate the `#ifdef KALEIDOSCOPE_VIRTUAL_BUILD`.

```cpp
}
} // namespace simulator
} // namespace kaleidoscope

#endif
```

## Test recording with Aglais

[Aglais](https://github.com/CapeLeidokos/Aglais.git) is an I/O recorder data format, 
a library and a set of tools for USB-HID reporting keyboards.
It interfaces nicely with Kaleidoscope-Simulator.

Using Aglais, Kaleidoscope-Simulator can replay a session that was 
recorded on the original physical keyboard. Any key-action 
as well as all relevant HID report information is recorded with its exact timing.

The idea behind this approach is that it is much more convenient to
record a sample session on the original keyboard and use it for testing,
than defining (programming) complex testing setups. This makes testing
sophisticated plugins like Qukeys or One-Shot much more convenient.

A test is allowed to pass only when the exact sequence and timing of HID
reports generated by the simulator match those of the physical keyboard
during the recorded run. Only 100% equivalence is acceptable.

To record Aglais data-sets (documents), a dedicated 
Kaleidoscope plugin [Kaleidoscope-Simulator-Recorder](https://github.com/CapeLeidokos/Kaleidoscope-Simulator-Recorder.git) exists.
See the examples in its `examples` folder to find out how
it is used with a firmware sketch.

**Important:** Always take care that the configuration of the firmware
that is used for recording matches the firmware that is later running in
the simulator.

## Examples

There are several examples demonstrating Kaleidoscope-Simulator's features
and how to use the API. All examples reside in the `examples` directory tree.

To run an example, enter the following in your console window.

```
cd examples

make <relative path to subdirectory containing a tests.h file>
```

## Doxygen documentation

To generate Kaleidoscope-Simulator's API documentation with [doxygen](http://doxygen.nl/)
make sure doxygen is installed on you unixoid system (GNU/Linux & macOS) and run

```
make doc
```

in the root directory of plugin Kaleidoscope-Simulator.
