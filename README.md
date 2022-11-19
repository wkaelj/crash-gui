# crash-gui
crash-gui is a cross platform library for displaying error messeges. It is able to create a new application with the error window, allowing it to survive a crash of the original application.

## Features


---
## Build Intructions

### Makefile
The project can be built using the makefile. The default target will build the static library.

If the static library is being used, the whole application will have to link the gtk libraries. Using `pkg-config`, this can be done by adding ``` `pkg-config gtk4 --libs` ``` to the linker flags.

- `make static` - build the static library, `libcrashgui-static.a`
- `make shared` - build the shared library, `libcrashgui-shared.so`

### Demo
The demo can be build using the makefile
```
make test
```

---
## Plans
- Windows support
- MacOS / Universal support with a window rendered using vulkan
- A option to show stack trace