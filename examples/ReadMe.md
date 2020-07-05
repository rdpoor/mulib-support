# mulib examples
The `mulib/examples` directory contains ready-to-run applications that use
mulib.

## Organization

Directories inside the `mulib/examples/` are organized by IDE (Interactive
Development Environments), such as `MPLAB/`` for the Microchip MPLAB
X IDE.  

Within each IDE-specific subdirectory, you'll find the name of a development
platform, usually a development board, such as "SAME54 Xplained Pro" for the
Microchip SAME54 development board.

## Running the examples

In order to run one of the mulib examples, you will need one of the IDEs
found in the `mulib/examples/` directory, and one of the development boards
inside that IDE's subdirectory.

The recipe for compiling and running an application varies across different
IDEs, so consult the IDE documentation on how to download and compile the
example app.  That said, _usually_ there will be a project file that you can
open with your IDE, and from there you can click on "compile and run."

## Serial I/O

Most of the examples write to a serial port, and in addition some will read
from the serial port.  The mulib/examples have standardized a baud rate of
115200 and serial framing of 8n1 (eight data bits, no parity bit, one stop bit).

Typically, you'll need a terminal emulator such as
[Tera Term](https://osdn.net/projects/ttssh2/),
[Cool Term](https://www.freeware.the-meiers.org/) or
[PuTTY](https://putty.org/).
