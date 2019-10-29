# Examples of mulib for the SAMD21 XPlained board

## mulibDemo1

The classic "blinky" app that flashes an LED.  Demonstrates the mu_sched and
mu_evt objects.

## mulibDemo2

Demonstrates how to add a custom idle task to the scheduler.  (Hint: it's easy.)
This idle task puts the SAMD21 into STANDBY mode when the scheduler has nothing
else to do.  Average processor current is reduced to 23.5 uA down from the 1.25 mA
typical for mulibDemo1.

## To rebuild a project from a fresh Atmel START file

* Double click on the .atzip file.  Atmel Studio 7 will launch.
* Complete the dialog panel to create project and solution
* Since we're providing our own mulib_demo.c file containing the `main` entry
  point, we need to disable the existing `main`.  In the Solution Explorer,
  select `edbg_uart_example.c`.  Right click to bring the context menu, select
  `Properties`.  Set `Build Action` to `None`
* For examples that need to print floating point numbers via `printf()`, select
  `Project => Properties => Toolchain => ARM/GNU Linker => General`.  Ensure that
  `--specs=nano.specs` is selected.  Then in `ARM/GNU Linker => Miscellaneous =>
  Other objects` add `-u _printf_float`
* In the Solution Explorer, right click on the Project Name (not the Solution
  Name), `Add => New Folder` and create `mu_src`.
* Right click on the `mu_src` folder you created and select `Add => Add Existing
  Item`.  In the file dialog, navigate to the `mu_src` folder in the mulib
  source directory and select all the files in that directory.  _Do not click
  Add!_  Instead, click on the down-arrow on the Add button and select
  `Add As Link`.
* To include unit test files, perform the same steps for adding source files,
  but with the `mu_tests` directory instead of the `mu_src` directory.
* At a minimum, you must copy three additional files to the top-level directory
  specific to your application and platform:
  * `main.c`
  * `mu_port.h`
  * `mu_port.c`
* To add the three new files to the project, right click on the Project and then
  `Add => Add Existing Item...`.  Navigate to the top-level source directory and
  select `main.c, mu_port.h, mu_port.c` and click `Add`.
* Extend the include path to find the new files: Select the Project, click on
  `Project => Properties => Toolchain => ARM/GNU C Compiler => Directories`.
  Add the directory for mulib/mu_src to the include path.

## cheap_ftoa()

With `--specs=nano.specs -u _printf_float`:

    `0x00006188                _etext = .`

With `--specs=nano.specs` only:

    `0x00003648                _etext = .`
