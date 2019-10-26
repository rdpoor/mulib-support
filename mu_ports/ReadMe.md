# ports directory
This directory holds library functions for specific software frameworks and
processor architectures.  The naming methodology is still evolving.  Note to
self: WWFRD?  (What would FreeRTOS do?)

The build system for a given target should include /ports/<target>/ on its
source and include paths.
