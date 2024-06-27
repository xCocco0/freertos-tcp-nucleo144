# FreeRTOS Plus TCP on STM32F429ZI
This is a template project for running FreeRTOS with the Plus-TCP addon on STM32F4 boards. This project comes with example tasks for showcasing basic sending/receiving capabilities.

### Disclaimer

This repository comprises part of various resources found online such that:
- STM32CubeF4 ([Github Repository](https://github.com/STMicroelectronics/STM32CubeF4))
- Code generated using STM32CubeMX ([ST Website](https://www.st.com/en/development-tools/stm32cubemx.html))
- printf_stdarg ([Github Repository](https://github.com/htibosch/freertos_plus_projects))
- stm32f4xx HAL drivers ([Github Repository](https://github.com/STMicroelectronics/stm32f4xx_hal_driver))

Please check the respective links for proper licensing.

# Building 

The project is setup as a Makefile project. In order to compile the binary run

```sh
make
```

and it will build the target binary in the ``build/`` directory. The default name for the binary is ``test.bin``, you can change this name by modifying ``TARGET`` in the Makefile.

The script will by default build ctags for the projects, you can control this behaviour by overriding the ``CTAGS`` variable, either in the Makefile or by using
```sh
make CTAGS=0
```

You can instead disable the debug features by setting ``DEBUG`` variable as 0. While ``DEBUG`` is equal to 1 (by default), the compiler will use the ``-g`` and ``-Og`` options, and a ``DEBUG`` macro will be defined in the c sources.

This repository also contains a handy script ``setup``, which will call st-link utilities for flashing, and openocd/gdb for debugging. Running
```sh
make install
```
will call ``setup flash`` to flash the board.
