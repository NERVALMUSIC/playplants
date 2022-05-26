##Installation steps

- Install the [arduino NRF52 core](https://github.com/sandeepmistry/arduino-nRF5) `Working on release 0.7`
- Add the folder "Planter" to your local packages folder in the arduino15 dirctory:
    "C:\Users\<USER>\AppData\Local\Arduino15\packages\sandeepmistry\hardware\nRF5\0.7.0"
- Modify the text file "boards.txt" to include the definitions and menu options of the new board added:
```
Planter.name=Planter

Planter.upload.tool=sandeepmistry:openocd
Planter.upload.target=nrf52
Planter.upload.maximum_size=524288

Planter.bootloader.tool=sandeepmistry:openocd

Planter.build.mcu=cortex-m4
Planter.build.f_cpu=16000000
Planter.build.board=Planter
Planter.build.core=nRF5
Planter.build.variant=Planter
Planter.build.variant_system_lib=
Planter.build.extra_flags=-DNRF52
Planter.build.float_flags=-mfloat-abi=hard -mfpu=fpv4-sp-d16
Planter.build.ldscript=nrf52_xxaa.ld

Planter.menu.softdevice.none=None
Planter.menu.softdevice.none.softdevice=none
Planter.menu.softdevice.none.softdeviceversion=

Planter.menu.softdevice.s132=S132
Planter.menu.softdevice.s132.softdevice=s132
Planter.menu.softdevice.s132.softdeviceversion=2.0.1
Planter.menu.softdevice.s132.upload.maximum_size=409600
Planter.menu.softdevice.s132.build.extra_flags=-DNRF52 -DS132 -DNRF51_S132
Planter.menu.softdevice.s132.build.ldscript=armgcc_s132_nrf52832_xxaa.ld

Planter.menu.lfclk.lfrc=RC Oscillator
Planter.menu.lfclk.lfrc.build.lfclk_flags=-DUSE_LFRC
Planter.menu.lfclk.lfsynt=Synthesized
Planter.menu.lfclk.lfsynt.build.lfclk_flags=-DUSE_LFSYNT
```